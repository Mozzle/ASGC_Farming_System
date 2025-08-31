/*!
 *  @file Adafruit_AS7341.cpp
 *
 * 	I2C Driver for the Library for the AS7341 11-Channel Spectral Sensor
 *
 * 	This is a library for the Adafruit AS7341 breakout:
 * 	https://www.adafruit.com/product/4698
 *
 * 	Adafruit invests time and resources providing this open source code,
 *  please support Adafruit and open-source hardware by purchasing products from
 * 	Adafruit!
 *
 *  Copyright 2020 Bryan Siepert for Adafruit Industries
 *
 * 	BSD (see license.txt)
 */

/* Update by P. Chwalek to be compatible for STM32 C++ project (non-Arduino) */

/* CONVERTED FROM C++ TO C BY Dylan Collier									 */

#include "Adafruit_AS7341.h"

#include "main.h" // For switchboard functionality

static uint8_t last_spectral_int_source = 0;
static I2C_HandleTypeDef *i2c_han = NULL;///< Pointer to I2C bus interface
static uint8_t i2c_addr = 0;
static uint16_t _channel_readings[12];
static as7341_waiting_t _readingState;

/*!
 *    @brief  Sets up the hardware and initializes I2C
 *    @param  i2c_address
 *            The I2C address to be used.
 *    @param  wire
 *            The Wire object to be used for I2C connections.
 *    @param  sensor_id
 *            The unique ID to differentiate the sensors from others
 *    @return True if initialization was successful, otherwise false.
 * 
 * 		NOTE: THIS ORIGINALLY HAD DEFAULT VALUES. BEWARE
 */
bool Adafruit_AS7341_begin(uint8_t i2c_address, I2C_HandleTypeDef *i2c_handle,
		int32_t sensor_id) {

	i2c_han = i2c_handle;
	i2c_addr = i2c_address << 1;

	return Adafruit_AS7341__init(sensor_id);
}

/*!  @brief Initializer for post i2c/spi init
 *   @param sensor_id Optional unique ID for the sensor set
 *   @returns True if chip identified and initialized
 */
bool Adafruit_AS7341__init(int32_t sensor_id) {

	if (AS7341_ENABLED == SYS_FEATURE_DISABLED) {
		return true;
	}

	// make sure we're talking to the right chip
	if ((Adafruit_AS7341_readRegisterByte(AS7341_WHOAMI) & 0xFC) != (AS7341_CHIP_ID << 2)) {
		return false;
	}

	Adafruit_AS7341_powerEnable(true);

	// CONFIG VALUES
	Adafruit_AS7341_setATIME(100);
	Adafruit_AS7341_setASTEP(999);
	Adafruit_AS7341_setGain(AS7341_GAIN_256X);
	return true;
}

/********************* EXAMPLE EXTRACTS **************/
// maybe return a typedef enum
/**
 * @brief Returns the flicker detection status
 *
 * @return int8_t
 */
int8_t Adafruit_AS7341_getFlickerDetectStatus(void) {
	return (int8_t) Adafruit_AS7341_readRegisterByte(AS7341_FD_STATUS);
}

/**
 * @brief Returns the ADC data for a given channel
 *
 * @param channel The ADC channel to read
 * @return uint16_t The measured data for the currently configured sensor
 */
uint16_t Adafruit_AS7341_readChannel(as7341_adc_channel_t channel) {
	// each channel has two bytes, so offset by two for each next channel

	if (AS7341_ENABLED == SYS_FEATURE_DISABLED) {
		return 1; // AS7341 disabled value
	}

	uint8_t data[2];
	Adafruit_AS7341_readRegister((uint16_t) (AS7341_CH0_DATA_L + 2 * channel), data, 2);
	return (((uint16_t) data[1]) << 8) | data[0];
}

/**
 * @brief Returns the reading data for the specified color channel
 *
 *  call `readAllChannels` before reading to update the stored readings
 *
 * @param channel The color sensor channel to read
 * @return uint16_t The measured data for the selected sensor channel
 */
uint16_t Adafruit_AS7341_getChannel(as7341_color_channel_t channel) {
	return _channel_readings[channel];
}

/**
 * @brief fills the provided buffer with the current measurements for Spectral
 * channels F1-8, Clear and NIR
 *
 * @param readings_buffer Pointer to a buffer of length 10 or more to fill with
 * sensor data
 * @return true: success false: failure
 */
