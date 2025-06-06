#pragma once

/*
 * based on core_cm4.h CMSIS Cortex-M4 Core Peripheral Access Layer Header File
 * V4.10  18. March 2015
 */

#include <stdint.h>

/** CMSIS_Core_InstructionInterface CMSIS Core Instruction Interface */

/* GNU gcc specific functions */

/* Define macros for porting to both thumb1 and thumb2.
 * For thumb1, use low register (r0-r7), specified by constrant "l"
 * Otherwise, use general registers, specified by constrant "r" */
#if defined(__thumb__) && !defined(__thumb2__)
#define __CMSIS_GCC_OUT_REG(r) "=l"(r)
#define __CMSIS_GCC_USE_REG(r) "l"(r)
#else
#define __CMSIS_GCC_OUT_REG(r) "=r"(r)
#define __CMSIS_GCC_USE_REG(r) "r"(r)
#endif

/** \brief  No Operation

	No Operation does nothing. This instruction can be used for code alignment purposes.
 */
__attribute__((always_inline)) static inline void __NOP(void) { __asm volatile("nop"); }

/** \brief  Wait For Interrupt

	Wait For Interrupt is a hint instruction that suspends execution
	until one of a number of events occurs.
 */
__attribute__((always_inline)) static inline void __WFI(void) { __asm volatile("wfi"); }

/** \brief  Wait For Event

	Wait For Event is a hint instruction that permits the processor to enter
	a low-power state until one of a number of events occurs.
 */
__attribute__((always_inline)) static inline void __WFE(void) { __asm volatile("wfe"); }

/** \brief  Send Event

	Send Event is a hint instruction. It causes an event to be signaled to the CPU.
 */
__attribute__((always_inline)) static inline void __SEV(void) { __asm volatile("sev"); }

/** \brief  Instruction Synchronization Barrier

	Instruction Synchronization Barrier flushes the pipeline in the processor,
	so that all instructions following the ISB are fetched from cache or
	memory, after the instruction has been completed.
 */
__attribute__((always_inline)) static inline void __ISB(void) { __asm volatile("isb 0xF" ::: "memory"); }

/** \brief  Data Synchronization Barrier

	This function acts as a special kind of Data Memory Barrier.
	It completes when all explicit memory accesses before this instruction complete.
 */
__attribute__((always_inline)) static inline void __DSB(void) { __asm volatile("dsb 0xF" ::: "memory"); }

/** \brief  Data Memory Barrier

	This function ensures the apparent order of the explicit memory operations before
	and after the instruction, without ensuring their completion.
 */
__attribute__((always_inline)) static inline void __DMB(void) { __asm volatile("dmb 0xF" ::: "memory"); }

/** \brief  Reverse byte order (32 bit)

	This function reverses the byte order in integer value.

	\param [in]    value  Value to reverse
	\return               Reversed value
 */
__attribute__((always_inline)) static inline uint32_t __REV(uint32_t value) { return __builtin_bswap32(value); }

/** \brief  Reverse byte order (16 bit)

	This function reverses the byte order in two unsigned short values.

	\param [in]    value  Value to reverse
	\return               Reversed value
 */
__attribute__((always_inline)) static inline uint32_t __REV16(uint32_t value) {
	uint32_t result;

	__asm volatile("rev16 %0, %1" : __CMSIS_GCC_OUT_REG(result) : __CMSIS_GCC_USE_REG(value));
	return (result);
}

/** \brief  Reverse byte order in signed short value

	This function reverses the byte order in a signed short value with sign extension to integer.

	\param [in]    value  Value to reverse
	\return               Reversed value
 */
__attribute__((always_inline)) static inline int32_t __REVSH(int32_t value) { return (short)__builtin_bswap16(value); }

/** \brief  Rotate Right in unsigned value (32 bit)

	This function Rotate Right (immediate) provides the value of the contents of a register rotated by a variable number of bits.

	\param [in]    value  Value to rotate
	\param [in]    value  Number of Bits to rotate
	\return               Rotated value
 */
