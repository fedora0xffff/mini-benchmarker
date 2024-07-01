#include <string>
#include <chrono>

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
        using Time = std::chrono::time_point<std::chrono::steady_clock>;

        template <typename Ratio>
        using TimePeriod = const std::chrono::duration<double, Ratio>;
    public:

        template <typename Str>
        BenchData(Str&& name, Time begin_time)
        : bench_name(std::forward<Str>(name)),
        begin(b),
        end({}) 
        {}

        BenchData(std::string&& name, Time begin_time, Time end_time);

        /// @brief sets the end time point
        /// @param end_time - the measured end time point
        /// @return measured duration between begin and end
        void SetEnd(Time end_time);
    public:

        template <typename T>
        TimePeriod<T> GetEllapsedTimeAs(T ratio) const
        {
            return std::chrono::duration<double, ratio>(end - begin);
        }

    public:
        std::string bench_name;
        Time begin;
        Time end;
    };

} //namespace bench