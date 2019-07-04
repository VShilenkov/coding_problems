#include "number.hpp"

#include <iostream>

int main( )
{
    typedef unsigned long int T;

    constexpr T       N = 1'000UL;
    std::vector<bool> is_prime(N, true);

    vs::prime::init(is_prime.begin( ), is_prime.end( ));

    std::vector<bool>::const_reverse_iterator it = is_prime.rbegin( );

    auto primitiveRootTest = [&is_prime](const T& number) {
        return (true) ? vs::modular::isPrimitiveRoot(10UL, number, is_prime.cbegin( ), is_prime.cend( ))
                      : (vs::modular::multiplicativeOrder(10UL, number) + 1U == number);
    };

    auto prime = [&it, &is_prime] { return std::distance(it, is_prime.crend( )) - 1U; };

    while (!(*it && primitiveRootTest(static_cast<T>(prime( )))) && (6U < prime( ))) { ++it; }

    std::cout << prime( ) << std::endl;
    return 0;
}
