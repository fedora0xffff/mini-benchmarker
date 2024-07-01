#include "../include/benchmark.h"

using namespace bench;

bench::BenchData::BenchData(std::string &&name, Time b, Time e)
    : bench_name(std::move(name)),
      begin(b),
      end(e)
{
}

void bench::BenchData::SetEnd(Time e)
{
    end = e;
}

