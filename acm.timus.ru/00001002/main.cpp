/*
 *  @see    http://acm.timus.ru/problem.aspx?space=1&num=1002
 */

#include <iostream>
#include <string>
#include <set>
#include <cassert>

static const size_t letters_number = 26U;
static const unsigned char conversion_array[] = {'2', '2', '2', // a, b, c
                                                 '3', '3', '3', // d, e, f
                                                 '4', '4',      // g, h
                                                 '1', '1',      // i, j
                                                 '5', '5',      // k, l
                                                 '6', '6',      // m, n
                                                 '0',           // o
                                                 '7',           // p
                                                 '0',           // q
                                                 '7', '7',      // r, s
                                                 '8', '8', '8', // t, u, v
                                                 '9', '9', '9', // w, x, y
                                                 '0'};          // z

std::string encode(const std::string & input)
{
    std::string::const_iterator i = input.cbegin();
    const std::string::const_iterator e = input.cend();
    std::string result = "";
    while (i!= e) result += conversion_array[*i++ - 'a'];
    return result;
}

bool startWith(const std::string encodedWord, const std::string simpleWord)
{
    assert(simpleWord.length() > 0U);
    if (simpleWord.length() > encodedWord.length()) return false;
    const std::string t = encode(simpleWord);
    return (encodedWord.substr(0U, simpleWord.length()) == t);
}

size_t solve( const std::string & phNumber
            , const std::set<std::string> & dic
            , std::string & decodedString
            , size_t depth)
{
    assert(depth < 30U);
    std::set<std::string>::const_iterator e = dic.cend();
    size_t shortest = phNumber.length();
    std::string bestmsg = decodedString;
    std::string message = decodedString;

    for (size_t i=0U; i<letters_number; ++i)
    {
        if (phNumber[0] == conversion_array[i])
        {
            std::string letter(1,i + 'a');
            std::set<std::string>::const_iterator dic_range_i = 
                dic.lower_bound(letter);
            while ((dic_range_i != e) && ((*dic_range_i)[0] == letter[0]))
            {
                if (phNumber == *dic_range_i)
                {
                    decodedString += *dic_range_i;
                    return 1U;
                }
                else if (startWith(phNumber, *dic_range_i))
                {
                    std::string newPhone = phNumber.substr((*dic_range_i).length());
                    std::string newMessage = message + *dic_range_i;
                    size_t len = solve(newPhone, dic, newMessage, depth + 1U);
                    if (len + 1U < shortest)
                    {
                        shortest = len + 1U;
                        bestmsg = newMessage;
                    }
                }

                ++dic_range_i;
            }
        }
    }

    return 0U;
}

int main()
{
    std::string phoneNumber;
    while ((std::cin >> phoneNumber) && (phoneNumber != "-1"))
    {
        size_t dicSize = 0U;
        std::cin >> dicSize;
        std::set<std::string> dictionary;
        while (dicSize-- > 0U) 
        {
            std::string word;
            std::cin >> word;
            dictionary.insert(word);
        }
        std::string message = "";
        solve(phoneNumber, dictionary, message, 0U);
    }

    return 0;
}