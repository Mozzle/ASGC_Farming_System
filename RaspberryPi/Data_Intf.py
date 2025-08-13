import time
import pigpio

SDA_PIN=18
SCL_PIN=19

I2C_ADDR=9

class Data_Interface:
   global pi

   def __init__(self):
      pi = pigpio.pi()

      if not pi.connected:
         exit()

      # Add pull-ups in case external pull-ups haven't been added
      pi.set_pull_up_down(SDA_PIN, pigpio.PUD_UP)
      pi.set_pull_up_down(SCL_PIN, pigpio.PUD_UP)

      # Respond to BSC slave activity
      e = pi.event_callback(pigpio.EVENT_BSC, self.i2c_loop)

      pi.bsc_i2c(I2C_ADDR) # Configure BSC as I2C slave 
      time.sleep(600)

   def i2c_loop(id, tick):

      status, bytes_rec, data = pi.bsc_i2c(I2C_ADDR) #status, num bytes, data

      # If we received data
      if bytes_rec:
         print(data[:-1])

         # Match the pkt_id
         match data[0]:
            case 0:
               print("Yeah!")
               # Process the data based on the packet structure
               # Forward Data to necessary consumer.
               # "echo [gcode] >> /tmp/printer" to send gcode to CNC
            case 1:
               print("Something!")
            case 2:
               print("Else!")
            case _:
               print("okay")


pi = None
Interface = Data_Interface()
e.cancel()

pi.bsc_i2c(0) # Disable BSC peripheral

pi.stop()

