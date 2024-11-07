#include "../include/minibench.h"

using namespace bench;

FuncBenchmarker &bench::FuncBenchmarker::GetInstance()
{
    static FuncBenchmarker bench;
    return bench;
}
void bench::FuncBenchmarker::clear()
{
    bench_funcs_data_.clear();
}

bench::ScopedBench::ScopedBench(std::string name)
: bench_data_(std::move(name), 
std::chrono::steady_clock::now())
{
}

bench::ScopedBench::~ScopedBench() 
{
    bench_data_.SetEnd(std::chrono::steady_clock::now());
    std::cout << Dump();
}

std::string bench::ScopedBench::Dump() const
{   
    using namespace std::chrono;
    std::ostringstream os; 
    os << bench_data_.message << ":\n"
        << "Duration, ns: " << bench_data_.GetEllapsedTimeAs<nanoseconds>() << '\n'
        << "Duration, ms: " << bench_data_.GetEllapsedTimeAs<milliseconds>() << '\n'
        << "Duration, sec: " << bench_data_.GetEllapsedTimeAs<seconds>() << '\n';
    return os.str();
}