__attribute__((always_inline)) static inline uint32_t __ROR(uint32_t op1, uint32_t op2) {
	return (op1 >> op2) | (op1 << (32 - op2));
}

/** \brief  Breakpoint

	This function causes the processor to enter Debug state.
	Debug tools can use this to investigate system state when the instruction at a particular address is reached.

	\param [in]    value  is ignored by the processor.
				   If required, a debugger can use it to store additional information about the breakpoint.
 */
#define __BKPT(value) __asm volatile("bkpt " #value)

/** \brief  Reverse bit order of value

	This function reverses the bit order of the given value.

	\param [in]    value  Value to reverse
	\return               Reversed value
 */
__attribute__((always_inline)) static inline uint32_t __RBIT(uint32_t value) {
	uint32_t result;
	__asm volatile("rbit %0, %1" : "=r"(result) : "r"(value));
	return (result);
}

/** \brief  Count leading zeros

	This function counts the number of leading zeros of a data value.

	\param [in]  value  Value to count the leading zeros
	\return             number of leading zeros in value
 */
#define __CLZ __builtin_clz

/** \brief  LDR Exclusive (8 bit)

	This function executes a exclusive LDR instruction for 8 bit value.

	\param [in]    ptr  Pointer to data
	\return             value of type uint8_t at (*ptr)
 */
__attribute__((always_inline)) static inline uint8_t __LDREXB(volatile uint8_t *addr) {
	uint32_t result;
	__asm volatile("ldrexb %0, %1" : "=r"(result) : "Q"(*addr));
	return ((uint8_t)result); /* Add explicit type cast here */
}

/** \brief  LDR Exclusive (16 bit)

	This function executes a exclusive LDR instruction for 16 bit values.

	\param [in]    ptr  Pointer to data
	\return        value of type uint16_t at (*ptr)
 */
__attribute__((always_inline)) static inline uint16_t __LDREXH(volatile uint16_t *addr) {
	uint32_t result;
	__asm volatile("ldrexh %0, %1" : "=r"(result) : "Q"(*addr));
	return ((uint16_t)result); /* Add explicit type cast here */
}

/** \brief  LDR Exclusive (32 bit)

	This function executes a exclusive LDR instruction for 32 bit values.

	\param [in]    ptr  Pointer to data
	\return        value of type uint32_t at (*ptr)
 */
__attribute__((always_inline)) static inline uint32_t __LDREXW(volatile uint32_t *addr) {
	uint32_t result;

	__asm volatile("ldrex %0, %1" : "=r"(result) : "Q"(*addr));
	return (result);
}

/** \brief  STR Exclusive (8 bit)

	This function executes a exclusive STR instruction for 8 bit values.

	\param [in]  value  Value to store
	\param [in]    ptr  Pointer to location
	\return          0  Function succeeded
	\return          1  Function failed
 */
__attribute__((always_inline)) static inline uint32_t __STREXB(uint8_t value, volatile uint8_t *addr) {
	uint32_t result;

	__asm volatile("strexb %0, %2, %1" : "=&r"(result), "=Q"(*addr) : "r"((uint32_t)value));
	return (result);
}

/** \brief  STR Exclusive (16 bit)

	This function executes a exclusive STR instruction for 16 bit values.

	\param [in]  value  Value to store
	\param [in]    ptr  Pointer to location
	\return          0  Function succeeded
	\return          1  Function failed
 */
__attribute__((always_inline)) static inline uint32_t __STREXH(uint16_t value, volatile uint16_t *addr) {
	uint32_t result;

	__asm volatile("strexh %0, %2, %1" : "=&r"(result), "=Q"(*addr) : "r"((uint32_t)value));
	return (result);
}

/** \brief  STR Exclusive (32 bit)

	This function executes a exclusive STR instruction for 32 bit values.

	\param [in]  value  Value to store
	\param [in]    ptr  Pointer to location
	\return          0  Function succeeded
	\return          1  Function failed
 */
