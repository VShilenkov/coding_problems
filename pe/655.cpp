#include "number.hpp"

#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>

constexpr unsigned int log_debug_flag = 0U;

using std::cout;
using std::endl;

using std::chrono::duration_cast;
using std::chrono::high_resolution_clock;

using number_t = unsigned long long int;
using order_t  = unsigned short int;

number_t palindrome_5_bf(const number_t&, number_t* const);
number_t palindrome_5(const number_t&, number_t* const);
number_t palindrome_16_bf(const number_t&);
number_t palindrome_16(const number_t&, const number_t* const, number_t** const);
number_t palindrome_18_bf(const number_t&);
number_t palindrome_18(const number_t&, const number_t*, number_t** const);
number_t palindrom_10_7_19_32( );
void     partition(const order_t&, const order_t&, const order_t&, const order_t&);
void     partition_even(const order_t&, const order_t&, const order_t&, const order_t&);
number_t palindrome_single_ring(const order_t&, const number_t&, const number_t* const, number_t** const);
number_t
         palindrome_single_ring_even(const order_t&, const number_t&, const number_t* const, const number_t* const* const);
number_t palindrom_two_ring(const order_t&, const number_t&, number_t*, number_t*);
number_t
palindrome_double_ring_even(const order_t&, const number_t&, const number_t* const, const number_t* const* const);

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

    number_t minus_base_2 = vs::modular::complement(base_2, factor);    // i1|i2|i|i2|i1 = 0 (factor)
    for (number_t i = 0U; i < base; ++i)                                // i1|i2|0|i2|i1 + i|0|0 = 0 (factor)
    {                                                                   // i1|i2|0|i2|i1 + i*100 = 0 (factor)
        result += ring[(i * minus_base_2) % factor];                    // i1|i2|0|i2|i1 =  -i*100 (factor)
    }                                                                   // i1|i2|0|i2|i1 =  i*(-100) (factor)

    delete[] ring;
    return result;
}

number_t palindrome_16_bf(const number_t& factor)
{
    constexpr number_t base   = 10ULL;
    const number_t     up     = vs::power(base, 8U);
    const number_t     shift  = up % factor;
    number_t           result = 0U;
    for (number_t i = up / base; i < up; ++i)
    {
        number_t n = (i * shift) % factor + vs::reverseNumber(i) % factor;
        if (0U == (n % factor)) { ++result; }
    }
    return result;
}

number_t palindrome_16(const number_t& factor, const number_t* const reversed, number_t** const reversed_fixed)
{
    constexpr number_t base         = 10ULL;
    constexpr order_t  middle_order = 4U;

    number_t* ring = new number_t[factor];
    for (number_t i = 0U; i < factor; ++i) { ring[i] = 0U; }

    const order_t  ring_order = 8U - middle_order;
    const number_t ring_count = vs::power(base, ring_order);
    const number_t ring_shift = vs::modular::power<number_t>(base, 16U - ring_order, factor);

    for (number_t i = ring_count / base; i < ring_count; ++i)
    { ring[((i * ring_shift) % factor + reversed[i] % factor) % factor]++; }

    const number_t  middle_count = vs::power(base, middle_order);
    const number_t  middle_shift = vs::modular::complement(ring_count, factor);
    const number_t* reversed_f   = reversed_fixed[middle_order - 1U];

    number_t result = 0U;
    for (number_t i = 0U; i < middle_count; ++i)
    { result += ring[(((i * middle_count) + reversed_f[i]) * middle_shift) % factor]; }

    delete[] ring;
    return result;
}

number_t palindrome_18_bf(const number_t& factor)
{
    constexpr number_t base   = 10ULL;
    const number_t     up     = vs::power(base, 9U);
    const number_t     shift  = up % factor;
    number_t           result = 0U;
    for (number_t i = up / base; i < up; ++i)
    {
        number_t n = (i * shift) % factor + vs::reverseNumber(i) % factor;
        if (0U == (n % factor)) { ++result; }
    }
    return result;
}

