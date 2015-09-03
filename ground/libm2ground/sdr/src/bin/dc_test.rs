extern crate time;
extern crate sdr;

use sdr::downconverter::Downconverter;

fn main() {
    let n_samples = 10e6 as usize;
    let n_repeats = 5u64;
    let mut dc = Downconverter::new(2);
    let mut x: Vec<u16> = Vec::with_capacity(n_samples);
    for _ in 0..(n_samples/8) {
        x.push(2048+3); x.push(2048+7); x.push(2048+4); x.push(2048+6);
        x.push(2048-3); x.push(2048-7); x.push(2048-4); x.push(2048-6);
    }

    let t0 = time::precise_time_ns();
    for _ in 0..n_repeats {
        let y = dc.process(&x);
        println!("y[end]={}", y[y.len()-1]);
    }
    let t1 = time::precise_time_ns();
    println!("Took avg {:e} seconds per {:e} samples",
             ((t1-t0)/n_repeats) as f64 / 1e9, n_samples as f64);
}
