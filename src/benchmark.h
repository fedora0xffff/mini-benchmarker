#pragma once 
#include <chrono>
#include <string>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <stack>

namespace bench {

    /// @brief helps avoid compiler optimizations for a more precise benchmarking
    /// @tparam T may be anything that would make sense in this context
    /// @param t a param that must not be optimized, e.g., int a = 0;  NoOptimization(a);
    template <typename T>
    inline void NoOptimization(T& t) {
        asm volatile("" : "+r"(t));
    } 

    //TODO: check this one out
    inline void NoOptimizationEmptyAsm() {
        asm volatile("" :);
    } 

    /// @brief comprises a benchmark task data 
    struct BenchData {
        using time_t = std::chrono::time_point<std::chrono::steady_clock>;

        BenchData(std::string&& name, time_t begin_time);
        BenchData(std::string&& name, time_t begin_time, time_t end_time);
        void SetEnd(time_t end_time);
        std::string bench_name;
        time_t begin;
        time_t end;
    };

    class BasicBenchmarker {
        using TaskId = int;
        using BenchEvents = std::map<TaskId, BenchData>;
    public:
        BasicBenchmarker() = default;
        void StartBenchmark(std::string&& name);
        void StartBenchmark();
        void EndBenchmark();
    public:
        std::string Dump() const;
    private:
        TaskId GiveId();

        int task_ctr_ = 0;
        std::stack<TaskId> bench_tasks_stack_;
        BenchEvents bench_tasks_data_;
    };

    class FuncBenchmarker {
        using BenchEvents = std::list<BenchData>;
    public:
        FuncBenchmarker() = default;

        template <typename Lambda>
        void MeasureFunction(Lambda&& lambda, const std::string& lambda_name)
        {
            auto lmbd = std::forward<decltype(lambda)>(lambda);

            auto begin = std::chrono::steady_clock::now();
            lmbd();
            auto end = std::chrono::steady_clock::now();

            bench_funcs_data_.emplace_back(std::move(lambda_name), begin, end);
        }

        std::string Dump() const;

    private:
        BenchEvents bench_funcs_data_;
    };

    //TODO: define macros for ease
    //for func bench a macro must get lambdas name, if it is not provided? Can it?

} //namespace bench