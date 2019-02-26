
#include "number.hpp"

#include <chrono>
#include <iostream>
#include <typeinfo>
#include <utility>

using std::cout;
using std::endl;

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

using number_t = unsigned long long int;
using order_t  = unsigned short int;

number_t palindrome_5(const number_t& factor, number_t* const reversed)
{
    const number_t base   = 10ULL;
    const number_t base_2 = base * base;

    number_t* const ring = new number_t[factor];
    for (number_t i = 0U; i < factor; ++i) { ring[i] = 0U; }

    number_t result = 0U;
    for (number_t i = base; i < base_2; ++i)
    {
        number_t r = reversed[i];                                        // r -> i1|i2 -> i2|i1
        number_t l = (i * base_2) % factor;                              // l -> i1|i2 -> i1|i2|0|0
        if (0U == (((i / base) * base_2 + r) % factor)) { ++result; }    // i1|i2|i1 = 0 mod (factor)
        if (0U == ((l + r) % factor)) { ++result; }                      // i1|i2|i2|i1 = 0 mod (factor)
        ring[(l * 10U + r) % factor]++;                                  // i1|i2|0|i2|i1 = a mod (factor); ring[a]++
    }

    number_t minus_base_2 = vs::complementModular(base_2, factor);    // i1|i2|i|i2|i1 = 0 (factor)
    for (number_t i = 0U; i < base; ++i)                              // i1|i2|0|i2|i1 + i|0|0 = 0 (factor)
    {                                                                 // i1|i2|0|i2|i1 + i*100 = 0 (factor)
        result += ring[(i * minus_base_2) % factor];                  // i1|i2|0|i2|i1 =  -i*100 (factor)
    }                                                                 // i1|i2|0|i2|i1 =  i*(-100) (factor)

    delete[] ring;
    return result;
}

number_t palindrome_5_bf(const number_t& factor, number_t* const reversed)
{
    const number_t base   = 10ULL;
    const number_t base_2 = base * base;
    const number_t base_3 = base_2 * base;

    number_t result = 0U;
    for (number_t i = 1U; i < base; ++i)
    {
        if (0U == (i % factor)) ++result;
        if (0U == (((base + 1U) * i) % factor)) ++result;
    }
    for (number_t i = base; i < base_2; ++i)
    {
        if (0U == ((i / base) * base_2 + reversed[i]) % factor) ++result;
        if (0U == (i * base_2 + reversed[i]) % factor) ++result;
    }
    for (number_t i = base_2; i < base_3; ++i)
    {
        if (0U == ((i / base) * base_3 + reversed[i]) % factor) ++result;
    }
    return result;
}

number_t palindrom_10_7_19_16_b( )
{
    constexpr number_t prime = 10000019ULL;

    constexpr number_t ten_7  = 10000000ULL;
    constexpr number_t ten_8  = ten_7 * 10U;
    number_t           result = 0U;
    for (number_t i = ten_7; i < ten_8; ++i)
    {
        number_t n = ((i * ten_8 % prime) % prime + vs::reverseNumber(i) % prime) % prime;
        if (n == 0U)
        {
            // cout << i << vs::reverseNumber(i) << endl;
            ++result;
        }
    }
    return result;
}

number_t palindrom_10_7_19_18_b( )
{
    constexpr number_t prime = 10000019ULL;

    constexpr number_t ten_7  = 10000000ULL;
    constexpr number_t ten_8  = ten_7 * 10U;
    constexpr number_t ten_9  = ten_8 * 10U;
    number_t           result = 0U;
    for (number_t i = ten_8; i < ten_9; ++i)
    {
        number_t n = ((i * ten_9 % prime) % prime + vs::reverseNumber(i) % prime) % prime;
        if (n == 0U)
        {
            // cout << i << '\t' << vs::reverseNumber(i) << endl;
            ++result;
        }
    }
    return result;
}

number_t palindrom_10_7_19_16( )
{
    constexpr number_t prime = 10000019ULL;
    constexpr number_t ten_4 = 10000ULL;
    constexpr number_t ten_5 = ten_4 * 10U;

    number_t* ring = new number_t[prime];

    for (number_t i = 0U; i < prime; ++i) ring[i] = 0U;

    const number_t ten_11 = vs::powerModular(10ULL, 11U, prime);

    for (number_t i = ten_4; i < ten_5; ++i) { ring[((i * ten_11) % prime + vs::reverseNumber(i)) % prime]++; }

    number_t result = 0U;

    for (number_t i = 0U; i < 1000U; ++i)
    {
        number_t n  = i * 1000U + vs::reverseNumberFixedLength(i, 3U);
        number_t c  = (n * ten_5) % prime;
        number_t cc = vs::complementModular(c, prime);
        result += ring[cc];
    }

    return result;
}