bool Adafruit_AS7341_readAllChannels(uint16_t *readings_buffer) {

	if (AS7341_ENABLED == SYS_FEATURE_DISABLED) {
		return true;
	}

	Adafruit_AS7341_setSMUXLowChannels(true);        // Configure SMUX to read low channels
	Adafruit_AS7341_enableSpectralMeasurement(true); // Start integration
	Adafruit_AS7341_delayForData(0);                 // I'll wait for you for all time

	bool low_success = Adafruit_AS7341_readRegister(AS7341_CH0_DATA_L, (uint8_t*) readings_buffer,
			12);

	Adafruit_AS7341_setSMUXLowChannels(false);       // Configure SMUX to read high channels
	Adafruit_AS7341_enableSpectralMeasurement(true); // Start integration
	Adafruit_AS7341_delayForData(0);                 // I'll wait for you for all time

	return low_success
			&& Adafruit_AS7341_readRegister(AS7341_CH0_DATA_L, (uint8_t*) &readings_buffer[6], 12);
}

/**
 * @brief starts the process of getting readings from all channels without using
 * delays
 *
 * @return true: success false: failure (a bit arbitrary)
 */
bool Adafruit_AS7341_startReading(void) {
	_readingState = AS7341_WAITING_START; // Start the measurement please
	Adafruit_AS7341_checkReadingProgress();               // Call the check function to start it
	return true;
}

/**
 * @brief runs the process of getting readings from all channels without using
 * delays.  Should be called regularly (ie. in loop()) Need to call
 * startReading() to initialise the process Need to call getAllChannels() to
 * transfer the data into an external buffer
 *
 * @return true: reading is complete false: reading is incomplete (or failed)
 */
bool Adafruit_AS7341_checkReadingProgress() {
	if (_readingState == AS7341_WAITING_START) {
		Adafruit_AS7341_setSMUXLowChannels(true);        // Configure SMUX to read low channels
		Adafruit_AS7341_enableSpectralMeasurement(true); // Start integration
		_readingState = AS7341_WAITING_LOW;
		return false;
	}

	if (!Adafruit_AS7341_getIsDataReady() || _readingState == AS7341_WAITING_DONE)
		return false;

	if (_readingState == AS7341_WAITING_LOW) // Check of getIsDataRead() is already done
			{
		Adafruit_AS7341_readRegister(AS7341_CH0_DATA_L, (uint8_t*) _channel_readings, 12);

		Adafruit_AS7341_setSMUXLowChannels(false);       // Configure SMUX to read high channels
		Adafruit_AS7341_enableSpectralMeasurement(true); // Start integration
		_readingState = AS7341_WAITING_HIGH;
		return false;
	}

	if (_readingState == AS7341_WAITING_HIGH) // Check of getIsDataRead() is already done
			{
		_readingState = AS7341_WAITING_DONE;
		Adafruit_AS7341_readRegister(AS7341_CH0_DATA_L, (uint8_t*) &_channel_readings[6], 12);
		return true;
	}

	return false;
}

/**
 * @brief transfer all the values from the private result buffer into one
 * nominated
 *
 * @param readings_buffer Pointer to a buffer of length 12 (THERE IS NO ERROR
 * CHECKING, YE BE WARNED!)
 *
 * @return true: success false: failure
 */
bool Adafruit_AS7341_getAllChannels(uint16_t *readings_buffer) {

	if (AS7341_ENABLED == SYS_FEATURE_DISABLED) {
		for (int i = 0; i < 12; i++) {
			readings_buffer[i] = 1; // AS7341 disabled value
		}
		return true;
	}

	for (int i = 0; i < 12; i++)
		readings_buffer[i] = _channel_readings[i];
	return true;
}

/**
 * @brief Delay while waiting for data, with option to time out and recover
 *
 * @param waitTime the maximum amount of time to wait
 * @return none
 */
void Adafruit_AS7341_delayForData(uint32_t waitTime) {
	if (waitTime == 0) // Wait forever
			{
		while (!Adafruit_AS7341_getIsDataReady()) {
			HAL_Delay(1);
		}
		return;
	}
	if (waitTime > 0) // Wait for that many milliseconds
			{
		uint32_t elapsedMillis = 0;
		while ((!Adafruit_AS7341_getIsDataReady()) && (elapsedMillis < waitTime)) {
			HAL_Delay(1);
			elapsedMillis++;
		}
		return;
	}
	if (waitTime < 0) {
		// For future use?
		return;
	}
}

