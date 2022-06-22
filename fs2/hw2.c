#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disk.h"
#include "hw1.h"
#include "hw2.h"

FileDescTable* pFileDescTable;
//FileSysInfo* pFileSysInfo;
FileTable* pFileTable;

int OpenFile(const char* name, OpenFlag flag)
{   

    //TRUNCATE
    if(flag==0){

    //CREATE
    }else if(flag==1){
        // 길이 가져오기
        int s=strlen(name);
        
        // name 담김
        char str[100];
        // 이름들
        char*names[100];int i=0;
        memcpy(str,name,100);
        char * nameptr=strtok(str,"/");
        while(nameptr!=NULL){
            names[i]=nameptr;
            i++;
            nameptr=strtok(NULL,"/");
        }

        
        char s2[100]="";
        for(int t=0;t<i-1;t++){
            char s1[100]="/";
            strcat(s1,names[t]);
            strcat(s2,s1);
        }
        //

        Directory * a=OpenDirectory(s2);
        // 생성할  파일 이름
        char filename[100];
        strcpy(filename,names[i-1]);
        //printf("%s 이름이다\n",filename);
        // 그냥 diretry 블럭에 쓰기만하면댐
        Inode*inode=malloc(sizeof(Inode));
        int inodeNum=a->inodeNum;
        
        GetInode(inodeNum,inode);
        // 이제 빈공간을 찾아주자
        // 다이렉트부터
        
        for(int i=0;i<4;i++){
            
            int ptr=inode->dirBlockPtr[i];
            // 생성해줘야 하니깐
            // 파일을 생성하는거임
            if(ptr==0){
                int freeDataRegionNum=GetFreeBlockNum();
                DirEntry*drn[8];DirEntry*drn2[8];
                for(int t=0;t<8;t++){
                    drn[t]=malloc(sizeof(DirEntry));
                    drn2[t]=malloc(sizeof(DirEntry));
                }
                int rPtr=inode->dirBlockPtr[i-1];
                char*rBlock=malloc(512);
                memset(rBlock,0,512);DevReadBlock(rPtr,rBlock);
                for(int t=0;t<8;t++){
                    memcpy(drn2[t],rBlock+t*sizeof(DirEntry),sizeof(DirEntry));
                }
                for(int t=0;t<8;t++){
                    if(strcmp(drn2[t]->name,".")==0||strcmp(drn2[t]->name,"..")==0||
                    strcmp(drn2[t]->name,"...")==0||strcmp(drn2[t]->name,"....")==0){
                        memcpy(drn[t],drn2[t],sizeof(DirEntry));
                    }
                }
                
                memset(rBlock,0,512);
                for(int t=0;t<8;t++){
                    memcpy(rBlock+t*sizeof(DirEntry),drn[t],sizeof(DirEntry));
                }
                DevWriteBlock(freeDataRegionNum,rBlock);
                SetBlockBytemap(freeDataRegionNum);
                ptr=freeDataRegionNum;
                inode->dirBlockPtr[i]=freeDataRegionNum;
                PutInode(inodeNum,inode);
                for(int t=0;t<8;t++){
                    free(drn[t]);free(drn2[t]);
                }
                
            }
            
            char*block=malloc(512);
            DevReadBlock(ptr,block);
            DirEntry*drn3[8];
            for(int t=0;t<8;t++){
                drn3[t]=malloc(sizeof(DirEntry));
            }
            for(int t=0;t<8;t++){
                memcpy(drn3[t],block+t*sizeof(DirEntry),sizeof(DirEntry));
            }
            int flag=0;
            for(int t=0;t<8;t++){
                if(strcmp(drn3[t]->name,filename)==0){
                    flag=1;
                    break;
                }
                if(flag==1){
                    break;
                }

                if(strcmp(drn3[t]->name,"")==0){
                    
                    strcpy(drn3[t]->name,filename);
                    int freeinodenum=GetFreeInodeNum();
                    drn3[t]->inodeNum=freeinodenum;
                    char*nblock=malloc(512);
                    memset(nblock,0,512);
                    for(int j=0;j<8;j++){
                        memcpy(nblock+j*sizeof(DirEntry),drn3[j],sizeof(DirEntry));
                    }
                    // 쓰기
                    DevWriteBlock(ptr,nblock);
                    free(nblock);
                    // inode 설정
                    Inode*pInode=malloc(sizeof(Inode));
                    pInode->allocBlocks;
                    pInode->dirBlockPtr;
                    pInode->indirectBlockPtr;
                    pInode->type=FILE_TYPE_FILE;
                    pInode->size=0;
                    int freeInodeno=GetFreeInodeNum();
                    PutInode(freeInodeno,pInode);
                    SetInodeBytemap(freeInodeno);
                    free(pInode);
                    
                    // fis 처리하기 내일 꼭 하자
                    char*newBlock=malloc(512);
                    memset(newBlock,0,512);
                    DevReadBlock(0,newBlock);

                    // file descriptor table 과 file object 설정하기
                    return 1;       
                }
            }
            




        }
        
        //파일디스크립터 관련처리하기

        

        
    //APPEND
    }else{

    }


}


// desc 와 filetable은 1대1 매칭 되는거같음
int WriteFile(int fileDesc, char* pBuffer, int length)
{   
    // 데이터 쓸꺼니깐 블록 할당해주고
    // 파일 디스크립터 찾아서 inode 일단 획득하기
    DescEntry dentry=pFileDescTable->pEntry[fileDesc];
    int fileTableIndex=dentry.fileTableIndex;
    int inodeno=pFileTable->pFile[fileTableIndex].inodeNum;
    // 해당파일객체
    File file=pFileTable->pFile[fileTableIndex];
    // inode 설정하기
    Inode*inode=malloc(sizeof(Inode));
    GetInode(inodeno,inode);
    //direct 부터 4개밖ㅇ ㅔ못쓴다.
    for(int i=0;i<4;i++){
        int ptr=inode->dirBlockPtr[i];
        // 파일 데이터를 위한 블록 생성하기
        if(ptr==0){
            int freeBlockNum=GetFreeBlockNum();
            SetBlockBytemap(freeBlockNum);
            inode->dirBlockPtr[i]=freeBlockNum;
            ptr=freeBlockNum;
        }
        // ptr에 그냥 쓰면 된다.
        char*block=malloc(512);
        memset(block,0,512);
        memcpy(block,pBuffer,512);
        DevWriteBlock(ptr,block);
        free(block);
        file.bUsed=1;
        file.fileOffset+=length;
        pFileTable->pFile[fileTableIndex]=file;
        // fsi 업데이트하기!!!!!!!!!!!!
        return 1;
    }
    //다음 indirect
    // 6개 할거임
    int iPtr=inode->indirectBlockPtr;
    if(iPtr==0){
        int freeBlockno=GetFreeBlockNum();
        SetBlockBytemap(freeBlockno);
        char*block=malloc(512);
        memcpy(block,&freeBlockno,4);
        DevWriteBlock(freeBlockno,block);
        iPtr=freeBlockno;
        free(block);
    }
    for(int i=0;i<128;i++){
        char*block=malloc(512);
        DevReadBlock(iPtr,block);
        int nptr;
        memcpy(&nptr,block+i*sizeof(int),4);
        if(nptr==0){
            char*block2=malloc(512);
            memcpy(block2,pBuffer,length);
            int freeBlockno=GetFreeBlockNum();
            DevWriteBlock(freeBlockno,block2);
            SetBlockBytemap(freeBlockno);
            memcpy(block+i*sizeof(int),&freeBlockno,4);
            DevWriteBlock(iPtr,block);
            // fsi 업데이트하기!!!!!!!!!!!!
            return 1;
        }else{
            continue;
        }
        
    }

    // fsi 업데이트하기!!!!!!!!!!!!
}


