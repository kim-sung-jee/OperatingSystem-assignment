#ifndef __FILESYSTEM_H__
#define __FILESYSTEM_H__

#include "disk.h"


// ------- Caution -----------------------------------------
#define FS_DISK_CAPACITY		(BLOCK_SIZE*512) 	


#define NUM_OF_DIRECT_BLOCK_PTR	(5)	/* direct block pointer의 개수 */
#define MAX_NAME_LEN     	(28)


#define FILESYS_INFO_BLOCK    (0) /* file system info block no. */
#define INODE_BYTEMAP_BLOCK_NUM  (1) /* inode bytemap block no. */
#define BLOCK_BYTEMAP_BLOCK_NUM  (2) /* block bytemap block no. */
#define INODELIST_BLOCK_FIRST   (3) /* the first block no. of inode list */
#define INODELIST_BLOCKS        (4) /* the number of blocks in inode list */


// ----------------------------------------------------------


//총 32바이트임
//블록내 앤트리(총128개)에 저장되는 정보임
//블록당 16개씩 저장..?
typedef struct  __dirEntry {
     char name[MAX_NAME_LEN];        // file name
     int inodeNum; 
} DirEntry;

//4+4+4+4+4=20바이트???
//32바이트인데 나중에
typedef struct _Inode {
	int 	allocBlocks;//몇번블록?	블록당 16개 잇으니깐
	//
	int 	size;
	int 	type;
	//포인터 하나당 몇개의 블록을 가지나면
	//1개의 블럭?에 16개의 DirEntry 가짐
    int		dirBlockPtr[NUM_OF_DIRECT_BLOCK_PTR];	// block pointers
	// 이것도 포인터 5개
	//음..블럭 5개
	int		indirectBlockPtr;				// indirect block pointer
} Inode;

//inode bytemap에서 inodeno 번째 바이트를
//1로 설정하고 다시 가상디스크(파일)에 저장한다.
void SetInodeBytemap(int inodeno);
//inode bytemap에서 inodeno번째 바이트를
//0으로 설정하고 다시 가상디스크(파일)에 저장한다. 
void ResetInodeBytemap(int inodeno);
//block bytemap에서 blkno 번째 바이트를 
//1로 설정하고 다시 가상디스크(파일)에 저장한다.
void SetBlockBytemap(int blkno);
//block bytemap에서 blkno번째 바이트를
//1로 설정하고 다시 가상디스크(파일)에 저장한다.
void ResetBlockBytemap(int blkno);
//pInode의 정보를 inodeno번째 inode에 저장한다
void PutInode(int inodeno, Inode* pInode);
//inodeno 를 디스크에서 읽어서 pinode가 지정하는
//메모리 공간으로 저장한다.
void GetInode(int inodeno, Inode* pInode);
// 할당되지 않은 inode번호를 획득한다.
// first fit searching 방법을 사용하고
// 당연히 inodebytemap에서 찾아냄
int GetFreeInodeNum(void);
// 할당되지 않은 blocknum번호를 획득한다.
// first fit searching 방법을 사용하고
// 당연히 blockbytemap에서 찾아냄
int GetFreeBlockNum(void);
// 블럭 넘버,몇번 인덱스인지(아마 블록은 배열형식으로 구성되엇을거)
// 16개니간..128개? idx가 128개까지잇음
//number은 값임
//entry의 크기는 4바이트 즉 블록당128개의 앤트리가 잇음
// 이거 주소가 담김
void PutIndirectBlockEntry(int blkno, int index, int number);
//
int GetIndirectBlockEntry(int blkno, int index);
//direct임
// 앤트리당 32바이트니간
// 블럭당 16개 들어간다...
void PutDirEntry(int blkno, int index, DirEntry* pEntry);
//
void GetDirEntry(int blkno, int index, DirEntry* pEntry);


#endif /* FILESYSTEM_H_ */
