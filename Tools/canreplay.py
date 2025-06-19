import can
import sys, time
from scapy.all import rdpcap, load_layer

file = sys.argv[1]
if len(sys.argv > 3):
    period = float(sys.argv[2])
else:
    period = 0.01

load_layer('can')

packets = rdpcap(file)
bus = can.Bus(interface='socketcan', channel='can0')

for packet in packets:
    packet_can = packet['CAN']
    print(packet_can.data)

    msg = can.Message(arbitration_id=packet_can.identifier, is_extended_id=False, 
        data=packet_can.data)
    bus.send(msg)
    time.sleep(period)

bus.shutdown()