int ReadFile(int fileDesc, char* pBuffer, int length)
{   
    // fd 에서 파일 object 획득하기
    DescEntry dentry=pFileDescTable->pEntry[fileDesc];
    int fileTableIndex=dentry.fileTableIndex;
    int inodeno=pFileTable->pFile[fileTableIndex].inodeNum;
    File file=pFileTable->pFile[fileTableIndex];
    int fileOffset=file.fileOffset;
    // 파일 오프셋에서 시작해서 읽어야함
    // 시작은 0부터임
    int i=fileOffset/512;
    Inode*inode=malloc(sizeof(Inode));
    GetInode(inodeno,inode);
    if(0<=i&&i<=3){
        int ptr=inode->dirBlockPtr[0];
        char*block=malloc(512);
        DevReadBlock(ptr,block);
        memcpy(pBuffer,block,length);
        file.fileOffset+=512;
        pFileTable->pFile[fileTableIndex]=file;
        return 1;
    }
    // indirect에서 읽기
    int ptr=inode->indirectBlockPtr;
    char*block=malloc(512);
    DevReadBlock(ptr,block);
    int nptr;
    memcpy(&nptr,block+(i-4)*4,sizeof(int));
    char*block2=malloc(512);
    DevReadBlock(nptr,block2);
    memcpy(pBuffer,block2,length);
    file.fileOffset+=512;
    pFileTable->pFile[fileTableIndex]=file;
    return 1;
    

}

int CloseFile(int fileDesc)
{
    // 해당 파일 디스크립터 찾아서 닫아준다.

    // for(int i=0;i<DESC_ENTRY_NUM;i++){
    //     DescEntry descentry=pFileDescTable->pEntry[i];
    //     if(descentry.bUsed==1){
    //         for(int t=0;t<MAX_FILE_NUM;t++){
    //             File file=pFileTable->pFile[descentry.fileTableIndex];
    //             if(file.inodeNum==file)
    //         }
    //     }
    // }
}

int RemoveFile(char* name)
{

}


