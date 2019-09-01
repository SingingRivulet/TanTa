#ifndef blog_utils
#define blog_utils
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char * textFileRead(const char* filename){
    printf("load file:%s ",filename);
    char* text;
    FILE *pf = fopen(filename,"r");
    if(pf==NULL){
        printf("fail \n");
        return NULL;
    }
    fseek(pf,0,SEEK_END);
    long lSize = ftell(pf);
    // 用完后需要将内存free掉
    text=(char*)malloc(lSize+1);
    rewind(pf); 
    fread(text,sizeof(char),lSize,pf);
    text[lSize] = '\0';
    fclose(pf);
    printf("success \n");
    return text;
}

#define SEND_FILE_HANDLER_CREATE(name_,path_,mime_) \
    char * send_file_handler_data_##name_; \
    int send_file_handler_len_##name_; \
    void send_file_handler_init_##name_(){ \
        printf("load file:"path_); \
        send_file_handler_len_##name_  = 0; \
        send_file_handler_data_##name_ = NULL; \
        FILE *pf = fopen(path_,"r"); \
        if(pf==NULL){ \
            return; \
        } \
        fseek(pf,0,SEEK_END); \
        send_file_handler_len_##name_ = ftell(pf); \
        send_file_handler_data_##name_=(char*)malloc(send_file_handler_len_##name_); \
        rewind(pf);  \
        fread(send_file_handler_data_##name_,sizeof(char),send_file_handler_len_##name_,pf); \
        fclose(pf); \
        printf(" len:%d\n",send_file_handler_len_##name_); \
    } \
    void send_file_handler_close_##name_(){ \
        if(send_file_handler_data_##name_) \
            free(send_file_handler_data_##name_); \
    } \
    LWAN_HANDLER(send_file_handler_callback_##name_){ \
        response->mime_type = mime_; \
        if(send_file_handler_len_##name_>0){ \
            lwan_strbuf_set_static( \
                response->buffer, \
                send_file_handler_data_##name_, \
                send_file_handler_len_##name_); \
            return HTTP_OK; \
        } \
        return HTTP_NOT_FOUND; \
    }
#define SEND_FILE_HANDLER_INIT(name_) \
    send_file_handler_init_##name_()
#define SEND_FILE_HANDLER_CLOSE(name_) \
    send_file_handler_close_##name_()
#define SEND_FILE_HANDLER(name_) \
    LWAN_HANDLER_REF(send_file_handler_callback_##name_)

#endif