# Generate C lookup tables for systematic LDPC (256, 128) code from
# CCSDS 231.1-O-1. We output 128 sets of four 32 bit integers, with each set
# representing the 128 bits of a column of the parity matrix W defined
# row-wise in Table 2-2, which forms the right half of the generator matrix.
# Since the target is little endian, we also reverse the byte order in the
# ints, so that the byte at the first/lowest memory address is the first 8
# bits of the parity block.

from __future__ import print_function, division
import numpy as np
np.set_printoptions(threshold='nan', linewidth='nan')

# Method one, generate W (the parity block of a systematic generator G)
# from the hex constants in the report.
Wh = np.unpackbits(np.array((
    (0x73, 0xF5, 0xE8, 0x39,
     0x02, 0x20, 0xCE, 0x51,
     0x36, 0xED, 0x68, 0xE9,
     0xF3, 0x9E, 0xB1, 0x62),
    (0xBA, 0xC8, 0x12, 0xC0,
     0xBC, 0xD2, 0x43, 0x79,
     0x47, 0x86, 0xD9, 0x28,
     0x5A, 0x09, 0x09, 0x5C),
    (0x7D, 0xF8, 0x3F, 0x76,
     0xA5, 0xFF, 0x4C, 0x38,
     0x8E, 0x6C, 0x0D, 0x4E,
     0x02, 0x5E, 0xB7, 0x12),
    (0xBA, 0xA3, 0x7B, 0x32,
     0x60, 0xCB, 0x31, 0xC5,
     0xD0, 0xF6, 0x6A, 0x31,
     0xFA, 0xF5, 0x11, 0xBC)), dtype=np.uint8), axis=1)
W = np.empty((128, 128), dtype=np.uint8)
M = 32

for row in range(128):
    shift = (row % 32)
    parent = row // 32
    # Each block gets rolled once per row
    for m in range(8):
        W[row][m*M:(m+1)*M] = np.roll(Wh[parent][m*M:(m+1)*M], shift, axis=0)

Wp = np.packbits(W.T, axis=1)

# Enable the prints here to output a column-oriented 2kB of the entire W
# section, allowing fast C encoding at the expensive of flash space.
# print("static const uint32_t ldpc_256_128_gen_p[128][4] = {")
for row in range(128):
    rowints = []
    for i in range(4):
        uint32 = (
            Wp[row][i*4 + 3] << 24 |
            Wp[row][i*4 + 2] << 16 |
            Wp[row][i*4 + 1] << 8 |
            Wp[row][i*4 + 0] << 0)
        rowints.append(uint32)
    # print("  {", ", ".join("0x{:08X}".format(u32) for u32 in rowints), "},")
# print("};")
# See ldpc_ecc.c at revision c4472ab596d21 for example code using this.


# We can try it out with an example codeword (also good to check against the C)
G = np.hstack((np.eye(128), W))
x = np.unpackbits(np.array((
    0xA7, 0x15, 0x66, 0x01, 0x40, 0x0C, 0x02, 0x79, 0xC5, 0x47, 0x47, 0xA0,
    0xA0, 0x7F, 0x48, 0xF6
), dtype=np.uint8))
cw1 = (np.dot(x, G) % 2).astype(np.uint8)
# print(" ".join("{:02X}".format(b) for b in np.packbits(cw1)))

# Method two, generate H (the full parity check matrix) from the definition in
# the report, then compute W and thus G from H.
M0 = 0 * M
M1 = 1 * M
M2 = 2 * M
M3 = 3 * M
M4 = 4 * M
M5 = 5 * M
M6 = 6 * M
M7 = 7 * M
M8 = 8 * M

IM = np.eye(M, dtype=np.uint8)
ZM = IM * 0
PM = lambda i: np.roll(IM, i % M, axis=1)
H = np.empty((128, 256), dtype=np.uint8)

H[M0:M1, M0:M1] = (IM + PM(31)) % 2
H[M1:M2, M0:M1] = PM(28)
H[M2:M3, M0:M1] = PM(8)
H[M3:M4, M0:M1] = PM(18)

H[M0:M1, M1:M2] = PM(15)
H[M1:M2, M1:M2] = (IM + PM(30)) % 2
H[M2:M3, M1:M2] = PM(0)
H[M3:M4, M1:M2] = PM(30)

H[M0:M1, M2:M3] = PM(25)
H[M1:M2, M2:M3] = PM(29)
H[M2:M3, M2:M3] = (IM + PM(28)) % 2
H[M3:M4, M2:M3] = PM(0)

H[M0:M1, M3:M4] = PM(0)
H[M1:M2, M3:M4] = PM(24)
H[M2:M3, M3:M4] = PM(1)
H[M3:M4, M3:M4] = (IM + PM(30)) % 2

H[M0:M1, M4:M5] = ZM
H[M1:M2, M4:M5] = IM
H[M2:M3, M4:M5] = PM(29)
H[M3:M4, M4:M5] = PM(25)

H[M0:M1, M5:M6] = PM(20)
H[M1:M2, M5:M6] = ZM
H[M2:M3, M5:M6] = IM
H[M3:M4, M5:M6] = PM(26)

H[M0:M1, M6:M7] = PM(12)
H[M1:M2, M6:M7] = PM(1)
H[M2:M3, M6:M7] = ZM
H[M3:M4, M6:M7] = IM

H[M0:M1, M7:M8] = IM
H[M1:M2, M7:M8] = PM(20)
H[M2:M3, M7:M8] = PM(21)
H[M3:M4, M7:M8] = ZM

P = H[:, M4:]
Q = H[:, :M4]
Pinv = np.linalg.inv(P)
Pinv *= np.linalg.det(P)
Pinv %= 2
Pinv = Pinv.round().astype(np.uint8)
assert np.all(np.dot(Pinv, P) % 2 == np.eye(128))
W2 = (np.dot(Pinv, Q)).T % 2
G2 = np.hstack((np.eye(128), W2))

assert np.all(W == W2)
assert np.all(G == G2)

cw2 = (np.dot(x, G2) % 2).astype(np.uint8)
assert np.all(cw1 == cw2)
