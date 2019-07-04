#ifndef VS_NUMBER_HPP
#define VS_NUMBER_HPP

#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

namespace vs {

typedef unsigned short int ns_base_t;
typedef unsigned short int ns_length_t;

constexpr ns_base_t default_base = 10U;

template<typename T, typename U>
inline typename std::common_type<T, U>::type max(const T& l, const U& r)
{
    return ((l < r) ? (r) : (l));
}

template<typename T, typename U>
inline typename std::common_type<T, U>::type min(const T& l, const U& r)
{
    return ((l < r) ? (l) : (r));
}

template<typename T, typename U>
typename std::common_type<T, U>::type gcd(T lhs, U rhs)
{
    while (rhs)
    {
        lhs %= rhs;
        std::swap(lhs, rhs);
    }
    return lhs;
}

template<typename T, typename U>
typename std::common_type<T, U>::type lcm(T lhs, U rhs)
{
    return (lhs && rhs) ? (lhs / vs::gcd(lhs, rhs) * rhs) : (0U);
}

template<typename T, typename B = ns_base_t>
inline bool isDigit(const T& number, const B& base = default_base)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type must be unsigned integer type");
    static_assert(std::is_integral<B>::value && std::is_unsigned<B>::value, "base type must be unsigned integer type");
    return (number < base);
}

template<typename T, typename E>
inline T power(T number, E exponent)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type must be unsigned integer type");
    static_assert(std::is_integral<E>::value && std::is_unsigned<E>::value,
                  "exponent type must be unsigned integer type");

    T result = 1U;
    while (0U < exponent)
    {
        if (exponent & 1U) { result *= number; }
        number *= number;
        exponent >>= 1U;
    }
    return result;
}

template<typename T, typename S = ns_length_t, typename B = ns_base_t>
inline T shiftLeft(const T& number, const S& shift, const B& base = default_base)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type must be unsigned integer type");
    static_assert(std::is_integral<S>::value && std::is_unsigned<S>::value, "shift type must be unsigned integer type");
    static_assert(std::is_integral<B>::value && std::is_unsigned<B>::value, "base type must be unsigned integer type");

    static_assert(std::is_convertible<B, T>::value, "base type must be convertible to number type");

    return (shift == 0U) ? (number) : ((shift == 1U) ? (number * base) : (number * power<T, S>(base, shift)));
}

template<typename T, typename S = ns_length_t, typename B = ns_base_t>
inline T shiftRight(const T& number, const S& shift, const B& base = default_base)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type must be unsigned integer type");
    static_assert(std::is_integral<S>::value && std::is_unsigned<S>::value, "shift type must be unsigned integer type");
    static_assert(std::is_integral<B>::value && std::is_unsigned<B>::value, "base type must be unsigned integer type");

    static_assert(std::is_convertible<B, T>::value, "base type must be convertible to number type");

    return (shift == 0U) ? (number) : ((shift == 1U) ? (number / base) : (number / power<T, S>(base, shift)));
}

template<typename T, typename B = ns_base_t>
inline std::size_t countDigits(T number, const B& base = default_base)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type must be unsigned integer type");
    static_assert(std::is_integral<B>::value && std::is_unsigned<B>::value, "base type must be unsigned integer type");

    std::size_t result = 0U;
    // if (0U == number) return 1U;
    while (0U < number)
    {
        number = shiftRight(number, 1U, base);
        ++result;
    }

    return result;
}

template<typename T, typename B = unsigned short int>
T reverseNumber(T number, const B& base = default_base)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type must be unsigned integer type");
    static_assert(std::is_integral<B>::value && std::is_unsigned<B>::value, "base type must be unsigned integer type");

    static_assert(std::is_convertible<B, T>::value, "base type must be convertible to number type");

    T result = 0U;

    while (0U < number)
    {
        result = shiftLeft(result, 1U);
        result += (number % base);
        number = shiftRight(number, 1U);
    }
    return result;
}

template<typename T, typename U = unsigned short int, typename B = unsigned short int>
T reverseNumberFixedLength(T number, U length = 0U, const B& base = default_base)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type must be unsigned integer type");
    static_assert(std::is_integral<U>::value && std::is_unsigned<U>::value,
                  "length type must be unsigned integer type");
    static_assert(std::is_integral<B>::value && std::is_unsigned<B>::value, "base type must be unsigned integer type");

    static_assert(std::is_convertible<B, T>::value, "base type must be convertible to number type");

    T result = 0U;

    while (0U < number)
    {
        result = shiftLeft(result, 1U);
        result += (number % base);
        number = shiftRight(number, 1U);
        if (0U < length) length--;
    }

    return shiftLeft(result, length);
}

