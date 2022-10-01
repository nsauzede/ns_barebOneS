#ifndef TYPES_H__
#define TYPES_H__

#ifdef _LP64
typedef unsigned long uint64_t;
typedef unsigned int uint32_t;
#define PRIx64 "lx"
#define PRIx32 "x"
#else
typedef unsigned long long uint64_t;
typedef unsigned long uint32_t;
#define PRIx64 "llx"
#define PRIx32 "lx"
#endif
#define PRIx8 "x"
#define PRIx16 "x"

typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef unsigned long uintptr_t;
typedef unsigned long size_t;

#endif/*TYPES_H__*/
