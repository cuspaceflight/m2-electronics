import time
import ctypes
import numpy as np
newclib = ctypes.cdll.LoadLibrary("./libnewcldpcd.so")
#rlib = ctypes.cdll.LoadLibrary("./libldpcd.so")
dbl_ptype = np.ctypeslib.ndpointer(dtype=np.double, ndim=1)
u8_ptype = np.ctypeslib.ndpointer(dtype=np.uint8, ndim=1)
newcdecode = newclib.ldpc_decode
newcdecode.argtypes = [dbl_ptype, u8_ptype]
#rdecode = rlib.ldpc_decode
#rdecode.argtypes = [dbl_ptype, u8_ptype]
newclib.ldpc_init()

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

repeats = 100
snr_db = 3.0
snr = 10.0**(snr_db/10.0)
sigma = np.sqrt(1.0 / snr)
#np.random.seed(1)

c_times = []
r_times = []
c_errors = []
r_errors = []
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
    r_llr = np.log(p_x0_y / p_x1_y)
    c_llr = r_llr[:]

    #r_result = np.zeros(16, np.uint8)
    #t0 = time.time()
    #rdecode(r_llr, r_result)
    #t1 = time.time()
    #r_times.append(t1 - t0)
    #if not np.all(r_result == packed):
        #r_errors.append(repeat)

    c_result = np.zeros(16, np.uint8)
    t0 = time.time()
    newcdecode(c_llr, c_result)
    t1 = time.time()
    c_times.append(t1 - t0)
    if not np.all(c_result == packed):
        c_errors.append(repeat)

    if np.all(c_result == 0xFF) or np.all(c_result == 0x00):
        print("Repeat", repeat)
        print("Packed:  ", packed)
        print("c_result:", c_result)

    #if repeat in set(c_errors) - set(r_errors):

        #print("Repeat", repeat)
        #print("Packed:  ", packed)
        #print("r_result:", r_result)
        #print("c_result:", c_result)
        #print("LLRs:")
        #print("{", ", ".join("{:.60f}".format(f) for f in c_llr), "}")

#print("Rust: SNR {:.1f}dB, PER {:.4e}, {} repeats, Avg Time {:.6f}s"
      #.format(10*np.log10(snr), len(r_errors)/repeats, repeats,
              #np.mean(r_times)))

print("NewC: SNR {:.1f}dB, PER {:.4e}, {} repeats, Avg Time {:.6f}s"
      .format(10*np.log10(snr), len(c_errors)/repeats, repeats,
              np.mean(c_times)))

print("Repeats that differed:", set(c_errors) - set(r_errors))
