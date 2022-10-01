#include "task.h"
#include "test/test.h"
#if 0
#include <inttypes.h>
#pragma pack(1)
typedef struct {
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
    uint32_t ss;
    uint16_t filler;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
} irq_regs_t;

typedef struct {
    uint32_t ebp;
    uint32_t zero;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} irq_ctx_t;
#pragma pack()

static int kbhits = 0;

static int jiffies = 0;

struct task;
typedef struct task {
    struct task *next;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} task_t;
static task_t tasks[10] = {0};
int ntasks = 0;
static task_t *current = 0;

void disable(){}
void enable(){}

void create_task(void *ptr) {
    disable();
    tasks[ntasks].eip = (uintptr_t)ptr;
    if (current) tasks[ntasks].next = 0;
    current = &tasks[ntasks++];
}

void switch_task() {
    if (current && current->next && current != current->next) {
        current->eip = ctx->eip;
        current->cs = ctx->cs;
        current->eflags = ctx->eflags;
        current = current->next;
    }
}

static void dummy1() {
    int y = 1;
    while (1) {
        static int count = 0;
        gotoxy(0, y);
        printf("%s: jiffies=%d count=%d\n", __func__, jiffies, count++);
    }
}

static void dummy2() {
    int y = 2;
    while (1) {
        static int count = 0;
        gotoxy(0, y);
        printf("%s: jiffies=%d count=%d\n", __func__, jiffies, count++);
    }
}

void schedule() {
    create_task(dummy1);
    enable();
    while (1) {
        static int count = 0;
        for (int i = 0; i < jiffies*1+10; i++) {
        gotoxy(0, 1);
        printf("hi jiffies=%d count=%d\n", jiffies, count++);
            asm volatile("hlt");
        }
        halt();
    }
}

void kernel_main(void) {
    schedule();
    gotoxy(0, 1);
    panic("[toto=%s dead=%" PRIx16 " ffff0=%p ac1dc0d3=%" PRIx32 " 1234=%d]", "toto", 0xdead, 0xffff0, 0xac1dc0d3, 1234);
}
#endif
TEST_F(task, Mock) {
    TEST_LOG("Testing task..\n");
    EXPECT_EQ(42, task_Mock());
}
