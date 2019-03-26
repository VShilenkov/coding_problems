#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using T = unsigned int;
using V = std::vector<T>;

int main(int argc, char* argv[])
{
    if (argc != 2U) { return 1; }

    std::ifstream input;
    input.open(argv[1]);
    if (!input) { return 1; }

    T expect_to_read = 1U;
    V previus_line(100U, 0U);

    for (T i = 0U; i < expect_to_read; ++i)
    {
        T read = 0U;
        V current_line(100U, 0U); // contains max valued path to the current item

        while ((read < expect_to_read) && (input))
        {
            T number = 0U;
            input >> number;
            if (input)
            {
                T l = 0U;
                T r = 0U;
                if (read == 0U) { l = previus_line[read]; }
                else if (read == expect_to_read - 1U)
                {
                    l = previus_line[read - 1U];
                }
                else
                {
                    l = previus_line[read - 1U];
                    r = previus_line[read];
                }

                current_line[read] = number + ((l < r) ? (r) : (l));
                ++read;
            }
        }

        if (0U != read)
        {
            ++expect_to_read;
            previus_line = current_line;
        }
    }

    input.close( );
    std::cout << *std::max_element(previus_line.cbegin( ), previus_line.cend( ));
    return 0;
}