int MakeDirectory(char* name)
{   
    // 길이 가져오기
    int s=strlen(name);
    
    // name 담김
    char str[100];
    // 이름들
    char*names[100];int i=0;
    memcpy(str,name,100);
    char * nameptr=strtok(str,"/");
    while(nameptr!=NULL){
        names[i]=nameptr;
        i++;
        nameptr=strtok(NULL,"/");
    }
    int c=i;
    int size=i;
    
    //  ~~parsing
    //상위 디렉토리의 inodeno 변수임!
    // 처음에는 루트니깐 루트는 0번
    int inodeno=0;
    int parentList[100];
    int q=0;
    while(0<c){
        char*nowname=names[size-c];
        c-=1;
        Inode*inode=malloc(sizeof(Inode));
        GetInode(inodeno,inode);
        parentList[q]=inodeno;
        q+=1;
        // 만약에 다 통과해서 c가 0이 되면 생성해줘야 할 차례임
        // 빈공간을 찾아보자
        if(c==0){
            int flag=0;
            // 일단 directPtr에서 찾아보자
            for(int i=0;i<NUM_OF_DIRECT_BLOCK_PTR;i++){
                //상위 디렉토리의 inode에서 읽어오깅!
                int ptr=inode->dirBlockPtr[i];
                //printf("%d ptr 입니다 \n",ptr);
                char*block=malloc(BLOCK_SIZE);
                // 새로 생성해주기 
                // pinode
                if(ptr==0){
                    
                    int freeBlockNum=GetFreeBlockNum();
                    char*blcok=malloc(BLOCK_SIZE);
                    memset(blcok,0,BLOCK_SIZE);
                    DevWriteBlock(freeBlockNum,blcok);
                    free(blcok);
                    // printf("i 입니다 %d\n",i);
                    // printf("프리블럭넘버는 %d\n",freeBlockNum);
                    // 원래 inode 꺼임
                    
                    memcpy(&(inode->dirBlockPtr[i]),&freeBlockNum,sizeof(int));
                    // inode만 설정하기
                    PutInode(inodeno,inode);

                    // direntry 블럭 설정하기
                    // DevReadBlock(freeBlockNum,block);

                    DirEntry*dden[NUM_OF_DIRENT_PER_BLK];
                    for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                        dden[t]=malloc(sizeof(DirEntry));
                    }
                    // 자기꺼 설정해주고 
                    strcpy(dden[0]->name,".");
                    dden[0]->inodeNum=inodeno;
                    
                    // 새로 블럭에 할당하기
                    char*nBlock=malloc(BLOCK_SIZE);
                    
                    memset(nBlock,0,BLOCK_SIZE);
                    if(size==1){
                        memcpy(nBlock,dden[0],sizeof(DirEntry));    
                    }else if(size==2){
                        
                        strcpy(dden[1]->name,"..");
                        dden[1]->inodeNum=parentList[0];
                        memcpy(nBlock,dden[0],sizeof(DirEntry));
                        memcpy(nBlock+sizeof(DirEntry),dden[1],sizeof(DirEntry));
                    }else if(size==3){
                        
                        strcpy(dden[1]->name,"..");
                        dden[1]->inodeNum=parentList[1];
                        strcpy(dden[2]->name,"...");
                        dden[2]->inodeNum=parentList[0];
                        memcpy(nBlock,dden[0],sizeof(DirEntry));
                        memcpy(nBlock+sizeof(DirEntry),dden[1],sizeof(DirEntry));
                        memcpy(nBlock+sizeof(DirEntry)*2,dden[2],sizeof(DirEntry));                           
                    }



                    DevWriteBlock(freeBlockNum,nBlock);
                    SetBlockBytemap(freeBlockNum);
                    free(nBlock);
                    for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                        free(dden[t]);
                    }
                    // ptr로 안넘겨 줬엇다. ........................
                    ptr=freeBlockNum;
                }
                // 아니면 그냥 쓰기
                memset(block,0,BLOCK_SIZE);
                DevReadBlock(ptr,block);
                //printf("%d ptr 이빈다\n",ptr);
                //DirEntry는 블럭당 8개다
                DirEntry*den[NUM_OF_DIRENT_PER_BLK];
                // 일단 할당해주고
                for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                    den[t]=malloc(sizeof(DirEntry));
                }
                // 읽어와주기
                for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                    memcpy(den[t],block+t*sizeof(DirEntry),sizeof(DirEntry));
                }
                // 빈 Entry를 찾아야 하는디
                for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                    // 빈 entry 찾앗다!
                    if(strcmp(den[t]->name,"")==0){
                        //printf("%s 와 %d\n",nowname,t);
                        
                        //t는 entry의 인덱스임
                        strcpy(den[t]->name,nowname);
                        int freeInodeNo=GetFreeInodeNum();
                        den[t]->inodeNum=freeInodeNo;
                        char*newBlock=malloc(BLOCK_SIZE);
                        for(int j=0;j<NUM_OF_DIRENT_PER_BLK;j++){
                            memcpy(newBlock+sizeof(DirEntry)*j,den[j],sizeof(DirEntry));
                        }
                        DevWriteBlock(ptr,newBlock);

                        // 블럭에 쓰기는 완료
                        free(newBlock);
                        //printf("ptr %d , size %d \n",ptr,size);
                        // 새로운 디렉토리를 위해 공간할당해주기
                        char*abc=malloc(BLOCK_SIZE);
                        memset(abc,0,BLOCK_SIZE);
                        DirEntry*dirEntry1=malloc(sizeof(DirEntry));
                        DirEntry*dirEntry2=malloc(sizeof(DirEntry));
                        DirEntry*dirEntry3=malloc(sizeof(DirEntry));
                        DirEntry*dirEntry4=malloc(sizeof(DirEntry));
                        // 자기자신
                        strcpy(dirEntry1->name,".");
                        dirEntry1->inodeNum=freeInodeNo;
                        // (부모꺼)
                        // 현재 inodeno를 찾아서 루트 디렉토리까지 올라가줘야함
                        // 근데 너무 힘ㄷ믈다. 그냥 깊이 3까지 하자..
                        strcpy(dirEntry2->name,"..");
                        dirEntry2->inodeNum=inodeno;
                        int freeDataRegionNum=GetFreeBlockNum();
                        char*blcok=malloc(BLOCK_SIZE);
                        memset(blcok,0,BLOCK_SIZE);
                        free(blcok);
                        DevWriteBlock(freeDataRegionNum,blcok);
                        //printf("%d blockno\n",freeDataRegionNum);
                        
                        //  생성할 direntry
                        if(size==1){
                            memcpy(abc,dirEntry1,sizeof(DirEntry));
                            memcpy(abc+sizeof(DirEntry),dirEntry2,sizeof(DirEntry));
                            DevWriteBlock(freeDataRegionNum,abc);
                        }else if(size==2){
                            
                            strcpy(dirEntry3->name,"...");
                            dirEntry3->inodeNum=parentList[0];
                            memcpy(abc,dirEntry1,sizeof(DirEntry));
                            memcpy(abc+sizeof(DirEntry),dirEntry2,sizeof(DirEntry));
                            memcpy(abc+sizeof(DirEntry)*2,dirEntry3,sizeof(DirEntry));
                            
                            char aa[MAX_NAME_LEN];
                            strcpy(aa,abc+sizeof(DirEntry)*3);
                            //printf("%s\n",aa);

                            DevWriteBlock(freeDataRegionNum,abc);
                        }else if(size==3){
                            strcpy(dirEntry3->name,"...");
                            dirEntry3->inodeNum=parentList[1];
                            strcpy(dirEntry4->name,"....");
                            dirEntry4->inodeNum=parentList[0];
                            memcpy(abc,dirEntry1,sizeof(DirEntry));
                            memcpy(abc+sizeof(DirEntry),dirEntry2,sizeof(DirEntry));
                            memcpy(abc+sizeof(DirEntry)*2,dirEntry3,sizeof(DirEntry));
                            memcpy(abc+sizeof(DirEntry)*3,dirEntry4,sizeof(DirEntry));
                            DevWriteBlock(freeDataRegionNum,abc);
                        }
                        
                        free(abc);
                        
                        // 
                        // (4) 부분하기
                        Inode*pInode=malloc(sizeof(Inode));
                        GetInode(freeInodeNo,pInode);
                        pInode->dirBlockPtr[0]=freeDataRegionNum;
                        //allocBLocks?
                        pInode->allocBlocks=freeInodeNo;
                        pInode->type=FILE_TYPE_DIR;
                        // 이 inode의 사이즈인데
                        pInode->size=sizeof(Inode);
                        PutInode(freeInodeNo,pInode);
                        //printf("%d 입니다\n",freeDataRegionNum);
                        SetBlockBytemap(freeDataRegionNum);
                        SetInodeBytemap(freeInodeNo);

                        // 부모꺼도 바꿔줘야 하는대... 위에서 했다.
                        // FileSysInfo 도 변경해주기
                        char*nBlock=malloc(BLOCK_SIZE);
                        DevReadBlock(FILESYS_INFO_BLOCK,nBlock);
                        FileSysInfo* fsi=malloc(sizeof(FileSysInfo));
                        memcpy(&(fsi->blocks),nBlock,sizeof(int));
                        memcpy(&(fsi->rootInodeNum),nBlock+4,sizeof(int));
                        memcpy(&(fsi->diskCapacity),nBlock+8,sizeof(int));
                        memcpy(&(fsi->numAllocBlocks),nBlock+12,sizeof(int));
                        memcpy(&(fsi->numFreeBlocks),nBlock+16,sizeof(int));
                        memcpy(&(fsi->numAllocInodes),nBlock+20,sizeof(int));
                        memcpy(&(fsi->blockBytemapBlock),nBlock+24,sizeof(int));
                        memcpy(&(fsi->inodeBytemapBlock),nBlock+28,sizeof(int));
                        memcpy(&(fsi->inodeListBlock),nBlock+32,sizeof(int));
                        memcpy(&(fsi->dataRegionBlock),nBlock+36,sizeof(int));
                        
                        fsi->numAllocBlocks+=1;
                        fsi->numFreeBlocks-=1;
                        fsi->numAllocInodes+=1;
                        char*nnBlock=malloc(BLOCK_SIZE);
                        memcpy(nnBlock,fsi,sizeof(FileSysInfo));
                        DevWriteBlock(FILESYS_INFO_BLOCK,nnBlock);

                        free(nBlock);free(nnBlock);

                        return 1;
                    }
                }
            }

            // 여기서부터 indirect 에서 찾는거임
            int ptr=inode->indirectBlockPtr;
            // printf("SADFASDFAFS\n");
            // 새로 만들어 줘야 하면..
            if(ptr==0){
                int freeblocknum=GetFreeBlockNum();
                char*blcok=malloc(BLOCK_SIZE);
                memset(blcok,0,BLOCK_SIZE);
                DevWriteBlock(freeblocknum,blcok);
                free(blcok);
                char*nBlock=malloc(BLOCK_SIZE);
                memset(nBlock,0,BLOCK_SIZE);
                DevWriteBlock(freeblocknum,nBlock);
                SetBlockBytemap(freeblocknum);
                ptr=freeblocknum;
                // inode에도 반영해주기
                inode->indirectBlockPtr=ptr;
                PutInode(inodeno,inode);
                free(nBlock);
                
                
            }
            //printf("%d ptr 입니다\n",ptr);
            //printf("%d indirect 블럭 ptr 임\n",ptr);
            // 잇으면..bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb
            char*block=malloc(BLOCK_SIZE);
            memset(block,0,BLOCK_SIZE);
            DevReadBlock(ptr,block);
            //printf("%d ptr입니다 \n",ptr);
            // indriectblock읽어들여왓다.
            int nextPtr;
            for(int i=0;i<BLOCK_SIZE/sizeof(int);i++){
                memcpy(&nextPtr,block+i*sizeof(int),sizeof(int));
             
            
                // printf("%d 그리고 i는%d?\n",nextPtr,i);
                // 만약 비었다면
                if(nextPtr==0){
                    
                    //printf("%d irnderict 는 43\n",ptr);
                    // 새로운 블럭 할당해주기
                    // 여기서는 direntry 블럭임
                    int freeblocknum=GetFreeBlockNum();
                    memcpy(block+i*sizeof(int),&freeblocknum,sizeof(int));
                    
                    DevWriteBlock(ptr,block);
                    SetBlockBytemap(freeblocknum);
                    //printf("%d 프리블럭넘버\n",freeblocknum);
                    // 통과
                    // char*blcok=malloc(BLOCK_SIZE);
                    // memset(blcok,0,BLOCK_SIZE);
                    // DevReadBlock(ptr,blcok);
                    // int a;
                    // memcpy(&a,blcok,sizeof(int));
                    // printf("블럭에 쓰여진 값은? %d\n",a);
                    //free(blcok);
                    DirEntry*dden[NUM_OF_DIRENT_PER_BLK];
                    for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                        dden[t]=malloc(sizeof(DirEntry));
                        memset(dden[t],0,sizeof(DirEntry));
                    }
                    // 자기꺼 설정해주고 
                    strcpy(dden[0]->name,".");
                    dden[0]->inodeNum=inodeno;
                        
                    // 새로 블럭에 할당하기
                    char*nBlock=malloc(BLOCK_SIZE);
                    memset(nBlock,0,BLOCK_SIZE);    

                    if(size==1){
                        memcpy(nBlock,dden[0],sizeof(DirEntry));    
                    }else if(size==2){
                        strcpy(dden[1]->name,"..");
                        //printf("파랜트리스트%d\n",parentList[0]);
                        dden[1]->inodeNum=parentList[0];
                        memcpy(nBlock,dden[0],sizeof(DirEntry));
                        memcpy(nBlock+sizeof(DirEntry),dden[1],sizeof(DirEntry));
                    }else if(size==3){
                        strcpy(dden[1]->name,"..");
                        dden[1]->inodeNum=parentList[1];
                        strcpy(dden[2]->name,"...");
                        dden[2]->inodeNum=parentList[0];
                        memcpy(nBlock,dden[0],sizeof(DirEntry));
                        memcpy(nBlock+sizeof(DirEntry),dden[1],sizeof(DirEntry));
                        memcpy(nBlock+sizeof(DirEntry)*2,dden[2],sizeof(DirEntry));                           
                    }



                    DevWriteBlock(freeblocknum,nBlock);
                    
                    free(nBlock);
                    for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                        free(dden[t]);
                    }
                    nextPtr=freeblocknum;
                    
                }
            
                int temp;

                // // block은 간접적인 포인터
                // printf("nextPtr %d\n",nextPtr);
                char*newBlock=malloc(BLOCK_SIZE);
                memset(newBlock,0,BLOCK_SIZE);
                DevReadBlock(nextPtr,newBlock);
                DirEntry*den[NUM_OF_DIRENT_PER_BLK];
                // 일단 할당해주고
                for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                    den[t]=malloc(sizeof(DirEntry));
                    memset(den[t],0,sizeof(DirEntry));
                }
                // 읽어와주기
                for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                    memcpy(den[t],newBlock+t*sizeof(DirEntry),sizeof(DirEntry));
                }
                // 빈 Entry를 찾아야 하는디
                for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                    // 빈 entry 찾앗다!
                    if(strcmp(den[t]->name,"")==0){
                        //printf("%d nextptr 입니다\n",nextPtr);
                        //printf("%s 와 %d\n",nowname,t);
                        //printf("%s 빈앤트리찾기\n",den[t]->name);
                        //t는 entry의 인덱스임
                        strcpy(den[t]->name,nowname);
                        int freeInodeNo=GetFreeInodeNum();
                        den[t]->inodeNum=freeInodeNo;
                        // 블럭에 쓰기를 해야 하는데 어케하지..
                        // 그냥 하자.
                        char*newBlock=malloc(BLOCK_SIZE);
                        memset(newBlock,0,BLOCK_SIZE);
                        for(int j=0;j<NUM_OF_DIRENT_PER_BLK;j++){
                            memcpy(newBlock+sizeof(DirEntry)*j,den[j],sizeof(DirEntry));
                        }
                        DevWriteBlock(nextPtr,newBlock);
                        // for(int j=0;j<NUM_OF_DIRENT_PER_BLK;j++){
                        //     printf("%s %d\n",den[j]->name,den[j]->inodeNum);
                        // }
                        // 블럭에 쓰기는 완료
                        free(newBlock);
                        
                        // 새로운 디렉토리를 위해 공간할당해주기
                        char*abc=malloc(BLOCK_SIZE);
                        DirEntry*dirEntry1=malloc(sizeof(DirEntry));
                        DirEntry*dirEntry2=malloc(sizeof(DirEntry));
                        DirEntry*dirEntry3=malloc(sizeof(DirEntry));
                        DirEntry*dirEntry4=malloc(sizeof(DirEntry));
                        // 자기자신
                        strcpy(dirEntry1->name,".");
                        dirEntry1->inodeNum=freeInodeNo;
                        // (부모꺼)
                        // 현재 inodeno를 찾아서 루트 디렉토리까지 올라가줘야함
                        // 근데 너무 힘ㄷ믈다. 그냥 깊이 3까지 하자..
                        strcpy(dirEntry2->name,"..");
                        dirEntry2->inodeNum=inodeno;
                        int freeDataRegionNum=GetFreeBlockNum();
                        char*blcok=malloc(BLOCK_SIZE);
                        memset(blcok,0,BLOCK_SIZE);
                        DevWriteBlock(freeDataRegionNum,blcok);
                        free(blcok);
                        memset(abc,0,BLOCK_SIZE);
                        // 루트 디렉토리에 생성할 direntry
                        if(size==1){
                            memcpy(abc,dirEntry1,sizeof(DirEntry));
                            memcpy(abc+sizeof(DirEntry),dirEntry2,sizeof(DirEntry));
                            DevWriteBlock(freeDataRegionNum,abc);
                        }else if(size==2){
                            strcpy(dirEntry3->name,"...");
                            //printf("%d 파랜트입니당\n",parentList[0]);
                            dirEntry3->inodeNum=parentList[0];
                            memcpy(abc,dirEntry1,sizeof(DirEntry));
                            memcpy(abc+sizeof(DirEntry),dirEntry2,sizeof(DirEntry));
                            memcpy(abc+sizeof(DirEntry)*2,dirEntry3,sizeof(DirEntry));
                            DevWriteBlock(freeDataRegionNum,abc);
                        }else if(size==3){
                            strcpy(dirEntry3->name,"...");
                            dirEntry3->inodeNum=parentList[1];
                            strcpy(dirEntry4->name,"....");
                            dirEntry4->inodeNum=parentList[0];
                            memcpy(abc,dirEntry1,sizeof(DirEntry));
                            memcpy(abc+sizeof(DirEntry),dirEntry2,sizeof(DirEntry));
                            memcpy(abc+sizeof(DirEntry)*2,dirEntry3,sizeof(DirEntry));
                            memcpy(abc+sizeof(DirEntry)*3,dirEntry4,sizeof(DirEntry));
                            DevWriteBlock(freeDataRegionNum,abc);
                        }
                        

                        
                        // 
                        // (4) 부분하기
                        Inode*pInode=malloc(sizeof(Inode));
                        GetInode(freeInodeNo,pInode);
                        pInode->dirBlockPtr[0]=freeDataRegionNum;
                        //allocBLocks?
                        pInode->allocBlocks=freeInodeNo;
                        pInode->type=FILE_TYPE_DIR;
                        // 이 inode의 사이즈인데
                        pInode->size=sizeof(DirEntry);
                        PutInode(freeInodeNo,pInode);
                        //printf("%d 입니다\n",freeDataRegionNum);
                        SetBlockBytemap(freeDataRegionNum);
                        SetInodeBytemap(freeInodeNo);

                        // 부모꺼도 바꿔줘야 하는대... 위에서 했다.
                        // FileSysInfo 도 변경해주기
                        char*nBlock=malloc(BLOCK_SIZE);
                        DevReadBlock(FILESYS_INFO_BLOCK,nBlock);
                        FileSysInfo* fsi=malloc(sizeof(FileSysInfo));
                        memcpy(&(fsi->blocks),nBlock,sizeof(int));
                        memcpy(&(fsi->rootInodeNum),nBlock+4,sizeof(int));
                        memcpy(&(fsi->diskCapacity),nBlock+8,sizeof(int));
                        memcpy(&(fsi->numAllocBlocks),nBlock+12,sizeof(int));
                        memcpy(&(fsi->numFreeBlocks),nBlock+16,sizeof(int));
                        memcpy(&(fsi->numAllocInodes),nBlock+20,sizeof(int));
                        memcpy(&(fsi->blockBytemapBlock),nBlock+24,sizeof(int));
                        memcpy(&(fsi->inodeBytemapBlock),nBlock+28,sizeof(int));
                        memcpy(&(fsi->inodeListBlock),nBlock+32,sizeof(int));
                        memcpy(&(fsi->dataRegionBlock),nBlock+36,sizeof(int));
                        
                        fsi->numAllocBlocks+=1;
                        fsi->numFreeBlocks-=1;
                        fsi->numAllocInodes+=1;
                        char*nnBlock=malloc(BLOCK_SIZE);
                        memcpy(nnBlock,fsi,sizeof(FileSysInfo));
                        DevWriteBlock(FILESYS_INFO_BLOCK,nnBlock);

                        free(nBlock);free(nnBlock);

                        return 1;
                    }
                }
                
            }


            // 꽉찾으면 못함
            return -1;
        }


        //해야 될 거는 현재 디렉토리 명 찾기
        //nowname 의 디렉토리를 찾아야함
        // 루트에서 시작하니깐 루트 부터 찾기
        // inode의 directPtr은 4개
        // indirectPtr은 1개
        //우선 directPtr에서 찾아본다.
        // 상위 디렉토리를 찾는 과정임
        int flag=0;
        for(int i=0;i<NUM_OF_DIRECT_BLOCK_PTR;i++){
            int ptr=inode->dirBlockPtr[i];
            char*block=malloc(BLOCK_SIZE);
            DevReadBlock(ptr,block);
            //DirEntry는 블럭당 8개임
            DirEntry*den[NUM_OF_DIRENT_PER_BLK];
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                den[t]=malloc(sizeof(DirEntry));
            }
            //block에서 읽어오기
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                memcpy(den[t],block+t*sizeof(DirEntry),sizeof(DirEntry));
            }
            //den[t] 에서 이름 같은게 잇으면 통과임
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
               
                if(strcmp(den[t]->name,nowname) == 0){
                    //printf("찾앗어요1\n");
                    flag=1;
                    // 다음 inode 하위 에서 찾을 수 있도록 설정
                    inodeno=den[t]->inodeNum;
                    // parentList 하기
                    //parentList[q]=inodeno;
                    //strcpy(parentList[q],den[t]->name);
                    q+=1;
                    break;
                }
            }
            if(flag==1){
                break;
            }
            free(block);
        }
        //없으면 IndirectPtr에서 찾아본다
        // directPtr에서 찾앗으면
        
        if(flag==1){
            continue;
        }
        //찾지 못햇으면

        int indirectPtr=inode->indirectBlockPtr;
        char*block=malloc(BLOCK_SIZE);
        DevReadBlock(indirectPtr,block);
        for(int i=0;i<BLOCK_SIZE/sizeof(int);i++){
            // ptr은 블록을 가리킴(여기서는 DirEntry임)
            int ptr;
            memcpy(&ptr,block+i*sizeof(int),sizeof(int));
            char*block2=malloc(BLOCK_SIZE);
            DevReadBlock(ptr,block2);
            // block2에는 DirEntry가 저장돼있따.

            DirEntry*den[NUM_OF_DIRENT_PER_BLK];
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                den[t]=malloc(sizeof(DirEntry));
            }
            //block에서 읽어오기
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                memcpy(den[t],block2+t*sizeof(DirEntry),sizeof(DirEntry));
            }
            //den[t] 에서 이름 같은게 잇으면 통과임
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                if(strcmp(den[t]->name,nowname)==0){
                    flag=1;
                    // 다음 inode 하위 에서 찾을 수 있도록 설정
                    inodeno=den[t]->inodeNum;
                    //parentList[q]=inodeno;
                    //strcpy(parentList[q],den[t]->name);
                    q+=1;
                    break;
                }
            }
            if(flag==1){
                break;
            }
            free(block2);

        }

        free(block);
        if(flag==0){
            return -1;
        }
    }
}


