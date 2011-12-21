#include "atomic.h"
#include <assert.h>

int main(int argc, const char *argv[])
{
    /* The following test cases just test the semantic correctness of the
     * atomic function, they does not test the acomicity. */

    uint32_t m = 1, r;

    // Succeed
    r = atomic_cmpxchg32(&m, 1, 2);
    assert(r == 1);
    assert(m == 2);
    // Fail
    r = atomic_cmpxchg32(&m, 1, 2);
    assert(r == 2);
    assert(m == 2);

    atomic_inc32(&m);
    assert(m == 3);

    atomic_add32(&m, -4);
    assert(m == -1);

    atomic_add32(&m, 12);
    assert(m == 11);

    // m is 0b1011 before
    atomic_and32(&m, 0x8);
    assert(m == 8);

    r = atomic_fetch_and_add32(&m, 2);
    assert(r == 8);
    assert(m == 10);

    uint32_t m64[2] = { -1, 0 };
    assert(atomic_cmpxchg8b(m64, -1, 0, 0xf, 0xf0));
    assert(m64[0] == 0xf);
    assert(m64[1] == 0xf0);
    assert(atomic_cmpxchg8b(m64, -1, 0, 0xf, 0xf0) == 0);

    uint64_t m128[2] = { -1, 0 };
    assert(atomic_cmpxchg16b(m128, -1, 0, 0xf, 0xf0));
    assert(m128[0] == 0xf);
    assert(m128[1] == 0xf0);
    assert(atomic_cmpxchg16b(m128, -1, 0, 0xf, 0xf0) == 0);

    return 0;
}
