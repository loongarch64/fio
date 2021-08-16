#ifndef ARCH_LOONGARCH_H
#define ARCH_LOONGARCH_H

#define FIO_ARCH	(arch_loongarch)

#define read_barrier()		__asm__ __volatile__("dbar 0": : :"memory")
#define write_barrier()		__asm__ __volatile__("dbar 0": : :"memory")
#define nop			__asm__ __volatile__("nop")

#if __loongarch_xlen == 64
#define CTZ "ctz.d"
#define CTO "cto.d"
static inline int arch_ffz(unsigned long bitmask)
{
	unsigned long count;
	if (~bitmask == 0)	/* ffz() in lib/ffz.h does this. */
		return 63;

	__asm__ __volatile__ (CTZ " %0, %1\n\t"
			      "bnez %0, 0f\n\t"
			      CTO " %0, %1\n\t"
			      "b 1f\n\t"
			      "0: move %0, $zero\n\t"
			      "1:\n\t"
			      :"=&r"(count)
			      :"r"(bitmask));

	return count;
}

#define ARCH_HAVE_FFZ

#endif

#endif
