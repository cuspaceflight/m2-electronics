from __future__ import print_function
import numpy as np

G = np.array((
    (1, 0, 0, 0, 0, 1, 1),
    (0, 1, 0, 0, 1, 0, 1),
    (0, 0, 1, 0, 1, 1, 0),
    (0, 0, 0, 1, 1, 1, 1))).T

H = np.array((
    (0, 0, 0, 1, 1, 1, 1),
    (0, 1, 1, 0, 0, 1, 1),
    (1, 0, 1, 0, 1, 0, 1)))

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
    byte_a = list(int(d) for d in byte_b)
    syndrome_b = np.dot(H, byte_a) % 2
    syndrome = np.packbits(syndrome_b)[0] >> 5
    if syndrome:
        byte ^= (1 << (syndrome - 1))
    datanibble = byte >> 3
    datanibbles.append(datanibble)
print("static const uint8_t hamming_7_4_datanibbles[128] = {")
print(", ".join(list("0x{:02x}".format(nibble) for nibble in datanibbles)))
print("};")
