#include <chrono>
#include <iostream>

typedef std::chrono::system_clock timer_t;
typedef std::chrono::microseconds duration_t;

using std::chrono::duration_cast;

struct functionProfile
{
    size_t          _calls = 0;
    duration_t::rep _ticks = 0;
};

#define XSTRING(name) X(FN_##name, #name)

#define XFUNCTIONS                                                                                                     \
    XSTRING(main)                                                                                                      \
    XSTRING(INVALIDNAME)

#define X(a, b) a,
enum functionName
{
    XFUNCTIONS
};
#undef X

#define X(a, b) b,
const char* const _names[] = {XFUNCTIONS};
#undef X

static functionProfile _profile[FN_INVALIDNAME];

#define P_START(METHOD, CONTAINER)                                                                                     \
    ++CONTAINER[FN_##METHOD]._calls;                                                                                   \
    const timer_t::time_point b_##METHOD = timer_t::now( )
#define P_STOPP(METHOD, CONTAINER)                                                                                     \
    CONTAINER[FN_##METHOD]._ticks += duration_cast<duration_t>(timer_t::now( ) - b_##METHOD).count( )

int main( )
{
    P_START(main, _profile);
    // activity
    P_STOPP(main, _profile);

    for (size_t i = 0; i < FN_INVALIDNAME; ++i)
    {
        if (_profile[i]._calls)
        {
            std::cout.width(32);
            std::cout << _names[i] << ": ";
            std::cout.width(8);
            std::cout << _profile[i]._calls << '\t' << _profile[i]._ticks << std::endl;
        }
    }

    return 0;
}