template<typename T>
inline T integerSquareRootNewton(const T& number)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type must be unsigned integer type");
    T x = number;
    T y = (x + number / x) >> 1U;

    while ((y - x) > 1U)
    {
        x = y;
        y = (x + number / x) >> 1U;
    }
    return x;
}

namespace prime {

template<typename ForwardIt>
void init(ForwardIt b, ForwardIt e)
{
    static_assert(
        std::is_base_of<std::forward_iterator_tag, typename std::iterator_traits<ForwardIt>::iterator_category>::value,
        "ForwardIt must satisfy forward_iterator");
    static_assert(std::is_same<typename std::iterator_traits<ForwardIt>::value_type, bool>::value,
                  "ForwardIt value type must be bool");

    for (ForwardIt i(b); i < e; ++i) { *i = true; }
    ForwardIt i(b);
    ++i;
    for (++i; i < e; ++i)
    {
        if (*i)
        {
            ForwardIt                                                 j(i);
            typename std::iterator_traits<ForwardIt>::difference_type d(std::distance(b, i));
            for (std::advance(j, d); j < e; std::advance(j, d)) { *j = false; }
        }
    }
}

template<typename T, typename ForwardIt, typename OutputIt_f, typename OutputIt_p>
bool factorise(T n, ForwardIt primes_b, ForwardIt primes_e, OutputIt_f factors_b, OutputIt_p powers_b)
{
    static_assert(
        std::is_base_of<std::forward_iterator_tag, typename std::iterator_traits<ForwardIt>::iterator_category>::value,
        "ForwardIt must satisfy forward_iterator");
    static_assert(std::is_same<typename std::iterator_traits<ForwardIt>::value_type, bool>::value,
                  "ForwardIt value type must be bool");

    ForwardIt primes_limit(primes_b);
    std::advance(primes_limit, min(integerSquareRootNewton(n) + 1U, std::distance(primes_b, primes_e)));
    ForwardIt it(primes_b);

    for (std::advance(it, 2U); (it < primes_limit) && (1U < n); ++it)
    {
        if (*it)
        {
            T p = static_cast<T>(std::distance(primes_b, it));
            if (0U == n % p)
            {
                *factors_b++ = p;
                T power(0U);
                for (; 0U == n % p; ++power) { n /= p; }
                *powers_b++ = power;
            }
        }
    }

    if ((n > 1) && (n < std::distance(primes_b, primes_e)))
    {
        ForwardIt ff(primes_b);
        std::advance(ff, n);
        if (*ff)
        {
            *factors_b++ = n;
            *powers_b++  = 1U;
            n            = 1U;
        }
    }

    return (n == 1U);
}

}    // namespace prime

template<typename T, typename ForwardIt>
T EulersPhi(T n, ForwardIt primes_b, ForwardIt primes_e)
{
    static_assert(
        std::is_base_of<std::forward_iterator_tag, typename std::iterator_traits<ForwardIt>::iterator_category>::value,
        "ForwardIt must satisfy forward_iterator");
    static_assert(std::is_same<typename std::iterator_traits<ForwardIt>::value_type, bool>::value,
                  "ForwardIt value type must be bool");

    std::vector<T> factors, powers;
    T              result = n;
    if (prime::factorise(n, primes_b, primes_e, std::back_inserter(factors), std::back_inserter(powers)))
    {
        for (const T& factor : factors)
        {
            result /= factor;
            result *= (factor - 1U);
        }
    }

    return result;
}

namespace modular {

template<typename T, typename M = T>
inline typename std::common_type<T, M>::type complement(const T& number, const M& modulo)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type must be unsigned integer type");
    static_assert(std::is_integral<M>::value && std::is_unsigned<M>::value,
                  "modulo type must be unsigned integer type");

    const T n = number % modulo;
    return (0U == n) ? (0U) : (modulo - n);
}

