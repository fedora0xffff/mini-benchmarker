#include "../include/benchmark.h"

using namespace bench;

bench::BenchData::BenchData(std::string name, Time b, Time e)
    : bench_name(std::move(name)),
      begin(b),
      end(e)
{
}

bench::BenchData::BenchData(const BenchData &other)
: bench_name(other.bench_name),
begin(other.begin),
end(other.end)
{
}

BenchData& bench::BenchData::operator=(const BenchData &other)
{
    bench_name = other.bench_name;
    begin = other.begin;
    end = other.end;

    return *this;
}

BenchData& bench::BenchData::operator=(BenchData &&other)
{
    if (this != &other)
    {
        bench_name.swap(other.bench_name);
        begin = std::move(other.begin);
        end = std::move(other.end);
    }
    return *this;
}

bench::BenchData::BenchData(BenchData &&other)
{
    if (this != &other)
    {
        bench_name.swap(other.bench_name);
        begin = std::move(other.begin);
        end = std::move(other.end);
    }
}

void bench::BenchData::SetEnd(Time e)
{
    end = e;
}

