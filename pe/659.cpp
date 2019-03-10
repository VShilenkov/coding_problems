#include <chrono>
#include <iostream>
#include <set>
#include <vector>
#include<iomanip>

using std::cout;
using std::endl;
using std::setw;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

using ull_t      = unsigned long long int;
using duration_t = std::chrono::duration<double>;

constexpr bool log_enabled        = true;
constexpr bool unpack_result      = true;

constexpr ull_t arithmetic_word = 1'000'000'000ULL;

template<typename T = ull_t>
struct long_number
{
    long_number(const T& init, const T& length = arithmetic_word)
        : high(0U)
        , low(init)
        , word(length)
    {}

    long_number<T>& operator+=(const T& a)
    {
        low += a % word;
        high += (a / word) + (low / word);
        normalize( );
        return *this;
    }

    long_number<>& operator<<=(const size_t& s)
    {
        low <<= s;
        high <<= s;
        high += (low / word);
        normalize( );
        return *this;
    }

    T high;
    T low;

private:
    const T& word;

    inline void normalize( )
    {
        low %= word;
        high %= word;
    }
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const long_number<T>& n)
{
    os << n.high << '\'' << n.low;
    return os;
}

template<typename T, typename U = ull_t>
inline void unpack(T& a, const U& b = 1U)
{
    a <<= 2U;
    a += b;
}

int main( )
{
    constexpr ull_t k_count  = 10'000'000ULL;
    constexpr ull_t log_step = k_count / 100U;

    constexpr ull_t prime_lowest  = 1ULL;

    std::vector<ull_t> square_roots(k_count >> 1U, 2U);
    for (ull_t i = 2U; i < (k_count >> 1U); ++i)
    {
        ull_t j = square_roots[i - 1U];
        while ( (i << 2U) + 1U > j * j) ++j;
        square_roots[i] = --j;
    }

    std::set<ull_t>    primes;
    long_number<ull_t> sum(prime_lowest);

    high_resolution_clock::time_point start_point = high_resolution_clock::now( );

    for (ull_t i = 2U; i <= k_count; ++i)
    {
        ull_t number         = i * i;
        ull_t prime_treshold = (i >> 1U);    // for packed factors

        while (1U == (number % 5U))
        {
            number /= 5U;
            prime_treshold >>= 1U;
        }

        ull_t prime_highest = prime_lowest;
        for (const ull_t& prime_packed : primes)
        {
            if ((prime_packed > prime_treshold) || (prime_packed > number)) break;

            const ull_t prime = (prime_packed << 2U) + 1U;

            if (number % prime == prime_packed)
            {
                if (prime_packed > prime_highest) { prime_highest = prime_packed; }
                do
                {
                    number /= prime;
                    prime_treshold /= square_roots[prime_packed];
                } while (number % prime == prime_packed);
            }
        }

        if (number > 0U)
        {
            prime_highest = number;
            primes.insert(primes.cend( ), prime_highest);
        }

        sum += prime_highest;

        if (log_enabled && (0U == (i % log_step)))
        {
            const duration_t::rep t = duration_cast<duration_t>(high_resolution_clock::now( ) - start_point).count( );
            cout << std::setw(3) << i / log_step << "%\t" << setw(8) << t << '\t' << setw(14) << prime_highest << '\t' << setw(18) << sum << endl;
        }

    }

    if (unpack_result) { unpack(sum, k_count); }

    cout << sum << endl;
    cout << primes.size() << '\t' << *primes.rbegin() << endl;
    cout << duration_cast<duration_t>(high_resolution_clock::now( ) - start_point).count( ) << endl;

    return 0;
}

#if 0
            std::set<ull_t>::const_reverse_iterator ri =
                std::set<ull_t>::reverse_iterator(std::upper_bound(p.cbegin( ), p.cend( ), prime_treshold));
            //for (auto t: p) cout << t << ' ';
            //cout << endl << "ub:\t" << *ri << endl;
            for (; p.crend( ) != ri; ++ri)
            {
                const ull_t j = *ri;
                if (a % j == 0U)
                {
                    if (j > prime_highest) { prime_highest = j; }
                    do
                    {
                        a /= j;
                    } while ((a % j == 0U) && (j <= a));
                }
            }
#endif