int RemoveDirectory(char* name)
{
    // 길이 가져오기
    int s=strlen(name);
    
    // name 담김
    char str[100];
    // 이름들
    char*names[100];int i=0;
    memcpy(str,name,100);
    char * nameptr=strtok(str,"/");
    while(nameptr!=NULL){
        names[i]=nameptr;
        i++;
        nameptr=strtok(NULL,"/");
    }
    // 깊이는 2로 제한햇음..
    //names[0]의 inode를 일단 찾는다
    Inode*pInode = malloc(sizeof(Inode));
    GetInode(0,pInode);
    // 다음 inode 번호만 가져옿ㄴ다
    int inodeNum;
    // dir에서 찾기
    int flag=0;
    for(int i=0;i<4;i++){
        int ptr=pInode->dirBlockPtr[i];
        DirEntry*drnt[8];
        for(int t=0;t<8;t++){
            drnt[t]=malloc(sizeof(DirEntry));
        }
        char* block=malloc(BLOCK_SIZE);
        DevReadBlock(ptr,block);
        for(int t=0;t<8;t++){
            memcpy(drnt[t],block+t*sizeof(DirEntry),sizeof(DirEntry));
        }
        for(int t=0;t<8;t++){
            if(strcmp(drnt[t]->name,names[0])==0){
                inodeNum=drnt[t]->inodeNum;
                flag=1;
                break;
            }
        }
        if(flag==1){
            free(block);
            for(int j=0;j<8;j++){
                free(drnt[j]);
            }
            break;
        }
    }
    // indirect에서 찾기
    int ptr=pInode->indirectBlockPtr;
    char*bblock=malloc(512);
    DevReadBlock(ptr,bblock);
    for(int i=0;i<128;i++){
        int nPtr;
        memcpy(&nPtr,bblock+sizeof(int)*i,4);
        char*b=malloc(512);
        DevReadBlock(nPtr,b);
        DirEntry*drt[8];
        for(int t=0;t<8;t++){
            drt[t]=malloc(sizeof(DirEntry));
        }
        for(int t=0;t<8;t++){
            memcpy(drt[t],b+t*sizeof(DirEntry),sizeof(DirEntry));
        }
        for(int t=0;t<8;t++){
            if(strcmp(drt[t]->name,names[0])==0){
                inodeNum=drt[t]->inodeNum;
                flag=1;
                break;
            }
        }
        if(flag==1){
            free(b);
            for(int j=0;j<8;j++){
                free(drt[j]);
            }
            break;
        }
    }
    
    free(pInode);
    // 다음 으로 찾으면 끝
    Inode*ppInode=malloc(sizeof(Inode));
    GetInode(inodeNum,ppInode);
    // 다이렉트
    for(int i=0;i<4;i++){
        int nptr=ppInode->dirBlockPtr[i];
        DirEntry*drnt[8];
        for(int t=0;t<8;t++){
            drnt[t]=malloc(sizeof(DirEntry));
        }
        char* block=malloc(BLOCK_SIZE);
        DevReadBlock(nptr,block);
        for(int t=0;t<8;t++){
            memcpy(drnt[t],block+t*sizeof(DirEntry),sizeof(DirEntry));
        }
        for(int t=0;t<8;t++){
            if(strcmp(drnt[t]->name,names[1])==0){
                int ind=drnt[t]->inodeNum;
                strcpy(drnt[t]->name,"");
                drnt[t]->inodeNum=0;
                char* bb=malloc(512);
                memset(bb,0,512);
                for(int j=0;j<8;j++){
                    memcpy(bb+j*sizeof(DirEntry),drnt[j],sizeof(DirEntry));
                }
                DevWriteBlock(nptr,bb);
                Inode*inode=malloc(sizeof(Inode));
                GetInode(ind,inode);
                int blocknum=inode->allocBlocks;
                memset(inode,0,sizeof(Inode));
                PutInode(ind,inode);
                ResetInodeBytemap(ind);
                ResetBlockBytemap(blocknum);
                return 1;
            }
        }
    }


    // indirect에서 찾기
    int pptr=ppInode->indirectBlockPtr;
    memset(bblock,0,512);
    DevReadBlock(pptr,bblock);
    for(int i=0;i<128;i++){
        int nPtr;
        memcpy(&nPtr,bblock+i*4,4);
        char*b=malloc(512);
        memset(b,0,512);
        DevReadBlock(nPtr,b);
        DirEntry*drt[8];
        for(int t=0;t<8;t++){
            drt[t]=malloc(sizeof(DirEntry));
        }
        for(int t=0;t<8;t++){
            memcpy(drt[t],b+t*sizeof(DirEntry),sizeof(DirEntry));
        }
        for(int t=0;t<8;t++){
            if(strcmp(drt[t]->name,names[1])==0){
                int ind=drt[t]->inodeNum;
                strcpy(drt[t]->name,"");
                drt[t]->inodeNum=0;
                char* bb=malloc(512);
                memset(bb,0,512);
                for(int j=0;j<8;j++){
                    memcpy(bb+j*sizeof(DirEntry),drt[j],sizeof(DirEntry));
                }
                DevWriteBlock(nPtr,bb);
                Inode*inode=malloc(sizeof(Inode));
                GetInode(ind,inode);
                int blocknum=inode->allocBlocks;
                memset(inode,0,sizeof(Inode));
                PutInode(ind,inode);
                ResetInodeBytemap(ind);
                ResetBlockBytemap(blocknum);
                return 1;
            }
        }
    }

    return -1;

}

