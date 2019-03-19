#ifndef VS_NUMBER_HPP
#define VS_NUMBER_HPP

#include <type_traits>

namespace vs {

typedef unsigned short int ns_base_t;
typedef unsigned short int ns_length_t;

constexpr ns_base_t default_base = 10U;

template<typename T>
inline T max(const T& l, const T& r)
{
    return ((l < r) ? (r) : (l));
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

template<typename T, typename M>
inline M complementModular(const T& number, const M& modulo)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type must be unsigned integer type");
    static_assert(std::is_integral<M>::value && std::is_unsigned<M>::value,
                  "modulo type must be unsigned integer type");

    return (modulo - number % modulo) % modulo;
}

template<typename T, typename E, typename M>
T powerModular(T number, E exponent, const M& modulo)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type must be unsigned integer type");
    static_assert(std::is_integral<E>::value && std::is_unsigned<E>::value,
                  "exponent type must be unsigned integer type");
    static_assert(std::is_integral<M>::value && std::is_unsigned<M>::value,
                  "modulo type must be unsigned integer type");

    number %= modulo;
    T result = 1U;
    while (0U < exponent)
    {
        if (exponent & 1U)
        {
            result *= number;
            result %= modulo;
        }
        number *= number;
        number %= modulo;
        exponent >>= 1;
    }
    return result;
}

template<typename T, typename M>
inline T inverseModular(T number, const M& modulo)
{
    static_assert(std::is_integral<T>::value && std::is_unsigned<T>::value,
                  "number type must be unsigned integer type");
    static_assert(std::is_integral<M>::value && std::is_unsigned<M>::value,
                  "modulo type must be unsigned integer type");

    return vs::powerModular(number, modulo - 2U, modulo);
}

template<typename T, typename S = ns_length_t, typename M, typename B = ns_base_t>
inline T shiftLeftModular(const T& number, const S& shift, const M& modulo, const B& base = default_base)
{
    return ((number % modulo) * powerModular<T, S, M>(base, shift, modulo)) % modulo;
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

}    // namespace vs

#endif    // VS_NUMBER_HPP