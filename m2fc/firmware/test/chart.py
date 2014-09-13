import sys
import numpy as np
import matplotlib.pyplot as plt

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

state = np.loadtxt(sys.argv[1])
sensor = np.loadtxt(sys.argv[2])
mission = np.loadtxt(sys.argv[3])

tstate = state[:, 0]
dts = state[:, 1]
h = state[:, 2]
v = state[:, 3]
a = state[:, 4]

tsensor = sensor[:, 0]
hs = sensor[:, 1]
ha = sensor[:, 2]

tmission = mission[:, 0]
mission_states = mission[:, 1]

plt.subplot(3, 1, 1)
plt.plot(tsensor, hs, 'x', color='k', label="Sensor Readings")
plt.plot(tstate, h, color='b', lw=3, label="State Estimate")
if sys.argv[1][3:5] == "11":
    plt.xlim((280, 380))
elif sys.argv[1][3:5] == "07":
    plt.xlim((360, 430))
plt.ylim((-100, 1000))
for t, s in zip(tmission, mission_states):
    plt.axvline(t)
    ydisp = plt.gca().transAxes.transform((0, 1 - s/10.0))[1]
    ydata = plt.gca().transData.inverted().transform((0, ydisp))[1]
    plt.text(t, ydata, mission_states_map[int(s)])
plt.ylabel("Altitude (m)")
plt.legend()
plt.grid()

plt.subplot(3, 1, 2)
plt.plot(tstate, v, color='g', label="State Estimate")
for t, s in zip(tmission, mission_states):
    plt.axvline(t)
if sys.argv[1][3:5] == "11":
    plt.xlim((280, 380))
elif sys.argv[1][3:5] == "07":
    plt.xlim((360, 430))
for t, s in zip(tmission, mission_states):
    plt.axvline(t)
    ydisp = plt.gca().transAxes.transform((0, 1 - s/10.0))[1]
    ydata = plt.gca().transData.inverted().transform((0, ydisp))[1]
    plt.text(t, ydata, mission_states_map[int(s)])
plt.ylabel("Velocity (m/s)")
plt.legend()
plt.grid()

plt.subplot(3, 1, 3)
plt.plot(tsensor, ha, 'x', color='k', label="Sensor Readings")
plt.plot(tstate, a, color='r', lw=3, label="State Estimate")
for t, s in zip(tmission, mission_states):
    plt.axvline(t)
plt.ylabel("Acceleration (m/s/s)")
if sys.argv[1][3:5] == "11":
    plt.xlim((280, 380))
elif sys.argv[1][3:5] == "07":
    plt.xlim((360, 430))
for t, s in zip(tmission, mission_states):
    plt.axvline(t)
    ydisp = plt.gca().transAxes.transform((0, 1 - s/10.0))[1]
    ydata = plt.gca().transData.inverted().transform((0, ydisp))[1]
    plt.text(t, ydata, mission_states_map[int(s)])
plt.xlabel("Time (s)")
plt.legend()
plt.grid()

plt.show()
