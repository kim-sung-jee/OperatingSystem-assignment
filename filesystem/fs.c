#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "disk.h"
#include "fs.h"


void FileSySInit(){
    DevCreateDisk();
	
	char*a=(char*)malloc(sizeof(char)*BLOCK_SIZE);
	

	memset(a,0,BLOCK_SIZE);


	for(int i=0;i<512;i++){
		char*a=(char*)malloc(sizeof(char)*BLOCK_SIZE);
		DevWriteBlock(i,a);
	}
}


//이노드 바이트맵 즉 블럭 1번
//블럭당 바이트 512니깐
//64개?의 파일들에 대한 inode 저장 인지
//1바이트에 저장되니깐
//512개의 파일에 대한 설정정보를 저장하는지..
void SetInodeBytemap(int inodeno)
{
    char* content;
    DevReadBlock(INODE_BYTEMAP_BLOCK_NUM,content);

    for(int i=0;i<64;i++){
        printf("%c",content[i]);
    }


    content[inodeno]=1;
    
    for(int i=0;i<64;i++){
        printf("%c",content[i]);
    }
    


    DevWriteBlock(INODE_BYTEMAP_BLOCK_NUM,content);
}


void ResetInodeBytemap(int inodeno)
{
    char*content;
    DevReadBlock(INODE_BYTEMAP_BLOCK_NUM,content);

    content[inodeno]=0;

    DevWriteBlock(INODE_BYTEMAP_BLOCK_NUM,content);

}


void SetBlockBytemap(int blkno)
{
    char*content;
    DevReadBlock(INODE_BYTEMAP_BLOCK_NUM,content);

    content[blkno]=1;

    DevWriteBlock(INODE_BYTEMAP_BLOCK_NUM,content);


}


void ResetBlockBytemap(int blkno)
{
    char*content;
    DevReadBlock(INODE_BYTEMAP_BLOCK_NUM,content);
    content[blkno]=0;
    DevWriteBlock(INODE_BYTEMAP_BLOCK_NUM,content);
}


void PutInode(int inodeno, Inode* pInode)
{

}


void GetInode(int inodeno, Inode* pInode)
{   
    // 512바이트 = content 
    char*content;
    // 블록당 512바이트니깐 
    // 블록은 연속적이다..
    DevReadBlock(INODELIST_BLOCK_FIRST,content);
    int allocBlocks;
    int size;
    int type;
    int dirBlockPtr1;
    int dirBlockPtr2;
    int dirBlockPtr3;
    int dirBlockPtr4;
    int indirectBlockPtr;

    // 여기서 부터 할당
    int idx=inodeno*8;

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
    // 이부분 수정 필요
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