// 
void CreateFileSystem(void)
{   
    //시작!
    FileSysInit();
    //FileSySInfo를 설정하자
    // 루트 디렉토리부터 생성하자.

    //getfreeblocknum을 통해서 data region의 시작지점을 찾아준다.
    // 11번
    int freeDataRegionBlock=GetFreeBlockNum();
    //getfreeinodenum을 통해서 빈 inodenum을 찾아준다.
    // 0번
    int freeInodeListNum=GetFreeInodeNum();
    
    char* block=malloc(BLOCK_SIZE);
    

    DirEntry* drt=malloc(sizeof(DirEntry));
    strcpy(drt->name,".");
    drt->inodeNum=0;
    memcpy(block,drt,sizeof(DirEntry));
    DevWriteBlock(freeDataRegionBlock,block);

    free(block);

    char* block2 = malloc(BLOCK_SIZE);
    pFileSysInfo=malloc(sizeof(FileSysInfo));
    pFileSysInfo->blocks=512;
    pFileSysInfo->rootInodeNum=freeInodeListNum;
    pFileSysInfo->diskCapacity=FS_DISK_CAPACITY;
    pFileSysInfo->numAllocBlocks=1;
    pFileSysInfo->numFreeBlocks=512-11;
    pFileSysInfo->numAllocInodes=1;
    pFileSysInfo->blockBytemapBlock=BLOCK_BYTEMAP_BLOCK_NUM;
    pFileSysInfo->inodeBytemapBlock=INODE_BYTEMAP_BLOCK_NUM;
    pFileSysInfo->inodeListBlock=INODELIST_BLOCK_FIRST;
    pFileSysInfo->dataRegionBlock=(INODELIST_BLOCKS)+(INODELIST_BLOCK_FIRST);

    memcpy(block2,pFileSysInfo,sizeof(FileSysInfo));

    DevWriteBlock(FILESYS_INFO_BLOCK,block2);
    free(block2);

    SetBlockBytemap(freeDataRegionBlock);
    SetInodeBytemap(freeInodeListNum);


    //루트 디렉토리 파일에 한개블록 추가(11번블록) INODE에 반영하기
    Inode*ind=malloc(sizeof(Inode));
    GetInode(freeInodeListNum,ind);
    ind->allocBlocks=1;
    ind->dirBlockPtr[0]=freeDataRegionBlock;
    ind->type=FILE_TYPE_DIR;
    ind->size=BLOCK_SIZE*1;
    
    PutInode(freeInodeListNum,ind);
    
}
void OpenFileSystem(void)
{
    DevOpenDisk();
}

