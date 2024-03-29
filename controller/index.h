#ifndef blog_index
#define blog_index

#if defined(__cplusplus)
extern "C" {
#endif

#include <lwan/lwan.h>
#include <lwan/lwan-mod-serve-files.h>
#include "utils.h"
#include "model/model.h"

char * index_header=NULL;
int index_header_len;

char * index_footer=NULL;
int index_footer_len;

void controller_index_init(){
    index_header = textFileRead("./view/index/header.tpl");
    if(index_header)
        index_header_len=strlen(index_header);
    else
        index_header_len=0;
        
    index_footer = textFileRead("./view/index/footer.tpl");
    if(index_footer)
        index_footer_len=strlen(index_footer);
    else
        index_footer_len=0;
}

void controller_index_destroy(){
    if(index_header)free(index_header);
    if(index_footer)free(index_footer);
}

LWAN_HANDLER(index){
    const char * url = request->url.value;
    
    response->mime_type = "text/html;charset=utf-8";
    
    const char * classify   = lwan_request_get_query_param(request,"classify");
    
    if(request->url.len>0){
        const char * p;
        char c;
        p = url;
        while(1){
            c=*p;
            if(c=='\0')
                break;
            
            if((c>='a' && c<='f') || (c>='0' && c<='9') || c==':' || c=='-'){
                
            }else{
                return HTTP_NOT_FOUND;
            }
            
            ++p;
        }
    }
    
    int psgstatus;
    
    if(index_header_len>0)
        lwan_strbuf_append_str(response->buffer, index_header, index_header_len);
    
    if(request->url.len>0)
        psgstatus = model_passage_index_send(request->url.value , classify , response);
    else
        psgstatus = model_passage_index_send(NULL , classify ,response);
    
    if(index_footer_len!=0)
        lwan_strbuf_append_str(response->buffer, index_footer, index_footer_len);
    
    if(psgstatus)
        return HTTP_OK;
    else
        return HTTP_NOT_FOUND;
}

#if defined(__cplusplus)
}
#endif

#endif