#pragma once 

#include "../src/bench_data.h"
#include <chrono>
#include <string>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <stack>
#include <sstream>

namespace bench {

    class BasicBenchmarker {
        using TaskId = int;
        using BenchEvents = std::map<TaskId, BenchData>;
        BasicBenchmarker() = default;
    public:
        static BasicBenchmarker& GetInstance();

        template <typename T>
        void StartBenchmark(T&& name)
        {
            const auto taskId = GiveId(); 
            auto begin = std::chrono::steady_clock::now();
            bench_tasks_data_[taskId] = BenchData{std::forward<T>(name), begin};
            bench_tasks_stack_.push(taskId);
        }

        void StartBenchmark();
        void EndBenchmark();
    public:

        /// @brief dumps all benchmark events
        /// @return a string of formatted benchark event entries
        template <typename Ratio>
        std::string Dump(Ratio ratio) const
        {
            std::ostringstream os; 
            for (const auto& elem : bench_tasks_data_)
            {
                os << '#' << elem.first << ": " << elem.second.bench_name << '\n'
                    << "Duration: " << elem.second.GetEllapsedTimeAs(ratio);
            }
            return os.str();
        }

    private:
        TaskId GiveId();

        int task_ctr_ = 0;
        std::stack<TaskId> bench_tasks_stack_;
        BenchEvents bench_tasks_data_;
    };

    class FuncBenchmarker {
        using BenchEvents = std::list<BenchData>;
        FuncBenchmarker() = default;
    public:
        static FuncBenchmarker& GetInstance();
        template <typename Lambda>
        void MeasureFunction(Lambda&& lambda, const std::string& lambda_name)
        {
            auto lmbd = std::forward<decltype(lambda)>(lambda);

            auto begin = std::chrono::steady_clock::now();
            lmbd();
            auto end = std::chrono::steady_clock::now();

            bench_funcs_data_.emplace_back(lambda_name, begin, end);
        }

        template <typename Ratio>
        std::string Dump(Ratio ratio) const
        {
            std::ostringstream os; 
            for (const auto& elem : bench_funcs_data_)
            {
                int ctr = 1;
                os << '#' << ctr << ": " <<  elem.bench_name << '\n'
                    << "Duration: " << elem.GetEllapsedTimeAs(ratio);
            }
            return os.str();
        }

    private:
        BenchEvents bench_funcs_data_;
    };

} //namespace bench

#define BENCH_BEGIN(name) (bench::BasicBenchmarker::GetInstance().StartBenchmark((name)))
#define BENCH_END (bench::BasicBenchmarker::GetInstance().EndBenchmark()) 
#define BENCH_FUNC(FunctionObj) (bench::FuncBenchmarker::GetInstance().MeasureFunction(FunctionObj, #FunctionObj))