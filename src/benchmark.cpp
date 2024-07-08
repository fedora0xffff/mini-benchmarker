#include "../include/benchmark.h"

using namespace bench;

//BasicBenchmarker
BasicBenchmarker& BasicBenchmarker::GetInstance()
{
    static BasicBenchmarker bench;
    return bench;
}

void BasicBenchmarker::StartBenchmark()
{
    const auto taskId = GiveId(); 
    auto begin = std::chrono::steady_clock::now();
    bench_tasks_data_[taskId] = BenchData{"", begin};
    std::cout << "Task id pushed: " << taskId << std::endl;

    bench_tasks_stack_.push(taskId);
}

void BasicBenchmarker::EndBenchmark()
{
    auto end = std::chrono::steady_clock::now();
    const auto finishedTaskId = bench_tasks_stack_.top();
    std::cout << "Task id popped: " << finishedTaskId << std::endl;
    bench_tasks_stack_.pop();
    bench_tasks_data_[finishedTaskId].SetEnd(end);
}

BasicBenchmarker::TaskId BasicBenchmarker::GiveId()
{
    return ++task_ctr_;
}

FuncBenchmarker &bench::FuncBenchmarker::GetInstance()
{
    static FuncBenchmarker bench;
    return bench;
}