void CloseFileSystem(void)
{
    DevCloseDisk();
}
// name으로 찾아서
// 하위 자식들 접근 하는 거?
Directory* OpenDirectory(char* name)
{   

    // 깊이 제한 2로 하기/...
    // 길이 가져오기
    int s=strlen(name);
    
    // name 담김
    char str[100];
    // 이름들
    char*names[100];int i=0;
    memcpy(str,name,100);
    char * nameptr=strtok(str,"/");
    while(nameptr!=NULL){
        names[i]=nameptr;
        i++;
        nameptr=strtok(NULL,"/");
    }
    //Directory 구조체를 반환해준다.
    // 해당 directory의 inode 반환해주면댐
    int ss=i;

    // 1인경우 root에서 찾기
    if(ss==1){
        char*nowname=names[0];
    
        // directPtr 에서 찾기
        Inode*pinode=malloc(sizeof(Inode));
        memset(pinode,0,sizeof(Inode));
        GetInode(0,pinode);
        for(int i=0;i<4;i++){
            int dPtr=pinode->dirBlockPtr[i];
            char*block=malloc(BLOCK_SIZE);
            memset(block,0,BLOCK_SIZE);
            DevReadBlock(dPtr,block);
            DirEntry*drnt[8];
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                drnt[t]=malloc(sizeof(DirEntry));
            }
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                memcpy(drnt[t],block+sizeof(DirEntry)*t,sizeof(DirEntry));
            }
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                if(strcmp(drnt[t]->name,nowname)==0){
                    int inode=drnt[t]->inodeNum;
                    Directory* dir=malloc(sizeof(Directory));
                    dir->inodeNum=inode;
                    free(block);
                    for(int j=0;j<8;j++){
                        free(drnt[j]);
                    }
                    return dir;
                }
            }

        }
        // indirect에서 찾기
        int indirectPtr=pinode->indirectBlockPtr;
        char*block=malloc(BLOCK_SIZE);
        memset(block,0,BLOCK_SIZE);
        DevReadBlock(indirectPtr,block);
        for(int i=0;i<128;i++){
            int a;
            memcpy(&a,block+i*4,4);
            if(a!=0){
                char*nblock=malloc(BLOCK_SIZE);
                memset(nblock,0,BLOCK_SIZE);
                DevReadBlock(a,nblock);
                DirEntry* d[8];
                for(int t=0;t<8;t++){
                    d[t]=malloc(sizeof(DirEntry));
                }
                for(int t=0;t<8;t++){
                    memcpy(d[t],block+t*sizeof(DirEntry),sizeof(DirEntry));
                }
                for(int t=0;t<8;t++){
                    if(strcmp(d[t]->name,nowname)==0){
                        int inode=d[t]->inodeNum;
                        Directory*dir=malloc(sizeof(Directory));
                        dir->inodeNum=inode;
                        free(block);
                        for(int j=0;j<8;j++){
                            free(d[j]);
                        }
                        return dir;
                    }
                }
            }
        }
    // 깊이 1인경우는 완료함
    // 두단계 내려가기
    }else if(ss==2){

        char*nowname=names[0];
        int inodenumber;
        // directPtr 에서 찾기
        Inode*pinode=malloc(sizeof(Inode));
        memset(pinode,0,sizeof(Inode));
        GetInode(0,pinode);
        for(int i=0;i<4;i++){
            int dPtr=pinode->dirBlockPtr[i];
            char*block=malloc(BLOCK_SIZE);
            memset(block,0,BLOCK_SIZE);
            DevReadBlock(dPtr,block);
            DirEntry*drnt[8];
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                drnt[t]=malloc(sizeof(DirEntry));
            }
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                memcpy(drnt[t],block+sizeof(DirEntry)*t,sizeof(DirEntry));
            }
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                if(strcmp(drnt[t]->name,nowname)==0){
                    int inode=drnt[t]->inodeNum;
                    free(block);
                    for(int j=0;j<8;j++){
                        free(drnt[j]);
                    }
                    inodenumber=inode;
                }
            }

        }
        // indirect에서 찾기
        int indirectPtr=pinode->indirectBlockPtr;
        char*block2=malloc(BLOCK_SIZE);
        memset(block2,0,BLOCK_SIZE);
        DevReadBlock(indirectPtr,block2);
        for(int i=0;i<128;i++){
            int a;
            memcpy(&a,block2+i*4,4);
            if(a!=0){
                char*nblock=malloc(BLOCK_SIZE);
                memset(nblock,0,BLOCK_SIZE);
                DevReadBlock(a,nblock);
                DirEntry* d[8];
                for(int t=0;t<8;t++){
                    d[t]=malloc(sizeof(DirEntry));
                }
                for(int t=0;t<8;t++){
                    memcpy(d[t],block2+t*sizeof(DirEntry),sizeof(DirEntry));
                }
                for(int t=0;t<8;t++){
                    if(strcmp(d[t]->name,nowname)==0){
                        int inode=d[t]->inodeNum;
                        
                        free(block2);
                        for(int j=0;j<8;j++){
                            free(d[j]);
                        }
                        inodenumber=inode;
                    }
                }
            }
            
        }
        // 다음 단꼐 찾기
        char*nowname2=names[1];
        
        // directPtr 에서 찾기
        Inode*pinode2=malloc(sizeof(Inode));
        memset(pinode2,0,sizeof(Inode));
        GetInode(inodenumber,pinode2);
        //printf("inodenum 은 %d 입니다\n",inodenumber);
        for(int i=0;i<4;i++){
            int dPtr=pinode2->dirBlockPtr[i];
            char*block=malloc(BLOCK_SIZE);
            memset(block,0,BLOCK_SIZE);
            DevReadBlock(dPtr,block);
            DirEntry*drnt[8];
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                drnt[t]=malloc(sizeof(DirEntry));
            }
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                memcpy(drnt[t],block+sizeof(DirEntry)*t,sizeof(DirEntry));
            }
            for(int t=0;t<NUM_OF_DIRENT_PER_BLK;t++){
                // printf("direntry 이름은 %s 입니다\n",drnt[t]->name);
                if(strcmp(drnt[t]->name,nowname2)==0){
                    int inode=drnt[t]->inodeNum;
                    Directory* dir=malloc(sizeof(Directory));
                    dir->inodeNum=inode;
                    free(block);
                    for(int j=0;j<8;j++){
                        free(drnt[j]);
                    }
                    
                    return dir;
                }
            }

        }
        // indirect에서 찾기
        int indirectPtr2=pinode2->indirectBlockPtr;
        char*block3=malloc(BLOCK_SIZE);
        memset(block3,0,BLOCK_SIZE);
        DevReadBlock(indirectPtr2,block3);
        //printf("indirectPtr2 는 :%d\n",indirectPtr2);
        for(int i=0;i<128;i++){
            int a;
            memcpy(&a,block3+i*4,4);
            
            if(a!=0){
                //printf("a 입니다: %d\n",a);
                char*nblock=malloc(BLOCK_SIZE);
                memset(nblock,0,BLOCK_SIZE);
                DevReadBlock(a,nblock);
                DirEntry* d[8];
                for(int t=0;t<8;t++){
                    d[t]=malloc(sizeof(DirEntry));
                }
                for(int t=0;t<8;t++){
                    memcpy(d[t],nblock+t*sizeof(DirEntry),sizeof(DirEntry));
                }
                for(int t=0;t<8;t++){
                    // printf("%s %d 입니다\n",d[t]->name,d[t]->inodeNum);
                    if(strcmp(d[t]->name,nowname2)==0){
                        int inode=d[t]->inodeNum;
                        Directory*dir=malloc(sizeof(Directory));
                        dir->inodeNum=inode;
                        free(block3);
                        for(int j=0;j<8;j++){
                            free(d[j]);
                        }
                        free(nblock);
                        return dir;
                    }
                }
            }
        }
    }
    printf("여기까지오면 실패한검니다...\n");
}

