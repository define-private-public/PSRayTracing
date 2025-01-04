use std::{env, vec};
use std::f64::consts::PI;
use std::time::Instant;
use rand::Rng;
use rand_mt::Mt;

const TWO_PI: f64 = 2.0 * PI;


struct Vec2 {
    x: f64,
    y: f64,
}

impl Vec2 {
    fn length_squared(&self) -> f64 {
        return (self.x * self.x) + (self.y * self.y);
    }
}


struct Vec3 {
    x: f64,
    y: f64,
    z: f64,
}

impl Vec3 {
    fn length_squared(&self) -> f64 {
        return (self.x * self.x) + (self.y * self.y) + (self.z * self.z);
    }
}


struct VecGenerator {
    rng: Mt     // 32 Bit Mersenne twister
}

impl VecGenerator {
    fn new(seed: i32) -> Self {
        return VecGenerator {
            rng: Mt::new(seed as u32)
        }
    }

    fn num(&mut self, a: f64, b: f64) -> f64 {
        return self.rng.gen_range(a..b);
    }

    fn rejection_in_unit_disk(&mut self) -> Vec2 {
        loop {
            let v = Vec2 {
                x: self.num(-1.0, 1.0),
                y: self.num(-1.0, 1.0),
            };

            if v.length_squared() < 1.0 {
                return v;
            }
        }
    }

    fn analytical_in_unit_disk(&mut self) -> Vec2 {
        let r = self.num(0.0, 1.0).sqrt();
        let theta = self.num(0.0, TWO_PI);
        let x = r * theta.cos();
        let y = r * theta.sin();

        return Vec2 {x: x, y: y};
    }

    fn rejection_in_unit_sphere(&mut self) -> Vec3 {
        loop {
            let v = Vec3 {
                x: self.num(-1.0, 1.0),
                y: self.num(-1.0, 1.0),
                z: self.num(-1.0, 1.0),
            };

            if v.length_squared() < 1.0 {
                return v;
            }
        }
    }

    fn analytical_in_unit_sphere(&mut self) -> Vec3 {
        let r = self.num(0.0, 1.0).cbrt();
        let theta = self.num(0.0, TWO_PI);
        let phi = self.num(-1.0, 1.0).acos();

        let sin_theta = theta.sin();
        let cos_theta = theta.cos();
        let sin_phi = phi.sin();
        let cos_phi = phi.cos();

        let x = r * sin_phi * cos_theta;
        let y = r * sin_phi * sin_theta;
        let z = r * cos_phi;

        return Vec3 {x: x, y: y, z: z};
    }
}


fn compute_mean(times: Vec<u128>) -> u128 {
    let sum: u128 = times.iter().sum();
    let div = times.len() as u128;
    return sum / div;
}

fn compute_bad_median(times: Vec<u128>) -> u128 {
    let mut times_sorted = times.clone();
    times_sorted.sort();
    return times_sorted[times_sorted.len() / 2];
}



fn run_benchmark(rng_seed: i32, num_runs: usize, num_points: usize) {
    let mut r2d_times: Vec<u128> = vec![];
    let mut a2d_times: Vec<u128> = vec![];
    let mut r3d_times: Vec<u128> = vec![];
    let mut a3d_times: Vec<u128> = vec![];

    // Store the points in buckets (despite never using them)
    let mut vec2_bucket: Vec<Vec2> = Vec::with_capacity(num_points);
    let mut vec3_bucket: Vec<Vec3> = Vec::with_capacity(num_points);


    println!("run number: rejection_2d_ms, analytical_2d_ms, rejection_3d_ms, analytical_3d_ms");

    for i in 0..num_runs {
        let seed = rng_seed + (i as i32);    // Each run has a unique seed,

        // Rejection 2D
        vec2_bucket.clear();
        let mut rng_r2d = VecGenerator::new(seed);
        let start_r2d = Instant::now();
        for _ in 0..num_points {
            let v = rng_r2d.rejection_in_unit_disk();
            vec2_bucket.push(v);
        }
        let end_r2d = start_r2d.elapsed();
        let r2d_time_ms = end_r2d.as_millis();


        // Analytical 2D
        vec2_bucket.clear();
        let mut rng_a2d = VecGenerator::new(seed);
        let start_a2d = Instant::now();
        for _ in 0..num_points {
            let v = rng_a2d.analytical_in_unit_disk();
            vec2_bucket.push(v);
        }
        let end_a2d = start_a2d.elapsed();
        let a2d_time_ms = end_a2d.as_millis();


        // Rejection 3D
        vec3_bucket.clear();
        let mut rng_r3d = VecGenerator::new(seed);
        let start_r3d = Instant::now();
        for _ in 0..num_points {
            let v = rng_r3d.rejection_in_unit_sphere();
            vec3_bucket.push(v);
        }
        let end_r3d = start_r3d.elapsed();
        let r3d_time_ms = end_r3d.as_millis();


        // Analytical 3D
        vec3_bucket.clear();
        let mut rng_a3d = VecGenerator::new(seed);
        let start_a3d = Instant::now();
        for _ in 0..num_points {
            let v = rng_a3d.analytical_in_unit_sphere();
            vec3_bucket.push(v);
        }
        let end_a3d = start_a3d.elapsed();
        let a3d_time_ms = end_a3d.as_millis();


        // Print results and store
        println!("{}: {}, {}, {}, {}", i, r2d_time_ms, a2d_time_ms, r3d_time_ms, a3d_time_ms);
        r2d_times.push(r2d_time_ms);
        a2d_times.push(a2d_time_ms);
        r3d_times.push(r3d_time_ms);
        a3d_times.push(a3d_time_ms);
    }

    print!("");

    // Do some final math stuff
    println!("mean: {}, {}, {}, {}",
        compute_mean(r2d_times.clone()),
        compute_mean(a2d_times.clone()),
        compute_mean(r3d_times.clone()),
        compute_mean(a3d_times.clone())
    );
    println!("median: {}, {}, {}, {}",
        compute_bad_median(r2d_times.clone()),
        compute_bad_median(a2d_times.clone()),
        compute_bad_median(r3d_times.clone()),
        compute_bad_median(a3d_times.clone())
);
}


fn main() {
    let args: Vec<String> = env::args().collect();

    // TODO have suggestions be the default we use for the tests
    let rng_seed: i32 = args[1].parse().expect("Need a valid seed number (e.g. 1337)");
    let num_runs: usize = args[2].parse().expect("Number of times to run the test (e.g. 500)");
    let num_points: usize = args[3].parse().expect("Number of points to test (e.g. 1000000)");

    println!("Testing with {} points, {} runs...    [rng_seed={}]", num_points, num_runs, rng_seed);
    run_benchmark(rng_seed, num_runs, num_points);
    println!("(all times are measured in milliseconds)");
}
