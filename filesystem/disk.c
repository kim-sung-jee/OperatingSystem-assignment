#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include "disk.h"

int fd; 

//가상디스크 생성함.. 파일로 생성된다.
//Inode 는 32bytes 
//Block 하나당 512bytes 이다
//Block 당 inode 개수는 16개 Block 개수(InodeList담는) 4개
// 즉 64개의 파일을 관리할 수 있게 된다.
// 가상디스크의 전체 크기는? 512*512 개다
//521 빼기 1 은 520 빼기 2 는 518 빼기4는 514개
//514개가 block region인데
//
void DevCreateDisk(void)
{
    fd = open("MY_DISK", O_RDWR | O_CREAT | O_TRUNC, 0644);
}

// my_disk 파일 열어서  파일 디스크립터 획득
void DevOpenDisk(void)
{
	fd = open("MY_DISK", O_RDWR);
}

void __DevMoveBlock(int blkno){
    lseek(fd, (off_t)+(BLOCK_SIZE*blkno),SEEK_SET);
}

void DevReadBlock(int blkno, char* pBuf)
{
   __DevMoveBlock(blkno);
   read(fd, pBuf, BLOCK_SIZE);
}

void DevWriteBlock(int blkno, char* pBuf)
{
   __DevMoveBlock(blkno);
   write(fd, pBuf, BLOCK_SIZE);
}


void DevCloseDisk(void)
{
    close(fd);
}
