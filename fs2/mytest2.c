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
    MakeDirectory("/tmp");
    MakeDirectory("/usr");
    MakeDirectory("/etc");
    MakeDirectory("/home");
    /* make home directory */
    char dirName[MAX_NAME_LEN] = {0};
    int i=0;
    for (i = 0;i < 27;i++)
    {
        memset(dirName, 0, MAX_NAME_LEN);
        sprintf(dirName, "/home/u%d", i);
        MakeDirectory(dirName);
    }
    
    // Inode* pinode=malloc(sizeof(Inode));
    // GetInode(4,pinode);
    // int no=pinode->dirBlockPtr[0];
    // char*block=malloc(BLOCK_SIZE);
    // DevReadBlock(22,block);
    // DirEntry*drnt[8];
    // for(int i=0;i<8;i++){
    //     drnt[i]=malloc(sizeof(DirEntry));
    // }
    // for(int i=0;i<8;i++){
    //     memcpy(drnt[i],block+i*sizeof(DirEntry),sizeof(DirEntry));
    // }
    // for(int i=0;i<8;i++){
    //     printf("%s %d \n",drnt[i]->name,drnt[i]->inodeNum);
    // }
    // char*block=malloc(BLOCK_SIZE);
    // DevReadBlock(39,block);
    // DirEntry*drnt[8];
    // for(int i=0;i<8;i++){
    //     drnt[i]=malloc(sizeof(DirEntry));
    // }
    // for(int i=0;i<8;i++){
    //     memcpy(drnt[i],block+i*sizeof(DirEntry),sizeof(DirEntry));
    // }
    // printf("--------------\n");
    // printf("%d\n",GetFreeBlockNum());
    // for(int i=0;i<8;i++){
    //     printf("%s %d\n",drnt[i]->name,drnt[i]->inodeNum);
    // }
    // // int indirect;
    // Inode*iii=malloc(sizeof(Inode));
    // GetInode(0,iii);
    // memcpy(&indirect,&(iii->dirBlockPtr[1]),sizeof(int));
    // printf("%d dlqlsek\n",indirect);
    // Inode*inode=malloc(sizeof(Inode));
    // GetInode(8,inode);
    // //12 . 19
    // printf("%d\n%d\n",inode->dirBlockPtr[0],inode->dirBlockPtr[1]);
}
