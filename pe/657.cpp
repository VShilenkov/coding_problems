#include <iostream>
#include <vector>

using std::cout;
using std::endl;

int main( )
{
    typedef unsigned                  character_t;
    typedef std::vector<character_t>  word_t;
    typedef std::vector<word_t>       dictionary_t;
    typedef std::vector<dictionary_t> library_t;

    constexpr size_t alphabet = 3U;
    constexpr size_t complete = (1 << alphabet) - 1U;
    constexpr size_t length   = 7U;
    library_t        library;

    dictionary_t initial;

    for (size_t c = 0; c < alphabet; ++c)
    {
        word_t w;
        w.push_back(c);
        initial.push_back(w);
    }

    library.push_back(initial);

    for (size_t i = 0; i < length; ++i)
    {
        dictionary_t d;
        for (word_t w : library[i])
        {
            for (word_t v : library[0])
            {
                word_t n(w);
                for (character_t c : v) n.push_back(c);
                d.push_back(n);
            }
        }
        library.push_back(d);
    }

    size_t total_uncomplete = 0U;
    for (dictionary_t d : library)
    {
        size_t word_counter   = 0U;
        size_t complete_words = 0U;
        for (word_t w : d)
        {
            ++word_counter;
            size_t completness = 0U;
            for (character_t c : w) (completness |= (1U << c));
            if (completness == complete) ++complete_words;
        }
        size_t uncomplete = word_counter - complete_words;
        total_uncomplete += uncomplete;
        cout << word_counter << '\t' << complete_words << '\t' << uncomplete << '\t' << total_uncomplete + 1U<< endl;
    }

    return 0;
}