// pDir로 받아서
// Fileinfo로 넘어간다..
static int count=0;
// 해당 inode의 총 디렉토리 개수를 센다.
void myFunc(int inodenum){
    Inode*pinode=malloc(sizeof(Inode));
    GetInode(inodenum,pinode);
    //우선 direct 부터 탐색
    int flag=0;
    DirEntry*drnt[8];
    for(int i=0;i<4;i++){
        int ptr=pinode->dirBlockPtr[i];
        // printf("ptr 입니다:%d\n",ptr);
        char*block=malloc(BLOCK_SIZE);
        DevReadBlock(ptr,block);
        
        for(int t=0;t<8;t++){
            drnt[t]=malloc(sizeof(DirEntry));
        }
        for(int t=0;t<8;t++){
            memcpy(drnt[t],block+t*sizeof(DirEntry),sizeof(DirEntry));
        }
        for(int t=0;t<8;t++){
            if(strcmp(drnt[t]->name,".")==0||strcmp(drnt[t]->name,"..")==0||
            strcmp(drnt[t]->name,"...")==0||strcmp(drnt[t]->name,"....")==0){
                continue;
            }
            if(strcmp(drnt[t]->name,"")==0){
                flag=1;
            }
            if(flag==1){
                break;
            }
            count+=1;
        }
        if(flag==1){
            break;
        }
    }
    for(int i=0;i<8;i++){
        free(drnt[i]);
    }
    
    if(flag==1){
        return ;
    }
    
    // 그래도 남앗으면 indirect 도 탐색해주기
    int iPtr=pinode->indirectBlockPtr;
    char*block=malloc(BLOCK_SIZE);
    DevReadBlock(iPtr,block);
    for(int i=0;i<128;i++){
        int a;
        
        memcpy(&a,block+i*sizeof(int),sizeof(int));
       
        if(a==0){
            flag=1;
            break;
        }
        if(flag==1){
            break;
        }
        char*nblock=malloc(BLOCK_SIZE);
        DevReadBlock(a,nblock);
        DirEntry*drntry[8];
        for(int t=0;t<8;t++){
            drntry[t]=malloc(sizeof(DirEntry));
        }
        for(int t=0;t<8;t++){
            memcpy(drntry[t],nblock+t*sizeof(DirEntry),sizeof(DirEntry));
        }
        for(int t=0;t<8;t++){
            //printf("%s 이름은?\n",drntry[t]->name);
            if(strcmp(drntry[t]->name,".")==0||strcmp(drntry[t]->name,"..")==0||
            strcmp(drntry[t]->name,"...")==0||strcmp(drntry[t]->name,"....")==0){
                continue;
            }
            if(strcmp(drntry[t]->name,"")==0){
                flag=1;
            }
            if(flag==1){
                break;
            }
            count+=1;
        }
        if(flag==1){
            break;
        }
    }
   
    free(block);
    return ;
}   