number_t palindrom_one_ring(const order_t&  order,
                            const number_t& prime,
                            number_t*       middle_pallindrome,
                            number_t*       reversed,
                            const order_t&  base = 10ULL)
{
    const order_t prime_order = (1U == (order & 1U)) ? (5U) : (6U);

    const order_t ring_order = (order - prime_order) >> 1U;

    number_t* ring = new number_t[prime];

    for (number_t i = 0U; i < prime; ++i) { ring[i] = 0U; }

    const order_t  ring_shift_order = order - ring_order;
    const number_t ring_shift       = vs::powerModular(base, ring_shift_order, prime);
    const number_t ring_up          = vs::power(10ULL, ring_order);
    const number_t ring_dn          = ring_up / 10U;

    for (number_t i = ring_dn; i < ring_up; ++i) { ring[((i * ring_shift) % prime + reversed[i]) % prime]++; }

    const number_t ten_m  = vs::powerModular(10ULL, ring_order, prime);
    number_t       result = 0U;

    for (number_t i = 0U; i < 1000U; ++i)
    { result += ring[vs::complementModular(middle_pallindrome[i] * ten_m, prime)]; }
    delete[] ring;
    return result;
}

std::pair<number_t, number_t> palindrom_one_ring_two_order(const order_t&        order,
                                                           const number_t&       prime,
                                                           const number_t* const pallindrome_even,
                                                           const number_t* const pallindrome_odd,
                                                           const number_t* const reversed)
{
    constexpr order_t prime_order_odd = 5U;
    const order_t     ring_lenght     = (order - prime_order_odd) >> 1U;

    number_t* ring = new number_t[prime << 1U];

    const order_t  ring_shift_order      = prime_order_odd + ring_lenght;
    const number_t ring_shift_multiplier = vs::powerModular(10ULL, ring_shift_order, prime);
    const number_t ring_upper_limit      = vs::power(10ULL, ring_lenght);

    for (number_t i = ring_upper_limit / 10U; i < ring_upper_limit; ++i)
    {
        const number_t t = (i * ring_shift_multiplier) % prime;
        ring[(t + reversed[i]) % prime]++;
        ring[prime + (t * 10ULL + reversed[i]) % prime]++;
    }

    const number_t middle_shift_multiplier = vs::complementModular(vs::powerModular(10ULL, ring_lenght, prime), prime);
    std::pair<number_t, number_t> result(0U, 0U);

    for (number_t i = 0U; i < 1000UL; ++i)
    {
        result.first += ring[(pallindrome_odd[i] * middle_shift_multiplier) % prime];
        result.second += ring[prime + (pallindrome_even[i] * middle_shift_multiplier) % prime];
    }

    delete[] ring;

    return result;
}

number_t palindrom_10_7_19_18( )
{
    constexpr number_t prime = 10000019ULL;
    constexpr number_t ten_6 = 1000000ULL;

    number_t* ring_o = new number_t[prime];
    number_t* ring_i = new number_t[prime];

    for (number_t i = 0U; i < prime; ++i) { ring_o[i] = ring_i[i] = 0U; }

    const number_t ten_15 = vs::powerModular(10ULL, 15U, prime);
    const number_t ten_12 = vs::powerModular(10ULL, 12U, prime);

    for (number_t i = 0ULL; i < 1000ULL; ++i)
    {
        if (i > 99UL) { ring_o[((i * ten_15) % prime + vs::reverseNumber(i) % prime) % prime]++; }
        ring_i[((i * ten_12) % prime + (vs::reverseNumberFixedLength(i, 3U) * 1000U) % prime) % prime]++;
    }

    number_t result = 0U;
    for (number_t i = 0U; i < 1000U; ++i)
    {
        number_t n  = (i * 1000U + vs::reverseNumberFixedLength(i, 3U));
        number_t c  = (n * ten_6) % prime;
        number_t cc = vs::complementModular(c, prime);
        for (number_t j = 0; j < prime; ++j)
        {
            number_t index = (cc < j) ? (cc + prime - j) : cc - j;
            result += ring_o[j] * ring_i[index];
            // if ((ring_o[j] * ring_i[index]) != 0U)
            // if ((i == 1) && (j == 1582014))
            //{ cout << n << '\t' << j << '\t' << index << '\t' << ring_o[j] << '\t' << ring_i[index] << endl; }
        }
        // if (i == 1) break;
    }
    return result;
}

