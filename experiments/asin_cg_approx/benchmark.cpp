#include <iostream>
#include <chrono>
#include <cmath>
#include <random>
#include <vector>
#include "pcg_random.hpp"
using namespace std;

constexpr double Pi = 3.14159265358979323846;
constexpr double HalfPi = Pi / 2.0;

class RNG
{
private:
    pcg32 rng_engine;

public:
    explicit RNG(const std::string& seed_str)
    {
        std::seed_seq seed(seed_str.begin(), seed_str.end());
        rng_engine.seed(seed);
    }

    double num(const double a = -1.0, const double b = 1.0)
    {
        std::uniform_real_distribution<double> dist(a, b);
        return dist(rng_engine);
    }
};



double asin_cg(const double x)
{
    // Original Minimax coefficients
    constexpr double a0 = 1.5707288;
    constexpr double a1 = -0.2121144;
    constexpr double a2 = 0.0742610;
    constexpr double a3 = -0.0187293;

    // Strip sign
    const double abs_x = abs(x);

    // Evaluate polynomial using Horner's method
    double p = a3 * abs_x + a2;
    p = p * abs_x + a1;
    p = p * abs_x + a0;

    // Apply sqrt term and pi/2 offset
    const auto x_diff = static_cast<double>(sqrt(1.0 - abs_x));
    const double result = HalfPi - (x_diff * p);

    // Restore sign natively
    return copysign(result, x);
}


int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        cout << "Usage: " << endl;
        cout << argv[0] << " <rng_seed:string> <num_runs:int> <num_samples:size_t>" << endl;
        return 1;
    }

    const string rng_seed = argv[1];
    const int num_runs = stoi(argv[2]);
    const size_t num_samples = stoll(argv[3]);
    cout << "RNG Seed: " << rng_seed << endl;
    cout << "Number of Runs: " << num_runs << endl;
    cout << "Number of Samples: " << num_samples << endl;

    // Accumulators to prevent the compiler from optimizing the loops away
    double sum_std = 0.0;
    double sum_cg = 0.0;
    chrono::duration<double, milli> total_duration_std;
    chrono::duration<double, milli> total_duration_cg;

    // Perform `n` runs of the benchmark, using a different RNG each time
    for (size_t run = 0; run < num_runs; run++)
    {
        // Generate random values using the adapted RNG class
        RNG rng(rng_seed + to_string(run));
        vector<double> values(num_samples);
        for (size_t i = 0; i < num_samples; i++)
        {
            values[i] = rng.num();
        }

        // Benchmark std::asin()
        const auto start_std = chrono::steady_clock::now();
        for (size_t i = 0; i < num_samples; i++)
        {
            sum_std += asin(values[i]);
        }
        const auto end_std = chrono::steady_clock::now();
        total_duration_std += (end_std - start_std);

        // Benchmark asin_cg()
        const auto start_cg = chrono::steady_clock::now();
        for (size_t i = 0; i < num_samples; i++)
        {
            sum_cg += asin_cg(values[i]);
        }
        const auto end_cg = chrono::steady_clock::now();
        total_duration_cg += (end_cg - start_cg);

        cout << "." << flush;
    }
    cout << endl;

    // Print Results
    cout << "std::asin() time:  " << total_duration_std.count() << " ms" << endl;
    cout << "asin_cg() time:    " << total_duration_cg.count() << " ms" << endl;

    // Print the sums so the compiler is forced to execute the logic
    const double diff = abs(sum_std - sum_cg);
    const double error = diff / sum_std;
    cout << "Verification sums:" << endl;
    cout << "  std::asin():  " << sum_std << endl;
    cout << "  asin_cg():    " << sum_cg << endl;
    cout << "  Difference:   " << diff << endl;
    cout << "  Error:        " << (error * 100.0) << " %" << endl;

    // Calculate performance ratio
    const double speedup =  total_duration_std.count() / total_duration_cg.count();
    cout << "Speedup: " << speedup << "x faster" << endl << endl;

    return 0;
}