number_t palindrome_18(const number_t& factor, const number_t* reversed, number_t** const reversed_fixed)
{
    constexpr number_t base         = 10ULL;
    constexpr order_t  middle_order = 4U;

    number_t* ring = new number_t[factor];
    for (number_t i = 0U; i < factor; ++i) { ring[i] = 0U; }

    const order_t  ring_order = 9U - middle_order;
    const number_t ring_count = vs::power(base, ring_order);
    const number_t ring_shift = vs::modular::power<number_t>(base, 18U - ring_order, factor);

    for (number_t i = ring_count / base; i < ring_count; ++i)
    { ring[((i * ring_shift) % factor + reversed[i] % factor) % factor]++; }

    const number_t  middle_count = vs::power(base, middle_order);
    const number_t  middle_shift = vs::modular::complement(ring_count, factor);
    const number_t* reversed_f   = reversed_fixed[middle_order - 1U];

    number_t result = 0U;
    for (number_t i = 0U; i < middle_count; ++i)
    { result += ring[(((i * middle_count) + reversed_f[i]) * middle_shift) % factor]; }

    delete[] ring;
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

    const number_t ten_25 = vs::modular::power<number_t>(10ULL, 25U, prime);

    for (number_t i = ten_6; i < ten_7; ++i)
    { ring_o[((i * ten_25) % prime + vs::reverseNumber(i) % prime) % prime]++; }

    const number_t ten_19 = vs::modular::power<number_t>(10ULL, 19U, prime);

    for (number_t i = 0UL; i < ten_6; ++i)
    { ring_i[((i * ten_19) % prime + (vs::reverseNumberFixedLength(i, 6U) * ten_7) % prime) % prime]++; }

    const number_t ten_13 = vs::modular::power<number_t>(10ULL, 13U, prime);
    number_t       result = 0U;
    for (number_t i = 0U; i < 1000U; ++i)
    {
        number_t n  = (i * 1000U + vs::reverseNumberFixedLength(i, 3U));
        number_t c  = (n * ten_13) % prime;
        number_t cc = vs::modular::complement(c, prime);
        for (number_t j = 0; j < prime; ++j)
        {
            number_t index = (cc < j) ? (cc + prime - j) : cc - j;
            result += ring_o[j] * ring_i[index];
        }
    }
    delete[] ring_i;
    delete[] ring_o;
    return result;
}

void partition(const order_t& order,
               const order_t& middle_order_max,
               const order_t& ring_order_max,
               const order_t& factor_order)
{
    const order_t complexity_error       = order + factor_order;
    order_t       single_ring_complexity = complexity_error;
    order_t       double_ring_complexity = complexity_error;

    order_t single_ring_ring_order   = 0U;
    order_t single_ring_middle_order = 0U;

    order_t double_ring_ring_outer_order = 0U;
    order_t double_ring_ring_inner_order = 0U;
    order_t double_ring_middle_order     = 0U;

    if ((order < (static_cast<order_t>((ring_order_max + middle_order_max) << 1U)) + 1U) && (1U < order))
    {
        single_ring_middle_order = order >> 2U;

        if (middle_order_max < single_ring_middle_order) { single_ring_middle_order = middle_order_max; }

        const order_t residual = (order % 4U);
        if (((1U == residual) || (3U == residual)) && (middle_order_max < single_ring_middle_order + 1U))
        { --single_ring_middle_order; }

        if ((2U == residual) && (single_ring_middle_order < middle_order_max)) { ++single_ring_middle_order; }

        single_ring_ring_order = static_cast<order_t>((order - (single_ring_middle_order << 1U)) >> 1U);
        single_ring_middle_order += (order & 1U);
        single_ring_complexity = vs::max(single_ring_ring_order, single_ring_middle_order);

        single_ring_ring_order <<= 1U;
        single_ring_middle_order = order - single_ring_ring_order;

        if (log_debug_flag)
        {
            cout << "ring_middle_ring:\t" << single_ring_complexity << '\t' << (single_ring_ring_order >> 1U) << '-'
                 << (single_ring_middle_order >> 1U) << '-'
                 << single_ring_middle_order - (single_ring_middle_order >> 1U) << '-' << (single_ring_ring_order >> 1U)
                 << endl;
        }
    }

    if ((order < (((ring_order_max << 1U) + middle_order_max) << 1U)) && (3U < order))
    {
        for (order_t middle = 0U; middle < static_cast<order_t>(middle_order_max << 1U) + 1U; ++middle)
        {
            const order_t outer_order  = order - middle;
            order_t       ring_i_order = outer_order >> 2U;
            order_t       ring_o_order = ring_i_order;

            const order_t residual = (outer_order % 4U);
            if (2U == residual) ++ring_i_order;
            if (3U == residual) ++ring_o_order;

            order_t biggest_ring_order = ring_i_order + (outer_order & 1U);
            if (biggest_ring_order > ring_order_max) continue;

            order_t current_complexity = vs::max(biggest_ring_order, order_t(((middle + 1U) >> 1U) + factor_order));

            if (current_complexity < double_ring_complexity)
            {
                double_ring_complexity       = current_complexity;
                double_ring_ring_inner_order = ring_i_order;
                double_ring_ring_outer_order = ring_o_order;
                double_ring_middle_order     = middle;
            }
        }

        double_ring_ring_outer_order <<= 1U;
        double_ring_ring_inner_order = order - double_ring_middle_order - double_ring_ring_outer_order;

        if (log_debug_flag)
        {
            cout << "ring_ring_middle_ring_ring:\t" << double_ring_complexity << '\t'
                 << (double_ring_ring_outer_order >> 1U) << '-' << (double_ring_ring_inner_order >> 1U) << '-'
                 << (double_ring_middle_order >> 1U) << '-'
                 << (double_ring_middle_order - (double_ring_middle_order >> 1U)) << '-'
                 << double_ring_ring_inner_order - (double_ring_ring_inner_order >> 1U) << '-'
                 << (double_ring_ring_outer_order >> 1U) << endl;
        }
    }
}

