#ifndef _DEBUG_H
#define _DEBUG_H

#include <stdio.h>
#include <assert.h>

/* Always let the compiler see debug code for long term maintenance.
 * Refer to http://stackoverflow.com/a/1644898/306935 */

#ifdef DEBUG
#define DEBUG_TEST 1
#else
#define DEBUG_TEST 0
#endif

# define DPRINTF(fmt, ...) \
    do { if (DEBUG_TEST) fprintf(stderr, "%s:%d: %s: " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); } while (0)

# define ASSERT_DPRINTF(exp, fmt, ...) \
    do { \
        if (DEBUG_TEST && !(exp)) \
            fprintf(stderr, "%s:%d: %s: " fmt, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
        assert(exp); \
    } while (0)

#endif /* _DEBUG_H */
