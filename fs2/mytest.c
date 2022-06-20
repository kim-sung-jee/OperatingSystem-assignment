#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include "hw1.h"
#include "hw2.h"


int main(){
    CreateFileSystem();
    int a=MakeDirectory("/abc");
    char dirName[MAX_NAME_LEN] = {0};
    for (int i = 0;i < 9;i++)
    {
        memset(dirName, 0, MAX_NAME_LEN);
        sprintf(dirName, "/abc/u%d", i);
        a=MakeDirectory(dirName);
    }
    //printf("%d\n",a);
    Inode*inode=malloc(sizeof(Inode));
    GetInode(8,inode);
    //12 . 19
    printf("%d\n%d\n",inode->dirBlockPtr[0],inode->dirBlockPtr[1]);
}