/**
 * @brief Take readings for F1-8, Clear and NIR and store them in a buffer
 *
 * @return true: success false: failure
 */
bool Adafruit_AS7341_ReadAllChannels(void) {
	if (AS7341_ENABLED == SYS_FEATURE_DISABLED) {
		return true;
	}

	return Adafruit_AS7341_readAllChannels(_channel_readings);
}

void Adafruit_AS7341_setSMUXLowChannels(bool f1_f4) {
	Adafruit_AS7341_enableSpectralMeasurement(false);
	Adafruit_AS7341_setSMUXCommand(AS7341_SMUX_CMD_WRITE);
	if (f1_f4) {
		Adafruit_AS7341_setup_F1F4_Clear_NIR();
	} else {
		Adafruit_AS7341_setup_F5F8_Clear_NIR();
	}
	Adafruit_AS7341_enableSMUX();
}

/**
 * @brief Sets the power state of the sensor
 *
 * @param enable_power true: on false: off
 */
void Adafruit_AS7341_powerEnable(bool enable_power) {
	uint8_t enable_reg = Adafruit_AS7341_readRegisterByte(AS7341_ENABLE);
	enable_reg = Adafruit_AS7341_modifyBitInByte(enable_reg, (uint8_t) enable_power, 0);
	Adafruit_AS7341_writeRegisterByte(AS7341_ENABLE, enable_reg);
}

/**
 * @brief Disable Spectral reading, flicker detection, and power
 *
 * */
void Adafruit_AS7341_disableAll(void) {
	Adafruit_AS7341_writeRegisterByte(AS7341_ENABLE, 0);
}

/**
 * @brief Enables measurement of spectral data
 *
 * @param enable_measurement true: enabled false: disabled
 * @return true: success false: failure
 */
bool Adafruit_AS7341_enableSpectralMeasurement(bool enable_measurement) {

	uint8_t enable_reg = Adafruit_AS7341_readRegisterByte(AS7341_ENABLE);
	enable_reg = Adafruit_AS7341_modifyBitInByte(enable_reg, (uint8_t) enable_measurement, 1);

	return Adafruit_AS7341_writeRegisterByte(AS7341_ENABLE, enable_reg);
}

bool Adafruit_AS7341_enableSMUX(void) {
	bool success = Adafruit_AS7341_modifyRegisterBit(AS7341_ENABLE, true, 4);

	int timeOut = 1000; // Arbitrary value, but if it takes 1000 milliseconds then
											// something is wrong
	int count = 0;
	while (Adafruit_AS7341_checkRegisterBit(AS7341_ENABLE, 4) && count < timeOut) {
		HAL_Delay(1);
		count++;
	}
	if (count >= timeOut)
		return false;
	else
		return success;
}

bool Adafruit_AS7341_enableFlickerDetection(bool enable_fd) {
	return Adafruit_AS7341_modifyRegisterBit(AS7341_ENABLE, enable_fd, 6);
}

/**
 * @brief Get the GPIO pin direction setting
 *
 * @return `AS7341_OUTPUT` or `AS7341_INPUT`
 */
as7341_gpio_dir_t Adafruit_AS7341_getGPIODirection(void) {
	return (as7341_gpio_dir_t) Adafruit_AS7341_checkRegisterBit(AS7341_GPIO2, 2);
}

/**
 * @brief Set the GPIO pin to be used as an input or output
 *
 * @param gpio_direction The IO direction to set
 * @return true: success false: failure
 */
bool Adafruit_AS7341_setGPIODirection(as7341_gpio_dir_t gpio_direction) {
	return Adafruit_AS7341_modifyRegisterBit(AS7341_GPIO2, gpio_direction, 2);
}

/**
 * @brief Get the output inversion setting for the GPIO pin
 *
 * @return true: GPIO output inverted false: GPIO output normal
 */
bool Adafruit_AS7341_getGPIOInverted(void) {
	return Adafruit_AS7341_checkRegisterBit(AS7341_GPIO2, 3);
}

/**
 * @brief Invert the logic of then GPIO pin when used as an output
 *
 * @param gpio_inverted **When true** setting the gpio value to **true will
 * connect** the GPIO pin to ground. When set to **false**, setting the GPIO pin
 * value to **true will disconnect** the GPIO pin from ground
 * @return true: success false: failure
 */
