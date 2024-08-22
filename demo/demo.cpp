#include "../include/benchmark.h"
#include <sstream>
#include <string>
#include <string_view>
#include <vector>


using namespace bench;

namespace BenchOptions
{
    using namespace std::literals;
    constexpr auto scoped = "scoped"sv;
    constexpr auto function = "function"sv;
}

std::string GetHelp()
{
    std::ostringstream os;
    os << "Usage: demo <benchmark_type> <complexity>, \nwhere complexity is the number\n of elements to insert to a vector,\n"
        << "benchmark_type is a form of benchmarking: \n"
        << "- scoped - show a benchmark result of a single code section\n"
        << "- function - show benchmark results of several functions\n";
    return os.str();
}

void ManyPushbacksNoReserve(int num_elements)
{
    std::vector<char> bytes;
    for (int i = 0; i < num_elements; ++i)
    {
        bytes.push_back(i);
        NoOptimization(bytes[i] = bytes[i]);
    }
    NoOptimization(bytes.size());
}

void ManyPushbacksWithReserve(int num_elements)
{
    std::vector<char> bytes;
    bytes.reserve(num_elements);
    for (int i = 0; i < num_elements; ++i)
    {
        bytes.push_back(i);
        NoOptimization(bytes[i] = bytes[i]);
    }
    NoOptimization(bytes.size());
}

void FunctionsBenchmarkDemo(int num_elements)
{
    auto lambda_one = [num_elements]() {
        ManyPushbacksNoReserve(num_elements);
    };

    auto lambda_two = [num_elements]() {
        ManyPushbacksWithReserve(num_elements);
    };

    MEASURE_FUNC(lambda_one, "ManyPushbacksNoReserve");
    MEASURE_FUNC(lambda_two, "ManyPushbacksWithReserve");

    PRINT_RES;
}

void ScopedBenchDemo(int num_elements)
{
   {
        LOG_DURATION("ManyPushbacksNoReserve");
        ManyPushbacksNoReserve(num_elements);
   }

   {
        LOG_DURATION("ManyPushbacksWithReserve");
        ManyPushbacksWithReserve(num_elements);
   }
}


int main(int argc, char** argv)
{
    int complexity = 10000;

    if (argc < 2)
    {
        std::cout << GetHelp() << std::endl;
        return 0;
    }
    else if (argc == 3)
    {
        complexity = std::stoi(argv[2]);
    }

    const auto option = argv[1];

    if (BenchOptions::scoped.compare(option) == 0)
    {
        ScopedBenchDemo(complexity);
    }
    else if (BenchOptions::function.compare(option) == 0)
    {
        FunctionsBenchmarkDemo(complexity);
    }
    else 
    {
        std::cout << GetHelp() << std::endl;
    }

    return 0;
}