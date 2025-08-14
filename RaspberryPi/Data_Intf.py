import time
import pigpio
import I2C_Packets
from subprocess import call

SDA_PIN=18
SCL_PIN=19

I2C_ADDR=9

class Data_Interface:
   global pi
   global e

   ''' ------------------------------------------------------------------------
   Data_Interface Init

      Creates the I2C data interface using the pigpio library
   ------------------------------------------------------------------------ '''
   def __init__(self):
      pi = pigpio.pi()
      pkt_rec_count = 0
      pkt_success_count = 0

      if not pi.connected:
         exit()

      # Add pull-ups in case external pull-ups haven't been added
      pi.set_pull_up_down(SDA_PIN, pigpio.PUD_UP)
      pi.set_pull_up_down(SCL_PIN, pigpio.PUD_UP)

      # Respond to BSC slave activity, registering the i2c_loop as callback function
      e = pi.event_callback(pigpio.EVENT_BSC, self.i2c_loop)

      pi.bsc_i2c(I2C_ADDR) # Configure BSC as I2C slave
      time.sleep(600)

   ''' ------------------------------------------------------------------------
   i2c_loop

      The main callback loop that is called whenever BSC (Broadcom Serial
      Controller) activity is detected. Handles receipt, processing, and
      responding to I2C packets from the Nucleo Board.
   ------------------------------------------------------------------------ '''
   def i2c_loop(id, tick):

      status, bytes_rec, data = pi.bsc_i2c(I2C_ADDR) #status, num bytes, data

      # If we received data
      if bytes_rec:
         #print(data[:-1])
         pkt_rec_count += 1

         if len(data) is not I2C_Packets.RPI_I2C_PACKET_SIZE:
            # Error of some kind
            # Maybe send back a 'data not received' pkt
            print("ERROR: Packet length mismatch!")
            pass

         # Match the pkt_id
         match data[I2C_Packets.PACKET_ID]:

            # ------------------------ ERROR PKT ID ---------------------------
            case I2C_Packets.RPI_ERR_PKT_ID:
               print("Yeah!")

            # ------------------------ GCODE PKT ID ---------------------------
            case I2C_Packets.RPI_GCODE_PKT_ID:
               # Parse the data into the packet struct
               pkt = I2C_Packets.RPI_I2C_Packet_GCode(data)

               # If packet is valid
               if pkt.valid is True:
                  # Send the gcode to the SKR MINI E3 via the terminal
                  terminal_cmd = "echo " + pkt.gcode_str + " >> /tmp/printer"
                  call(terminal_cmd)

                  if pkt.gcode_str == "G28":
                     pkt_success_count += 1

                  print("GCode [" + pkt_success_count + "/" + pkt_rec_count + "]: " + pkt.gcode_str)

            # ---------------------- AHT20 DATA PKT ID ------------------------
            case I2C_Packets.RPI_AHT20_PKT_ID:
               print("Else!")

            # ---------------------- WATER DATA PKT ID ------------------------
            case I2C_Packets.RPI_WATER_DATA_PKT_ID:
               pass
            
            # --------------------- BUTTONS DATA PKT ID -----------------------
            case I2C_Packets.RPI_BUTTONS_PKT_ID:
               pass
            
            # -------------------- NET POT STATUS PKT ID ----------------------
            case I2C_Packets.RPI_NET_POT_STATUS_PKT_ID:
               pass
            
            # ---------------- GET AXES DATA REQUEST PKT ID -------------------
            case I2C_Packets.RPI_GET_AXES_POS_PKT_ID:
               pass

            # ----------------------- DEFAULT CASE ----------------------------
            case _:
               print("okay")

         

''' ------------------------------------------------------------------------
   Program Entry Point
   ------------------------------------------------------------------------ '''
# Create the global pgio object
pi = None
e = None
# Start the interface
Interface = Data_Interface()

# If the interface exits, gracefully shut down
e.cancel()
pi.bsc_i2c(0) # Disable BSC peripheral
pi.stop()