bool Adafruit_AS7341_setGPIOInverted(bool gpio_inverted) {
	return Adafruit_AS7341_modifyRegisterBit(AS7341_GPIO2, gpio_inverted, 3);
}

/**
 * @brief Read the digital level of the GPIO pin, high or low
 *
 * @return true: GPIO pin level is high false: GPIO pin level is low
 */
bool Adafruit_AS7341_getGPIOValue(void) {
	return Adafruit_AS7341_checkRegisterBit(AS7341_GPIO2, 0);
}

/**
 * @brief Set the digital level of the GPIO pin, high or low
 *
 * @param gpio_high The GPIO level to set. Set to true to disconnect the pin
 * from ground. Set to false to connect the gpio pin to ground. This can be used
 * to connect the cathode of an LED to ground to turn it on.
 * @return true: success false: failure
 */
bool Adafruit_AS7341_setGPIOValue(bool gpio_high) {
	return Adafruit_AS7341_modifyRegisterBit(AS7341_GPIO2, gpio_high, 1);
}

bool Adafruit_AS7341_setSMUXCommand(as7341_smux_cmd_t command) {

	return Adafruit_AS7341_modifyRegisterMultipleBit(AS7341_CFG6, command, 3, 2);
}

/**
 * @brief Enable control of an attached LED on the LDR pin
 *
 * @param enable_led true: LED enabled false: LED disabled
 * @return true: success false: failure
 */
bool Adafruit_AS7341_enableLED(bool enable_led) {
	Adafruit_AS7341_setBank(true); // Access 0x60-0x74
	bool result = Adafruit_AS7341_modifyRegisterBit(AS7341_CONFIG, enable_led, 3)
			&& Adafruit_AS7341_modifyRegisterBit(AS7341_LED, enable_led, 7);
	Adafruit_AS7341_setBank(false); // Access registers 0x80 and above (default)
	return result;
}

/**
 * @brief Set the current limit for the LED
 *
 * @param led_current_ma the value to set in milliamps. With a minimum of 4. Any
 * amount under 4 will be rounded up to 4
 *
 * Range is 4mA to 258mA
 * @return true: success false: failure
 */
bool Adafruit_AS7341_setLEDCurrent(uint16_t led_current_ma) {
	// check within permissible range
	if (led_current_ma > 258) {
		return false;
	}
	if (led_current_ma < 4) {
		led_current_ma = 4;
	}
	Adafruit_AS7341_setBank(true); // Access 0x60 0x74

	bool result = Adafruit_AS7341_modifyRegisterMultipleBit(AS7341_LED,
			(uint8_t) ((led_current_ma - 4) / 2), 0, 7);
	Adafruit_AS7341_setBank(false); // Access registers 0x80 and above (default)
	return result;
}

/**
 * @brief Sets the active register bank
 *
 * The AS7341 uses banks to organize the register making it nescessary to set
 * the correct bank to access a register.
 *

 * @param low **true**:
 * **false**: Set the current bank to allow access to registers with addresses
 of `0x80` and above
 * @return true: success false: failure
 */
bool Adafruit_AS7341_setBank(bool low) {
	return Adafruit_AS7341_modifyRegisterBit(AS7341_CFG0, low, 4);
}

/**
 * @brief Sets the threshold below which spectral measurements will trigger
 * interrupts when the APERS count is reached
 *
 * @param low_threshold the new threshold
 * @return true: success false: failure
 */
bool Adafruit_AS7341_setLowThreshold(uint16_t low_threshold) {
	return Adafruit_AS7341_writeRegister(AS7341_SP_LOW_TH_L, (uint8_t*) &low_threshold, 2);
}

/**
 * @brief Returns the current low thighreshold for spectral measurements
 *
 * @return int16_t The current low threshold
 */
uint16_t Adafruit_AS7341_getLowThreshold(void) {
	uint8_t data[2];
	Adafruit_AS7341_readRegister(AS7341_SP_LOW_TH_L, data, 2);
	return (((uint16_t) data[1]) << 8) | data[0];
}

/**
 * @brief Sets the threshold above which spectral measurements will trigger
 * interrupts when the APERS count is reached
 *
 * @param high_threshold
 * @return true: success false: failure
 */
