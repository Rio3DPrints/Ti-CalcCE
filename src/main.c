#include <stdio.h>
#include <ti/getcsc.h>

int main(void) {
    printf("Hello TI-84!");
    while(!os_GetCSC());
    return 0;
}
