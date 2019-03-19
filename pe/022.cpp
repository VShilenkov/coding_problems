#include <fstream>
#include <iostream>
#include <map>

int main(int argc, char* argv[])
{
    if (argc != 2U) { return 1; }

    std::ifstream input;
    input.open(argv[1]);
    if (!input) { return 1; }

    std::map<std::string, unsigned int> d;
    std::string                         name;
    while (input >> name)
    {
        unsigned score = 0U;
        for (char c : name) { score += (c - 'A') + 1U; }
        d[name] = score;
    }

    size_t   index = 1U;
    unsigned sum   = 0U;
    for (auto a = d.cbegin( ); a != d.cend( ); ++a)
    {
        unsigned score = index * a->second;
        sum += score;
        ++index;
    }
    std::cout << sum;
    return 0;
}