bool Adafruit_AS7341_setHighThreshold(uint16_t high_threshold) {
	return Adafruit_AS7341_writeRegister(AS7341_SP_HIGH_TH_L, (uint8_t*) &high_threshold, 2);
}

/**
 * @brief Returns the current high thighreshold for spectral measurements
 *
 * @return int16_t The current high threshold
 */
uint16_t Adafruit_AS7341_getHighThreshold(void) {
	uint8_t data[2];
	Adafruit_AS7341_readRegister(AS7341_SP_HIGH_TH_L, data, 2);
	return (((uint16_t) data[1]) << 8) | data[0];
}

/**
 * @brief Enable Interrupts based on spectral measurements
 *
 * @param enable_int true: enable false: disable
 * @return true: success false: falure
 */
bool Adafruit_AS7341_enableSpectralInterrupt(bool enable_int) {
	return Adafruit_AS7341_modifyRegisterBit(AS7341_INTENAB, enable_int, 3);
}

/**
 * @brief Enabled system interrupts
 *
 * @param enable_int Set to true to enable system interrupts
 * @return true: success false: failure
 */
bool Adafruit_AS7341_enableSystemInterrupt(bool enable_int) {
	return Adafruit_AS7341_modifyRegisterBit(AS7341_INTENAB, enable_int, 0);
}

// Spectral Interrupt Persistence.
// Defines a filter for the number of consecutive
// occurrences that spectral data must remain outside
// the threshold range between SP_TH_L and
// SP_TH_H before an interrupt is generated. The
// spectral data channel used for the persistence filter
// is set by SP_TH_CHANNEL. Any sample that is
// inside the threshold range resets the counter to 0.

/**
 * @brief Sets the number of times an interrupt threshold must be exceeded
 * before an interrupt is triggered
 *
 * @param cycle_count The number of cycles to trigger an interrupt
 * @return true: success false: failure
 */
bool Adafruit_AS7341_setAPERS(as7341_int_cycle_count_t cycle_count) {
	return Adafruit_AS7341_modifyRegisterMultipleBit(AS7341_PERS, cycle_count, 0, 4);
}

/**
 * @brief Set the ADC channel to use for spectral thresholds including
 * interrupts, automatic gain control, and persistance settings
 *
 * @param channel The channel to use for spectral thresholds. Must be a
 * as7341_adc_channel_t **except for** `AS7341_ADC_CHANNEL_5`
 * @return true: success false: failure
 */
bool Adafruit_AS7341_setSpectralThresholdChannel(
		as7341_adc_channel_t channel) {
	if (channel == AS7341_ADC_CHANNEL_5) {
		return false;
	}

	return Adafruit_AS7341_modifyRegisterMultipleBit(AS7341_CFG12, channel, 0, 3);
}

/**
 * @brief Returns the current value of the Interupt status register
 *
 * @return uint8_t
 */
uint8_t Adafruit_AS7341_getInterruptStatus(void) {
	return (uint8_t) Adafruit_AS7341_readRegisterByte(AS7341_STATUS);
}

/**
 * @brief Returns the status of the spectral measurement threshold interrupts
 *
 * @return true: interrupt triggered false: interrupt not triggered
 */
bool Adafruit_AS7341_spectralInterruptTriggered(void) {
	return Adafruit_AS7341_checkRegisterBit(AS7341_STATUS, 3);
}

/**
 * @brief Clear the interrupt status register
 *
 * @return true: success false: failure
 */
bool Adafruit_AS7341_clearInterruptStatus(void) {
	return Adafruit_AS7341_writeRegisterByte(AS7341_STATUS, 0xFF);
}

/**
 * @brief The current state of the spectral measurement interrupt status
 * register
 *
 * @return uint8_t The current status register
 */
uint8_t Adafruit_AS7341_spectralInterruptSource(void) {
	uint8_t spectral_int_source = Adafruit_AS7341_readRegisterByte(AS7341_STATUS3);
	last_spectral_int_source = spectral_int_source;
	return spectral_int_source;
}

/**
 * @brief The status of the low threshold interrupt
 *
 * @return true: low interrupt triggered false: interrupt not triggered
 */
bool Adafruit_AS7341_spectralLowTriggered(void) {
	return (last_spectral_int_source & AS7341_SPECTRAL_INT_LOW_MSK) > 0;
}

