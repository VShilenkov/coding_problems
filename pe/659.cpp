#include <chrono>
#include <iomanip>
#include <iostream>
#include <list>
#include <set>

using std::cout;
using std::endl;
using std::list;
using std::ostream;
using std::set;
using std::setfill;
using std::setw;
using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

using ull_t      = unsigned long long int;
using duration_t = std::chrono::duration<double>;

namespace {
constexpr bool  log_enabled   = true;
constexpr bool  unpack_result = true;
constexpr ull_t k_count       = 10'000'000ULL;
constexpr ull_t log_step      = k_count / 100U;

constexpr ull_t  cache_size = k_count >> 4U;
constexpr size_t roots_size = k_count >> 1U;
}    // namespace

template<typename T = ull_t>
struct long_number
{
private:
    static const T arithmetic_word = 1'000'000'000ULL;

public:
    long_number(const T& init)
        : high(0U)
        , low(init)
    {}

    const long_number<T>& operator+=(const T& a)
    {
        low += a % arithmetic_word;
        high += (a / arithmetic_word) + (low / arithmetic_word);
        normalize( );
        return *this;
    }

    const long_number<>& operator<<=(const size_t& s)
    {
        low <<= s;
        high <<= s;
        high += (low / arithmetic_word);
        normalize( );
        return *this;
    }

    template<typename U>
    friend ostream& operator<<(ostream&, const long_number<U>&);

private:
    inline void normalize( )
    {
        low %= arithmetic_word;
        high %= arithmetic_word;
    }

    T high;
    T low;
};

template<typename T>
ostream& operator<<(ostream& os, const long_number<T>& n)
{
    os << setw(4) << n.high / 1'000'000ULL << '\'';
    char f = os.fill('0');
    os << setw(3) << (n.high % 1'000'000ULL) / 1'000ULL << '\'' << setw(3) << n.high % 1'000ULL << '\'';
    os << setw(3) << n.low / 1'000'000ULL << '\'' << setw(3) << (n.low % 1'000'000ULL) / 1'000ULL << '\'' << setw(3)
       << n.low % 1'000ULL << setfill(f);
    return os;
}

template<typename T, typename U = ull_t>
inline const T& unpack(T& a, const U& b = 1U)
{
    a <<= 2U;
    a += b;
    return a;
}

template<typename T = ull_t>
T* init_cache(const T& size = cache_size)
{
    T* factors_packed_cache = new T[size];
    for (T i = 0U; i < size; ++i) { factors_packed_cache[i] = 0U; }
    return factors_packed_cache;
}

template<typename T = ull_t>
inline void in_cache(T&       number_packed,
                     T*       factors_packed_cache,
                     list<T>& factor_packed_seria,
                     T&       prime_packed_highest,
                     const T& size = cache_size)
{
    if (number_packed < size)
    {
        const ull_t& factor_packed_cached = factors_packed_cache[number_packed];
        if (0U == factor_packed_cached) { factor_packed_seria.push_front(number_packed); }
        else
        {
            if (prime_packed_highest < factor_packed_cached) { prime_packed_highest = factor_packed_cached; }
            number_packed = 0U;
        }
    }
}

template<typename T = ull_t>
T* init_square_roots(const T& size = roots_size)
{
    T* square_roots = new T[size];
    square_roots[1] = 2U;

    for (T i = 2U; i < size; ++i)
    {
        const T unpack_i      = (i << 2U) + 1U;
        T       previous_root = square_roots[i - 1U];
        while (unpack_i > previous_root * previous_root) { ++previous_root; }
        square_roots[i] = --previous_root;
    }

    return square_roots;
}

