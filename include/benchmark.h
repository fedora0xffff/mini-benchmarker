#pragma once

#include <chrono>
#include <string>
#include <iostream>
#include <map>
#include <list>
#include <string>
#include <sstream>

namespace bench {

    #define NO_OPT(x) asm volatile("" : "+r"((x))) 

    /// @brief helps avoid compiler optimizations for an expression 
    /// @tparam T - the resulting type of an expression 
    /// @param t the expression;
    template <typename T>
    inline void NoOptimization(T&& t) {
        asm volatile("" : "+r"(t));
    } 

    /// @brief comprises a benchmark task data 
    struct BenchData final {
        using Time = std::chrono::time_point<std::chrono::steady_clock>;

        template <typename Ratio>
        using TimePeriod = const std::chrono::duration<long double, Ratio>;
    public:
        BenchData() = default;
        BenchData(std::string str, Time begin_time, Time end_time = {});
        ~BenchData() = default;

        /// @brief sets the end time point
        /// @param end_time - the measured end time point
        /// @return measured duration between begin and end
        void SetEnd(Time end_time);
    public:

        template <typename T>
        auto GetEllapsedTimeAs() const
        {
            using namespace std::literals;
            auto dur = end - begin;
            return std::chrono::duration_cast<T>(dur).count();
        }

    public:
        std::string message;
        Time begin;
        Time end;
    };

    class ScopedBench {
    public:
        ScopedBench(std::string name);
        virtual ~ScopedBench();
    public:
        /// @brief dumps current benchmark event
        /// @return the formatted string 
        std::string Dump() const;

    private:
        BenchData bench_data_;
    };

    class FuncBenchmarker final {
        using BenchEvents = std::list<BenchData>;
        FuncBenchmarker() = default;
    public:
        static FuncBenchmarker& GetInstance();

        void clear();

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
            std::string unit;
            if (typeid(Ratio) == typeid(std::chrono::milliseconds)) {
                unit = "ms";
            }
            else if (typeid(Ratio) == typeid(std::chrono::seconds)) {
                unit = "s";
            }
            else if (typeid(Ratio) == typeid(std::chrono::nanoseconds)) {
                unit = "ns";
            }
            else {
                unit = "";
            }
            for (const auto& elem : bench_funcs_data_)
            {
                int ctr = 1;
                os << '#' << ctr << ": " <<  elem.message << '\n'
                    << "Duration: " << elem.GetEllapsedTimeAs<Ratio>() << '\n';
                ++ctr;
            }
            return os.str();
        }

    private:
        BenchEvents bench_funcs_data_;
    };
} //namespace bench

#define MEASURE_FUNC(func, name) \
    bench::FuncBenchmarker::GetInstance().MeasureFunction(func, name);

#define PRINT_RES \ 
    std::cout << bench::FuncBenchmarker::GetInstance().Dump<std::chrono::milliseconds>();

#define PRINT_RES_NS \ 
    std::cout << bench::FuncBenchmarker::GetInstance().Dump<std::chrono::nanoseconds>();

#define PRINT_RES_SEC \ 
    std::cout << bench::FuncBenchmarker::GetInstance().Dump<std::chrono::seconds>();

#define CLEAR (bench::FuncBenchmarker::GetInstance().clear())

#define UNIQ_ID_IMPL(lineno) _a_local_var_##lineno
#define UNIQ_ID(lineno) UNIQ_ID_IMPL(lineno)

#define LOG_DURATION(message) \
  ScopedBench UNIQ_ID(__LINE__){message};