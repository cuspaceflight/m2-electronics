extern crate time;
extern crate sdr;
use sdr::cic::CIC;

fn main() {
    let q = 5usize;
    let r = 16usize;
    let n_samples = 100e6 as usize;
    let n_repeats = 5u64;

    let mut cic = CIC::new(q, r);
    let mut x: Vec<i16> = Vec::with_capacity(n_samples);
    for _ in 0..(n_samples/8) {
        x.push(3); x.push(7); x.push(4); x.push(6);
        x.push(3); x.push(7); x.push(4); x.push(6);
    }
    let t0 = time::precise_time_ns();
    for _ in 0..n_repeats {
        let y = cic.process(&x);
        println!("y[end]={}", y[y.len()-1]);
    }
    let t1 = time::precise_time_ns();
    println!("Took avg {:e} seconds per {:e} samples",
             ((t1-t0)/n_repeats) as f64 / 1e9, n_samples as f64);
}
