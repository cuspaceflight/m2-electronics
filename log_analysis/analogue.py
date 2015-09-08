import sys
import json
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
    states = []
    se_h = []
    se_v = []
    se_a = []
    sg_t = []
    sg1 = []
    sg2 = []
    sg3 = []
    tc_t = []
    tc1 = []
    tc2 = []
    tc3 = []

    last_timestamp = 0
    t_correction = 0

    f = open(sys.argv[1], "rb")
    while True:
        try:
            packet = f.read(16)
        except ValueError:
            break
        if len(packet) != 16:
            break

        timestamp = struct.unpack("i", packet[:4])[0]
        if timestamp < last_timestamp:
            t_correction += 0xFFFFFFFF
        last_timestamp = timestamp
        timestamp += t_correction
        mode, channel, _, _ = struct.unpack("BBBB", packet[4:8])

        if channel == 0x31:
            data = struct.unpack("hhhh", packet[8:])
            sg_t.append(timestamp)
            sg1.append(data[0])
            sg2.append(data[1])
            sg3.append(data[2])
        elif channel == 0x32:
            data = struct.unpack("hhhh", packet[8:])
            tc_t.append(timestamp)
            tc1.append(data[0])
            tc2.append(data[1])
            tc3.append(data[2])

    sg_t = np.array(sg_t) / 168E6
    tc_t = np.array(tc_t) / 168E6

    #plt.plot(lg_accel_t, lg_accel_x, label="x")
    #plt.plot(lg_accel_t, lg_accel_y, label="y")
    #plt.plot(lg_accel_t, lg_accel_z, label="z")
    #plt.title("Low G Accelerometer")
    #plt.legend()
    #plt.show()
    #plt.plot(hg_accel_t, hg_accel_x, label="x")
    #plt.plot(hg_accel_t, hg_accel_y, label="y")
    #plt.plot(hg_accel_t, hg_accel_z, label="z")
    #plt.title("High G Accelerometer")
    #plt.legend()
    #plt.show()
    #plt.plot(baro_t, pressures)
    #plt.title("Pressure")
    #plt.show()
    #plt.plot(baro_t, temperatures)
    #plt.title("Barometer Temperature")
    #plt.show()
    #plt.plot(se_h, label="Height")
    #plt.plot(se_v, label="Velocity")
    #plt.plot(se_a, label="Acceleration")
    #plt.title("State Estimation")
    #plt.legend()
    #plt.show()
    plt.plot(sg_t, sg1, label="SG1")
    plt.plot(sg_t, sg2, label="SG2")
    plt.plot(sg_t, sg3, label="SG3")
    plt.title("Strain Gauges")
    plt.legend()
    plt.show()
    plt.plot(tc_t, tc1, label="TC1")
    plt.plot(tc_t, tc2, label="TC2")
    plt.plot(tc_t, tc3, label="TC3")
    plt.title("Thermocouples")
    plt.legend()
    plt.show()

if __name__ == "__main__":
    main()
