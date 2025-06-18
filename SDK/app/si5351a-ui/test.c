#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <limits.h> // For PTHREAD_STACK_MIN
int main() {
    pthread_attr_t attr;
    int ret;

    pthread_attr_init(&attr);
    printf("PTHREAD_STACK_MIN: %zu\n", (size_t)PTHREAD_STACK_MIN);
    ret = pthread_attr_setstacksize(&attr, PTHREAD_STACK_MIN);  // 设置线程栈大小

    if (ret != 0)
        printf("Error: %s\n", strerror(ret));  // 打印错误
    else
        printf("pthread_attr_setstacksize success\n");

    return 0;
}
