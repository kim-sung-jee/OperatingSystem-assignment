#include "fs.h"
#include "disk.h"
#include <stdio.h>
#include<stdlib.h>
#include<memory.h>

int main(){
	
	FileSysInit();


	struct _Inode *inode1=malloc(32);
	inode1->allocBlocks=132;
	inode1->size=15;
	inode1->type=1;
	inode1->dirBlockPtr[0]=1;
	inode1->dirBlockPtr[1]=5;
	inode1->dirBlockPtr[2]=12;
	inode1->dirBlockPtr[3]=13;
	inode1->indirectBlockPtr=17;
	
	PutInode(0,inode1);

	char* a = (char*)malloc(sizeof(char)*BLOCK_SIZE);
	DevReadBlock(INODELIST_BLOCK_FIRST,a);
	int b=1;
	memcpy(&b,a+4,sizeof(int));
	printf("%d\n",b);

	// struct _Inode *inode2=malloc(32);
	// //memset(inode2,0,32);

	// GetInode(0,inode2);
	// printf("%d",inode2->allocBlocks);
	return 0;
}
