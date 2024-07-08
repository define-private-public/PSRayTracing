// This is a simple (and somewhat poor) benchmarking test to
// illustrate that `noexcept` can improve performance (in some cases)
//
// You'll need C++17/C++20 to compile, use -O3 (or Release for MSVC)
//
// You'll have to supply three arugments: <seed_number> <list_size> <iterations>
// E.g.:  1337 9999999 500

#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <chrono>
using namespace std;


vector<int> generate_random_numbers(const int n, mt19937 r_dev)
{
  vector<int> numbers;
  numbers.reserve(n);

  // Fill with [1, N]
  for (int i = 1; i <= n; i++)
  {
    numbers.push_back(i);
  }

  // Randomize
  shuffle(numbers.begin(), numbers.end(), r_dev);

  return numbers;
}


#define MAKE_FIND_FUNCTION(FUNCTION_NAME, NOEXCEPT)               \
int FUNCTION_NAME(const vector<int> &numbers, const int x) NOEXCEPT   \
{                                                                     \
  for (int i = 0; i < numbers.size(); i++)                            \
  {                                                                   \
    if (numbers[i] == x)                                              \
    {                                                                 \
      return i;                                                       \
    }                                                                 \
  }                                                                   \
  return -1;                                                          \
}
MAKE_FIND_FUNCTION(find_index_plain, )
MAKE_FIND_FUNCTION(find_index_noexcept, noexcept)


int64_t compute_mean(const vector<int64_t> &vec)
{
  const int64_t sum = accumulate(vec.begin(), vec.end(), 0);
  return sum / vec.size();
}

int64_t compute_bad_median(vector<int64_t> vec)
{
  // Note this isn't a proper/good median computation, just something quick for our purposes
  sort(vec.begin(), vec.end());
  return vec[vec.size() / 2];
}


struct BenchmarkResult
{
  int64_t average_time_plain_ns = 0;
  int64_t average_time_noexcept_ns = 0;
  int64_t median_time_plain_ns = 0;
  int64_t median_time_noexcept_ns = 0;
};


BenchmarkResult run_benchmark(const int rng_seed, const int number_of_runs, const int search_size)
{

  vector<int64_t> runtimes_plain;
  vector<int64_t> runtimes_noexcept;
  vector<int> bucket;     // Place to store the indicies that we found.

  for (int i = 0; i < number_of_runs; i++)
  {
    // Create the random object (and have a fresh RNG to work with each time)
    mt19937 rng(rng_seed + i);
    uniform_int_distribution<int> rng_distributor(1, search_size);

    // For each run genrate a new number set and something to find (and give it a chance to go out of bounds)
    const vector<int> numbers = generate_random_numbers(search_size, rng);
    const int n = rng_distributor(rng) + static_cast<int>(rng_distributor(rng) * 0.10);

    // First do the plain function
    auto start_time = chrono::system_clock::now();
    const int a = find_index_plain(numbers, n);
    auto end_time = chrono::system_clock::now();
    auto runtime_ns = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
    runtimes_plain.push_back(runtime_ns.count());

    // Second do the one with noexcept
    start_time = chrono::system_clock::now();
    const int b = find_index_noexcept(numbers, n);
    end_time = chrono::system_clock::now();
    runtime_ns = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time);
    runtimes_noexcept.push_back(runtime_ns.count());

    bucket.push_back(a);
    bucket.push_back(b);
  }

  BenchmarkResult result;
  result.average_time_plain_ns = compute_mean(runtimes_plain);
  result.average_time_noexcept_ns = compute_mean(runtimes_noexcept);
  result.median_time_plain_ns = compute_bad_median(runtimes_plain);
  result.median_time_noexcept_ns = compute_bad_median(runtimes_noexcept);
  return result;
}


int main(int argc, char *argv[])
{
  const int rng_seed = stoi(argv[1]);
  const int num_elements = stoi(argv[2]);
  const int num_times = stoi(argv[3]);

  cout << "Checking " << num_elements << " elements, " << num_times << " times..." << endl;
  const BenchmarkResult br = run_benchmark(rng_seed, num_times, num_elements);

  cout << "  plain average time:       " << br.average_time_plain_ns << " ns" << endl;
  cout << "  `noexcept` average time:  " << br.average_time_noexcept_ns << " ns" << endl;
  cout << endl;
  cout << "  plain median time:       " << br.median_time_plain_ns << " ns" << endl;
  cout << "  `noexcept` median time:  " << br.median_time_noexcept_ns << " ns" << endl;

  return 0;
}
