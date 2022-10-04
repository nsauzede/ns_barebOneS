#include "gdt.h"
#include "libc.h"
#include "vid.h"
#include "types.h"

#pragma pack(1)
typedef struct {
    uint16_t size;
#ifdef _LP64
    uint64_t offset;
#else
    uint32_t offset;
#endif
} gdtr_t;
#pragma pack()

// segment selector
//15    3       2       1   0
//Index         TI      RPL

#pragma pack(1)
typedef struct {
#if 1
    uint16_t limit;
    uint16_t base;
    uint8_t baseHA;
    uint8_t acc_byte;
    uint8_t limitH:4, flags:4;
    uint8_t baseHB;
#else
    uint16_t base;
    uint16_t limit;
    uint8_t baseHB;
    uint8_t flags:4, limitH:4;
    uint8_t acc_byte;
    uint8_t baseHA;
#endif
} gdt_t;

#if 1
typedef union {
    uint64_t u;
//    uint32_t u;
    gdt_t g;
} gdt_u;
#pragma pack()
#endif

#if 0
static uint64_t htonq(uint64_t u0) {
    uint64_t u = 0;
    u = (u << 8) | (u0 & 0xff);u0 >>= 8;
    u = (u << 8) | (u0 & 0xff);u0 >>= 8;
    u = (u << 8) | (u0 & 0xff);u0 >>= 8;
    u = (u << 8) | (u0 & 0xff);u0 >>= 8;
    u = (u << 8) | (u0 & 0xff);u0 >>= 8;
    u = (u << 8) | (u0 & 0xff);u0 >>= 8;
    u = (u << 8) | (u0 & 0xff);u0 >>= 8;
    u = (u << 8) | (u0 & 0xff);u0 >>= 8;
    return u;
}
#endif

void dump_sel(uint16_t sel) {
    printk("sel=%" PRIx16 " RPL=%d TI=%s index=%d\n", sel, sel & 3, sel & 4 ? "LDT" : "GDT", sel >> 3);
}

void dump_attr(uint16_t attr) {
    printk("attr=%" PRIx16 " G=%d DB=%d L=%d res=%d - P=%d DPL=%d S=%d E=%d DC=%d RW=%d A=%d\n",
        attr, (attr & 0x8000) >> 15, (attr & 0x4000) >> 14, (attr & 0x2000) >> 13, (attr & 0x1000) >> 12,
        (attr & 0x80) >> 7, (attr & 0x60) >> 5, (attr & 0x10) >> 4, (attr & 0x8) >> 3, (attr & 0x4) >> 2, (attr & 0x2) >> 1, attr & 0x1
    );
}

void dump_gdt_(const gdt_t *gdtp) {
//    const gdt_u *g = (gdt_u *)gdtp;
//        printk("%" PRIx64 "\n", g->u);
#if 0
//        printk("baseHB=%02" PRIx8 " ", gdtp->baseHB);
    printk("flags=%" PRIx8 " ", gdtp->flags);
//        printk("limitH=%" PRIx8 " ", gdtp->limitH);
    printk("acc_byte=%02" PRIx8 " ", gdtp->acc_byte);
//        printk("baseHA=%02" PRIx8 " ", gdtp->baseHA);
//        printk("baseL=%04" PRIx16 " ", gdtp->base);
//        printk("limitL=%04" PRIx16 " ", gdtp->limit);
#endif
#if 1
    dump_attr((gdtp->flags << 8) | gdtp->acc_byte);
    uint32_t base = (gdtp->baseHB << (16 + 8)) | (gdtp->baseHA << 16) | gdtp->base;
    printk("base=%08" PRIx32 " ", base);
    uint32_t limit = (gdtp->limitH << 16) | gdtp->limit;
    printk("limit=%08" PRIx32 " ", limit);
#endif
    printk("\n");
}

void dump_gdt() {
    extern const gdtr_t gdt;
    const gdtr_t *gdtr = &gdt;
    printk("%s: gdtr=%p\n", __func__, gdtr);
    if (gdtr) {
        int size = gdtr->size + 1;
        gdt_t *gdtp = (gdt_t *)(uintptr_t)gdtr->offset;
        printk("%s: size=%d\n", __func__, size);
        printk("%s: offset=%p\n", __func__, gdtp);
        if (gdtp) {
            for (size_t i = 0; i < size / sizeof(gdt_t); i++) {
//                printk("%s:  gdt#%d sizeof(gdt)=%d ptr=%p\n", __func__, i, (int)sizeof(gdt_t), gdtp);
                dump_gdt_(gdtp++);
            }
        }
    }

    uint16_t cs, ds;
    asm volatile(
        "mov %%cs,%%ax\n"
        "mov %%ax,%0\n"
        : "=g"(cs)
        :
        : "ax"
    );
    asm volatile(
        "mov %%ds,%%ax\n"
        "mov %%ax,%0\n"
        : "=g"(ds)
        :
        : "ax"
    );
    printk("cs=%" PRIx16 " ds=%" PRIx16 "\n", cs, ds);
    dump_sel(cs);
    dump_sel(ds);
}