void partition_even(const order_t& order,
                    const order_t& middle_order_max,
                    const order_t& ring_order_max,
                    const order_t& factor_order)
{
    assert((0U == (order & 1U)) && "order must be even");

    const order_t complexity_error       = order + factor_order;
    order_t       single_ring_complexity = complexity_error;
    order_t       double_ring_complexity = complexity_error;

    order_t single_ring_middle_order = order >> 2U;

    order_t double_ring_middle_order     = 0U;
    order_t double_ring_ring_inner_order = 0U;

    if ((order < static_cast<order_t>((ring_order_max + middle_order_max) << 1U) + 1U) && (1U < order))
    {
        if (middle_order_max < single_ring_middle_order) { single_ring_middle_order = middle_order_max; }

        if ((2U == (order % 4U)) && (single_ring_middle_order < middle_order_max)) { ++single_ring_middle_order; }

        order_t single_ring_ring_order = (order >> 1U) - single_ring_middle_order;
        single_ring_complexity         = vs::max(single_ring_ring_order, single_ring_middle_order);

        if (log_debug_flag)
        {
            cout << "ring_middle_ring:\t\t" << single_ring_complexity << '\t' << single_ring_ring_order << '-'
                 << single_ring_middle_order << '-' << single_ring_middle_order << '-' << single_ring_ring_order
                 << endl;
        }
    }

    if ((order < (((ring_order_max << 1U) + middle_order_max) << 1U)) && (3U < order))
    {
        for (order_t middle = 0U; middle < middle_order_max + 1U; ++middle)
        {
            const order_t rings_order  = (order >> 1U) - middle;
            order_t       ring_i_order = rings_order >> 1U;

            if (1U == (rings_order & 1U)) ++ring_i_order;

            if (ring_i_order > ring_order_max) continue;

            order_t current_complexity = vs::max(ring_i_order, order_t(middle + factor_order));

            if (current_complexity < double_ring_complexity)
            {
                double_ring_complexity       = current_complexity;
                double_ring_ring_inner_order = ring_i_order;
                double_ring_middle_order     = middle;
            }
        }

        if (log_debug_flag)
        {
            order_t double_ring_ring_outer_order =
                (order >> 1U) - double_ring_middle_order - double_ring_ring_inner_order;
            cout << "ring_ring_middle_ring_ring:\t" << double_ring_complexity << '\t' << double_ring_ring_outer_order
                 << '-' << double_ring_ring_inner_order << '-' << double_ring_middle_order << '-'
                 << double_ring_middle_order << '-' << double_ring_ring_inner_order << '-'
                 << double_ring_ring_outer_order << endl;
        }
    }
}