__attribute__((always_inline)) static inline uint32_t __STREXW(uint32_t value, volatile uint32_t *addr) {
	uint32_t result;

	__asm volatile("strex %0, %2, %1" : "=&r"(result), "=Q"(*addr) : "r"(value));
	return (result);
}

/** \brief  Remove the exclusive lock

	This function removes the exclusive lock which is created by LDREX.

 */
__attribute__((always_inline)) static inline void __CLREX(void) { __asm volatile("clrex" ::: "memory"); }

/** \brief  Signed Saturate

	This function saturates a signed value.

	\param [in]  value  Value to be saturated
	\param [in]    sat  Bit position to saturate to (1..32)
	\return             Saturated value
 */
#define __SSAT(ARG1, ARG2)                                               \
	({                                                                   \
		uint32_t __RES, __ARG1 = (ARG1);                                 \
		__asm("ssat %0, %1, %2" : "=r"(__RES) : "I"(ARG2), "r"(__ARG1)); \
		__RES;                                                           \
	})

/** \brief  Unsigned Saturate

	This function saturates an unsigned value.

	\param [in]  value  Value to be saturated
	\param [in]    sat  Bit position to saturate to (0..31)
	\return             Saturated value
 */
#define __USAT(ARG1, ARG2)                                               \
	({                                                                   \
		uint32_t __RES, __ARG1 = (ARG1);                                 \
		__asm("usat %0, %1, %2" : "=r"(__RES) : "I"(ARG2), "r"(__ARG1)); \
		__RES;                                                           \
	})

/** \brief  Rotate Right with Extend (32 bit)

	This function moves each bit of a bitstring right by one bit.
	The carry input is shifted in at the left end of the bitstring.

	\param [in]    value  Value to rotate
	\return               Rotated value
 */
__attribute__((always_inline)) static inline uint32_t __RRX(uint32_t value) {
	uint32_t result;

	__asm volatile("rrx %0, %1" : __CMSIS_GCC_OUT_REG(result) : __CMSIS_GCC_USE_REG(value));
	return (result);
}

/** \brief  LDRT Unprivileged (8 bit)

	This function executes a Unprivileged LDRT instruction for 8 bit value.

	\param [in]    ptr  Pointer to data
	\return             value of type uint8_t at (*ptr)
 */
__attribute__((always_inline)) static inline uint8_t __LDRBT(volatile uint8_t *addr) {
	uint32_t result;

	__asm volatile("ldrbt %0, %1" : "=r"(result) : "Q"(*addr));
	return ((uint8_t)result); /* Add explicit type cast here */
}

/** \brief  LDRT Unprivileged (16 bit)

	This function executes a Unprivileged LDRT instruction for 16 bit values.

	\param [in]    ptr  Pointer to data
	\return        value of type uint16_t at (*ptr)
 */
__attribute__((always_inline)) static inline uint16_t __LDRHT(volatile uint16_t *addr) {
	uint32_t result;

	__asm volatile("ldrht %0, %1" : "=r"(result) : "Q"(*addr));
	return ((uint16_t)result); /* Add explicit type cast here */
}

/** \brief  LDRT Unprivileged (32 bit)

	This function executes a Unprivileged LDRT instruction for 32 bit values.

	\param [in]    ptr  Pointer to data
	\return        value of type uint32_t at (*ptr)
 */
__attribute__((always_inline)) static inline uint32_t __LDRT(volatile uint32_t *addr) {
	uint32_t result;

	__asm volatile("ldrt %0, %1" : "=r"(result) : "Q"(*addr));
	return (result);
}

/** \brief  STRT Unprivileged (8 bit)

	This function executes a Unprivileged STRT instruction for 8 bit values.

	\param [in]  value  Value to store
	\param [in]    ptr  Pointer to location
 */
__attribute__((always_inline)) static inline void __STRBT(uint8_t value, volatile uint8_t *addr) {
	__asm volatile("strbt %1, %0" : "=Q"(*addr) : "r"((uint32_t)value));
}

/** \brief  STRT Unprivileged (16 bit)

	This function executes a Unprivileged STRT instruction for 16 bit values.

	\param [in]  value  Value to store
	\param [in]    ptr  Pointer to location
 */
