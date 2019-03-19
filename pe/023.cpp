#include <iostream>

using T = unsigned int;

int main( )
{
    constexpr T N = 28'123U;
    T           sigma1[N + 1U];
    T           numbers[N + 1U];
    T           abundant[N + 1U];
    T           represented[N + 1U];

    for (T i = 0U; i <= N; ++i)
    {
        sigma1[i]      = 1U;
        numbers[i]     = i;
        abundant[i]    = 0U;
        represented[i] = 0U;
    }

    for (T i = 2U; i <= N; ++i)
    {
        if (numbers[i] != 1U)
        {
            const T prime = numbers[i];
            for (T j = prime; j <= N; j += prime)
            {
                T nominator = 1U;

                while (numbers[j] % prime == 0U)
                {
                    nominator *= prime;
                    numbers[j] /= prime;
                }

                nominator *= i;
                --nominator;
                sigma1[j] *= nominator;
                sigma1[j] /= (prime - 1U);
            }
        }
    }

    for (T i = 2U; i <= N; ++i)
    {
        if (sigma1[i] > 2 * i) { abundant[i] = 1U; }
    }

    for (T i = 12U; i <= N; ++i)
    {
        if (abundant[i] == 1U)
        {
            for (T j = i; j <= N; ++j)
            {
                if (abundant[j] == 1U)
                {
                    if (i + j <= N) represented[i + j] = 1U;
                }
            }
        }
    }

    T sum = 0U;
    for (T i = 1U; i <= N; ++i)
    {
        if (represented[i] == 0U) { sum += i; }
    }

    std::cout << sum;

    return 0;
}