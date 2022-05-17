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
    // 파일을 읽기,쓰기 전용으로 연다.
    // 파일이 없으면 생성한다.
    // 파일이 이미 존재하면 기존 파일의 내용을 무시하면서 연다.
    // 권한은 0644 
    //사이즈는...?
    fd = open("MY_DISK", O_RDWR | O_CREAT | O_TRUNC, 0644);
}

// my_disk 파일 열어서  파일 디스크립터 획득
void DevOpenDisk(void)
{
	fd = open("MY_DISK", O_RDWR);
}
//fd를 움직여 블록 움직이게한다?.
// SEEK_SET 은 파일의 시작 위치를 기준으로함
void __DevMoveBlock(int blkno){
    // 블럭 단위로 움직인다.
    lseek(fd, (off_t)+(BLOCK_SIZE*blkno),SEEK_SET);
}
// char은 1바이트 이니깐..
// char* 는 4바이트 혹은 8바이트 아마 8바이트일것 8바이트 맞음
// pBuf가 가리키는 거는 512 바이트 의 공간
//
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