number_t palindrome_single_ring(const order_t&        order,
                                const number_t&       factor,
                                const number_t* const reversed,
                                number_t** const      reversed_fixed)
{
    constexpr number_t base             = 10ULL;
    constexpr order_t  middle_order_max = 4U;

    const order_t is_order_odd  = order & 1U;
    const order_t quarter_order = order >> 2U;
    const order_t middle_order  = (quarter_order + is_order_odd < (middle_order_max + 1U - is_order_odd))
                                     ? (quarter_order)
                                     : (middle_order_max - is_order_odd);

    number_t* ring = new number_t[factor];
    for (number_t i = 0U; i < factor; ++i) { ring[i] = 0U; }

    const order_t  ring_order       = static_cast<order_t>((order - (middle_order << 1U) - is_order_odd) >> 1U);
    const number_t ring_count       = vs::power(base, ring_order);
    const order_t  ring_shift_order = order - ring_order;
    const number_t ring_shift       = vs::modular::power<number_t>(base, ring_shift_order, factor);

    for (number_t i = ring_count / base; i < ring_count; ++i)
    { ring[((i * ring_shift) % factor + reversed[i] % factor) % factor]++; }

    const number_t  middle_count_exponent = middle_order + is_order_odd;
    const number_t  middle_count          = vs::power(base, middle_count_exponent);
    const number_t  middle_shift          = vs::modular::complement(ring_count, factor);
    const number_t* reversed_f            = reversed_fixed[middle_order + is_order_odd - 1U];
    const number_t  divider               = vs::power(base, is_order_odd);

    number_t result = 0U;
    for (number_t i = 0U; i < middle_count; ++i)
    { result += ring[((((i / divider) * middle_count) % factor + reversed_f[i] % factor) * middle_shift) % factor]; }

    delete[] ring;
    return result;
}

number_t palindrome_single_ring_even(const order_t&               order,
                                     const number_t&              factor,
                                     const number_t* const        reversed,
                                     const number_t* const* const reversed_fixed)
{
    assert(0U == (order & 1U));

    constexpr number_t base         = 10ULL;
    constexpr order_t  middle_order = 4U;

    number_t* ring_odd  = new number_t[factor];
    number_t* ring_even = new number_t[factor];
    for (number_t i = 0U; i < factor; ++i) { ring_odd[i] = ring_even[i] = 0U; }

    const order_t  ring_order       = (order >> 1U) - middle_order;
    const order_t  ring_shift_order = order - ring_order - 1U;
    const number_t ring_shift       = vs::modular::power<number_t>(base, ring_shift_order, factor);
    const number_t ring_count       = vs::power(base, ring_order);

    for (number_t i = ring_count / base; i < ring_count; ++i)
    {
        const number_t left_ring = (i * ring_shift) % factor;
        ring_odd[(left_ring + reversed[i]) % factor]++;
        ring_even[(left_ring * base + reversed[i]) % factor]++;
    }

    const number_t        middle_count = vs::power(base, middle_order);
    const number_t        middle_shift = vs::modular::complement(ring_count, factor);
    const number_t* const reversed_f   = reversed_fixed[middle_order - 1U];

    number_t result_odd  = 0U;
    number_t result_even = 0U;
    for (number_t i = 0U; i < middle_count; ++i)
    {
        result_odd += ring_odd[((((i / base) * middle_count + reversed_f[i]) % factor) * middle_shift) % factor];
        result_even += ring_even[(((i * middle_count + reversed_f[i]) % factor) * middle_shift) % factor];
    }

    if (log_debug_flag) cout << result_odd << '\t' << result_even << endl;

    delete[] ring_odd;
    delete[] ring_even;

    return result_odd + result_even;
}

number_t
palindrom_two_ring(const order_t& order, const number_t& prime, number_t* middle_pallindrome, number_t* reversed)
{
    const order_t prime_effective_length = (order & 1U) ? (5U) : (6U);

    const order_t rings_length = (order - prime_effective_length) >> 1U;

    const order_t ring_i_length = rings_length >> 1U;
    const order_t ring_o_length = rings_length - ring_i_length;

    const order_t shift_ol = order - ring_o_length;
    const order_t shift_il = shift_ol - ring_i_length;
    const order_t shift_ir = ring_o_length;
    const order_t shift_m  = rings_length;

    number_t* ring_o = new number_t[prime];
    number_t* ring_i = new number_t[prime];

    for (number_t i = 0U; i < prime; ++i) { ring_o[i] = ring_i[i] = 0U; }
    // fill outer ring

    const number_t ten_ol = vs::modular::power<number_t>(10ULL, shift_ol, prime);
    const number_t or_up  = vs::power(10ULL, ring_o_length);
    const number_t or_dn  = or_up / 10ULL;

    for (number_t i = or_dn; i < or_up; ++i) { ring_o[((i * ten_ol) % prime + reversed[i]) % prime]++; }

    // fill inner ring

    const number_t ten_il = vs::modular::power<number_t>(10ULL, shift_il, prime);
    const number_t ten_ir = vs::modular::power<number_t>(10ULL, shift_ir, prime);
    const number_t ir_up  = vs::power(10ULL, ring_i_length);

    for (number_t i = 0ULL; i < ir_up; ++i)
    { ring_i[((i * ten_il) % prime + (vs::reverseNumberFixedLength(i, ring_i_length) * ten_ir) % prime) % prime]++; }

    // play with middle part

    const number_t ten_m  = vs::modular::power<number_t>(10ULL, shift_m, prime);
    number_t       result = 0U;

    for (number_t i = 0U; i < 1000U; ++i)
    {
        number_t cc = vs::modular::complement(middle_pallindrome[i] * ten_m, prime);
        for (number_t j = 0; j < prime; ++j) { result += ring_o[j] * ring_i[((cc < j) ? (cc + prime) : (cc)) - j]; }
    }
    delete[] ring_o;
    delete[] ring_i;
    return result;
}