template<typename T, typename U = T, typename M = T>
inline typename std::common_type<T, U, M>::type sub(const T& lhs, const U& rhs, const M& modulo)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type T must be unsigned integer type");
    static_assert(std::is_integral<U>::value && std::is_unsigned<U>::value,
                  "number type U must be unsigned integer type");
    static_assert(std::is_integral<M>::value && std::is_unsigned<M>::value,
                  "modulo type must be unsigned integer type");

    T l = lhs % modulo;
    U r = rhs % modulo;
    if (l == r) return 0U;
    return (r < l) ? (l - r) : complement(r, modulo) + l;
}

template<typename T, typename U = T, typename M = T>
inline typename std::common_type<T, U, M>::type add(const T& lhs, const U& rhs, const M& modulo)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type T must be unsigned integer type");
    static_assert(std::is_integral<U>::value && std::is_unsigned<U>::value,
                  "number type U must be unsigned integer type");
    static_assert(std::is_integral<M>::value && std::is_unsigned<M>::value,
                  "modulo type must be unsigned integer type");

    T l = lhs % modulo;
    U r = rhs % modulo;

    return (0U == r) ? (l) : sub(l, complement(r, modulo), modulo);
}

template<typename T, typename U = T, typename M = T>
inline T multiply(const T& lhs, const U& rhs, const M& modulo)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type T must be unsigned integer type");
    static_assert(std::is_integral<U>::value && std::is_unsigned<U>::value,
                  "number type U must be unsigned integer type");
    static_assert(std::is_integral<M>::value && std::is_unsigned<M>::value,
                  "modulo type must be unsigned integer type");

    typename std::common_type<T, U>::type lfactor = lhs % modulo;
    typename std::common_type<T, U>::type rfactor = rhs % modulo;

    if (lfactor < rfactor) { std::swap(lfactor, rfactor); }

    T product = 0U;
    while (0U < rfactor)
    {
        if (rfactor & 1U) { product = add(product, lfactor, modulo); }
        lfactor = add(lfactor, lfactor, modulo);
        rfactor >>= 1U;
    }

    return product;
}

template<typename T, typename E = T, typename M = T>
inline T power(T base, T exponent, const T& modulo)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value, "base type must be unsigned integer type");
    static_assert(std::is_integral<E>::value && std::is_unsigned<E>::value,
                  "exponent type must be unsigned integer type");
    static_assert(std::is_integral<M>::value && std::is_unsigned<M>::value,
                  "modulo type must be unsigned integer type");

    base %= modulo;
    T result = 1U;
    while (0U < exponent)
    {
        if (exponent & 1U) { result = multiply(result, base, modulo); }
        base = multiply(base, base, modulo);

        exponent >>= 1;
    }
    return result;
}

template<typename T, typename M = T>
inline T multiplicativeOrder(const T& number, const M& modulo)
{
    T power = 1U;
    if (gcd(number, modulo) == 1U)
    {
        for (T p = number % modulo; p != 1UL; p = multiply(p, number, modulo)) { ++power; }
    }
    else
    {
        power = 0U;
    }
    return power;
}

template<typename T, typename M>
inline T inverse(T number, const M& modulo)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type must be unsigned integer type");
    static_assert(std::is_integral<M>::value && std::is_unsigned<M>::value,
                  "modulo type must be unsigned integer type");

    T order = multiplicativeOrder(number, modulo);
    return (order) ? power(number, order - 1U, modulo) : 0U;
}

template<typename T, typename S = ns_length_t, typename M, typename B = ns_base_t>
inline T shiftLeft(const T& number, const S& shift, const M& modulo, const B& base = default_base)
{
    return ((number % modulo) * power(base, shift, modulo)) % modulo;
}

template<typename T, typename ForwardIt>
inline bool isPrimitiveRoot(const T& number, const T& modulo, ForwardIt primes_b, ForwardIt primes_e)
{
    T              eulerPhi = vs::EulersPhi(modulo, primes_b, primes_e);
    std::vector<T> factors, powers;
    bool           result =
        prime::factorise(eulerPhi, primes_b, primes_e, std::back_inserter(factors), std::back_inserter(powers));

    if (result)
    {
        for (typename std::vector<T>::const_iterator f = factors.begin( );
             f < factors.end( ) && (result = (power(number, eulerPhi / (*f), modulo) != 1U));
             ++f)
        {}
    }

    return result;
}

}    // namespace modular

}    // namespace vs

#endif    // VS_NUMBER_HPP
