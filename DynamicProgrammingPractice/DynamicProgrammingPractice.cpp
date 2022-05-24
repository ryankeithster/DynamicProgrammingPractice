// The file compares the relative performance of 3 different approaches to calculate the nth fibonacci number.
// The most interesting part of the use of "constexpr" in the third implementation of the fibonacci function.
// This makes the calculation essentially instantaneous at runtime.

#include <iostream>
#include <array>
#include <chrono>

/// <summary>
/// Naive calculation of nth fibonacci number.
/// </summary>
/// <param name="n"></param>
/// <returns></returns>
int fibonacci(int n)
{
    if (n <= 2)
    {
        return 1;
    }

    return fibonacci(n - 1) + fibonacci(n - 2);
}

/// <summary>
/// Vastly improved implementation of fibonacci calculation using memoization.
/// </summary>
/// <param name="n"></param>
/// <param name="memo"></param>
/// <returns></returns>
template <std::size_t N>
int fibonacci(int n, std::array<int, N>& memo)
{
    if (memo[n - 1] > 0)
    {
        return memo[n - 1];
    }

    if (n <= 2 && n > 0)
    {
        memo[n - 1] = 1;
        return 1;
    }

    memo[n - 1] = fibonacci(n - 1, memo) + fibonacci(n - 2, memo);
    return memo[n - 1];
}

/// <summary>
/// Best implementation of fibonacci calculation, using memoization and constexpr.
/// Constexpr allows the value to be calculated "instantaneously", ie at compile time instead of runtime.
/// </summary>
/// <param name="n"></param>
/// <param name="memo"></param>
/// <returns></returns>
template <std::size_t N>
constexpr int fibonacciC(int n, std::array<int, N>& memo)
{
    if (memo[n - 1] > 0)
    {
        return memo[n - 1];
    }

    if (n <= 2)
    {
        memo[n - 1] = 1;
        return 1;
    }

    memo[n - 1] = fibonacciC(n - 1, memo) + fibonacciC(n - 2, memo);
    return memo[n - 1];
}

int main()
{
    constexpr int fibN = 42;
    std::array<int, fibN> memoA = { 0, };
    int result1 = { 0 }, result2 = { 0 }, result3 = { 0 }, result4 = { 0 };

    auto start = std::chrono::high_resolution_clock::now();
    result1 = fibonacci(fibN);
    auto stop = std::chrono::high_resolution_clock::now();
    auto durationClassic = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    start = std::chrono::high_resolution_clock::now();
    result3 = fibonacci(fibN, memoA);
    stop = std::chrono::high_resolution_clock::now();
    auto durationArray = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    start = std::chrono::high_resolution_clock::now();
    result4 = fibonacciC(fibN, memoA);
    stop = std::chrono::high_resolution_clock::now();
    auto durationArrayConstexpr = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << "fibonacci(" << fibN << ") = " << result1 << std::endl;
    std::cout << "time = " << durationClassic.count() << " us" << std::endl << std::endl;

    std::cout << "fibonacci<array>(" << fibN << ") = " << result3 << std::endl;
    std::cout << "time = " << durationArray.count() << " us" << std::endl << std::endl;

    std::cout << "fibonacci<array, constexpr>(" << fibN << ") = " << result4 << std::endl;
    std::cout << "time = " << durationArrayConstexpr.count() << " us" << std::endl << std::endl;
}