number_t palindrome_double_ring_even(const order_t&               order,
                                     const number_t&              factor,
                                     const number_t* const        reversed,
                                     const number_t* const* const reversed_fixed)
{
    constexpr number_t base           = 10ULL;
    constexpr order_t  ring_order_max = 7U;
    const order_t middle_order = (order < (ring_order_max << 2U) + 1U) ? 0U : ((order - (ring_order_max << 2U)) >> 1U);

    number_t* ring_outer_odd  = new number_t[factor];
    number_t* ring_outer_even = new number_t[factor];
    number_t* ring_inner_odd  = new number_t[factor];
    number_t* ring_inner_even = new number_t[factor];

    for (number_t i = 0U; i < factor; ++i)
    { ring_outer_odd[i] = ring_outer_even[i] = ring_inner_odd[i] = ring_inner_even[i] = 0U; }

    order_t ring_outer_order = ring_order_max;
    order_t ring_inner_order = ring_order_max;

    if (0U == middle_order)
    {
        ring_outer_order = (order >> 2U);
        ring_inner_order = (order >> 1U) - ring_outer_order;
    }

    const order_t ring_inner_shift_right_order = ring_outer_order;
    const order_t ring_outer_shift_left_order  = order - ring_outer_order - 1U;
    const order_t middle_shift_order           = ring_outer_order + ring_inner_order;
    const order_t ring_inner_shift_left_order =
        (0U == middle_order) ? middle_shift_order : ring_outer_shift_left_order - ring_inner_order;

    if (log_debug_flag)
    {
        cout << ring_outer_order << '\t' << ring_outer_shift_left_order << '\t'
             << ((!middle_order) ? (ring_inner_order - 1U) : ring_inner_order) << '\t' << ring_inner_shift_left_order
             << '\t' << ((!middle_order) ? middle_order : middle_order - 1U) << '\t' << middle_shift_order << '\t'
             << ring_inner_order << '\t' << ring_inner_shift_right_order << '\t' << ring_outer_order << endl;
    }

    const number_t ring_inner_shift_right = vs::modular::power<number_t>(base, ring_inner_shift_right_order, factor);
    const number_t ring_outer_shift_left  = vs::modular::power<number_t>(base, ring_outer_shift_left_order, factor);
    const number_t middle_shift =
        vs::modular::complement(vs::modular::power<number_t>(base, middle_shift_order, factor), factor);
    const number_t ring_inner_shift_left = vs::modular::power<number_t>(base, ring_inner_shift_left_order, factor);

    const number_t ring_inner_count = vs::power(base, ring_inner_order);
    const number_t ring_outer_count = vs::power(base, ring_outer_order);

    for (number_t i = ring_outer_count / base; i < ring_outer_count; ++i)
    {
        const number_t left_outer = (i * ring_outer_shift_left) % factor;
        ring_outer_odd[(left_outer + reversed[i]) % factor]++;
        ring_outer_even[(left_outer * base + reversed[i]) % factor]++;
    }

    for (number_t i = 0U; i < ring_inner_count; ++i)
    {
        const number_t r = (vs::reverseNumberFixedLength(i, ring_inner_order) * ring_inner_shift_right) % factor;
        if (0U == middle_order)
        {
            ring_inner_odd[(((i / base) * ring_inner_shift_left) + r) % factor]++;
            ring_inner_even[((i * ring_inner_shift_left) + r) % factor]++;
        }
        else
        {
            ring_inner_odd[(i * ring_inner_shift_left + r) % factor]++;
            ring_inner_even[(i * base * ring_inner_shift_left + r) % factor]++;
        }
    }

    number_t              middle_count = vs::power(base, middle_order);
    const number_t* const reversed_f   = reversed_fixed[(0U == middle_order) ? 0U : middle_order - 1U];

    number_t result_odd  = 0U;
    number_t result_even = 0U;
    for (number_t i = 0U; i < middle_count; ++i)
    {
        number_t c_odd  = (((i / base) * middle_count + reversed_f[i]) * middle_shift) % factor;
        number_t c_even = ((i * middle_count + reversed_f[i]) * middle_shift) % factor;

        for (number_t j = 0U; j < factor; ++j)
        {
            result_odd += ring_outer_odd[j] * ring_inner_odd[((c_odd < j) ? (c_odd + factor) : c_odd) - j];
            result_even += ring_outer_even[j] * ring_inner_even[((c_even < j) ? (c_even + factor) : c_even) - j];
        }
    }
    if (log_debug_flag) cout << result_odd << '\t' << result_even << endl;

    delete[] ring_outer_odd;
    delete[] ring_outer_even;
    delete[] ring_inner_odd;
    delete[] ring_inner_even;
    return result_even + result_odd;
}