/**
 * @brief The status of the high threshold interrupt
 *
 * @return true: high interrupt triggered false: interrupt not triggered
 */
bool Adafruit_AS7341_spectralHighTriggered(void) {
	return (last_spectral_int_source & AS7341_SPECTRAL_INT_HIGH_MSK) > 0;
}

/**
 * @brief
 *
 * @return true: success false: failure
 */
bool Adafruit_AS7341_getIsDataReady() {
	return Adafruit_AS7341_checkRegisterBit(AS7341_STATUS2, 6);
}

/**
 * @brief Configure SMUX for sensors F1-4, Clear and NIR
 *
 */
void Adafruit_AS7341_setup_F1F4_Clear_NIR() {
	// SMUX Config for F1,F2,F3,F4,NIR,Clear
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x00), (uint8_t)(0x30)); // F3 left set to ADC2
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x01), (uint8_t)(0x01)); // F1 left set to ADC0
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x02), (uint8_t)(0x00)); // Reserved or disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x03), (uint8_t)(0x00)); // F8 left disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x04), (uint8_t)(0x00)); // F6 left disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x05), (uint8_t)(0x42)); // F4 left connected to ADC3/f2 left connected to ADC1
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x06), (uint8_t)(0x00)); // F5 left disbled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x07), (uint8_t)(0x00)); // F7 left disbled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x08), (uint8_t)(0x50)); // CLEAR connected to ADC4
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x09), (uint8_t)(0x00)); // F5 right disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0A), (uint8_t)(0x00)); // F7 right disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0B), (uint8_t)(0x00)); // Reserved or disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0C), (uint8_t)(0x20)); // F2 right connected to ADC1
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0D), (uint8_t)(0x04)); // F4 right connected to ADC3
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0E), (uint8_t)(0x00)); // F6/F8 right disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0F), (uint8_t)(0x30)); // F3 right connected to AD2
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x10), (uint8_t)(0x01)); // F1 right connected to AD0
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x11), (uint8_t)(0x50)); // CLEAR right connected to AD4
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x12), (uint8_t)(0x00)); // Reserved or disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x13), (uint8_t)(0x06)); // NIR connected to ADC5
}

/**
 * @brief Configure SMUX for sensors F5-8, Clear and NIR
 *
 */
void Adafruit_AS7341_setup_F5F8_Clear_NIR() {
	// SMUX Config for F5,F6,F7,F8,NIR,Clear
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x00), (uint8_t)(0x00)); // F3 left disable
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x01), (uint8_t)(0x00)); // F1 left disable
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x02), (uint8_t)(0x00)); // reserved/disable
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x03), (uint8_t)(0x40)); // F8 left connected to ADC3
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x04), (uint8_t)(0x02)); // F6 left connected to ADC1
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x05), (uint8_t)(0x00)); // F4/ F2 disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x06), (uint8_t)(0x10)); // F5 left connected to ADC0
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x07), (uint8_t)(0x03)); // F7 left connected to ADC2
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x08), (uint8_t)(0x50)); // CLEAR Connected to ADC4
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x09), (uint8_t)(0x10)); // F5 right connected to ADC0
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0A), (uint8_t)(0x03)); // F7 right connected to ADC2
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0B), (uint8_t)(0x00)); // Reserved or disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0C), (uint8_t)(0x00)); // F2 right disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0D), (uint8_t)(0x00)); // F4 right disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0E), (uint8_t)(0x24)); // F8 right connected to ADC2/ F6 right connected to ADC1
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0F), (uint8_t)(0x00)); // F3 right disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x10), (uint8_t)(0x00)); // F1 right disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x11), (uint8_t)(0x50)); // CLEAR right connected to AD4
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x12), (uint8_t)(0x00)); // Reserved or disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x13), (uint8_t)(0x06)); // NIR connected to ADC5
}

/**
 * @brief Configure SMUX for flicker detection
 *
 */
void Adafruit_AS7341_FDConfig() {
	// SMUX Config for Flicker- register (0x13)left set to ADC6 for flicker
	// detection
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x00), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x01), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x02), (uint8_t)(0x00)); // reserved/disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x03), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x04), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x05), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x06), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x07), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x08), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x09), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0A), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0B), (uint8_t)(0x00)); // Reserved or disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0C), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0D), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0E), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x0F), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x10), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x11), (uint8_t)(0x00)); // disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x12), (uint8_t)(0x00)); // Reserved or disabled
	Adafruit_AS7341_writeRegisterByte((uint8_t)(0x13), (uint8_t)(0x60)); // Flicker connected to ADC5 to left of 0x13
}