static int indirectBlockIndex=0;
static int directPtrIndex=0;
static int dEntryIndex=0;
FileInfo* ReadDirectory(Directory* pDir)
{   
    //count 가 0이라면 카운트 변수 설정해주고 시작하기
    if(count==0){
        myFunc(pDir->inodeNum);
       
        directPtrIndex=0;dEntryIndex=0;
    }else if(count==-1){
        return NULL;
    }
    
    FileInfo*finfo=malloc(sizeof(FileInfo));
    


    Inode*pInode=malloc(sizeof(Inode));
    Inode*ppInode=malloc(sizeof(Inode));
    GetInode(pDir->inodeNum,pInode);
    // 우선 direct 에서 탐색
    for(directPtrIndex;directPtrIndex<4;){
        //printf("directPtrIndex 번호 : %d\n",directPtrIndex);
        int dPtr=pInode->dirBlockPtr[directPtrIndex];
        if(dPtr==0){
            directPtrIndex=0;
            dEntryIndex=0;
            return NULL;
        }
        DirEntry*d[8];
        for(int t=0;t<8;t++){
            d[t]=malloc(sizeof(DirEntry));
        }
        char*b=malloc(BLOCK_SIZE);
        DevReadBlock(dPtr,b);
        for(int t=0;t<8;t++){
            memcpy(d[t],b+t*sizeof(DirEntry),sizeof(DirEntry));
        }

        for(dEntryIndex;dEntryIndex<8;){
            if(strcmp(d[dEntryIndex]->name,"")==0){
               
                directPtrIndex=0;
                dEntryIndex=0;
                return NULL;
            }
            GetInode(d[dEntryIndex]->inodeNum,ppInode);
            
            finfo->filetype=ppInode->type;
      
            finfo->numBlocks=ppInode->allocBlocks;
            
            strcpy(finfo->name,d[dEntryIndex]->name);
            finfo->size=ppInode->size;
            finfo->inodeNum=ppInode->allocBlocks;
            dEntryIndex+=1;
            for(int j=0;j<8;j++){
                free(d[j]);
            }
            free(ppInode);
            return finfo;
        }
        dEntryIndex=0;
        directPtrIndex+=1;
    }

    // indirect 탐색하기
    int indirectPtr=pInode->indirectBlockPtr;
    char*block=malloc(512);
    DevReadBlock(indirectPtr,block);
    for(indirectBlockIndex;indirectBlockIndex<128;){
        int nPtr;
        memcpy(&nPtr,block+indirectBlockIndex*sizeof(int),4);
        if(nPtr==0){
            indirectBlockIndex=0;
            directPtrIndex=0;
            dEntryIndex=0;
            return NULL;
        }
        DirEntry*d[8];
        for(int t=0;t<8;t++){
            d[t]=malloc(sizeof(DirEntry));
        }
        char*b=malloc(BLOCK_SIZE);
        DevReadBlock(nPtr,b);
        for(int t=0;t<8;t++){
            memcpy(d[t],b+t*sizeof(DirEntry),sizeof(DirEntry));
        }
        for(dEntryIndex;dEntryIndex<8;){
            if(strcmp(d[dEntryIndex]->name,"")==0){
                indirectBlockIndex=0;
                directPtrIndex=0;
                dEntryIndex=0;
                return NULL;
            }
            GetInode(d[dEntryIndex]->inodeNum,ppInode);
            
            finfo->filetype=ppInode->type;
            // 이거 먼지 모르겟음
            finfo->numBlocks=ppInode->allocBlocks;
            strcpy(finfo->name,d[dEntryIndex]->name);
            finfo->size=ppInode->size;
            finfo->inodeNum=ppInode->allocBlocks;
            dEntryIndex+=1;
            for(int j=0;j<8;j++){
                free(d[j]);
            }
            free(ppInode);
            return finfo;
        }
        dEntryIndex=0;
        directPtrIndex+=1;
        indirectBlockIndex+=1;
    }

    

    
}

int CloseDirectory(Directory* pDir)
{

}
