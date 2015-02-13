#include "ucc.h"

static inline void
write(uchar data){
  __gaia_write(data);
}

// copy
// Routines to let C code use special GAIA instructions.
static inline uchar
inb(ushort port)
{
  __asm("\
      mov r2, [rbp+4] \n\
      ld  r1, r2, 0   \n\
      ret             \n\
  ");
}

static inline void
insl(int port, void *addr, int cnt)
{
  __asm("\
      mov r1, [rbp+4]     \n\
      mov r2, [rbp+8]     \n\
      mov r3, [rbp+12]    \n\
    insl_L1:              \n\
      blt r3, 0, insl_l2  \n\
      sub r3, r3, 1       \n\
      ld  r4, r1, 0       \n\
      st  r4, r2, 0       \n\
      add r2, r2, 4       \n\
      add r3, r3, 4       \n\
      br  insl_L1         \n\
    insl_L2:              \n\
      ret                 \n\
  ");
}

static inline void
outb(ushort port, uchar data)
{
  __asm("\
      mov r1, [rbp+4]   \n\
      mov r2, [rbp+8]   \n\
      st  r2, r1, 0     \n\
      ret               \n\
  ");
}

static inline void
outw(ushort port, ushort data)
{
  __asm("\
      mov r1, [rbp+4]   \n\
      mov r2, [rbp+8]   \n\
      st  r2, r1, 0     \n\
      ret               \n\
  ");
}

static inline void
outsl(int port, const void *addr, int cnt)
{
  __asm("\
      mov r1, [rbp+4]       \n\
      mov r2, [rbp+8]       \n\
      mov r3, [rbp+12]      \n\
    outsl_L1:               \n\
      blt r3, 0, outsl_l2   \n\
      sub r3, r3, 1         \n\
      ld  r4, r2, 0         \n\
      st  r4, r1, 0         \n\
      add r2, r2, 4         \n\
      add r3, r3, 4         \n\
      br  outsl_L1          \n\
    outsl_L2:               \n\
      ret                   \n\
  ");
}

static inline void
stosb(void *addr, int data, int cnt)
{
  __asm("\
      mov r1, [rbp+4]       \n\
      mov r2, [rbp+8]       \n\
      mov r3, [rbp+12]      \n\
    stosb_L1:               \n\
      blt r3, 0, stosb_l2   \n\
      sub r3, r3, 1         \n\
      st  r2, r1, 0         \n\
      add r3, r3, 4         \n\
      br  stosb_L1          \n\
    stosb_L2:               \n\
      ret                   \n\
  ");
}

static inline void
stosl(void *addr, int data, int cnt)
{
  __asm("\
      mov r1, [rbp+4]       \n\
      mov r2, [rbp+8]       \n\
      mov r3, [rbp+12]      \n\
    stosl_L1:               \n\
      blt r3, 0, stosl_l2   \n\
      sub r3, r3, 1         \n\
      st  r2, r1, 0         \n\
      add r3, r3, 4         \n\
      br  stosl_L1          \n\
    stosl_L2:               \n\
      ret                   \n\
  ");
}

//struct segdesc;
/*
static inline void
lgdt(struct segdesc *p, int size)
{
  volatile ushort pd[3];

  pd[0] = size-1;
  pd[1] = (uint)p;
  pd[2] = (uint)p >> 16;

  //asm volatile("lgdt (%0)" : : "r" (pd));
  }*/

//struct gatedesc;

/*
static inline void
lidt(struct gatedesc *p, int size)
{
  volatile ushort pd[3];

  pd[0] = size-1;
  pd[1] = (uint)p;
  pd[2] = (uint)p >> 16;

  //asm volatile("lidt (%0)" : : "r" (pd));
  }*/

static inline void
ltr(ushort sel)
{
  //asm volatile("ltr %0" : : "r" (sel));
}

static inline void
loadgs(ushort v)
{
  //asm volatile("movw %0, %%gs" : : "r" (v));
}

static inline void
cli(void)
{
  outb(0x2104, 0);
}

static inline void
sti(void)
{
  outb(0x2104, 1);
}

// read interrupt flag
static inline uchar
readiflg(void)
{
  return inb(0x2104);
}

static inline uint
xchg(volatile uint *addr, uint newval)
{
  uint result;
  
  // The + in "+m" denotes a read-modify-write operand.
  /*
  asm volatile("lock; xchgl %0, %1" :
               "+m" (*addr), "=a" (result) :
               "1" (newval) :
               "cc");
  */
  return result;
}

static inline uint
rcr2(void)
{
  uint val;
  //asm volatile("movl %%cr2,%0" : "=r" (val));
  return val;
}

static inline void
lcr3(uint val) 
{
  //asm volatile("movl %0,%%cr3" : : "r" (val));
}

//PAGEBREAK: 36
// Layout of the trap frame built on the stack by the
// hardware and by trapasm.S, and passed to trap().
struct trapframe {
  // general registers
  uint r31;
  uint r30;
  uint r29;
  uint r28;
  uint r27;
  uint r26;
  uint r25;
  uint r24;
  uint r23;
  uint r22;
  uint r21;
  uint r20;
  uint r19;
  uint r18;
  uint r17;
  uint r16;
  uint r15;
  uint r14;
  uint r13;
  uint r12;
  uint r11;
  uint r10;
  uint r9;
  uint r8;
  uint r7;
  uint r6;
  uint r5;
  uint r4;
  uint r3;
  uint r2;
  uint r1;

  // rest of trap frame
  uint trapno;

  // below here defined by x86 hardware
  uint err;
  uint eip;

  // below here only when crossing rings, such as from user to kernel
  uint esp;
};
