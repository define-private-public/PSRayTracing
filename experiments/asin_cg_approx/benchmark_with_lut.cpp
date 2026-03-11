#include <iostream>
#include <chrono>
#include <cmath>
#include <random>
#include <algorithm>
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

double asin_cg_const_fma(const double x)
{
    // Original Minimax coefficients
    constexpr double a0 = 1.5707288;
    constexpr double a1 = -0.2121144;
    constexpr double a2 = 0.0742610;
    constexpr double a3 = -0.0187293;

    // Strip sign
    const double abs_x = std::abs(x);

    // Evaluate polynomial using Horner's method with FMA
    const double p = std::fma(std::fma(std::fma(a3, abs_x, a2), abs_x, a1), abs_x, a0);

    // Apply sqrt term and pi/2 offset
    const double x_diff = std::sqrt(1.0 - abs_x);
    const double result = HalfPi - (x_diff * p);

    // Restore sign natively
    return std::copysign(result, x);
}

double asin_cg_biased_lut(const double x)
{
    constexpr int LUT_SIZE = 32;
    static double lut[LUT_SIZE];
    static bool initialized = false;

    if (!initialized)
    {
        for (int i = 0; i < LUT_SIZE; ++i)
        {
            const double t = static_cast<double>(i) / (LUT_SIZE - 1);
            // x = 1 - (1-t)^2
            const double x_val = 1.0 - (1.0 - t) * (1.0 - t);
            lut[i] = std::asin(x_val);
        }
        initialized = true;
    }

    const double x_abs = std::abs(x);
    const double x_clamped = std::min(x_abs, 1.0);

    // Inverse mapping: t = 1 - sqrt(1 - x_abs)
    const double t_mapped = 1.0 - std::sqrt(1.0 - x_clamped);
    
    // Interpolate in t-space [0, 1]
    const double f_index = t_mapped * (LUT_SIZE - 1);
    const int index = static_cast<int>(f_index);
    if (index >= LUT_SIZE - 1) return std::copysign(lut[LUT_SIZE - 1], x);
    
    const double fraction = f_index - index;
    const double y_abs = (1.0 - fraction) * lut[index] + fraction * lut[index + 1];

    return std::copysign(y_abs, x);
}

