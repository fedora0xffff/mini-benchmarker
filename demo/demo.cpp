#include "../include/benchmark.h"
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

using namespace bench;

namespace BenchOptions
{
    using namespace std::literals;
    constexpr auto serial = "serial"sv;
    constexpr auto single = "single"sv;
    constexpr auto function = "function"sv;
    constexpr auto functions = "functions"sv;
    constexpr auto all = "all"sv;
}

std::string GetHelp()
{
    std::ostringstream os;
    os << "Usage: demo <benchmark_type> <complexity>, \nwhere complexity is the number of elements to insert to a vector,\n"
        << "benchmark_type is a form of benchmarking: \n"
        << "- single - show a benchmark result of a single code section\n"
        << "- serial - show a benchmark result of several code sections that can overlap\n"
        << "- functions - show benchmark results of several functions\n"
        << "- all - demonstrate all forms";
    return os.str();
}

void ManyPushbacksNoReserve(int num_elements)
{
    std::vector<char> bytes;
    for (int i = 0; i < num_elements; ++i)
    {
        //NoOptimization(i);
        NO_OPT(i);
        bytes.push_back(i);
    }
    NO_OPT(bytes[bytes.size()-1]);
    //NoOptimization(bytes);
    std::cout << bytes.size() << std::endl;
}

void ManyPushbacksWithReserve(int num_elements)
{
    std::vector<char> bytes;
    bytes.reserve(num_elements);
    for (int i = 0; i < num_elements; ++i)
    {
        // using NoOptimization with @i for loop optimization ellision
        //NoOptimization(i);
        NO_OPT(i);
        bytes.push_back(i);
    }
    // same, using NoOptimization with @bytes to guarantee that vector is filled
    //NoOptimization(bytes);
    NO_OPT(bytes[bytes.size()-1]);
    std::cout << bytes.size() << std::endl;
}

void FunctionsBenchmarkDemo(int num_elements)
{
    auto lambda_one = [num_elements]() {
        ManyPushbacksNoReserve(num_elements);
    };

    auto lambda_two = [num_elements]() {
        ManyPushbacksWithReserve(num_elements);
    };

    BENCH_FUNC(lambda_one);
    BENCH_FUNC(lambda_two);

    std::cout << "Several functions:" << std::endl;
    BENCH_PRINT_FUNC_Nanosec;
}

void SingleCodeSectionDemo(int num_elements)
{
    BENCH_START;
    ManyPushbacksNoReserve(num_elements);
    BENCH_END;

    std::cout << "Single code section:" << std::endl;
    BENCH_PRINT_Nanosec;
}

void SeveralCodeSectionsDemo(int num_elements)
{
    
    BENCH_START_NAMED("section1: ManyPushbacksNoReserveDemo");
    ManyPushbacksNoReserve(num_elements);
    BENCH_END;

    BENCH_START_NAMED("section2: ManyPushbacksWithReserveDemo");
    ManyPushbacksWithReserve(num_elements);
    BENCH_END;

    std::cout << "Several code sections:" << std::endl;
    BENCH_PRINT_FUNC_Milisec;
}

int main(int argc, char** argv)
{
    int complexity = 10000;

    if (argc < 2)
    {
        std::cout << GetHelp() << std::endl;
    }
    else if (argc == 3)
    {
        complexity = std::stoi(argv[2]);
    }

    const auto option = argv[1];

    if (BenchOptions::all.compare(option) == 0)
    {
        SeveralCodeSectionsDemo(complexity);
        FunctionsBenchmarkDemo(complexity);

    }
    else if (BenchOptions::serial.compare(option) == 0)
    {
        SeveralCodeSectionsDemo(complexity);
    }
    else if (BenchOptions::single.compare(option) == 0)
    {
        SingleCodeSectionDemo(complexity);
    }
    else if (BenchOptions::functions.compare(option) == 0)
    {
        FunctionsBenchmarkDemo(complexity);
    }
    else 
    {
        std::cout << GetHelp() << std::endl;
    }

    return 0;
}