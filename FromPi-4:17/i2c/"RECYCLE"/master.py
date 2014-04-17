import smbus
import time

bus = smbus.SMBus(1)

for a in range(0,512):
    bus.write_byte(0x70, a) # 0x20: address of the detected slave
    time.sleep(0.1)
