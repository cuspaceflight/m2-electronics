import serial
import sys
s = serial.Serial(sys.argv[1], 38400)
s.write("\x7E\x7E\x7E")
