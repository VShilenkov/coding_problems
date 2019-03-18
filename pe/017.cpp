#include <iostream>

using T = unsigned int;

int main( )
{
    constexpr T ones[]  = {0U, 3U, 3U, 5U, 4U, 4U, 3U, 5U, 5U, 4U};
    constexpr T teens[] = {0U, 6U, 6U, 8U, 8U, 7U, 7U, 9U, 8U, 8U};
    constexpr T tens[]  = {0U, 3U, 6U, 6U, 5U, 5U, 5U, 7U, 6U, 6U};

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

    constexpr T hundred_word_length = 7U;
    constexpr T and_word_lenth      = 3U;
    constexpr T prefix              = hundred_word_length + and_word_lenth;

    T sum_total = 11U + base * sum_first_ninty_nine - (base - 1U) * and_word_lenth;
    // onethousand_word_lenth + (sum till hundred for each hundred) - and_word_lenth for 100, 200, 300,...,900
    for (T i = 1U; i < base; ++i)
    { sum_total += (ones[i] + prefix) * hundred; }    // go trough each hundred 100, 200, ..., 900
    std::cout << sum_total;
}