#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "disk.h"
#include "fs.h"

void FileSysInit(void){
	DevCreateDisk();
	char * a = (char*)malloc(sizeof(char)*BLOCK_SIZE);

	memset(a,0,BLOCK_SIZE);
	

	for(int i=0;i<512;i++){
		DevWriteBlock(i,a);
	}
	
}


void SetInodeBytemap(int inodeno)
{
	char* content=(char*)malloc(sizeof(char)*BLOCK_SIZE);
	DevReadBlock(INODE_BYTEMAP_BLOCK_NUM,content);



	content[inodeno]=1;
	DevWriteBlock(INODE_BYTEMAP_BLOCK_NUM,content);


}


void ResetInodeBytemap(int inodeno)
{
	char*content=(char*)malloc(sizeof(char)*BLOCK_SIZE);
	DevReadBlock(INODE_BYTEMAP_BLOCK_NUM,content);
	
	



	content[inodeno]=0;
	

	DevWriteBlock(INODE_BYTEMAP_BLOCK_NUM,content);
}


void SetBlockBytemap(int blkno)
{
	char*content=(char*)malloc(sizeof(char)*BLOCK_SIZE);
	DevReadBlock(INODE_BYTEMAP_BLOCK_NUM,content);

	content[blkno]=1;

	DevWriteBlock(INODE_BYTEMAP_BLOCK_NUM,content);
}


void ResetBlockBytemap(int blkno)
{
	char*content=(char*)malloc(sizeof(char)*BLOCK_SIZE);
	DevReadBlock(INODE_BYTEMAP_BLOCK_NUM,content);
	content[blkno]=0;
	DevWriteBlock(INODE_BYTEMAP_BLOCK_NUM,content);
}


void PutInode(int inodeno, Inode* pInode)
{	
	char*content=(char*)malloc(sizeof(char)*BLOCK_SIZE);

	// find block
	if(0<=inodeno&&inodeno<=15){
        DevReadBlock(INODELIST_BLOCK_FIRST,content);
		
    }else if(16<=inodeno&&inodeno<=31){
        DevReadBlock(INODELIST_BLOCK_FIRST+1,content);
    }else if(32<=inodeno&&inodeno<=47){
        DevReadBlock(INODELIST_BLOCK_FIRST+2,content);
    }else if(48<=inodeno&&inodeno<=63){
        DevReadBlock(INODELIST_BLOCK_FIRST+3,content);
    }

	// variable
	int allocBlocks=pInode->allocBlocks;
    int size=pInode->size;
    int type=pInode->type;
    int dirBlockPtr1=pInode->dirBlockPtr[0];
    int dirBlockPtr2=pInode->dirBlockPtr[1];
    int dirBlockPtr3=pInode->dirBlockPtr[2];
    int dirBlockPtr4=pInode->dirBlockPtr[3];
    int indirectBlockPtr=pInode->indirectBlockPtr;

	int idx=(inodeno%16)*8;
	

	memcpy(content+(idx)*4,&allocBlocks,sizeof(int));
    memcpy(content+(idx+1)*4,&size,sizeof(int));
    memcpy(content+(idx+2)*4,&type,sizeof(int));
    memcpy(content+(idx+3)*4,&dirBlockPtr1,sizeof(int));
    memcpy(content+(idx+4)*4,&dirBlockPtr2,sizeof(int));
    memcpy(content+(idx+5)*4,&dirBlockPtr3,sizeof(int));
    memcpy(content+(idx+6)*4,&dirBlockPtr4,sizeof(int));
    memcpy(content+(idx+7)*4,&indirectBlockPtr,sizeof(int));
	

	if(0<=inodeno&&inodeno<=15){
		DevWriteBlock(INODELIST_BLOCK_FIRST,content);
    }else if(16<=inodeno&&inodeno<=31){
        DevWriteBlock(INODELIST_BLOCK_FIRST+1,content);
    }else if(32<=inodeno&&inodeno<=47){
        DevWriteBlock(INODELIST_BLOCK_FIRST+2,content);
    }else if(48<=inodeno&&inodeno<=63){
        DevWriteBlock(INODELIST_BLOCK_FIRST+3,content);
    }

	

	


}


void GetInode(int inodeno, Inode* pInode)
{
	char*content=(char*)malloc(sizeof(char)*BLOCK_SIZE);

	if(0<=inodeno&&inodeno<=15){
        DevReadBlock(INODELIST_BLOCK_FIRST,content);
    }else if(16<=inodeno&&inodeno<=31){
        DevReadBlock(INODELIST_BLOCK_FIRST+1,content);
    }else if(32<=inodeno&&inodeno<=47){
        DevReadBlock(INODELIST_BLOCK_FIRST+2,content);
    }else if(48<=inodeno&&inodeno<=63){
        DevReadBlock(INODELIST_BLOCK_FIRST+3,content);
    }
	// variable
	int allocBlocks;
    int size;
    int type;
    int dirBlockPtr1;
    int dirBlockPtr2;
    int dirBlockPtr3;
    int dirBlockPtr4;
    int indirectBlockPtr;

	int idx=(inodeno%16)*8;
	
	

	memcpy(&allocBlocks,content+(idx)*4,sizeof(int));
    memcpy(&size,content+(idx+1)*4,sizeof(int));
    memcpy(&type,content+(idx+2)*4,sizeof(int));
    memcpy(&dirBlockPtr1,content+(idx+3)*4,sizeof(int));
    memcpy(&dirBlockPtr2,content+(idx+4)*4,sizeof(int));
    memcpy(&dirBlockPtr3,content+(idx+5)*4,sizeof(int));
    memcpy(&dirBlockPtr4,content+(idx+6)*4,sizeof(int));
    memcpy(&indirectBlockPtr,content+(idx+7)*4,sizeof(int));

	
	

    pInode->allocBlocks=allocBlocks;
    pInode->size=size;
    pInode->type=type;
    pInode->indirectBlockPtr=indirectBlockPtr;
	

	pInode->dirBlockPtr[0]=dirBlockPtr1;
    pInode->dirBlockPtr[1]=dirBlockPtr2;
    pInode->dirBlockPtr[2]=dirBlockPtr3;
    pInode->dirBlockPtr[3]=dirBlockPtr4;



}


int GetFreeInodeNum(void)
{

}


int GetFreeBlockNum(void)
{

}

void PutIndirectBlockEntry(int blkno, int index, int number)
{

}

int GetIndirectBlockEntry(int blkno, int index)
{

}

void PutDirEntry(int blkno, int index, DirEntry* pEntry)
{

}

void GetDirEntry(int blkno, int index, DirEntry* pEntry)
{

}
