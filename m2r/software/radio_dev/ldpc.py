# Generate C lookup tables for systematic LDPC (256, 128) code from
# CCSDS 231.1-O-1. We output 128 sets of four 32 bit integers, with each set
# representing the 128 bits of a column of the parity matrix W defined
# row-wise in Table 2-2, which forms the right half of the generator matrix.
# Since the target is little endian, we also reverse the byte order in the
# ints, so that the byte at the first/lowest memory address is the first 8
# bits of the parity block.

from __future__ import print_function, division
import numpy as np
np.set_printoptions(threshold='nan')

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

for row in range(1, 128+1):
    shift = (row % 32) - 1
    parent_row = (row - 1) // 32
    W[row - 1] = np.roll(Wh[parent_row], shift)

Wp = np.packbits(W.T, axis=1)

print("static const uint32_t ldpc_256_128_parity[128][4] = {")
for row in range(128):
    rowints = []
    for i in range(4):
        uint32 = (
            Wp[row][i*4 + 3] << 24 |
            Wp[row][i*4 + 2] << 16 |
            Wp[row][i*4 + 1] << 8 |
            Wp[row][i*4 + 0] << 0)
        rowints.append(uint32)
    print("    {", ", ".join("0x{:08X}".format(u32) for u32 in rowints), "},")
print("};")
