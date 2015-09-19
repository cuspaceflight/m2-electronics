import sys
import struct
import numpy as np
from collections import defaultdict


def main():
    if len(sys.argv) != 2:
        print("Usage: {} <logfile>".format(sys.argv[0]))
        return

    times = []
    channels = defaultdict(int)

    last_timestamp = 0
    t_correction = 0

    with open(sys.argv[1], "rb") as f:
        data = f.read()
    offset = 0
    n_packets = len(data) // 16
    while True:
        packet = data[offset*16:(offset+1)*16]
        if len(packet) != 16:
            print("Got non-16-byte packet, stopping.")
            break
        offset += 1
        if offset > n_packets:
            print("Finished reading file, stopping.")
            break
        if offset % 100000 == 0:
            print("\r{}/{} packets ({:.0f}%)"
                  .format(offset, n_packets, offset*100/n_packets), end='')
        timestamp = struct.unpack("i", packet[:4])[0]
        if timestamp < last_timestamp:
            t_correction += 0xFFFFFFFF
        last_timestamp = timestamp
        timestamp += t_correction
        mode, channel, _, _ = struct.unpack("BBBB", packet[4:8])
        times.append(timestamp)
        channels[channel] += 1

    print("Read {} packets".format(len(times)))

    times_s = np.array(times) / 168e6
    diffs = np.diff(times_s)
    print("Min dt={:.3f}ms, Max dt={:.3f}ms, "
          "Mean dt={:.3f}ms, Std. Dev={:.3f}ms"
          .format(diffs.min()*1000, diffs.max()*1000,
                  diffs.mean()*1000, diffs.std()*1000))

    print("Total timespan: {:.2f}s".format(times_s[-1] - times_s[0]))


if __name__ == "__main__":
    main()
