#include <iostream>
#include <vector>

using T = unsigned long long int;

class long_number
{
public:
    long_number(const T& t)
        : impl(1U, t)
    {}

    long_number& operator*=(const T& m)
    {
        T carry = 0U;
        for (size_t i = 0U; i < impl.size( ); ++i)
        {
            T a     = impl[i] * m + carry;
            impl[i] = a % word;
            carry   = a / word;
        }
        if (carry) impl.push_back(carry);
        return *this;
    }

    T digit_sum( ) const
    {
        T a = 0U;
        for (T c : impl)
        {
            while (c > 0U)
            {
                a += c % 10U;
                c /= 10U;
            }
        }
        return a;
    }

private:
    static constexpr unsigned long long int word = 1'000'000'000ULL;
    std::vector<T>                          impl;
};

int main( )
{
    long_number a(1U);
    size_t      two_counter = 0;
    for (T i = 2U; i <= 100U; ++i)
    {
        T ii = i;
        while ((ii & 1U) == 0U)
        {
            ++two_counter;
            ii >>= 1U;
        }
        while ((ii % 5U) == 0)
        {
            --two_counter;
            ii /= 5U;
        }

        a *= ii;
    }

    while (two_counter > 0U)
    {
        a *= 2U;
        --two_counter;
    }

    std::cout << a.digit_sum( );
    return 0;
}