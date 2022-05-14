#include "fs.h"
#include "disk.h"
#include <stdio.h>
#include<stdlib.h>

#include<string.h>
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

	
	// DevReadBlock(INODELIST_BLOCK_FIRST,a);
	// for(int i=0;i<32;i+=4){
	// 	int b;
	// 	memcpy(&b,a+i,sizeof(int));
	// 	printf("%d\n",b);
	// }
	
	
	struct _Inode *inode2=malloc(32);
	// memset(inode2,0,32);

	GetInode(0,inode2);
	printf("%d",inode2->size);
	return 0;
}
