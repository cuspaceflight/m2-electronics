import time
import ctypes
import numpy as np
rlib = ctypes.cdll.LoadLibrary("./target/release/libldpcd.so")
dbl_ptype = np.ctypeslib.ndpointer(dtype=np.double, ndim=1)
u8_ptype = np.ctypeslib.ndpointer(dtype=np.uint8, ndim=1)
rdecode = rlib.ldpc_decode
rdecode.argtypes = [dbl_ptype, u8_ptype]

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
    for m in range(8):
        W[row][m*M:(m+1)*M] = np.roll(Wh[parent][m*M:(m+1)*M], shift, axis=0)

Wp = np.packbits(W.T, axis=1)
G = np.hstack((np.eye(128), W))

snrs_db = [2.5, 3, 3.5, 4, 4.5, 5]

for snr_db in snrs_db:
    snr = 10.0**(snr_db/10.0)
    sigma = np.sqrt(1.0 / snr)
    times = []
    errors = 0
    repeats = 500
    for repeat in range(repeats):
        np.random.seed(repeat)
        packed = np.random.randint(0, 256, 16).astype(np.uint8)
        msg = np.unpackbits(packed)
        cw = np.dot(msg, G) % 2
        noise = np.random.randn(256) * sigma
        x = 2 * cw.astype(np.double) - 1
        y = x + noise
        p_y_xp = np.exp(-(y-1)**2 / (2*sigma**2))
        p_y_xm = np.exp(-(y+1)**2 / (2*sigma**2))
        p_x1_y = p_y_xp / (p_y_xp + p_y_xm)
        p_x0_y = 1 - p_x1_y
        llr = np.log(p_x0_y / p_x1_y)
        result = np.empty(16, np.uint8)
        t0 = time.time()
        rdecode(llr, result)
        t1 = time.time()
        times.append(t1 - t0)
        if not np.all(result == packed):
            errors += 1
    packets_per_sec = 1.0/np.mean(times)
    print("SNR {:.2f}dB, BER {:.4e}, {} Repeats, "
          "Avg Time {:.6f}s, {:.0f} packets/s"
          .format(snr_db, errors/float(repeats), repeats, np.mean(times),
                  packets_per_sec))