double asin_cg_hardcoded_lut(const double x)
{
    static constexpr int LUT_SIZE = 128;
    static constexpr double lut[LUT_SIZE] = {
        0.0, 0.007874097115293794, 0.015748682488653538, 0.02362424465069653, 0.03150127267756553, 0.03938025646474971, 0.047261687002179094, 0.0551460566510229, 0.06303385942262774, 0.07092559126003768, 0.07882175032254665, 0.08672283727374265, 0.09462935557351422, 0.10254181177450215, 0.11046071582349296, 0.11838658136826676, 0.1263199260704293, 0.13426127192477713, 0.14221114558576606, 0.1501700787016767, 0.15813860825709522, 0.16611727692435682, 0.17410663342462795, 0.1821072328993376, 0.19011963729270268, 0.19814441574613212, 0.2061821450053359, 0.214233409841011, 0.22229880348402553, 0.2303789280760768, 0.23847439513685514, 0.24658582604881113, 0.2547138525606886, 0.26285911731106315, 0.2710222743732029, 0.2792039898226572, 0.2874049423290726, 0.2956258237738371, 0.30386733989526565, 0.31213021096315985, 0.32041517248470786, 0.32872297594382993, 0.33705438957623546, 0.3454101991826218, 0.35379120898263544, 0.36219824251241356, 0.3706321435687482, 0.3790937772031541, 0.3875840307693892, 0.3961038150282615, 0.4046540653138798, 0.41323574276585046, 0.42184983563230877, 0.4304973606490957, 0.43917936450085704, 0.4478969253703547, 0.45665115458285194, 0.4654431983530571, 0.4742742396428133, 0.48314550013848867, 0.49205824235788476, 0.5010137718974325, 0.5100134398315113, 0.5190586452769144, 0.5281508381368129, 0.5372915220400554, 0.5464822574933111, 0.5557246652654373, 0.5650204300255633, 0.5743713042587614, 0.583779112485869, 0.593245755817067, 0.6027732168722795, 0.6123635651053904, 0.6220189625737509, 0.6317416701995707, 0.6415340545756484, 0.6513985953746197, 0.6613378934286569, 0.671354679555476, 0.6814518242168673, 0.6916323481079568, 0.7018994337893937, 0.7122564384909738, 0.7227069082343386, 0.7332545934448722, 0.7439034662494524, 0.7546577396881275, 0.7655218891051583, 0.776500676029453, 0.7875991749079128, 0.7988228031195971, 0.810177354776568, 0.8216690389120541, 0.8333045227724635, 0.8450909810722429, 0.8570361522467624, 0.8691484029576303, 0.8814368023794377, 0.8939112081432907, 0.9065823662526353, 0.9194620278506791, 0.9325630864470654, 0.9458997401609874, 0.959487684787998, 0.9733443451609219, 0.9874891545133722, 1.0019438946035708, 1.0167331135666144, 1.0318846443631011, 1.0474302550929808, 1.063406474619689, 1.0798556549471363, 1.096827358981345, 1.1143802044015552, 1.1325843613576951, 1.1515250117376858, 1.1713072651072176, 1.1920633591677634, 1.2139635935297486, 1.237233675241562, 1.2621837727980423, 1.2892606822812802, 1.3191505459728365, 1.353009101532212, 1.393091293808803, 1.4452226762591933, 1.5707963267948966
    };

    const double x_abs = std::abs(x);
    if (x_abs >= 1.0) return std::copysign(HalfPi, x);

    const double f_index = x_abs * (LUT_SIZE - 1);
    const int index = static_cast<int>(f_index);
    const double fraction = f_index - index;

    const double y_abs = (1.0 - fraction) * lut[index] + fraction * lut[index + 1];
    return std::copysign(y_abs, x);
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
    double sum_cg_const_fma = 0.0;
    double sum_cg_biased_lut = 0.0;
    double sum_cg_hardcoded_lut = 0.0;
    
    chrono::duration<double, milli> total_duration_std(0);
    chrono::duration<double, milli> total_duration_cg(0);
    chrono::duration<double, milli> total_duration_cg_const(0);
    chrono::duration<double, milli> total_duration_cg_const_fma(0);
    chrono::duration<double, milli> total_duration_cg_biased_lut(0);
    chrono::duration<double, milli> total_duration_cg_hardcoded_lut(0);

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

            // Benchmark asin_cg_const_fma()
            const auto start_cg_const_fma = chrono::steady_clock::now();
            const double as_cg_const_fma = asin_cg_const_fma(x);
            const auto end_cg_const_fma = chrono::steady_clock::now();

            // Benchmark asin_cg_biased_lut()
            const auto start_cg_biased_lut = chrono::steady_clock::now();
            const double as_cg_biased_lut = asin_cg_biased_lut(x);
            const auto end_cg_biased_lut = chrono::steady_clock::now();

            // Benchmark asin_cg_hardcoded_lut()
            const auto start_cg_hardcoded_lut = chrono::steady_clock::now();
            const double as_cg_hardcoded_lut = asin_cg_hardcoded_lut(x);
            const auto end_cg_hardcoded_lut = chrono::steady_clock::now();

            // Accumulate
            sum_std += as_std;
            sum_cg += as_cg;
            sum_cg_const += as_cg_const;
            sum_cg_const_fma += as_cg_const_fma;
            sum_cg_biased_lut += as_cg_biased_lut;
            sum_cg_hardcoded_lut += as_cg_hardcoded_lut;
            
            total_duration_std += (end_std - start_std);
            total_duration_cg += (end_cg - start_cg);
            total_duration_cg_const += (end_cg_const - start_cg_const);
            total_duration_cg_const_fma += (end_cg_const_fma - start_cg_const_fma);
            total_duration_cg_biased_lut += (end_cg_biased_lut - start_cg_biased_lut);
            total_duration_cg_hardcoded_lut += (end_cg_hardcoded_lut - start_cg_hardcoded_lut);
        }

        cout << "." << flush;
    }

    cout << endl;

    // Print Results
    cout << "std::asin() time:               " << total_duration_std.count() << " ms" << endl;
    cout << "asin_cg() time:                 " << total_duration_cg.count() << " ms" << endl;
    cout << "asin_cg_const() time:           " << total_duration_cg_const.count() << " ms" << endl;
    cout << "asin_cg_const_fma() time:       " << total_duration_cg_const_fma.count() << " ms" << endl;
    cout << "asin_cg_biased_lut() time:      " << total_duration_cg_biased_lut.count() << " ms" << endl;
    cout << "asin_cg_hardcoded_lut() time:   " << total_duration_cg_hardcoded_lut.count() << " ms" << endl;

    // Print the sums so the compiler is forced to execute the logic
    const double diff_cg = abs(sum_std - sum_cg);
    const double error_cg = diff_cg / sum_std;
    const double diff_cg_const = abs(sum_std - sum_cg_const);
    const double error_cg_const = diff_cg_const / sum_std;
    const double diff_cg_const_fma = abs(sum_std - sum_cg_const_fma);
    const double error_cg_const_fma = diff_cg_const_fma / sum_std;
    const double diff_cg_biased_lut = abs(sum_std - sum_cg_biased_lut);
    const double error_cg_biased_lut = diff_cg_biased_lut / sum_std;
    const double diff_cg_hardcoded_lut = abs(sum_std - sum_cg_hardcoded_lut);
    const double error_cg_hardcoded_lut = diff_cg_hardcoded_lut / sum_std;

    cout << "Verification sums:" << endl;
    cout << "  std::asin():               " << sum_std << endl;
    cout << "  asin_cg():                 " << sum_cg << " (Err: " << (error_cg * 100.0) << " %)" << endl;
    cout << "  asin_cg_const():           " << sum_cg_const << " (Err: " << (error_cg_const * 100.0) << " %)" << endl;
    cout << "  asin_cg_const_fma():       " << sum_cg_const_fma << " (Err: " << (error_cg_const_fma * 100.0) << " %)" << endl;
    cout << "  asin_cg_biased_lut():      " << sum_cg_biased_lut << " (Err: " << (error_cg_biased_lut * 100.0) << " %)" << endl;
    cout << "  asin_cg_hardcoded_lut():   " << sum_cg_hardcoded_lut << " (Err: " << (error_cg_hardcoded_lut * 100.0) << " %)" << endl;

    // Calculate performance ratio
    const double speedup_cg =  total_duration_std.count() / total_duration_cg.count();
    const double speedup_cg_const =  total_duration_std.count() / total_duration_cg_const.count();
    const double speedup_cg_const_fma =  total_duration_std.count() / total_duration_cg_const_fma.count();
    const double speedup_cg_biased_lut =  total_duration_std.count() / total_duration_cg_biased_lut.count();
    const double speedup_cg_hardcoded_lut =  total_duration_std.count() / total_duration_cg_hardcoded_lut.count();
    
    cout << "Speedup asin_cg:                 " << speedup_cg << "x faster" << endl;
    cout << "Speedup asin_cg_const:           " << speedup_cg_const << "x faster" << endl;
    cout << "Speedup asin_cg_const_fma:       " << speedup_cg_const_fma << "x faster" << endl;
    cout << "Speedup asin_cg_biased_lut:      " << speedup_cg_biased_lut << "x faster" << endl;
    cout << "Speedup asin_cg_hardcoded_lut:   " << speedup_cg_hardcoded_lut << "x faster" << endl << endl;

    return 0;
}
