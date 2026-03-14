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
    explicit RNG(const string& seed_str)
    {
        seed_seq seed(seed_str.begin(), seed_str.end());
        rng_engine.seed(seed);
    }

    double num(const double a = -1.0, const double b = 1.0)
    {
        uniform_real_distribution<double> dist(a, b);
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

    // Evaluate polynomial using Horner's method (all const)
    const double p = ((a3 * abs_x + a2) * abs_x + a1) * abs_x + a0;

    // Apply sqrt term and pi/2 offset
    const double x_diff = sqrt(1.0 - abs_x);
    const double result = HalfPi - (x_diff * p);

    // Restore sign natively
    return copysign(result, x);
}

double asin_cg_fma(const double x)
{
    // Original Minimax coefficients
    constexpr double a0 = 1.5707288;
    constexpr double a1 = -0.2121144;
    constexpr double a2 = 0.0742610;
    constexpr double a3 = -0.0187293;

    // Strip sign
    const double abs_x = abs(x);

    // Evaluate polynomial using Horner's method with FMA
    const double p = fma(fma(fma(a3, abs_x, a2), abs_x, a1), abs_x, a0);

    // Apply sqrt term and pi/2 offset
    const double x_diff = sqrt(1.0 - abs_x);
    const double result = HalfPi - (x_diff * p);

    // Restore sign natively
    return copysign(result, x);
}

double asin_cg_estrin(const double x)
{
    constexpr double a0 = 1.5707288;
    constexpr double a1 = -0.2121144;
    constexpr double a2 = 0.0742610;
    constexpr double a3 = -0.0187293;

    const double abs_x = abs(x);
    const double x2 = abs_x * abs_x;

    // (a3*x + a2)*x2 + (a1*x + a0) - Parallelizable by CPU
    const double p = (a3 * abs_x + a2) * x2 + (a1 * abs_x + a0);

    const double x_diff = sqrt(1.0 - abs_x);
    const double result = HalfPi - (x_diff * p);

    return copysign(result, x);
}