// TODO; check for valid values
/**
 * @brief Sets the integration time step count
 *
 * Total integration time will be `(ATIME + 1) * (ASTEP + 1) * 2.78µS`
 *
 * @param atime_value The integration time step count
 * @return true: success false: failure
 */
bool Adafruit_AS7341_setATIME(uint8_t atime_value) {
	return Adafruit_AS7341_writeRegisterByte(AS7341_ATIME, atime_value);
}

/**
 * @brief Returns the integration time step count
 *
 * Total integration time will be `(ATIME + 1) * (ASTEP + 1) * 2.78µS`
 *
 * @return uint8_t The current integration time step count
 */
uint8_t Adafruit_AS7341_getATIME() {
	return Adafruit_AS7341_readRegisterByte(AS7341_ATIME);
}

/**
 * @brief Sets the integration time step size
 *
 * @param astep_value Integration time step size in 2.78 microsecon increments
 * Step size is `(astep_value+1) * 2.78 uS`
 * @return true: success false: failure
 */
bool Adafruit_AS7341_setASTEP(uint16_t astep_value) {
	return Adafruit_AS7341_writeRegister(AS7341_ASTEP_L, (uint8_t*) &astep_value, 2);
}

/**
 * @brief Returns the integration time step size
 *
 * Step size is `(astep_value+1) * 2.78 uS`
 *
 * @return uint16_t The current integration time step size
 */
uint16_t Adafruit_AS7341_getASTEP() {
	uint8_t data[2];
	Adafruit_AS7341_readRegister(AS7341_ASTEP_L, data, 2);
	return (((uint16_t) data[1]) << 8) & data[0];
}

/**
 * @brief Sets the ADC gain multiplier
 *
 * @param gain_value The gain amount. must be an `as7341_gain_t`
 * @return true: success false: failure
 */
bool Adafruit_AS7341_setGain(as7341_gain_t gain_value) {

	return Adafruit_AS7341_writeRegisterByte(AS7341_CFG1, gain_value);
	// AGAIN bitfield is only[0:4] but the rest is empty
}

/**
 * @brief Returns the ADC gain multiplier
 *
 * @return as7341_gain_t The current ADC gain multiplier
 */
as7341_gain_t Adafruit_AS7341_getGain() {
	return (as7341_gain_t) Adafruit_AS7341_readRegisterByte(AS7341_CFG1);
}

/**
 * @brief Returns the integration time
 *
 * The integration time is `(ATIME + 1) * (ASTEP + 1) * 2.78µS`
 *
 * @return long The current integration time in ms
 */
long Adafruit_AS7341_getTINT() {
	uint16_t astep = Adafruit_AS7341_getASTEP();
	uint8_t atime = Adafruit_AS7341_getATIME();

	return (atime + 1) * (astep + 1) * 2.78 / 1000;
}

/**
 * @brief Converts raw ADC values to basic counts
 *
 * The basic counts are `RAW/(GAIN * TINT)`
 *
 * @param raw The raw ADC values to convert
 *
 * @return float The basic counts
 */
float Adafruit_AS7341_toBasicCounts(uint16_t raw) {
	float gain_val = 0;
	as7341_gain_t gain = Adafruit_AS7341_getGain();
	switch (gain) {
	case AS7341_GAIN_0_5X:
		gain_val = 0.5;
		break;
	case AS7341_GAIN_1X:
		gain_val = 1;
		break;
	case AS7341_GAIN_2X:
		gain_val = 2;
		break;
	case AS7341_GAIN_4X:
		gain_val = 4;
		break;
	case AS7341_GAIN_8X:
		gain_val = 8;
		break;
	case AS7341_GAIN_16X:
		gain_val = 16;
		break;
	case AS7341_GAIN_32X:
		gain_val = 32;
		break;
	case AS7341_GAIN_64X:
		gain_val = 64;
		break;
	case AS7341_GAIN_128X:
		gain_val = 128;
		break;
	case AS7341_GAIN_256X:
		gain_val = 256;
		break;
	case AS7341_GAIN_512X:
		gain_val = 512;
		break;
	}
	return raw / (gain_val * (Adafruit_AS7341_getATIME() + 1) * (Adafruit_AS7341_getASTEP() + 1) * 2.78 / 1000);
}

