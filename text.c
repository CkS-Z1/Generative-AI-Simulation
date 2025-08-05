#include <stdio.h>
#include <stdlib.h>
#include <dos.h>

// 定义最大堆测试大小（单位：MB）
#define MAX_HEAP_SIZE 16

// 测试堆内存分配
void test_heap() {
    unsigned long size;
    int i;
    void *mem;
    for (i = 1; i <= MAX_HEAP_SIZE; i++) {
        size = 1024 * 1024 * i; // 每次增加1MB
        mem = malloc(size);
        if (mem) {
            printf("[Heap] Success: Allocated %dMB of heap memory.\n", i);
            free(mem);
        } else {
            printf("[Heap] Failed: Could not allocate %dMB of heap memory.\n", i);
            break;
        }
    }
}

// 测试堆栈内存分配（通过递归深度）
void stack_test(int depth) {
    char buffer[4096]; // 每次递归消耗4KB堆栈
    if (depth >= 1000) { // 设置最大递归深度
        printf("[Stack] Reached maximum depth: %d\n", depth);
        return;
    }
    stack_test(depth + 1); // 递归调用
}

int main() {
    // 输出DPMI驱动信息（需DPMI支持）
//    printf("DPMI Status: 0x%04X\n", _dpmi_get_version());

    // 测试堆内存
    printf("Testing heap allocation...\n");
    test_heap();

    // 测试堆栈内存
    printf("\nTesting stack allocation...\n");
    stack_test(1);

    return 0;
}