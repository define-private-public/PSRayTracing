#!/bin/env python3
#
# Python equivalent of the `.cpp` file.
# # Note this program is structured a little differently

import math
import time
import random
import statistics

# Constants
pi = math.pi
half_pi = pi / 2
two_pi = 2 * pi


class Vec2:
    __slots__ = ('x', 'y')

    def __init__(self, x_, y_):
        self.x = x_
        self.y = y_

    def length_squared(self):
        return (self.x * self.x) + (self.y * self.y)


class Vec3:
    __slots__ = ('x', 'y', 'z')

    def __init__(self, x_, y_, z_):
        self.x = x_
        self.y = y_
        self.z = z_

    def length_squared(self):
        return (self.x * self.x) + (self.y * self.y) + (self.z * self.z)


def rejection_in_unit_disk():
    while True:
        x = random.uniform(-1, 1)
        y = random.uniform(-1, 1)
        v = Vec2(x, y)

        if (v.length_squared() < 1):
            return v


def analytical_in_unit_disk():
    r = math.sqrt(random.uniform(0, 1))
    theta = random.uniform(0, two_pi)
    x = r * math.cos(theta)
    y = r * math.sin(theta)

    return Vec2(x, y)


def rejection_in_unit_sphere():
    while True:
        x = random.uniform(-1, 1)
        y = random.uniform(-1, 1)
        z = random.uniform(-1, 1)
        v = Vec3(x, y, z)

        if (v.length_squared() < 1):
            return v


def analytical_in_unit_sphere():
    r = math.cbrt(random.uniform(0, 1))
    theta = random.uniform(0, two_pi)
    phi = math.acos(random.uniform(-1, 1))

    sin_theta = math.sin(theta)
    cos_theta = math.cos(theta)
    sin_phi = math.sin(phi)
    cos_phi = math.cos(phi)

    x = r * sin_phi * cos_theta
    y = r * sin_phi * sin_theta
    z = r * cos_phi

    return Vec3(x, y, z)


# Returns how many seconds it took
def measure(rng_seed, num_points, method):
    bucket = [None] * num_points  # preallocate space

    random.seed(rng_seed)

    start_time = time.time()
    for i in range(0, num_points):
        bucket[i] = method()
    end_time = time.time()

    return (end_time - start_time)


def main():
    rng_seed = 1337
    num_runs = 500
    num_points = 1000000

    r2d_times = []
    a2d_times = []
    r3d_times = []
    a3d_times = []

    for i in range(0, num_runs):
        seed = rng_seed + i

        r2d = measure(seed, num_points, rejection_in_unit_disk)
        a2d = measure(seed, num_points, analytical_in_unit_disk)
        r3d = measure(seed, num_points, rejection_in_unit_sphere)
        a3d = measure(seed, num_points, analytical_in_unit_sphere)

        r2d_times.append(r2d)
        a2d_times.append(a2d)
        r3d_times.append(r3d)
        a3d_times.append(a3d)

        print(f'Run {i} done...')

    r2d_mean_time = statistics.mean(r2d_times)
    r2d_median_time = statistics.median(r2d_times)
    a2d_mean_time = statistics.mean(a2d_times)
    a2d_median_time = statistics.median(a2d_times)
    r3d_mean_time = statistics.mean(r3d_times)
    r3d_median_time = statistics.median(r3d_times)
    a3d_mean_time = statistics.mean(a3d_times)
    a3d_median_time = statistics.median(a3d_times)

    # Print results
    print(f'Time it takes to generate {num_points} points ({num_runs} num runs)')
    print( '  Rejection 2D:')
    print(f'    Mean: {r2d_mean_time:.3f} s')
    print(f'    Median: {r2d_median_time:.3f} s')
    print( '  Analytical 2D:')
    print(f'    Mean: {a2d_mean_time:.3f} s')
    print(f'    Median: {a2d_median_time:.3f} s')
    print( '  Rejection 3D:')
    print(f'    Mean: {r3d_mean_time:.3f} s')
    print(f'    Median: {r3d_median_time:.3f} s')
    print( '  Analytical 3D:')
    print(f'    Mean: {a3d_mean_time:.3f} s')
    print(f'    Median: {a3d_median_time:.3f} s')


if __name__ == '__main__':
    main()