number_t palindrom_10_7_19_32( )
{
    constexpr number_t prime = 10000019ULL;
    constexpr number_t ten_5 = 100000ULL;
    constexpr number_t ten_6 = ten_5 * 10U;
    constexpr number_t ten_7 = ten_6 * 10U;

    number_t* ring_o = new number_t[prime];
    number_t* ring_i = new number_t[prime];

    for (number_t i = 0U; i < prime; ++i) { ring_o[i] = ring_i[i] = 0U; }

    const number_t ten_25 = vs::powerModular(10ULL, 25U, prime);

    for (number_t i = ten_6; i < ten_7; ++i)
    { ring_o[((i * ten_25) % prime + vs::reverseNumber(i) % prime) % prime]++; }

    const number_t ten_19 = vs::powerModular(10ULL, 19U, prime);

    for (number_t i = 0UL; i < ten_6; ++i)
    { ring_i[((i * ten_19) % prime + (vs::reverseNumberFixedLength(i, 6U) * ten_7) % prime) % prime]++; }

    const number_t ten_13 = vs::powerModular(10ULL, 13U, prime);
    number_t       result = 0U;
    for (number_t i = 0U; i < 1000U; ++i)
    {
        number_t n  = (i * 1000U + vs::reverseNumberFixedLength(i, 3U));
        number_t c  = (n * ten_13) % prime;
        number_t cc = vs::complementModular(c, prime);
        for (number_t j = 0; j < prime; ++j)
        {
            number_t index = (cc < j) ? (cc + prime - j) : cc - j;
            result += ring_o[j] * ring_i[index];
        }
    }
    return result;
}

number_t
palindrom_two_ring(const order_t& order, const number_t& prime, number_t* middle_pallindrome, number_t* reversed)
{
    const order_t prime_effective_length = (order & 1U) ? (5U) : (6U);

    const order_t rings_length = (order - prime_effective_length) >> 1U;

    if (rings_length < 8U) return palindrom_one_ring(order, prime, middle_pallindrome, reversed);

    const order_t ring_i_length = rings_length >> 1U;
    const order_t ring_o_length = rings_length - ring_i_length;

    const order_t shift_ol = order - ring_o_length;
    const order_t shift_il = shift_ol - ring_i_length;
    const order_t shift_ir = ring_o_length;
    const order_t shift_m  = rings_length;

    // cout << ring_o_length << '\t' << shift_ol << '\t' << ring_i_length << '\t' << shift_il << '\t'
    //     << prime_effective_length << '\t' << shift_m << '\t' << ring_i_length << '\t' << shift_ir << '\t'
    //     << ring_o_length << endl;

    number_t* ring_o = new number_t[prime];
    number_t* ring_i = new number_t[prime];

    for (number_t i = 0U; i < prime; ++i) { ring_o[i] = ring_i[i] = 0U; }
    // fill outer ring

    const number_t ten_ol = vs::powerModular(10ULL, shift_ol, prime);
    const number_t or_up  = vs::power(10ULL, ring_o_length);
    const number_t or_dn  = or_up / 10ULL;

    for (number_t i = or_dn; i < or_up; ++i) { ring_o[((i * ten_ol) % prime + reversed[i]) % prime]++; }

    // fill inner ring

    const number_t ten_il = vs::powerModular(10ULL, shift_il, prime);
    const number_t ten_ir = vs::powerModular(10ULL, shift_ir, prime);
    const number_t ir_up  = vs::power(10ULL, ring_i_length);

    for (number_t i = 0ULL; i < ir_up; ++i)
    { ring_i[((i * ten_il) % prime + (vs::reverseNumberFixedLength(i, ring_i_length) * ten_ir) % prime) % prime]++; }

    // play with middle part

    const number_t ten_m  = vs::powerModular(10ULL, shift_m, prime);
    number_t       result = 0U;

    for (number_t i = 0U; i < 1000U; ++i)
    {
        number_t cc = vs::complementModular(middle_pallindrome[i] * ten_m, prime);
        for (number_t j = 0; j < prime; ++j) { result += ring_o[j] * ring_i[((cc < j) ? (cc + prime) : (cc)) - j]; }
    }
    delete[] ring_o;
    delete[] ring_i;
    return result;
}