/**
 * @brief Detect a flickering light
 * @return The frequency of a detected flicker or 1 if a flicker of
 * unknown frequency is detected
 */
uint16_t Adafruit_AS7341_detectFlickerHz(void) {
//  bool isEnabled = true;
//  bool isFdmeasReady = false;

	// disable everything; Flicker detect, smux, wait, spectral, power
	Adafruit_AS7341_disableAll();
	// re-enable power
	Adafruit_AS7341_powerEnable(true);

	// Write SMUX configuration from RAM to set SMUX chain registers (Write 0x10
	// to CFG6)
	Adafruit_AS7341_setSMUXCommand(AS7341_SMUX_CMD_WRITE);

	// Write new configuration to all the 20 registers for detecting Flicker
	Adafruit_AS7341_FDConfig();

	// Start SMUX command
	Adafruit_AS7341_enableSMUX();

	// Enable SP_EN bit
	Adafruit_AS7341_enableSpectralMeasurement(true);

	// Enable flicker detection bit
	Adafruit_AS7341_writeRegisterByte((uint8_t) AS7341_ENABLE, (uint8_t) 0x41);

	HAL_Delay(500); // SF 2020-08-12 Does this really need to be so long?
	uint16_t flicker_status = Adafruit_AS7341_getFlickerDetectStatus();
	Adafruit_AS7341_enableFlickerDetection(false);
	switch (flicker_status) {
	case 44:
		return 1;
	case 45:
		return 100;
	case 46:
		return 120;
	default:
		return 0;
	}
}

/**
 * @brief Write a byte to the given register
 *
 * @param addr Register address
 * @param val The value to set the register to
 */
bool Adafruit_AS7341_writeRegister(uint8_t mem_addr, uint8_t *val,
		uint16_t size) {
	if (HAL_OK
			== HAL_I2C_Mem_Write(i2c_han, i2c_addr, mem_addr, 1, val, size, 10)) {
		return true;
	} else {
		return false;
	}
}

bool Adafruit_AS7341_writeRegisterByte(uint8_t mem_addr, uint8_t val) {
	if (HAL_OK
			== HAL_I2C_Mem_Write(i2c_han, i2c_addr, mem_addr, 1, &val, 1, 10)) {
		return true;
	} else {
		return false;
	}
}

uint8_t Adafruit_AS7341_modifyBitInByte(uint8_t var, uint8_t value,
		uint8_t pos) {
	uint8_t mask = 1 << pos;
	return ((var & ~mask) | (value << pos));
}

bool Adafruit_AS7341_readRegister(uint16_t mem_addr, uint8_t *dest,
		uint16_t size) {
	if (HAL_OK
			== HAL_I2C_Mem_Read(i2c_han, i2c_addr, mem_addr, 1, dest, size, 10)) {
		return true;
	} else {
		return false;
	}
}

uint8_t Adafruit_AS7341_checkRegisterBit(uint16_t reg, uint8_t pos) {
	return (uint8_t) ((Adafruit_AS7341_readRegisterByte(reg) >> pos) & 0x01);
}

bool Adafruit_AS7341_modifyRegisterBit(uint16_t reg, bool value, uint8_t pos) {
	uint8_t register_value = Adafruit_AS7341_readRegisterByte(reg);
	register_value = Adafruit_AS7341_modifyBitInByte(register_value, (uint8_t) value, pos);

	return Adafruit_AS7341_writeRegisterByte(reg, register_value);
}

bool Adafruit_AS7341_modifyRegisterMultipleBit(uint16_t reg, uint8_t value,
		uint8_t pos, uint8_t bits) {
	uint8_t register_value = Adafruit_AS7341_readRegisterByte(reg);

	uint8_t mask = (1 << (bits)) - 1;
	value &= mask;

	mask <<= pos;
	register_value &= ~mask;          // remove the current data at that spot
	register_value |= value << pos; // and add in the new data

	return Adafruit_AS7341_writeRegisterByte(reg, register_value);
}

uint8_t Adafruit_AS7341_readRegisterByte(uint16_t mem_addr) {
	uint8_t data;
	HAL_I2C_Mem_Read(i2c_han, i2c_addr, mem_addr, 1, &data, 1, 10);
	return data;
}
