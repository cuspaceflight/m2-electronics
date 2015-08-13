from __future__ import print_function
import numpy as np

G = np.array((
    (1, 0, 0, 0),
    (0, 1, 0, 0),
    (0, 0, 1, 0),
    (0, 0, 0, 1),
    (1, 1, 1, 0),
    (0, 1, 1, 1),
    (1, 0, 1, 1)))

H = np.array((
    (1, 1, 1, 0, 1, 0, 0),
    (0, 1, 1, 1, 0, 1, 0),
    (1, 0, 1, 1, 0, 0, 1)))

bitflips = (None, 7, 6, 4, 5, 1, 2, 3)

codewords = []
for nibble in range(16):
    nibble_b = np.binary_repr(nibble, width=4)
    nibble_a = list(int(d) for d in nibble_b)
    codeword_b = np.dot(G, nibble_a) % 2
    codeword = np.packbits(codeword_b)[0] >> 1
    codewords.append(codeword)
print("static const uint8_t hamming_7_4_codewords[16] = {")
print(", ".join(list("0x{:02x}".format(codeword) for codeword in codewords)))
print("};")

print()

datanibbles = []
for byte in range(128):
    byte_b = np.binary_repr(byte, width=7)
    byte_a = np.array(list(int(d) for d in byte_b))
    syndrome_b = np.dot(H, byte_a) % 2
    syndrome = np.packbits(syndrome_b)[0] >> 5
    if syndrome:
        byte ^= (1 << (7 - bitflips[syndrome]))
    datanibble = byte >> 3
    datanibbles.append(datanibble)
    note = "*"*30 if syndrome == 0 else ""
    if None:
        print("{}: y={:02} z={}={} x={} {}"
              .format(byte_b, datanibble, syndrome_b, syndrome, datanibble,
                      note))
print("static const uint8_t hamming_7_4_datanibbles[128] = {")
print(", ".join(list("0x{:01x}".format(nibble) for nibble in datanibbles)))
print("};")
