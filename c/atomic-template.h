/* The following code may be included multiple times in a single file. */

#if DATA_BITS == 64
#  define DATA_TYPE uint64_t
#  define SUFFIX q
#elif DATA_BITS == 32
#  define DATA_TYPE uint32_t
#  define SUFFIX l
#elif DATA_BITS == 16
#  define DATA_TYPE uint16_t
#  define SUFFIX w
#elif DATA_BITS == 8
#  define DATA_TYPE uint8_t
#  define SUFFIX b
#else
#error unsupported data size
#endif

static __inline__ void glue(atomic_inc, DATA_BITS)(DATA_TYPE *p) {
    asm volatile(
        LOCK_PREFIX "inc"str(SUFFIX)" %0"
        : "+m"(*p)
        :
        : "cc");
}

static __inline__ void glue(atomic_add, DATA_BITS)(DATA_TYPE* addr,
        DATA_TYPE val) {
    asm volatile(
        LOCK_PREFIX "add"str(SUFFIX)" %1, %0"
        : "+m"(*addr)
        : "a"(val)
        : "cc");
}

/* Return previous value in addr. So if the return value is the same as oldval,
 * swap occured. */
static __inline__ DATA_TYPE glue(atomic_cmpxchg, DATA_BITS)(DATA_TYPE *addr,
        DATA_TYPE oldval, DATA_TYPE newval) {
    asm volatile(
        LOCK_PREFIX "cmpxchg"str(SUFFIX)" %2, %1"
        : "+a"(oldval), "+m"(*addr)
        : "q"(newval)
        : "cc");

    return oldval;
}

static __inline__ void glue(atomic_and, DATA_BITS)(DATA_TYPE *addr,
        DATA_TYPE mask) {
    asm volatile(
        LOCK_PREFIX "and"str(SUFFIX)" %1, %0"
        : "+m"(*addr)
        : "a"(mask)
        : "cc");
}

/* Return the old value in addr. */
static __inline__ DATA_TYPE glue(atomic_fetch_and_add, DATA_BITS)(
        DATA_TYPE* addr, DATA_TYPE val) {
    asm volatile(
        LOCK_PREFIX "xadd"str(SUFFIX)" %0, %1"
        : "+a"(val), "+m"(*addr)
        :
        : "cc");

    return val;
}

#undef DATA_BITS
#undef DATA_TYPE
#undef SUFFIX
