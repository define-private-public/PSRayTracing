// This is mean to be a minimal benchmark to show how a rejection sampling algorithm
// can be more performant than an analytical solution; in some cases.

#include <algorithm>
#include <chrono>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>
using namespace std;
using namespace chrono;

constexpr double pi = 3.141592653589793;
constexpr double two_pi = 2 * pi;


struct Vec2
{
  double x = 0;
  double y = 0;

  explicit constexpr Vec2(const double x_, const double y_) :
    x(x_),
    y(y_)
  { }

  inline double length_squared() const
  {
    return (x * x) + (y * y);
  }
};


struct Vec3
{
  double x = 0;
  double y = 0;
  double z = 0;

  explicit constexpr Vec3(const double x_, const double y_, const double z_) :
    x(x_),
    y(y_),
    z(z_)
  { }

  inline double length_squared() const
  {
    return (x * x) + (y * y) + (z * z);
  }
};


// Simple number and Vec3 "vending machine"
class RNG
{
  mt19937 _rng;
  uniform_real_distribution<double> _distributor;

public:
  explicit RNG(const int seed) :
    _rng(seed),
    _distributor(0, 1)
  { }

  double num(const double a, const double b)
  {
    const double n = _distributor(_rng);  // [0, 1]
    return (a + ((b - a) * n));
  }

  /*== Looping (greedy) methods ==*/

  Vec2 rejection_in_unit_disk()
  {
    while (true)
    {
      const Vec2 v(num(-1, 1), num(-1, 1));
      if (v.length_squared() < 1)
        return v;
    }
  }

  Vec3 rejection_in_unit_sphere()
  {
    while (true)
    {
      const Vec3 v(num(-1, 1), num(-1, 1), num(-1, 1));
      if (v.length_squared() < 1)
        return v;
    }
  }

  /*== Analytical (mathematically beautiful) methods  */

  Vec2 analytical_in_unit_disk()
  {
    const double r = sqrt(num(0, 1));
    const double theta = num(0, two_pi);
    const double x = r * cos(theta);
    const double y = r * sin(theta);

    return Vec2(x, y);
  }

  // Adapted from: https://karthikkaranth.me/blog/generating-random-points-in-a-sphere/
  Vec3 analytical_in_unit_sphere()
  {
    const double r = cbrt(num(0, 1));
    const double theta = num(0, two_pi);
    const double phi = acos(num(-1, 1));

    const double sin_theta = sin(theta);
    const double cos_theta = cos(theta);
    const double sin_phi = sin(phi);
    const double cos_phi = cos(phi);

    const double x = r * sin_phi * cos_theta;
    const double y = r * sin_phi * sin_theta;
    const double z = r * cos_phi;

    return Vec3(x, y, z);
  }
};


int64_t compute_mean(const vector<int64_t> &vec)
{
  const int64_t sum = accumulate(vec.begin(), vec.end(), 0);
  return sum / vec.size();
}


// Note: this isn't a proper/good median computation, just something quick for our purposes
int64_t compute_bad_median(vector<int64_t> vec)
{
  sort(vec.begin(), vec.end());
  return vec[vec.size() / 2];
}


void run_benchmark(const int rng_seed, const int number_of_runs, const int num_points_to_generate)
{
  vector<int64_t> rejection_2d_times;
  vector<int64_t> analytical_2d_times;
  vector<int64_t> rejection_3d_times;
  vector<int64_t> analytical_3d_times;

  // Store the points we want into these buckets
  vector<Vec2> vec2_bucket;
  vector<Vec3> vec3_bucket;
  vec2_bucket.reserve(num_points_to_generate);
  vec3_bucket.reserve(num_points_to_generate);

  cout << "run_number: rejection_2d_ms, analytical_2d_ms, rejection_3d_ms, analytical_3d_ms" << endl;

  for (int i = 0; i < number_of_runs; i++)
  {
    // Each run has a unique seed
    // Testing each function we'll start with the same RNG
    //   (duplicate code warning...)
    const int seed = rng_seed + 1;

    // Rejection 2D
    RNG rng_1(seed);
    const auto r2d_start = system_clock::now();
    int n = 0;
    while (n < num_points_to_generate)
    {
      vec2_bucket[n++] = rng_1.rejection_in_unit_disk();
    }
    const auto r2d_end = system_clock::now();
    const auto r2d_time_ms = duration_cast<milliseconds>(r2d_end - r2d_start).count();

    // Analytical 2D
    RNG rng_2(seed);
    const auto a2d_start = system_clock::now();
    n = 0;
    while (n < num_points_to_generate)
    {
      vec2_bucket[n++] = rng_2.analytical_in_unit_disk();
    }
    const auto a2d_end = system_clock::now();
    const auto a2d_time_ms = duration_cast<milliseconds>(a2d_end - a2d_start).count();

    // Rejection 3d
    RNG rng_3(seed);
    const auto r3d_start = system_clock::now();
    n = 0;
    while (n < num_points_to_generate)
    {
      vec3_bucket[n++] = rng_3.rejection_in_unit_sphere();
    }
    const auto r3d_end = system_clock::now();
    const auto r3d_time_ms = duration_cast<milliseconds>(r3d_end - r3d_start).count();

    // Analytical 3d
    RNG rng_4(seed);
    const auto a3d_start = system_clock::now();
    n = 0;
    while (n < num_points_to_generate)
    {
      vec3_bucket[n++] = rng_4.analytical_in_unit_sphere();
    }
    const auto a3d_end = system_clock::now();
    const auto a3d_time_ms = duration_cast<milliseconds>(a3d_end - a3d_start).count();

    cout << (i + 1) << ": "
         << r2d_time_ms << ", "
         << a2d_time_ms << ", "
         << r3d_time_ms << ", "
         << a3d_time_ms << ", " << endl;

    rejection_2d_times.push_back(r2d_time_ms);
    analytical_2d_times.push_back(a2d_time_ms);
    rejection_3d_times.push_back(r3d_time_ms);
    analytical_3d_times.push_back(a3d_time_ms);
  }

  cout << endl;

  // Do some final data stuff

  cout << "mean: "
       << compute_mean(rejection_2d_times) << ", "
       << compute_mean(analytical_2d_times) << ", "
       << compute_mean(rejection_3d_times) << ", "
       << compute_mean(analytical_3d_times) << endl;
  cout << "median: "
       << compute_bad_median(rejection_2d_times) << ", "
       << compute_bad_median(analytical_2d_times) << ", "
       << compute_bad_median(rejection_3d_times) << ", "
       << compute_bad_median(analytical_3d_times) << endl;
}




int main(int argc, char *argv[])
{
  const int rng_seed = stoi(argv[1]);     // e.g. 1337
  const int num_times = stoi(argv[2]);    // e.g. 500
  const int num_points = stoi(argv[3]);   // e.g. 1000000

  cout << "Testing with " << num_points << " points, " << num_times << " times..." << endl;
  run_benchmark(rng_seed, num_times, num_points);

  cout << "(all times measured are in milliseconds)" << endl;

  return 0;
}


