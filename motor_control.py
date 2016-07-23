import serial
import time

ser = serial.Serial( '/dev/ttyACM1', 115200 )


ser.write( 'INIT' )
time.sleep(1)
ser.write( 'SPEED 38' )
time.sleep(1)

for i in range( 10 ):
  for n in range( 10 ):
    ser.write( 'POS 1' )
    time.sleep( 1 )
  
  for n in range( 10 ):
    ser.write( 'POS -1' )
    time.sleep( 1 )
  