void naive( )
{

    ull_t* square_roots         = init_square_roots( );
    ull_t* factors_packed_cache = init_cache( );

    set<ull_t> primes_packed{1, 3, 4};

    constexpr ull_t    prime_packed_lowest = 1ULL;
    long_number<ull_t> sum(prime_packed_lowest);

    high_resolution_clock::time_point start_point        = high_resolution_clock::now( );
    duration_t::rep                   previous_step_long = 0.;

    list<ull_t> factor_packed_seria;

    for (ull_t i = 2U; i <= k_count; ++i)
    {
        ull_t number_packed         = i * i;
        ull_t prime_packed_treshold = (i >> 1U);
        ull_t prime_packed_highest  = prime_packed_lowest;

        factor_packed_seria.clear( );
        in_cache(number_packed, factors_packed_cache, factor_packed_seria, prime_packed_highest);

        for (const ull_t& prime_packed : primes_packed)
        {
            if ((prime_packed > prime_packed_treshold) || (prime_packed > number_packed)) break;

            const ull_t prime = (prime_packed << 2U) + 1U;

            if (prime_packed == (number_packed % prime))
            {
                if (prime_packed_highest < prime_packed) { prime_packed_highest = prime_packed; }
                do
                {
                    number_packed /= prime;
                    prime_packed_treshold /= square_roots[prime_packed];

                    in_cache(number_packed, factors_packed_cache, factor_packed_seria, prime_packed_highest);
                } while (prime_packed == (number_packed % prime));
            }
        }

        if (prime_packed_highest < number_packed)
        {
            prime_packed_highest = number_packed;
            primes_packed.insert(prime_packed_highest);
        }

        for (const ull_t& factor : factor_packed_seria)
        {
            const ull_t& prime_highest = (prime_packed_highest << 2U) + 1U;
            for (ull_t j = factor; j < cache_size; j = j * prime_highest + prime_packed_highest)
            {
                if (factors_packed_cache[j] < prime_highest) { factors_packed_cache[j] = prime_packed_highest; }
            }
        }
        sum += prime_packed_highest;

        if (log_enabled && (0U == (i % log_step)))
        {
            const duration_t::rep t = duration_cast<duration_t>(high_resolution_clock::now( ) - start_point).count( );
            const duration_t::rep c = t - previous_step_long;
            previous_step_long      = t;
            cout << setw(3) << i / log_step << '%' << setw(9) << t << setw(9) << c << setw(15) << prime_packed_highest
                 << sum << endl;
        }
    }

    if (unpack_result) { unpack(sum, k_count); }

    cout << sum << endl;
    cout << duration_cast<duration_t>(high_resolution_clock::now( ) - start_point).count( ) << endl;

    delete[] square_roots;
    delete[] factors_packed_cache;
}

int main( )
{
    constexpr ull_t K              = 10'000'001ULL;
    ull_t*          sequence       = new ull_t[K];
    ull_t*          factor_highest = new ull_t[K];

    high_resolution_clock::time_point s = high_resolution_clock::now( );

    for (ull_t i = 1U; i < K; ++i)
    {
        sequence[i]       = ((i * i) << 2U) + 1U;
        factor_highest[i] = 0U;
    }

    for (ull_t i = 1U; i < K; ++i)
    {
        const ull_t p = sequence[i];
        if (1U < p)    // after all previous iterations <i - sequnce[i] - prime
        {
            for (ull_t j = i, jj = p - i; j < K;
                 j += p, jj += p)    // j - first square root of -1 mod p, jj - srcond one
            {
                while (0U == (sequence[j] % p))
                {
                    sequence[j] /= p;
                }    // discard power of p in all further numbers devidable by p in the first root line
                if (factor_highest[j] < p) { factor_highest[j] = p; }    // update value of highest primary devisor

                if (jj < K)    // same for the second root
                {
                    while (0U == (sequence[jj] % p)) { sequence[jj] /= p; }
                    if (factor_highest[jj] < p) { factor_highest[jj] = p; }
                }
            }
        }
    }

    delete[] sequence;

    long_number<ull_t> sum(0);    // accumulate all the highest prime factors of each number
    for (ull_t i = 1U; i < K; ++i) { sum += factor_highest[i]; }

    delete[] factor_highest;

    cout << sum << '\t' << duration_cast<duration_t>(high_resolution_clock::now( ) - s).count( ) << endl;
    return 0;
}