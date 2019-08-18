#ifndef blog_utils
#define blog_utils
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char * textFileRead(const char* filename){
    char* text;
    FILE *pf = fopen(filename,"r");
    if(pf==NULL)
        return NULL;
    fseek(pf,0,SEEK_END);
    long lSize = ftell(pf);
    // 用完后需要将内存free掉
    text=(char*)malloc(lSize+1);
    rewind(pf); 
    fread(text,sizeof(char),lSize,pf);
    text[lSize] = '\0';
    return text;
}
#endif