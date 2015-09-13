import sys
import struct
import numpy as np
import matplotlib.pyplot as plt


def main():
    if len(sys.argv) != 2:
        print("Usage: {} <logfile>".format(sys.argv[0]))
        return

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
    tc1 = np.array(tc1)
    tc2 = np.array(tc2)
    tc3 = np.array(tc3)
    sg1 = np.array(sg1)
    sg2 = np.array(sg2)
    sg3 = np.array(sg3)

    #sg_t = sg_t[::10]
    #sg1 = sg1[::10]
    #sg2 = sg2[::10]
    #sg3 = sg3[::10]
    #plt.plot(sg_t, sg1, '-o', label="SG1")
    #plt.plot(sg_t, sg2, '-o', label="SG2")
    #plt.plot(sg_t, sg3, '-o', label="SG3")
    #plt.title("Strain Gauges")
    #plt.legend()
    #plt.show()

    tstep = 50000
    #tc_t = np.arange(0, tc1.size/2000, 1/2000)
    #for t0 in range(0, tc1.size, tstep):
        #plt.plot(tc_t[t0:t0+tstep], tc1[t0:t0+tstep], '-o', label="TC1")
        #plt.plot(tc_t[t0:t0+tstep], tc2[t0:t0+tstep], '-o', label="TC2")
        #plt.plot(tc_t[t0:t0+tstep], tc3[t0:t0+tstep], '-o', label="TC3")
        #plt.title("Thermocouples")
        #plt.legend()
        #plt.show()

    #sg_t = np.arange(0, sg1.size/20000, 1/20000)
    for t0 in range(0, sg1.size, tstep):
        plt.plot(sg_t[t0:t0+tstep], sg1[t0:t0+tstep], '-o', label="SG1")
        plt.plot(sg_t[t0:t0+tstep], sg2[t0:t0+tstep], '-o', label="SG2")
        plt.plot(sg_t[t0:t0+tstep], sg3[t0:t0+tstep], '-o', label="SG2")
        plt.title("Strain Gauges")
        plt.legend()
        plt.show()

if __name__ == "__main__":
    main()
