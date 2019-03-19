#include <iostream>

using T = unsigned int;

int main( )
{
    constexpr T N = 10'000U;
    T           sigma1[N + 1U];
    T           numbers[N + 1U];

    for (T i = 0U; i <= N; ++i)
    {
        sigma1[i]  = 1U;
        numbers[i] = i;
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

    for (T i = 2U; i <= N; ++i) { sigma1[i] -= i; }
    T sum = 0U;
    for (T i = 2U; i <= N; ++i)
    {
        if ((i != sigma1[i]) && (sigma1[i] <= N) && (i == sigma1[sigma1[i]])) { sum += i; }
    }

    std::cout << sum << std::endl;
    return 0;
}