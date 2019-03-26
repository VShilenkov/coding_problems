#include <iostream>


using T = unsigned int;

constexpr T ones[]  = {0U, 3U, 3U, 5U, 4U, 4U, 3U, 5U, 5U, 4U};
constexpr T teens[] = {0U, 6U, 6U, 8U, 8U, 7U, 7U, 9U, 8U, 8U};
constexpr T tens[]  = {0U, 3U, 6U, 6U, 5U, 5U, 5U, 7U, 6U, 6U};

constexpr T H  = 7U;    // "hundred" length
constexpr T A  = 3U;    // "and" length
constexpr T Th = 8U;    // "thousand" length

inline void naive( )
{
    constexpr T base              = 10U;
    constexpr T hundred           = base * base;
    constexpr T teens_lower_limit = base;
    constexpr T teens_upper_limit = base << 1U;

    T sum_first_ninty_nine = 0U;

    for (T i = 1U; i < hundred; ++i)
    {
        sum_first_ninty_nine +=
            (((teens_lower_limit < i) && (i < teens_upper_limit)) ? (teens[i - base])
                                                                  : (tens[i / base] + ones[i % base]));
    }

    T sum_total = ones[1] + Th + base * sum_first_ninty_nine - (base - 1U) * A;
    // onethousand_word_lenth + (sum till hundred for each hundred) - and_word_lenth for 100, 200, 300,...,900
    for (T i = 1U; i < base; ++i)
    {
        sum_total += (ones[i] + H + A) * hundred;    // go trough each hundred 100, 200, ..., 900
    }
    std::cout << sum_total;
}

inline void algo( )
{
    T sum_ones = 0U, sum_teens = 0U, sum_tens = 0U;

    for (T i = 1U; i < 10U; ++i)
    {
        sum_ones += ones[i];
        sum_teens += teens[i];
        sum_tens += tens[i];
    }

    sum_tens -= tens[1];

    std::cout << 10 * (19 * sum_ones + tens[1] + sum_teens + 10 * sum_tens) + 9 * (100 * (H + A) - A) + ones[1] + Th;
}

int main( )
{
    naive( );
    std::cout << std::endl;
    algo( );
    return 0;
}
