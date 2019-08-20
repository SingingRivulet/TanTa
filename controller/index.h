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
    response->mime_type = "text/html;charset=utf-8";
    
    const char * url = request->url.value;
    
    if(index_header_len>0)
        lwan_strbuf_append_str(response->buffer, index_header, index_header_len);
    
    if(request->url.len>0)
        model_passage_index_send(request->url.value , response);
    else
        model_passage_index_send(NULL,response);
    
    if(index_footer_len!=0)
        lwan_strbuf_append_str(response->buffer, index_footer, index_footer_len);
    
    return HTTP_OK;
}

#if defined(__cplusplus)
}
#endif

#endif