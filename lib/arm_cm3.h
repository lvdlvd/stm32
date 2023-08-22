#pragma once

// Based on core_cm3.h
// CMSIS Cortex-M3 Core Peripheral Access Layer Header File
// version  V5.1.1  27. March 2020

#include <stdint.h>

__attribute__((always_inline)) static inline void __enable_irq(void)
{
  __asm volatile ("cpsie i" : : : "memory");
}

__attribute__((always_inline)) static inline void __disable_irq(void)
{
  __asm volatile ("cpsid i" : : : "memory");
}

__attribute__((always_inline)) static inline uint32_t __get_CONTROL(void)
{
  uint32_t result;
  __asm volatile ("MRS %0, control" : "=r" (result) );
  return(result);
}

__attribute__((always_inline)) static inline void __set_CONTROL(uint32_t control)
{
  __asm volatile ("MSR control, %0" : : "r" (control) : "memory");
}

__attribute__((always_inline)) static inline uint32_t __get_IPSR(void)
{
  uint32_t result;
  __asm volatile ("MRS %0, ipsr" : "=r" (result) );
  return(result);
}

__attribute__((always_inline)) static inline uint32_t __get_APSR(void)
{
  uint32_t result;
  __asm volatile ("MRS %0, apsr" : "=r" (result) );
  return(result);
}

__attribute__((always_inline)) static inline uint32_t __get_xPSR(void)
{
  uint32_t result;
  __asm volatile ("MRS %0, xpsr" : "=r" (result) );
  return(result);
}

__attribute__((always_inline)) static inline uint32_t __get_PSP(void)
{
  uint32_t result;
  __asm volatile ("MRS %0, psp" : "=r" (result) );
  return(result);
}

__attribute__((always_inline)) static inline void __set_PSP(uint32_t topOfProcStack)
{
  __asm volatile ("MSR psp, %0" : : "r" (topOfProcStack) : );
}

__attribute__((always_inline)) static inline uint32_t __get_MSP(void)
{
  uint32_t result;
  __asm volatile ("MRS %0, msp" : "=r" (result) );
  return(result);
}

__attribute__((always_inline)) static inline void __set_MSP(uint32_t topOfMainStack)
{
  __asm volatile ("MSR msp, %0" : : "r" (topOfMainStack) : );
}

__attribute__((always_inline)) static inline uint32_t __get_PRIMASK(void)
{
  uint32_t result;
  __asm volatile ("MRS %0, primask" : "=r" (result) );
  return(result);
}

__attribute__((always_inline)) static inline void __set_PRIMASK(uint32_t priMask)
{
  __asm volatile ("MSR primask, %0" : : "r" (priMask) : "memory");
}

__attribute__((always_inline)) static inline void __enable_fault_irq(void)
{
  __asm volatile ("cpsie f" : : : "memory");
}

__attribute__((always_inline)) static inline void __disable_fault_irq(void)
{
  __asm volatile ("cpsid f" : : : "memory");
}

__attribute__((always_inline)) static inline uint32_t __get_BASEPRI(void)
{
  uint32_t result;
  __asm volatile ("MRS %0, basepri" : "=r" (result) );
  return(result);
}

__attribute__((always_inline)) static inline void __set_BASEPRI(uint32_t basePri)
{
  __asm volatile ("MSR basepri, %0" : : "r" (basePri) : "memory");
}

__attribute__((always_inline)) static inline void __set_BASEPRI_MAX(uint32_t basePri)
{
  __asm volatile ("MSR basepri_max, %0" : : "r" (basePri) : "memory");
}



__attribute__((always_inline)) static inline uint32_t __get_FAULTMASK(void)
{
  uint32_t result;
  __asm volatile ("MRS %0, faultmask" : "=r" (result) );
  return(result);
}

__attribute__((always_inline)) static inline void __set_FAULTMASK(uint32_t faultMask)
{
  __asm volatile ("MSR faultmask, %0" : : "r" (faultMask) : "memory");
}

__attribute__((always_inline)) static inline void __ISB(void)
{
  __asm volatile ("isb 0xF":::"memory");
}

__attribute__((always_inline)) static inline void __DSB(void)
{
  __asm volatile ("dsb 0xF":::"memory");
}

__attribute__((always_inline)) static inline void __DMB(void)
{
  __asm volatile ("dmb 0xF":::"memory");
}

