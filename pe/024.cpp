#include <algorithm>
#include <iostream>
#include <list>

void naive(unsigned permutation_number)
{
    unsigned a[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    for (unsigned i = 0; i < permutation_number; ++i)
    {
        std::next_permutation(a, a + 10U);
        // std::cout << std::endl;
    }
    for (unsigned c : a) { std::cout << c; }
    std::cout << std::endl;
}

void factorial_representaion(unsigned permutation_number)
{
    unsigned factor_representation[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (unsigned i = 1U; i < 11U; ++i)
    {
        factor_representation[10U - i] = permutation_number % i;
        permutation_number /= i;
    }

    std::list<unsigned> permutation = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for (unsigned i = 0U; i < 10U; ++i)
    {
        auto p = std::next(permutation.begin( ), factor_representation[i]);
        std::cout << *p;
        permutation.erase(p);
    }
    std::cout << std::endl;
}

int main( )
{
    unsigned permutation_number = 1'000'000U - 1U;

    naive(permutation_number);
    factorial_representaion(permutation_number);

    return 0;
}