#include "../include/benchmark.h"

using namespace bench;

void bench::BenchData::SetEnd(Time e)
{
    end = e;
}

bench::BenchData::BenchData(std::string str, Time begin_time, Time end_time)
: message(std::move(str)),
begin(begin_time), 
end(end_time)
{
}


