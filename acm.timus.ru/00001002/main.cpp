/*
 *  @see    http://acm.timus.ru/problem.aspx?space=1&num=1002
 */

#include <string>
#include <iostream>
#include <set>
#include <cassert>

typedef unsigned char letter_type;
typedef std::string   word_type;
typedef std::set<word_type> dictionary_type;

static const size_t RECURSION_DEPTH = 100U;

static const size_t      ALPHABET_LENGTH = 26U;
static const letter_type ENCODING_MAP[ALPHABET_LENGTH] = {'2', '2', '2', // a, b, c
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

size_t getLetterIndex(const letter_type & aInput, const size_t & aIndex = 0U)
{
    assert( !( '9' < aInput ) && !(aInput < '0') && "Input not in map");

    for (size_t i = aIndex; i < ALPHABET_LENGTH; ++i)
        if (aInput == ENCODING_MAP[i])
            return i;
    return ALPHABET_LENGTH;
}

letter_type getLetter(const size_t & aIndex)
{
    assert(aIndex < ALPHABET_LENGTH && "Index out of bounds");
    return 'a' + aIndex;
}

letter_type encode(const letter_type & aInput)
{
    assert(  (aInput < ALPHABET_LENGTH + 'a') 
          && !(aInput < 'a')
          && "Index out of bounds");

    return ENCODING_MAP[aInput - 'a'];
}

word_type encode(const word_type & aInput)
{
    word_type::const_iterator bInput = aInput.cbegin();
    const word_type::const_iterator eInput = aInput.cend();
    word_type result;
    while ( bInput != eInput ) 
        result += encode(*bInput++);
    return result;
}

dictionary_type::const_iterator getRangeBegin( const dictionary_type & aDictionary
                                             , const letter_type     & aLetter)
{
    word_type letter(1, aLetter);
    return aDictionary.lower_bound(letter);
}

template <typename T>
bool startsWith(const T & aInput, const T & aPrefix)
{
    typedef typename T::const_iterator cIterator;
    cIterator bInput  = aInput.cbegin();
    cIterator bPrefix = aPrefix.cbegin();
    const cIterator eInput  = aInput.cend();
    const cIterator ePrefix = aPrefix.cend();

    while ((bInput != eInput) && (bPrefix != ePrefix))
    {
        if (*bInput != *bPrefix) return false;
        ++bInput;
        ++bPrefix;
    }

    return bPrefix == ePrefix;
}

size_t solve( const word_type       & aEncodedMessage
            , const dictionary_type & aDictionary
            ,       word_type       & aDecodedMessage
            ,       size_t            aDepth)
{
    assert(aDepth < RECURSION_DEPTH && "Recursion depth reached");
    if (aEncodedMessage.empty()) return 0U;

    const dictionary_type::const_iterator eDictionary = aDictionary.cend();
    size_t minDecodedMessageLength = 0U;
    word_type minDecodedMessage = "";
    const word_type currentDecodedMessage = aDecodedMessage;

    letter_type firstEncodedLetter = aEncodedMessage[0];
    size_t firstPotentialLetterIndex = getLetterIndex(aEncodedMessage[0]);
    while (firstPotentialLetterIndex != ALPHABET_LENGTH)
    {
        letter_type firstPotentialLetter = getLetter(firstPotentialLetterIndex); 
        dictionary_type::const_iterator dictionaryIt = getRangeBegin(aDictionary
                                                        , firstPotentialLetter);
        while ( (dictionaryIt != eDictionary) 
              && ((*dictionaryIt)[0] == firstPotentialLetter))
        {
            word_type dictionaryWord = *dictionaryIt;
            word_type encodedDictionaryWord = encode(dictionaryWord);
            if (aEncodedMessage == encodedDictionaryWord)
            {
                aDecodedMessage += dictionaryWord;
                return 1U;
            }
            else if (startsWith(aEncodedMessage, encodedDictionaryWord))
            {
                word_type restEncodedMessage = 
                                aEncodedMessage.substr(dictionaryWord.length());
                word_type decodedMessage = currentDecodedMessage + dictionaryWord + ' ';
                size_t restDecodedMessageLength = solve(restEncodedMessage
                                                       , aDictionary
                                                       , decodedMessage
                                                       , aDepth + 1U);
                if (restDecodedMessageLength != 0U)
                {
                    if ((minDecodedMessageLength == 0U) 
                        || (restDecodedMessageLength < minDecodedMessageLength))
                    {
                        minDecodedMessageLength = restDecodedMessageLength + 1U;
                        minDecodedMessage = decodedMessage;
                    }
                }
            }

            ++dictionaryIt;
        }
        firstPotentialLetterIndex = getLetterIndex(aEncodedMessage[0]
                                             , firstPotentialLetterIndex + 1U);
    }

    aDecodedMessage = minDecodedMessage;
    return minDecodedMessageLength;
}

word_type findShortestDecoding( const word_type & aEncodedWord
                              , const dictionary_type & aDictionary)
{
    word_type encodedMessage = "";
    size_t length = solve(aEncodedWord, aDictionary, encodedMessage, 0U);
    return ((length) ? (encodedMessage) : ("No solution."));
}

int main()
{
    std::string encodedWord;
    while ((std::cin >> encodedWord) && (encodedWord != "-1"))
    {
        size_t dictionarySize = 0U;
        std::cin >> dictionarySize;
        dictionary_type dictionary;
        word_type word;
        while ((dictionarySize-- > 0U) && (std::cin >> word)) dictionary.insert(word);
        
        std::cout << findShortestDecoding(encodedWord, dictionary) << std::endl;
    }

    return 0;
}