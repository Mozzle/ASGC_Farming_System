''' ------------------------------------------------------------------------
I2C Packet IDs
------------------------------------------------------------------------ '''
RPI_ERR_PKT_ID				= 0
RPI_GCODE_PKT_ID            = 1
RPI_AHT20_PKT_ID            = 2
RPI_WATER_DATA_PKT_ID       = 3
RPI_BUTTONS_PKT_ID          = 4
RPI_NET_POT_STATUS_PKT_ID   = 5
RPI_GET_AXES_POS_PKT_ID     = 6

RPI_I2C_PACKET_SIZE 		= 128

PACKET_ID                   = 0
PACKET_VALID                = 1

''' ------------------------------------------------------------------------
RPI_GCODE_PKT_ID - GCode Packet
Packet Elements:
packet_id
valid
gcode_str
------------------------------------------------------------------------ '''
class RPI_I2C_Packet_GCode:
    def __init__(self, data):

        # Get packet ID
        self.packet_id = data[PACKET_ID]

        # If the packet ID is not the gcode packet
        if self.packet_id is not RPI_GCODE_PKT_ID:
            self.valid = False
        else:
            # Get packet validity from data
            self.valid = data[PACKET_VALID]

        self.gcode_str = data[2:66].decode('UTF-8').strip()

