#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct Point
{
   int xpos;
   int ypos;
};
//203496
//203497
int main(){
	struct Point * p = malloc(sizeof(struct Point));
    char*a=p;
    p->xpos=5;
    p->ypos=154;
    printf("%p\n",p);//11410152
    printf("%p\n",a);
    int d;
    memcpy(&d,a+4,4);
    printf("%d\n",d);


    printf("%p\n",a+1);
    printf("%p\n",p+1);//11410160
    printf("%p\n",p+2);//11410160
    printf("%p\n",&(p->xpos));
    printf("%p\n",&(p->ypos));
    //printf("%d",*(p+1));
    
    
}