int main( )
{
    number_t result = 0U;

    constexpr order_t reversed_order = 7U;
    const number_t    reversed_count = vs::power(10ULL, reversed_order);
    number_t* const   reversed       = new number_t[reversed_count];

    for (number_t i = 0U; i < reversed_count; ++i) reversed[i] = vs::reverseNumber(i);

    constexpr order_t middle_pallindromes_count = 1000U;

    number_t* reversed_3 = new number_t[middle_pallindromes_count];
    for (order_t i = 0U; i < middle_pallindromes_count; ++i) { reversed_3[i] = vs::reverseNumberFixedLength(i, 3U); }

    number_t* pallindrome_5 = new number_t[middle_pallindromes_count];
    number_t* pallindrome_6 = new number_t[middle_pallindromes_count];

    for (order_t i = 0U; i < middle_pallindromes_count; ++i)
    {
        pallindrome_5[i] = (i / 10U) * middle_pallindromes_count + reversed_3[i];
        pallindrome_6[i] = i * middle_pallindromes_count + reversed_3[i];
    }

    auto s = high_resolution_clock::now( );

    for (unsigned short i = 16U; i <= 20U; i += 2)
    {
        auto s = high_resolution_clock::now( );
        // number_t step =
        // palindrom_two_ring(i, 10000019ULL, (((i & 1U) == 1U)) ? (pallindrome_5) : (pallindrome_6), reversed);

        auto result_p = palindrom_one_ring_two_order(i, 10000019ULL, pallindrome_6, pallindrome_5, reversed);

        // a += step;
        result += result_p.first + result_p.second;

        auto step_duration = high_resolution_clock::now( ) - s;
        auto milliseconds  = duration_cast<std::chrono::milliseconds>(step_duration).count( );    // % 1000U;
        // auto seconds       = duration_cast<std::chrono::seconds>(step_duration).count( ) % 60U;
        // auto minutes       = duration_cast<std::chrono::minutes>(step_duration).count( );
        cout << '[' << i - 1U << "]\t" << result_p.first << endl;
        cout << '[' << i << "]\t" << result_p.second << "\t\t" << milliseconds
             << endl; /*<< minutes << ':' << seconds << '.'*/
    }

    auto total        = high_resolution_clock::now( ) - s;
    auto milliseconds = duration_cast<std::chrono::milliseconds>(total).count( );    //% 1000U;
    // auto seconds      = duration_cast<std::chrono::seconds>(total).count( ) % 60U;
    // auto minutes      = duration_cast<std::chrono::minutes>(total).count( );
    cout << result << '\t' << milliseconds << endl;    // minutes << ':' << seconds << '.' <<
    s      = high_resolution_clock::now( );
    result = 0U;
    for (unsigned short i = 15U; i <= 20U; ++i)
    {
        auto     s = high_resolution_clock::now( );
        number_t step =
            palindrom_two_ring(i, 10000019ULL, (((i & 1U) == 1U)) ? (pallindrome_5) : (pallindrome_6), reversed);

        // auto result = palindrom_one_ring_two_order(i, 10000019ULL, pallindrome_6, pallindrome_5, reversed);

        result += step;
        // a += result.first + result.second;

        auto step_duration = high_resolution_clock::now( ) - s;
        auto milliseconds  = duration_cast<std::chrono::milliseconds>(step_duration).count( );    //% 1000U;
        // auto seconds       = duration_cast<std::chrono::seconds>(step_duration).count( ) % 60U;
        // auto minutes       = duration_cast<std::chrono::minutes>(step_duration).count( );
        // cout << '[' << i - 1U << "]\t" << result.second << endl;
        cout << '[' << i << "]\t" << step << "\t\t" << milliseconds << endl;    // << minutes << ':' << seconds << '.'
    }
    total        = high_resolution_clock::now( ) - s;
    milliseconds = duration_cast<std::chrono::milliseconds>(total).count( );    // % 1000U;
    // seconds      = duration_cast<std::chrono::seconds>(total).count( ) % 60U;
    // minutes      = duration_cast<std::chrono::minutes>(total).count( );
    cout << result << '\t' << milliseconds << endl;    // << minutes << ':' << seconds << '.'

    delete[] pallindrome_5;
    delete[] pallindrome_6;
    delete[] reversed;

    return 0;
}