__attribute__((always_inline)) static inline void __STRHT(uint16_t value, volatile uint16_t *addr) {
	__asm volatile("strht %1, %0" : "=Q"(*addr) : "r"((uint32_t)value));
}

/** \brief  STRT Unprivileged (32 bit)

	This function executes a Unprivileged STRT instruction for 32 bit values.

	\param [in]  value  Value to store
	\param [in]    ptr  Pointer to location
 */
__attribute__((always_inline)) static inline void __STRT(uint32_t value, volatile uint32_t *addr) {
	__asm volatile("strt %1, %0" : "=Q"(*addr) : "r"(value));
}

/** CMSIS_Core_FunctionInterface */

/* GNU gcc specific functions */

/** \brief  Enable IRQ Interrupts

  This function enables IRQ interrupts by clearing the I-bit in the CPSR.
  Can only be executed in Privileged modes.
 */
__attribute__((always_inline)) static inline void __enable_irq(void) { __asm volatile("cpsie i" : : : "memory"); }

/** \brief  Disable IRQ Interrupts

  This function disables IRQ interrupts by setting the I-bit in the CPSR.
  Can only be executed in Privileged modes.
 */
__attribute__((always_inline)) static inline void __disable_irq(void) { __asm volatile("cpsid i" : : : "memory"); }

/** \brief  Get Control Register

	This function returns the content of the Control Register.

	\return               Control Register value
 */
__attribute__((always_inline)) static inline uint32_t __get_CONTROL(void) {
	uint32_t result;

	__asm volatile("MRS %0, control" : "=r"(result));
	return (result);
}

/** \brief  Set Control Register

	This function writes the given value to the Control Register.

	\param [in]    control  Control Register value to set
 */
__attribute__((always_inline)) static inline void __set_CONTROL(uint32_t control) {
	__asm volatile("MSR control, %0" : : "r"(control) : "memory");
}

/** \brief  Get IPSR Register

	This function returns the content of the IPSR Register.

	\return               IPSR Register value
 */
__attribute__((always_inline)) static inline uint32_t __get_IPSR(void) {
	uint32_t result;

	__asm volatile("MRS %0, ipsr" : "=r"(result));
	return (result);
}

/** \brief  Get APSR Register

	This function returns the content of the APSR Register.

	\return               APSR Register value
 */
__attribute__((always_inline)) static inline uint32_t __get_APSR(void) {
	uint32_t result;

	__asm volatile("MRS %0, apsr" : "=r"(result));
	return (result);
}

/** \brief  Get xPSR Register

	This function returns the content of the xPSR Register.

	\return               xPSR Register value
 */
__attribute__((always_inline)) static inline uint32_t __get_xPSR(void) {
	uint32_t result;

	__asm volatile("MRS %0, xpsr" : "=r"(result));
	return (result);
}

/** \brief  Get Process Stack Pointer

	This function returns the current value of the Process Stack Pointer (PSP).

	\return               PSP Register value
 */
__attribute__((always_inline)) static inline uint32_t __get_PSP(void) {
	register uint32_t result;

	__asm volatile("MRS %0, psp\n" : "=r"(result));
	return (result);
}

/** \brief  Set Process Stack Pointer

	This function assigns the given value to the Process Stack Pointer (PSP).

	\param [in]    topOfProcStack  Process Stack Pointer value to set
 */
__attribute__((always_inline)) static inline void __set_PSP(uint32_t topOfProcStack) {
	__asm volatile("MSR psp, %0\n" : : "r"(topOfProcStack) : "sp");
}

/** \brief  Get Main Stack Pointer

	This function returns the current value of the Main Stack Pointer (MSP).

	\return               MSP Register value
 */
__attribute__((always_inline)) static inline uint32_t __get_MSP(void) {
	register uint32_t result;

	__asm volatile("MRS %0, msp\n" : "=r"(result));
	return (result);
}

