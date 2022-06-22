for(int j=0;j<DESC_ENTRY_NUM;j++){
                
                DescEntry dentry=pFileDescTable->pEntry[j];
                if(dentry.bUsed==0){
                    dentry.bUsed=1;
                    File  file;
                    // 파일 객체
                    file.bUsed=1;
                    file.fileOffset=0;
                    file.inodeNum=freeInodeno;
                    // 디스크립터 테이블
                    pFileDescTable->pEntry[j].bUsed=1;
                    pFileDescTable->numUsedDescEntry+=1;
                    // 디스크립터 테이블의 해당 인덱스의 fileTableindex 설정
                    int index;
                    for(int z=0;z<MAX_FILE_NUM;j++){
                        if(pFileTable->pFile->bUsed==0){
                            // 파일 테이블
                            pFileTable->numUsedFile+=1;
                            pFileTable->pFile[z]=file;
            
                            dentry.fileTableIndex=z;
                            index=j;
                            break;
                        }
                    }
                    pFileDescTable->pEntry[j].fileTableIndex=index;
                    

                    return j;
                }else{
                    continue;
                }
            }