int mainaa( )
{
    using T = unsigned long long;

    // constexpr char o_sep = '\t';

    // constexpr T prime_e = 7U;
    // constexpr T prime_d = 19U;
    constexpr T base  = 10U;
    constexpr T prime = 10000019ULL;    // power(base, prime_e) + prime_d;

    auto s = high_resolution_clock::now( );

    const T ten_6 = 1000000ULL;
    const T ten_7 = 10000000ULL;

    unsigned* a1 = new unsigned[prime];
    unsigned* a2 = new unsigned[prime];

    T* inversedNumbers = new T[ten_7];

    for (T i = 0U; i < prime; ++i) { a1[i] = a2[i] = 0U; }
    for (T i = 0U; i < ten_7; ++i) { inversedNumbers[i] = vs::reverseNumberFixedLength(i, 7U); }

    T ten_19 = vs::powerModular(base, 19U, prime);
    T ten_25 = vs::powerModular(base, 25U, prime);

    for (T i = 1U; i < ten_7; ++i)
    {
        if (i < ten_6) { a2[((i * ten_7) % prime + (inversedNumbers[i] * ten_19) % prime) % prime]++; }

        a1[(i % prime + (inversedNumbers[i] * ten_25) % prime) % prime]++;
    }

    // for (T i = 0U; i < prime; ++i) { cout << a1[i] << o_sep << a2[i] << endl; }

    cout << (duration_cast<std::chrono::milliseconds>(high_resolution_clock::now( ) - s)).count( ) << endl;
    return 0;

    const T ten_4 = vs::power(base, 4ULL);
    const T ten_5 = vs::power(base, 5ULL);

    const T ten_17 = vs::powerModular(base, 17ULL, prime);
    const T ten_27 = vs::powerModular(base, 27ULL, prime);

    const T one_n    = vs::complementModular(1ULL, prime);
    const T ten_7_n  = vs::complementModular(ten_7, prime);
    const T ten_17_n = vs::complementModular(ten_17, prime);
    const T ten_27_n = vs::complementModular(ten_27, prime);

    const T ten_5_r = vs::inverseModular(ten_5, prime);

    const T ka1 = (ten_27_n * ten_5_r) % prime;
    const T kb1 = (one_n * ten_5_r) % prime;

    const T ka2 = (ten_17_n * ten_5_r) % prime;

    const T ka3 = (ten_7_n * ten_5_r) % prime;

    T* inversed   = new T[ten_5 - ten_4];
    T* inversed_m = new T[ten_5 - ten_4];

    T counter = 0U;

    for (T a1 = ten_4; a1 < ten_5; a1++)
    {
        inversed[a1 - ten_4]   = vs::reverseNumber(a1);
        inversed_m[a1 - ten_4] = (kb1 * inversed[a1 - ten_4]) % prime;
    }

    for (T a1 = ten_4; a1 < ten_5; a1++)
    {
        // auto s = chrono::high_resolution_clock::now();
        T z1 = ((ka1 * a1) % prime + inversed_m[a1 - ten_4]) % prime;
        z1 *= ten_5_r;
        z1 %= prime;

        for (T a2 = ten_5; a2 < ten_5; a2++)
        {
            T z2 = ((ka2 * a2) % prime + inversed_m[a2 - ten_4] + z1) % prime;
            z2 *= ten_5_r;
            z2 %= prime;

            for (T a3 = ten_4; a3 < ten_5; a3++)
            {
                T z3 = ((ka3 * a3) % prime + inversed_m[a3 - ten_4] + z2) % prime;
                if ((z3 < base * base) && ((z3 % base) == (z3 / base)))
                {
                    counter++;
                    // cout << a1 << '\t' << a2 << '\t' << a3 << '\t' << z3 << '\t' << inversed[a3 - ten_4] << '\t'
                    //   << inversed[a2 - ten_4] << '\t' << inversed[a1 - ten_4] << endl;
                }
            }
        }
        // cout << (a1 - ten_4) << "\t" << counter << "\t"
        //   << (duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now( ) - s)).count( ) <<
        //   endl;
    }
    return 0;
}