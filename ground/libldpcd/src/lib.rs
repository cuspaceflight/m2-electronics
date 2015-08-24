mod parity_check_matrix;
use parity_check_matrix::{N, K, is_codeword};

/// Threshold and pack `llrs` into a K-long binary message
fn pack_message(llrs: &[f64; N]) -> [u8; K/8] {
    let mut message = [0u8; K/8];
    for a in 0usize..N/2 {
        if llrs[a] <= 0.0 {
            message[a/8] |= 1 << (7 - (a % 8));
        }
    }
    message
}

#[test]
fn test_pack_message() {
    let x: [f64; N] = [
        -1.0,  1.0, -1.0,  1.0,  1.0, -1.0, -1.0, -1.0,  1.0,  1.0,  1.0, -1.0,
         1.0, -1.0,  1.0, -1.0,  1.0, -1.0, -1.0,  1.0,  1.0, -1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0,  1.0, -1.0,  1.0,  1.0,
         1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0, -1.0,  1.0,  1.0,
         1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0,  1.0,  1.0, -1.0, -1.0, -1.0,
        -1.0,  1.0,  1.0, -1.0, -1.0, -1.0,  1.0,  1.0,  1.0, -1.0,  1.0, -1.0,
         1.0, -1.0,  1.0,  1.0,  1.0, -1.0, -1.0, -1.0,  1.0, -1.0,  1.0,  1.0,
         1.0, -1.0, -1.0, -1.0, -1.0,  1.0, -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0,  1.0,  1.0,  1.0,  1.0,  1.0,  1.0, -1.0, -1.0, -1.0,
        -1.0, -1.0, -1.0, -1.0,  1.0, -1.0,  1.0,  1.0, -1.0,  1.0,  1.0,  1.0,
        -1.0, -1.0, -1.0, -1.0,  1.0, -1.0, -1.0,  1.0,  1.0,  1.0,  1.0, -1.0,
        -1.0, -1.0, -1.0,  1.0,  1.0,  1.0, -1.0,  1.0,  1.0,  1.0,  1.0, -1.0,
         1.0, -1.0,  1.0, -1.0, -1.0,  1.0, -1.0, -1.0, -1.0,  1.0, -1.0,  1.0,
        -1.0,  1.0, -1.0,  1.0, -1.0, -1.0,  1.0, -1.0,  1.0,  1.0, -1.0,  1.0,
        -1.0,  1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0,  1.0, -1.0,  1.0,  1.0,
         1.0,  1.0, -1.0, -1.0, -1.0, -1.0,  1.0, -1.0, -1.0, -1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,  1.0, -1.0, -1.0, -1.0,  1.0,  1.0, -1.0,  1.0, -1.0,
         1.0, -1.0,  1.0,  1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0,  1.0,
         1.0,  1.0,  1.0, -1.0, -1.0,  1.0,  1.0,  1.0, -1.0, -1.0, -1.0,  1.0,
         1.0, -1.0, -1.0,  1.0,  1.0, -1.0,  1.0,  1.0, -1.0,  1.0, -1.0,  1.0,
         1.0,  1.0,  1.0, -1.0, -1.0,  1.0, -1.0,  1.0,  1.0, -1.0, -1.0,  1.0,
        -1.0, -1.0, -1.0, -1.0
    ];
    let y = pack_message(&x);
    assert!(y == [
        0xA7, 0x15, 0x66, 0x01, 0x40, 0x0C, 0x02, 0x79, 0xC5, 0x47, 0x47, 0xA0,
        0xA0, 0x7F, 0x48, 0xF6]);

}

/// Decode from N `llrs` into a packed K-long message
///
/// Each llr is a log likelihood ratio with positive values more likely to be 0.
///
/// Returns the corresponding message bytes if decoding was possible, or None.
pub fn decode(llrs: &[f64; N]) -> Option<[u8; K/8]> {
    const MAX_ITERS: u32 = 100;
    let h = parity_check_matrix::h();
    let (conns_chk, conns_var) = parity_check_matrix::connections(&h);
    let mut r = [0.0f64; N];
    let mut u = [[0.0f64; K]; N];
    let mut v = [[0.0f64; N]; K];

    // Initialise r_a, v_(a->i) = llrs[a]
    for a in 0..N {
        r[a] = llrs[a];
        for v_ai in v[a].iter_mut() {
            *v_ai = llrs[a];
        }
    }

    for _ in 0..MAX_ITERS {
        // Check if we have a codeword yet
        if is_codeword(&r, &h) {
            return Some(pack_message(&r));
        }

        // Compute check node to variable node messages
        // u_(i->a) = 2 atanh(product_[b!=a](tanh( v_(b->i) / 2)))
        for i in 0..K {
            for &a in &conns_chk[i] {
                u[i][a] = 1.0;
                for &b in &conns_chk[i] {
                    if b != a {
                        u[i][a] *= (v[b][i] / 2.0).tanh();
                    }
                }
                u[i][a] = 2.0 * u[i][a].atanh();
            }
        }

        for a in 0..N {
            // Compute variable nodes to check nodes
            // v_(a->i) = llrs[a] + sum_[j!=i](u_(j->a))
            for &i in &conns_var[a] {
                v[a][i] = llrs[a];
                for &j in &conns_var[a] {
                    if j != i {
                        v[a][i] += u[j][a];
                    }
                }
            }

            // Compute current marginals
            r[a] = llrs[a];
            for i in 0..K {
                r[a] += u[i][a];
            }
        }
    }

    None
}

#[no_mangle]
pub extern fn ldpc_decode(llrs: &[f64; N], out: &mut[u8; K/8]) -> i32 {
    match decode(&llrs) {
        Some(result) => { *out = result; return 0; },
        None => { return 1; }
    }
}
