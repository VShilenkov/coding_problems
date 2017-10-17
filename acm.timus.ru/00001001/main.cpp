/*
 *  Для каждого числа Ai, начиная с последнего и заканчивая первым, в отдельной строке вывести его квадратный корень не 
 *  менее чем с четырьмя знаками после десятичной точки.
 *
 *  @see    http://acm.timus.ru/problem.aspx?space=1&num=1001
 */

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <iterator>
#include <vector>
#include <cmath>

typedef long double value_t;
typedef std::vector<value_t> vector_t;
typedef vector_t::iterator vector_fwd_iter_t;
typedef std::reverse_iterator<vector_fwd_iter_t> vector_rev_iter_t;


int main()
{
    vector_t v;

    value_t f;
    std::cout << std::fixed << std::setprecision(5);
    while (std::cin >> f) 
    {
        value_t p = std::pow(f, 0.5);
        value_t s = std::sqrt(f);
        v.push_back(p);
        std::cout << f << " " << f - p*p << " " << p*p << " " << p
                              << f - s*s << " " << s*s << " " << s << "|" << std::endl;
    }

    std::ostream_iterator<float> os_it(std::cout, "\n");

    vector_fwd_iter_t o_from(v.begin());
    vector_fwd_iter_t o_to(v.end());
    vector_rev_iter_t from(o_to);
    vector_rev_iter_t to(o_from);
    std::copy (from, to, os_it );

    return 0;
}