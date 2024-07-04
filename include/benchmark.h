#pragma once 

#include <chrono>
#include <string>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <stack>
#include <sstream>
#include <ratio>

namespace bench {

    #define NO_OPT() (asm volatile("" :))

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
        using Time = std::chrono::time_point<std::chrono::steady_clock>;

        template <typename Ratio>
        using TimePeriod = const std::chrono::duration<double, Ratio>;
    public:
        BenchData(std::string name, Time begin_time, Time end_time = {});

        /// @brief sets the end time point
        /// @param end_time - the measured end time point
        /// @return measured duration between begin and end
        void SetEnd(Time end_time);
    public:

        template <typename T>
        typename TimePeriod<T>::rep 
        GetEllapsedTimeAs() const
        {
            return std::chrono::duration<double, T>(end - begin).count();
        }

    public:
        std::string bench_name;
        Time begin;
        Time end;
    };

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
            bench_tasks_data_[taskId] = {std::forward<T>(name), begin};
            bench_tasks_stack_.push(taskId);
        }

        void StartBenchmark();
        void EndBenchmark();
    public:

        /// @brief dumps all benchmark events
        /// @return a string of formatted benchark event entries
        template <typename Ratio>
        std::string Dump() const
        {
            std::ostringstream os; 
            for (const auto& elem : bench_tasks_data_)
            {
                os << '#' << elem.first << ": " << elem.second.bench_name << '\n'
                    << "Duration: " << elem.second.GetEllapsedTimeAs<Ratio>();
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
        std::string Dump() const
        {
            std::ostringstream os; 
            for (const auto& elem : bench_funcs_data_)
            {
                int ctr = 1;
                os << '#' << ctr << ": " <<  elem.bench_name << '\n'
                    << "Duration: " << elem.GetEllapsedTimeAs<Ratio>();
            }
            return os.str();
        }

    private:
        BenchEvents bench_funcs_data_;
    };
} //namespace bench

//benchmark sections named or unnamed
#define BENCH_START_NAMED(name) bench::BasicBenchmarker::GetInstance().StartBenchmark(name)
#define BENCH_START (bench::BasicBenchmarker::GetInstance().StartBenchmark())
#define BENCH_END (bench::BasicBenchmarker::GetInstance().EndBenchmark()) 

//print sections benchmark results
#define BENCH_PRINT_Milisec {                                                     \
    const auto res = bench::BasicBenchmarker::GetInstance().Dump<std::milli>();   \
    std::cout << "in milliseconds:\n" << res << std::endl;                        \
}

#define BENCH_PRINT_Nanosec {                                                     \
    const auto res = bench::BasicBenchmarker::GetInstance().Dump<std::nano>();    \
    std::cout << "in nanoseconds:\n" << res << std::endl;                         \
}

//benchmark functions 
#define BENCH_FUNC(FunctionObj) (bench::FuncBenchmarker::GetInstance().MeasureFunction(FunctionObj, #FunctionObj))

//print function benchmark results
#define BENCH_PRINT_FUNC_Milisec {                                               \
    const auto res = bench::FuncBenchmarker::GetInstance().Dump<std::milli>();   \
    std::cout << "in milliseconds:\n" << res << std::endl;                       \
}

#define BENCH_PRINT_FUNC_Nanosec {                                               \
    const auto res = bench::FuncBenchmarker::GetInstance().Dump<std::nano>();    \
    std::cout << "in nanoseconds:\n" << res << std::endl;                        \
}

//get benchmarker instances to manage operations manually
#define BENCH_GET_BASIC (bench::BasicBenchmarker::GetInstance())
#define BENCH_GET_FUNCS (bench::FuncBenchmarker::GetInstance())