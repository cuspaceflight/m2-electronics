import sys
import struct
import numpy as np
import matplotlib.pyplot as plt


def main():
    if len(sys.argv) != 2:
        print("Usage: {} <logfile>".format(sys.argv[0]))
        return

    baro_t = []
    pressures = []
    temperatures = []
    hg_accel_t = []
    hg_accel_x = []
    hg_accel_y = []
    hg_accel_z = []
    lg_accel_t = []
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
            lg_accel_t.append(timestamp)
            lg_accel_x.append(data[0] / 265.)
            lg_accel_y.append(data[1] / 265.)
            lg_accel_z.append(data[2] / 256.)
        if channel == 0x20:
            hg_accel_t.append(timestamp)
            hg_accel_x.append(data[0] / 256.)
            hg_accel_y.append(data[1] / 256.)
            hg_accel_z.append(data[2] / 256.)
        if channel == 0x30:
            baro_t.append(timestamp)
            pressures.append(data[0])
            temperatures.append(data[1])

    lg_accel_t = np.array(lg_accel_t)
    hg_accel_t = np.array(hg_accel_t)
    baro_t = np.array(baro_t)

    lg_t_d = np.diff(lg_accel_t.T)
    hg_t_d = np.diff(hg_accel_t.T)
    baro_t_d = np.diff(baro_t.T)

    lg_t_d = lg_t_d[lg_t_d > 0]
    hg_t_d = hg_t_d[hg_t_d > 0]
    baro_t_d = baro_t_d[baro_t_d > 0]

    print("Mean time between LG samples:", lg_t_d.mean())
    print("std:", np.std(lg_t_d))
    print("")

    print("Mean time between HG samples:", hg_t_d.mean())
    print("std:", np.std(hg_t_d))
    print("")

    print("Mean time between baro samples:", baro_t_d.mean())
    print("std:", np.std(baro_t_d))
    print("")

    t = lg_accel_t / 168E6

    plt.plot(t[10000:10100], lg_accel_x[10000:10100], '-x')
    plt.plot(t[10000:10100], lg_accel_y[10000:10100], '-x')
    plt.plot(t[10000:10100], lg_accel_z[10000:10100], '-x')
    plt.show()

if __name__ == "__main__":
    main()
