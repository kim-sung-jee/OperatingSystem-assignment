#include "fs.h"
#include "disk.h"
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
int main(){
	
	FileSysInit();

	struct __dirEntry * direntry=malloc(32);
	//char b[MAX_NAME_LEN]="asdgasdasg";
	//direntry->name=b;
	strcpy(direntry->name,"akakakakak");
	direntry->inodeNum=14;

	PutDirEntry(131,3,direntry);

	struct __dirEntry * direntry2=malloc(32);
	int a=GetDirEntry(131,3,direntry2);
       	printf("\n");
	printf("%d",a);	
	
	RemoveDirEntry(131,3);
	printf("---------\n");
	struct __dirEntry * direntry3=malloc(32);
	printf("%d",GetDirEntry(131,3,direntry3));

	


//	printf("%d",direntry2->inodeNum);


	// PutIndirectBlockEntry(100,5,129);
	// int tmp=GetIndirectBlockEntry(100,5);
	// printf("%d",tmp);


	// struct _Inode *inode1=malloc(32);
	// inode1->allocBlocks=132;
	// inode1->size=15;
	// inode1->type=1;
	// inode1->dirBlockPtr[0]=1;
	// inode1->dirBlockPtr[1]=5;
	// inode1->dirBlockPtr[2]=12;
	// inode1->dirBlockPtr[3]=13;
	// inode1->indirectBlockPtr=17;
	
	// PutInode(14,inode1);

	/////
	// char* a =malloc(512);

	// DevReadBlock(3,a);


	
// a
	//memcpy(&c,a+48,sizeof(int));

	// memcpy(&c,a,sizeof(int));
	






	// printf("%d",inode2->size);


	//struct _Inode *inode2=malloc(32);
	

	//GetInode(14,inode2);











	// for(int i=0;i<13;i++){
	// 	SetBlockBytemap(i);
	// }
	// int a=GetFreeBlockNum();
	// printf("%d\n",a);
	// // DevReadBlock(INODELIST_BLOCK_FIRST,a);
	// // for(int i=0;i<32;i+=4){
	// // 	int b;
	// // 	memcpy(&b,a+i,sizeof(int));
	// // 	printf("%d\n",b);
	// // }



	// return 0;
}
