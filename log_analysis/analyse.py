import sys
import struct
import numpy as np
import matplotlib.pyplot as plt


def main():
    if len(sys.argv) != 2:
        print("Usage: {} <logfile>".format(sys.argv[0]))
        return

    pressures = []
    temperatures = []
    hg_accel_x = []
    hg_accel_y = []
    hg_accel_z = []
    lg_accel_x = []
    lg_accel_y = []
    lg_accel_z = []

    f = open(sys.argv[1], "rb")
    while True:
        try:
            packet = f.read(16)
        except ValueError:
            break
        timestamp = struct.unpack("i", packet[:4])
        a, b, mode, channel = struct.unpack("BBBB", packet[4:8])

        if a != 0 or b != 0:
            break

        if mode == 0:
            data = struct.unpack("8s", packet[8:])
        elif mode == 1:
            data = struct.unpack("q", packet[8:])
        elif mode == 2:
            data = struct.unpack("ii", packet[8:])
        elif mode == 3:
            data = struct.unpack("hhhh", packet[8:])
        elif mode == 4:
            data = struct.unpack("HHHH", packet[8:])

        if channel == 0x10:
            lg_accel_x.append(data[0] / 265.)
            lg_accel_y.append(data[1] / 265.)
            lg_accel_z.append(data[2] / 256.)
        if channel == 0x20:
            hg_accel_x.append(data[0] / 256.)
            hg_accel_y.append(data[1] / 256.)
            hg_accel_z.append(data[2] / 256.)
        if channel == 0x30:
            pressures.append(data[0])
            temperatures.append(data[1])

    plt.plot(lg_accel_x)
    plt.plot(lg_accel_y)
    plt.plot(lg_accel_z)
    plt.show()


if __name__ == "__main__":
    main()
