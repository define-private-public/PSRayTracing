#include <iostream>
#include <chrono>
#include <cmath>
#include <random>
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

double asin_cg_const(const double x)
{
    // Original Minimax coefficients
    constexpr double a0 = 1.5707288;
    constexpr double a1 = -0.2121144;
    constexpr double a2 = 0.0742610;
    constexpr double a3 = -0.0187293;

    // Strip sign
    const double abs_x = std::abs(x);

    // Evaluate polynomial using Horner's method (all const)
    const double p = ((a3 * abs_x + a2) * abs_x + a1) * abs_x + a0;

    // Apply sqrt term and pi/2 offset
    const double x_diff = std::sqrt(1.0 - abs_x);
    const double result = HalfPi - (x_diff * p);

    // Restore sign natively
    return std::copysign(result, x);
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
    double sum_cg_const = 0.0;
    chrono::duration<double, milli> total_duration_std(0);
    chrono::duration<double, milli> total_duration_cg(0);
    chrono::duration<double, milli> total_duration_cg_const(0);

    // Perform `n` runs of the benchmark, using a different RNG each time
    for (size_t run = 0; run < num_runs; run++)
    {
        RNG rng(rng_seed + to_string(run));

        for (size_t i = 0; i < num_samples; i++)
        {
            const double x = rng.num();

            // Benchmark std::asin()
            const auto start_std = chrono::steady_clock::now();
            const double as_std = std::asin(x);
            const auto end_std = chrono::steady_clock::now();

            // Benchmark asin_cg()
            const auto start_cg = chrono::steady_clock::now();
            const double as_cg = asin_cg(x);
            const auto end_cg = chrono::steady_clock::now();

            // Benchmark asin_cg_const()
            const auto start_cg_const = chrono::steady_clock::now();
            const double as_cg_const = asin_cg_const(x);
            const auto end_cg_const = chrono::steady_clock::now();

            // Accumulate
            sum_std += as_std;
            sum_cg += as_cg;
            sum_cg_const += as_cg_const;
            total_duration_std += (end_std - start_std);
            total_duration_cg += (end_cg - start_cg);
            total_duration_cg_const += (end_cg_const - start_cg_const);
        }

        cout << "." << flush;
    }

    cout << endl;

    // Print Results
    cout << "std::asin() time:      " << total_duration_std.count() << " ms" << endl;
    cout << "asin_cg() time:        " << total_duration_cg.count() << " ms" << endl;
    cout << "asin_cg_const() time:  " << total_duration_cg_const.count() << " ms" << endl;

    // Print the sums so the compiler is forced to execute the logic
    const double diff_cg = abs(sum_std - sum_cg);
    const double error_cg = diff_cg / sum_std;
    const double diff_cg_const = abs(sum_std - sum_cg_const);
    const double error_cg_const = diff_cg_const / sum_std;

    cout << "Verification sums:" << endl;
    cout << "  std::asin():      " << sum_std << endl;
    cout << "  asin_cg():        " << sum_cg << " (Err: " << (error_cg * 100.0) << " %)" << endl;
    cout << "  asin_cg_const():  " << sum_cg_const << " (Err: " << (error_cg_const * 100.0) << " %)" << endl;

    // Calculate performance ratio
    const double speedup_cg =  total_duration_std.count() / total_duration_cg.count();
    const double speedup_cg_const =  total_duration_std.count() / total_duration_cg_const.count();
    cout << "Speedup asin_cg:        " << speedup_cg << "x faster" << endl;
    cout << "Speedup asin_cg_const:  " << speedup_cg_const << "x faster" << endl << endl;

    return 0;
}
