#include <stdio.h>
#include<stdbool.h>

int main(void)
{
    char x[100];

    while(true) {
        // scanf(" %s",&x);
        fgets(x,100,stdin);
        
        printf("Value is %s\n",x);
    }
    return 0;
}