#include "benchmark.h"

using namespace bench;

bench::BenchData::BenchData(std::string&& name, time_t b)
: bench_name(std::move(name)),
      begin(b),
      end({})
{
}

bench::BenchData::BenchData(std::string &&name, time_t b, time_t e)
    : bench_name(std::move(name)),
      begin(b),
      end(e)
{
}

void bench::BenchData::SetEnd(time_t e)
{
    end = e;
}

//BasicBenchmarker
//TODO count overhead and substract
void bench::BasicBenchmarker::StartBenchmark(std::string&& name)
{
    auto begin = std::chrono::steady_clock::now();

    const auto taskId = GiveId(); 
    bench_tasks_data_[taskId] = BenchData{std::move(name), begin};
    bench_tasks_stack_.push(taskId);
}

void bench::BasicBenchmarker::StartBenchmark()
{
    auto begin = std::chrono::steady_clock::now();

    const auto taskId = GiveId(); 
    bench_tasks_data_[taskId] = BenchData{{}, begin};
    bench_tasks_stack_.push(taskId);
}

void bench::BasicBenchmarker::EndBenchmark()
{
    auto end = std::chrono::steady_clock::now();
    const auto finishedTaskId = bench_tasks_stack_.top();
    bench_tasks_stack_.pop();
    bench_tasks_data_[finishedTaskId].SetEnd(end);
}

std::string bench::BasicBenchmarker::Dump() const
{
    return std::string();
}

BasicBenchmarker::TaskId bench::BasicBenchmarker::GiveId()
{
    return ++task_ctr_;
}