int main( )
{
    constexpr number_t factor = 10000019ULL;
    constexpr number_t base   = 10ULL;
    number_t           result = 0U;

    auto s = high_resolution_clock::now( );

    constexpr order_t reversed_order = 7U;
    const number_t    reversed_count = vs::power(10ULL, reversed_order);
    number_t* const   reversed       = new number_t[reversed_count];

    for (number_t i = 0U; i < reversed_count; ++i) reversed[i] = vs::reverseNumber(i);

    constexpr order_t reversed_fixed_order = 4U;
    number_t**        reversed_fixed       = new number_t*[reversed_fixed_order];

    number_t reversed_fixed_count = 1U;
    for (order_t i = 0U; i < reversed_fixed_order; ++i)
    {
        reversed_fixed_count *= base;
        reversed_fixed[i]    = new number_t[reversed_fixed_count];
        reversed_fixed[i][0] = 0U;
    }

    reversed_fixed_count = 1U;
    for (order_t i = 0U; i < reversed_fixed_order; ++i)
    {
        reversed_fixed_count *= base;
        for (number_t j = reversed_fixed_count / base; j < reversed_fixed_count; ++j)
        {
            reversed_fixed[i][j] = reversed[j];
            for (order_t k = i + 1U; k < reversed_fixed_order; ++k)
            { reversed_fixed[k][j] = base * reversed_fixed[k - 1U][j]; }
        }
    }
    std::chrono::milliseconds::rep prp =
        duration_cast<std::chrono::milliseconds>(high_resolution_clock::now( ) - s).count( );
    cout << "[Preparation]\t" << prp << endl;

    std::chrono::milliseconds::rep total_ticks = 0U;
    for (order_t i = 16U; i < 23U; i += 2U)
    {
        s = high_resolution_clock::now( );

        number_t r = palindrome_single_ring_even(i, factor, reversed, reversed_fixed);
        result += r;
        std::chrono::milliseconds::rep d =
            duration_cast<std::chrono::milliseconds>(high_resolution_clock::now( ) - s).count( );

        total_ticks += d;
        cout << '[' << i - 1U << '-' << i << "]\t" << std::setw(10) << r << std::setw(11) << result << '\t' << d << '\t'
             << total_ticks << endl;
    }
    for (order_t i = 24U; i < 33U; i += 2)
    {
        s = high_resolution_clock::now( );

        number_t r = palindrome_double_ring_even(i, factor, reversed, reversed_fixed);
        result += r;
        std::chrono::milliseconds::rep d =
            duration_cast<std::chrono::milliseconds>(high_resolution_clock::now( ) - s).count( );

        total_ticks += d;
        cout << '[' << i - 1U << '-' << i << "]\t" << std::setw(10) << r << std::setw(11) << result << '\t' << d << '\t'
             << total_ticks << endl;
    }

    cout << "[Calculation]\t"
         << duration_cast<std::chrono::duration<double>>(std::chrono::milliseconds(total_ticks)).count( ) << endl;
    cout << "[Total]\t\t"
         << duration_cast<std::chrono::duration<double>>(std::chrono::milliseconds(total_ticks + prp)).count( ) << endl;
    return 0;
}
