import sys
import math
import struct
import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
    print("Usage: {} <binary logfile>".format(sys.argv[0]))
    sys.exit()

mission_states_map = {
    0: "Pad",
    1: "Ignition",
    2: "Powered Ascent",
    3: "Free Ascent",
    4: "Apogee",
    5: "Drogue Descent",
    6: "Main Release",
    7: "Main Descent",
    8: "Land",
    9: "Landed"
}

Rs = 8.31432
g0 = 9.80665
M = 0.0289644
Lb = [-0.0065, 0.0, 0.001, 0.0028, 0.0, -0.0028, -0.002]
Pb = [101325.0, 22632.10, 5474.89, 868.02, 110.91, 66.94, 3.96]
Tb = [288.15, 216.65, 216.65, 228.65, 270.65, 270.65, 214.65]
Hb = [0.0, 11000.0, 20000.0, 32000.0, 47000.0, 51000.0, 71000.0]

def p2a(p):
    for b in range(7):
        if p <= Pb[b] and p > Pb[b+1]:
            if Lb[b] == 0.0:
                return p2a_zl(p, b)
            else:
                return p2a_nzl(p, b)
    return -9999.0

def p2a_zl(p, b):
    hb = Hb[b]
    pb = Pb[b]
    tb = Tb[b]
    return hb + (Rs * tb)/(g0 * M) * (math.log(p) - math.log(pb))

def p2a_nzl(p, b):
    lb = Lb[b]
    hb = Hb[b]
    pb = Pb[b]
    tb = Tb[b]
    return hb + tb/lb * (math.pow(p/pb, (-Rs*lb)/(g0*M)) - 1)

se_t = []
se_dt = []
se_h = []
se_v = []
se_a = []
baro_t = []
baro_h = []
accel_t = []
accel_a = []
mission_t = []
mission_s = []
lga_y = []
hga_y = []
pyro_fire_t = []
pyro_fire_c = []
last_timestamp = 0
t_correction = 0

with open(sys.argv[1], "rb") as f:
    while True:
        try:
            packet = f.read(16)
        except ValueError:
            break
        if len(packet) != 16:
            break
        
        timestamp = struct.unpack("I", packet[:4])[0]
        mode, channel, _, _ = struct.unpack("BBBB", packet[4:8])
        if timestamp < last_timestamp:
            t_correction += 0xFFFFFFFF
        last_timestamp = timestamp
        timestamp += t_correction

        if channel == 0x11:
            data = struct.unpack("hhhh", packet[8:])
            axis, grav, _, _ = data
            print("LGA cal axis={} grav={}".format(axis, grav))
        if channel == 0x12:
            data = struct.unpack("hhhh", packet[8:])
            axis, grav, _, _ = data
            print("HGA cal axis={} grav={}".format(axis, grav))
        if channel == 0x50:
            data = struct.unpack("ff", packet[8:])
            se_t.append(timestamp / 168E6)
            se_h.append(data[1])
        if channel == 0x51:
            data = struct.unpack("ff", packet[8:])
            se_v.append(data[0])
            se_a.append(data[1])
        if channel == 0x52:
            data = struct.unpack("ff", packet[8:])
            baro_t.append(timestamp / 168E6)
            baro_h.append(p2a(data[0]))
        if channel == 0x53:
            data = struct.unpack("ff", packet[8:])
            accel_t.append(timestamp / 168E6)
            accel_a.append(data[0])
        if channel == 0x40:
            data = struct.unpack("ii", packet[8:])
            mission_t.append(timestamp / 168E6)
            mission_s.append(data[1])
        if channel == 0x20:
            x, y, z, _ = struct.unpack("hhhh", packet[8:])
            lga_y.append(y)
        if channel == 0x21:
            x, y, z, _ = struct.unpack("hhhh", packet[8:])
            hga_y.append(y)
        if channel == 0x61:
            ch1, ch2, ch3, _ = struct.unpack("hhhh", packet[8:])
            pyro_fire_t.append(timestamp / 168E6)
            if ch1:
                pyro_fire_c.append(1)
            elif ch2:
                pyro_fire_c.append(2)
            elif ch3:
                pyro_fire_c.append(3)

#plt.subplot(2, 1, 1)
#plt.plot(lga_y)
#plt.title("LGA")
#plt.subplot(2, 1, 2)
#plt.plot(hga_y)
#plt.title("HGA")
#plt.show()

# if we got a p1 and no p2, fix
se_t = se_t[:len(se_v)]
se_h = se_h[:len(se_v)]

plt.subplot(3, 1, 1)
plt.plot(baro_t, baro_h, 'x', color='k', label="Sensor Readings")
plt.plot(se_t, se_h, color='b', lw=3, label="State Estimate")
plt.ylim((-100, 1000))
for t, s in zip(mission_t, mission_s):
    plt.axvline(t)
    ydisp = plt.gca().transAxes.transform((0, 1 - s/10.0))[1]
    ydata = plt.gca().transData.inverted().transform((0, ydisp))[1]
    try:
        plt.text(t, ydata, mission_states_map[int(s)])
    except KeyError:
        print("Unknown mission state t={} s={}".format(t, s))
for t, c in zip(pyro_fire_t, pyro_fire_c):
    plt.axvline(t, color='r')
    plt.text(t, 0, str(c), color='r')
plt.ylabel("Altitude (m)")
plt.xlim((390, 470))
plt.legend()
plt.grid()

plt.subplot(3, 1, 2)
plt.plot(se_t, se_v, color='g', label="State Estimate")
for t, s in zip(mission_t, mission_s):
    plt.axvline(t)
    ydisp = plt.gca().transAxes.transform((0, 1 - s/10.0))[1]
    ydata = plt.gca().transData.inverted().transform((0, ydisp))[1]
    try:
        plt.text(t, ydata, mission_states_map[int(s)])
    except KeyError:
        print("Unknown mission state t={} s={}".format(t, s))
plt.ylabel("Velocity (m/s)")
plt.xlim((390, 470))
plt.legend()
plt.legend()
plt.grid()

plt.subplot(3, 1, 3)
plt.plot(accel_t, accel_a, 'x', color='k', label="Sensor Readings")
plt.plot(se_t, se_a, color='r', lw=3, label="State Estimate")
plt.xlim((390, 470))
plt.ylim((-30, 200))
plt.ylabel("Acceleration (m/s/s)")
for t, s in zip(mission_t, mission_s):
    plt.axvline(t)
    ydisp = plt.gca().transAxes.transform((0, 1 - s/10.0))[1]
    ydata = plt.gca().transData.inverted().transform((0, ydisp))[1]
    try:
        plt.text(t, ydata, mission_states_map[int(s)])
    except KeyError:
        print("Unknown mission state t={} s={}".format(t, s))
plt.xlabel("Time (s)")
plt.legend()
plt.legend()
plt.grid()

plt.show()