/** \brief  Set Main Stack Pointer

	This function assigns the given value to the Main Stack Pointer (MSP).

	\param [in]    topOfMainStack  Main Stack Pointer value to set
 */
__attribute__((always_inline)) static inline void __set_MSP(uint32_t topOfMainStack) {
	__asm volatile("MSR msp, %0\n" : : "r"(topOfMainStack) : "sp");
}

/** \brief  Get Priority Mask

	This function returns the current state of the priority mask bit from the Priority Mask Register.

	\return               Priority Mask value
 */
__attribute__((always_inline)) static inline uint32_t __get_PRIMASK(void) {
	uint32_t result;

	__asm volatile("MRS %0, primask" : "=r"(result));
	return (result);
}

/** \brief  Set Priority Mask

	This function assigns the given value to the Priority Mask Register.

	\param [in]    priMask  Priority Mask
 */
__attribute__((always_inline)) static inline void __set_PRIMASK(uint32_t priMask) {
	__asm volatile("MSR primask, %0" : : "r"(priMask) : "memory");
}

/** \brief  Enable FIQ

	This function enables FIQ interrupts by clearing the F-bit in the CPSR.
	Can only be executed in Privileged modes.
 */
__attribute__((always_inline)) static inline void __enable_fault_irq(void) { __asm volatile("cpsie f" : : : "memory"); }

/** \brief  Disable FIQ

	This function disables FIQ interrupts by setting the F-bit in the CPSR.
	Can only be executed in Privileged modes.
 */
__attribute__((always_inline)) static inline void __disable_fault_irq(void) { __asm volatile("cpsid f" : : : "memory"); }

/** \brief  Get Base Priority

	This function returns the current value of the Base Priority register.

	\return               Base Priority register value
 */
__attribute__((always_inline)) static inline uint32_t __get_BASEPRI(void) {
	uint32_t result;

	__asm volatile("MRS %0, basepri" : "=r"(result));
	return (result);
}

/** \brief  Set Base Priority

	This function assigns the given value to the Base Priority register.

	\param [in]    basePri  Base Priority value to set
 */
__attribute__((always_inline)) static inline void __set_BASEPRI(uint32_t value) {
	__asm volatile("MSR basepri, %0" : : "r"(value) : "memory");
}

/** \brief  Set Base Priority with condition

	This function assigns the given value to the Base Priority register only if BASEPRI masking is disabled,
  or the new value increases the BASEPRI priority level.

	\param [in]    basePri  Base Priority value to set
 */
__attribute__((always_inline)) static inline void __set_BASEPRI_MAX(uint32_t value) {
	__asm volatile("MSR basepri_max, %0" : : "r"(value) : "memory");
}

/** \brief  Get Fault Mask

	This function returns the current value of the Fault Mask register.

	\return               Fault Mask register value
 */
__attribute__((always_inline)) static inline uint32_t __get_FAULTMASK(void) {
	uint32_t result;

	__asm volatile("MRS %0, faultmask" : "=r"(result));
	return (result);
}

/** \brief  Set Fault Mask

	This function assigns the given value to the Fault Mask register.

	\param [in]    faultMask  Fault Mask value to set
 */
__attribute__((always_inline)) static inline void __set_FAULTMASK(uint32_t faultMask) {
	__asm volatile("MSR faultmask, %0" : : "r"(faultMask) : "memory");
}

/** \brief  Get FPSCR

	This function returns the current value of the Floating Point Status/Control register.

	\return               Floating Point Status/Control register value
 */
__attribute__((always_inline)) static inline uint32_t __get_FPSCR(void) {
	uint32_t result;

	/* Empty asm statement works as a scheduling barrier */
	__asm volatile("");
	__asm volatile("VMRS %0, fpscr" : "=r"(result));
	__asm volatile("");
	return (result);
}

/** \brief  Set FPSCR

	This function assigns the given value to the Floating Point Status/Control register.

	\param [in]    fpscr  Floating Point Status/Control value to set
 */
