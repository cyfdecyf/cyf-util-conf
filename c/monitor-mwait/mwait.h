#ifndef _MWAIT_H
#define _MWAIT_H

// Code copied from Linux kernel file arch/x86/include/asm/processor.h
// Thus it has the same license as Linux kernel.

static inline void cpuid(unsigned int *eax, unsigned int *ebx,
				unsigned int *ecx, unsigned int *edx)
{
	/* ecx is often an input as well as an output. */
	asm volatile("cpuid"
	    : "=a" (*eax),
	      "=b" (*ebx),
	      "=c" (*ecx),
	      "=d" (*edx)
	    : "0" (*eax), "2" (*ecx));
}


// ecx specifies optional extensions, edx specify optional hints
static inline void monitor(const void *eax, unsigned long ecx,
			     unsigned long edx)
{
	/* "monitor %eax, %ecx, %edx;" */
	asm volatile(".byte 0x0f, 0x01, 0xc8;"
		     :: "a" (eax), "c" (ecx), "d"(edx));
}

// eax may contain hint, ecx specifies optional extensions
static inline void mwait(unsigned long eax, unsigned long ecx)
{
	/* "mwait %eax, %ecx;" */
	asm volatile(".byte 0x0f, 0x01, 0xc9;"
		     :: "a" (eax), "c" (ecx));
}

#endif // _MWAIT_H