double asin_cg_estrin_fma(const double x)
{
    constexpr double a0 = 1.5707288;
    constexpr double a1 = -0.2121144;
    constexpr double a2 = 0.0742610;
    constexpr double a3 = -0.0187293;

    const double abs_x = abs(x);
    const double x2 = abs_x * abs_x;

    // Estrin's scheme using FMA
    const double p = fma(fma(a3, abs_x, a2), x2, fma(a1, abs_x, a0));

    const double x_diff = sqrt(1.0 - abs_x);
    const double result = HalfPi - (x_diff * p);

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
    double sum_cg_fma = 0.0;
    double sum_cg_estrin = 0.0;
    double sum_cg_estrin_fma = 0.0;

    chrono::duration<double, milli> total_duration_std(0);
    chrono::duration<double, milli> total_duration_cg(0);
    chrono::duration<double, milli> total_duration_cg_fma(0);
    chrono::duration<double, milli> total_duration_cg_estrin(0);
    chrono::duration<double, milli> total_duration_cg_estrin_fma(0);

    // Perform `n` runs of the benchmark, using a different RNG each time
    for (size_t run = 0; run < num_runs; run++)
    {
        RNG rng(rng_seed + to_string(run));

        for (size_t i = 0; i < num_samples; i++)
        {
            const double x = rng.num();

            // Benchmark std::asin()
            const auto start_std = chrono::steady_clock::now();
            const double as_std = asin(x);
            const auto end_std = chrono::steady_clock::now();

            // Benchmark asin_cg()
            const auto start_cg = chrono::steady_clock::now();
            const double as_cg = asin_cg(x);
            const auto end_cg = chrono::steady_clock::now();

            // Benchmark asin_cg_fma()
            const auto start_cg_fma = chrono::steady_clock::now();
            const double as_cg_fma = asin_cg_fma(x);
            const auto end_cg_fma = chrono::steady_clock::now();

            // Benchmark asin_cg_estrin()
            const auto start_cg_estrin = chrono::steady_clock::now();
            const double as_cg_estrin = asin_cg_estrin(x);
            const auto end_cg_estrin = chrono::steady_clock::now();

            // Benchmark asin_cg_estrin_fma()
            const auto start_cg_estrin_fma = chrono::steady_clock::now();
            const double as_cg_estrin_fma = asin_cg_estrin_fma(x);
            const auto end_cg_estrin_fma = chrono::steady_clock::now();

            // Accumulate
            sum_std += as_std;
            sum_cg += as_cg;
            sum_cg_fma += as_cg_fma;
            sum_cg_estrin += as_cg_estrin;
            sum_cg_estrin_fma += as_cg_estrin_fma;

            total_duration_std += (end_std - start_std);
            total_duration_cg += (end_cg - start_cg);
            total_duration_cg_fma += (end_cg_fma - start_cg_fma);
            total_duration_cg_estrin += (end_cg_estrin - start_cg_estrin);
            total_duration_cg_estrin_fma += (end_cg_estrin_fma - start_cg_estrin_fma);
        }

        cout << "." << flush;
    }

    cout << endl;

    // Print Results
    cout << "std::asin() time:              " << total_duration_std.count() << " ms" << endl;
    cout << "asin_cg() time:                " << total_duration_cg.count() << " ms" << endl;
    cout << "asin_cg_fma() time:            " << total_duration_cg_fma.count() << " ms" << endl;
    cout << "asin_cg_estrin() time:         " << total_duration_cg_estrin.count() << " ms" << endl;
    cout << "asin_cg_estrin_fma() time:     " << total_duration_cg_estrin_fma.count() << " ms" << endl;

    // Print the sums so the compiler is forced to execute the logic
    const double diff_cg = abs(sum_std - sum_cg);
    const double error_cg = diff_cg / sum_std;
    const double diff_cg_fma = abs(sum_std - sum_cg_fma);
    const double error_cg_fma = diff_cg_fma / sum_std;
    const double diff_cg_estrin = abs(sum_std - sum_cg_estrin);
    const double error_cg_estrin = diff_cg_estrin / sum_std;
    const double diff_cg_estrin_fma = abs(sum_std - sum_cg_estrin_fma);
    const double error_cg_estrin_fma = diff_cg_estrin_fma / sum_std;

    cout << "Verification sums:" << endl;
    cout << "  std::asin():              " << sum_std << endl;
    cout << "  asin_cg():                " << sum_cg << " (Err: " << (error_cg * 100.0) << " %)" << endl;
    cout << "  asin_cg_fma():            " << sum_cg_fma << " (Err: " << (error_cg_fma * 100.0) << " %)" << endl;
    cout << "  asin_cg_estrin():         " << sum_cg_estrin << " (Err: " << (error_cg_estrin * 100.0) << " %)" << endl;
    cout << "  asin_cg_estrin_fma():     " << sum_cg_estrin_fma << " (Err: " << (error_cg_estrin_fma * 100.0) << " %)" << endl;

    // Calculate performance ratio
    const double speedup_cg =  total_duration_std.count() / total_duration_cg.count();
    const double speedup_cg_fma =  total_duration_std.count() / total_duration_cg_fma.count();
    const double speedup_cg_estrin =  total_duration_std.count() / total_duration_cg_estrin.count();
    const double speedup_cg_estrin_fma =  total_duration_std.count() / total_duration_cg_estrin_fma.count();

    cout << "Speedup asin_cg:                " << speedup_cg << "x faster" << endl;
    cout << "Speedup asin_cg_fma:            " << speedup_cg_fma << "x faster" << endl;
    cout << "Speedup asin_cg_estrin:         " << speedup_cg_estrin << "x faster" << endl;
    cout << "Speedup asin_cg_estrin_fma:     " << speedup_cg_estrin_fma << "x faster" << endl << endl;

    return 0;
}