__attribute__((always_inline)) static inline void __set_FPSCR(uint32_t fpscr) {
	/* Empty asm statement works as a scheduling barrier */
	__asm volatile("");
	__asm volatile("VMSR fpscr, %0" : : "r"(fpscr) : "vfpcc");
	__asm volatile("");
}

/** CMSIS_SIMD_intrinsics CMSIS SIMD Intrinsics */

/* GNU gcc specific functions */

__attribute__((always_inline)) static inline uint32_t __SADD8(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("sadd8 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __QADD8(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("qadd8 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SHADD8(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("shadd8 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UADD8(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uadd8 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UQADD8(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uqadd8 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UHADD8(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uhadd8 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SSUB8(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("ssub8 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __QSUB8(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("qsub8 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SHSUB8(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("shsub8 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __USUB8(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("usub8 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UQSUB8(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uqsub8 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UHSUB8(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uhsub8 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SADD16(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("sadd16 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __QADD16(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("qadd16 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SHADD16(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("shadd16 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UADD16(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uadd16 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UQADD16(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uqadd16 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UHADD16(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uhadd16 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SSUB16(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("ssub16 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __QSUB16(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("qsub16 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SHSUB16(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("shsub16 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __USUB16(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("usub16 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UQSUB16(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uqsub16 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UHSUB16(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uhsub16 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SASX(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("sasx %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __QASX(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("qasx %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SHASX(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("shasx %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UASX(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uasx %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UQASX(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uqasx %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UHASX(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uhasx %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SSAX(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("ssax %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __QSAX(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("qsax %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SHSAX(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("shsax %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __USAX(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("usax %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UQSAX(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uqsax %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UHSAX(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uhsax %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __USAD8(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("usad8 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __USADA8(uint32_t op1, uint32_t op2, uint32_t op3) {
	uint32_t result;

	__asm volatile("usada8 %0, %1, %2, %3" : "=r"(result) : "r"(op1), "r"(op2), "r"(op3));
	return (result);
}

#define __SSAT16(ARG1, ARG2)                                               \
	({                                                                     \
		uint32_t __RES, __ARG1 = (ARG1);                                   \
		__asm("ssat16 %0, %1, %2" : "=r"(__RES) : "I"(ARG2), "r"(__ARG1)); \
		__RES;                                                             \
	})

#define __USAT16(ARG1, ARG2)                                               \
	({                                                                     \
		uint32_t __RES, __ARG1 = (ARG1);                                   \
		__asm("usat16 %0, %1, %2" : "=r"(__RES) : "I"(ARG2), "r"(__ARG1)); \
		__RES;                                                             \
	})

__attribute__((always_inline)) static inline uint32_t __UXTB16(uint32_t op1) {
	uint32_t result;

	__asm volatile("uxtb16 %0, %1" : "=r"(result) : "r"(op1));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __UXTAB16(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("uxtab16 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SXTB16(uint32_t op1) {
	uint32_t result;

	__asm volatile("sxtb16 %0, %1" : "=r"(result) : "r"(op1));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SXTAB16(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("sxtab16 %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SMUAD(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("smuad %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SMUADX(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("smuadx %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SMLAD(uint32_t op1, uint32_t op2, uint32_t op3) {
	uint32_t result;

	__asm volatile("smlad %0, %1, %2, %3" : "=r"(result) : "r"(op1), "r"(op2), "r"(op3));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SMLADX(uint32_t op1, uint32_t op2, uint32_t op3) {
	uint32_t result;

	__asm volatile("smladx %0, %1, %2, %3" : "=r"(result) : "r"(op1), "r"(op2), "r"(op3));
	return (result);
}

__attribute__((always_inline)) static inline uint64_t __SMLALD(uint32_t op1, uint32_t op2, uint64_t acc) {
	union llreg_u {
		uint32_t w32[2];
		uint64_t w64;
	} llr;
	llr.w64 = acc;

	__asm volatile("smlald %0, %1, %2, %3"
				   : "=r"(llr.w32[0]), "=r"(llr.w32[1])
				   : "r"(op1), "r"(op2), "0"(llr.w32[0]), "1"(llr.w32[1]));

	return (llr.w64);
}

__attribute__((always_inline)) static inline uint64_t __SMLALDX(uint32_t op1, uint32_t op2, uint64_t acc) {
	union llreg_u {
		uint32_t w32[2];
		uint64_t w64;
	} llr;
	llr.w64 = acc;

	__asm volatile("smlaldx %0, %1, %2, %3"
				   : "=r"(llr.w32[0]), "=r"(llr.w32[1])
				   : "r"(op1), "r"(op2), "0"(llr.w32[0]), "1"(llr.w32[1]));

	return (llr.w64);
}

__attribute__((always_inline)) static inline uint32_t __SMUSD(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("smusd %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SMUSDX(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("smusdx %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SMLSD(uint32_t op1, uint32_t op2, uint32_t op3) {
	uint32_t result;

	__asm volatile("smlsd %0, %1, %2, %3" : "=r"(result) : "r"(op1), "r"(op2), "r"(op3));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __SMLSDX(uint32_t op1, uint32_t op2, uint32_t op3) {
	uint32_t result;

	__asm volatile("smlsdx %0, %1, %2, %3" : "=r"(result) : "r"(op1), "r"(op2), "r"(op3));
	return (result);
}

__attribute__((always_inline)) static inline uint64_t __SMLSLD(uint32_t op1, uint32_t op2, uint64_t acc) {
	union llreg_u {
		uint32_t w32[2];
		uint64_t w64;
	} llr;
	llr.w64 = acc;

	__asm volatile("smlsld %0, %1, %2, %3"
				   : "=r"(llr.w32[0]), "=r"(llr.w32[1])
				   : "r"(op1), "r"(op2), "0"(llr.w32[0]), "1"(llr.w32[1]));

	return (llr.w64);
}

__attribute__((always_inline)) static inline uint64_t __SMLSLDX(uint32_t op1, uint32_t op2, uint64_t acc) {
	union llreg_u {
		uint32_t w32[2];
		uint64_t w64;
	} llr;
	llr.w64 = acc;

	__asm volatile("smlsldx %0, %1, %2, %3"
				   : "=r"(llr.w32[0]), "=r"(llr.w32[1])
				   : "r"(op1), "r"(op2), "0"(llr.w32[0]), "1"(llr.w32[1]));

	return (llr.w64);
}

__attribute__((always_inline)) static inline uint32_t __SEL(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("sel %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __QADD(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("qadd %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

__attribute__((always_inline)) static inline uint32_t __QSUB(uint32_t op1, uint32_t op2) {
	uint32_t result;

	__asm volatile("qsub %0, %1, %2" : "=r"(result) : "r"(op1), "r"(op2));
	return (result);
}

#define __PKHBT(ARG1, ARG2, ARG3)                                                              \
	({                                                                                         \
		uint32_t __RES, __ARG1 = (ARG1), __ARG2 = (ARG2);                                      \
		__asm("pkhbt %0, %1, %2, lsl %3" : "=r"(__RES) : "r"(__ARG1), "r"(__ARG2), "I"(ARG3)); \
		__RES;                                                                                 \
	})

#define __PKHTB(ARG1, ARG2, ARG3)                                                                  \
	({                                                                                             \
		uint32_t __RES, __ARG1 = (ARG1), __ARG2 = (ARG2);                                          \
		if (ARG3 == 0)                                                                             \
			__asm("pkhtb %0, %1, %2" : "=r"(__RES) : "r"(__ARG1), "r"(__ARG2));                    \
		else                                                                                       \
			__asm("pkhtb %0, %1, %2, asr %3" : "=r"(__RES) : "r"(__ARG1), "r"(__ARG2), "I"(ARG3)); \
		__RES;                                                                                     \
	})

__attribute__((always_inline)) static inline uint32_t __SMMLA(int32_t op1, int32_t op2, int32_t op3) {
	int32_t result;

	__asm volatile("smmla %0, %1, %2, %3" : "=r"(result) : "r"(op1), "r"(op2), "r"(op3));
	return (result);
}