__attribute__((always_inline)) static inline uint32_t __REV(uint32_t value)
{
  return __builtin_bswap32(value);
}

__attribute__((always_inline)) static inline uint32_t __REV16(uint32_t value)
{
  uint32_t result;
  __asm ("rev16 %0, %1" : "=r" (result) : "r" (value) );
  return result;
}

__attribute__((always_inline)) static inline int16_t __REVSH(int16_t value)
{
  return (int16_t)__builtin_bswap16(value);
}

__attribute__((always_inline)) static inline uint32_t __RBIT(uint32_t value)
{
  uint32_t result;
   __asm ("rbit %0, %1" : "=r" (result) : "r" (value) );
  return result;
}
__attribute__((always_inline)) static inline uint8_t __CLZ(uint32_t value)
{
  if (value == 0U)  {
    return 32U;
  }
  return __builtin_clz(value);
}

__attribute__((always_inline)) static inline uint8_t __LDREXB(volatile uint8_t *addr)
{
    uint32_t result;
   __asm volatile ("ldrexb %0, %1" : "=r" (result) : "Q" (*addr) );
   return ((uint8_t) result);
}

__attribute__((always_inline)) static inline uint16_t __LDREXH(volatile uint16_t *addr)
{
    uint32_t result;
   __asm volatile ("ldrexh %0, %1" : "=r" (result) : "Q" (*addr) );
   return ((uint16_t) result);
}

__attribute__((always_inline)) static inline uint32_t __LDREXW(volatile uint32_t *addr)
{
    uint32_t result;
   __asm volatile ("ldrex %0, %1" : "=r" (result) : "Q" (*addr) );
   return(result);
}

__attribute__((always_inline)) static inline uint32_t __STREXB(uint8_t value, volatile uint8_t *addr)
{
   uint32_t result;
   __asm volatile ("strexb %0, %2, %1" : "=&r" (result), "=Q" (*addr) : "r" ((uint32_t)value) );
   return(result);
}

__attribute__((always_inline)) static inline uint32_t __STREXH(uint16_t value, volatile uint16_t *addr)
{
   uint32_t result;
   __asm volatile ("strexh %0, %2, %1" : "=&r" (result), "=Q" (*addr) : "r" ((uint32_t)value) );
   return(result);
}

__attribute__((always_inline)) static inline uint32_t __STREXW(uint32_t value, volatile uint32_t *addr)
{
   uint32_t result;
   __asm volatile ("strex %0, %2, %1" : "=&r" (result), "=Q" (*addr) : "r" (value) );
   return(result);
}

__attribute__((always_inline)) static inline void __CLREX(void)
{
  __asm volatile ("clrex" ::: "memory");
}

__attribute__((always_inline)) static inline uint32_t __RRX(uint32_t value)
{
  uint32_t result;
  __asm volatile ("rrx %0, %1" : "=r" (result) : "r" (value) );
  return(result);
}

__attribute__((always_inline)) static inline uint8_t __LDRBT(volatile uint8_t *ptr)
{
    uint32_t result;
   __asm volatile ("ldrbt %0, %1" : "=r" (result) : "Q" (*ptr) );
   return ((uint8_t) result);
}

__attribute__((always_inline)) static inline uint16_t __LDRHT(volatile uint16_t *ptr)
{
    uint32_t result;
   __asm volatile ("ldrht %0, %1" : "=r" (result) : "Q" (*ptr) );
   return ((uint16_t) result);
}

__attribute__((always_inline)) static inline uint32_t __LDRT(volatile uint32_t *ptr)
{
    uint32_t result;
   __asm volatile ("ldrt %0, %1" : "=r" (result) : "Q" (*ptr) );
   return(result);
}

__attribute__((always_inline)) static inline void __STRBT(uint8_t value, volatile uint8_t *ptr)
{
   __asm volatile ("strbt %1, %0" : "=Q" (*ptr) : "r" ((uint32_t)value) );
}

__attribute__((always_inline)) static inline void __STRHT(uint16_t value, volatile uint16_t *ptr)
{
   __asm volatile ("strht %1, %0" : "=Q" (*ptr) : "r" ((uint32_t)value) );
}

__attribute__((always_inline)) static inline void __STRT(uint32_t value, volatile uint32_t *ptr)
{
   __asm volatile ("strt %1, %0" : "=Q" (*ptr) : "r" (value) );
}
