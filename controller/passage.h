#ifndef blog_passage
#define blog_passage

#if defined(__cplusplus)
extern "C" {
#endif

#include <lwan/lwan.h>
#include <lwan/lwan-mod-serve-files.h>
#include "utils.h"
#include "model/model.h"

char * psg_header_btitle=NULL;
int psg_header_btitle_len=0;

char * psg_header_atitle=NULL;
int psg_header_atitle_len=0;

char * psg_footer=NULL;
int psg_footer_len=0;

char * psg_nofound=NULL;
int psg_nofound_len=0;

void controller_passage_init(){
    psg_header_btitle = textFileRead("./view/passage/header_btitle.tpl");
    if(psg_header_btitle)
        psg_header_btitle_len=strlen(psg_header_btitle);
    else
        psg_header_btitle_len=0;
        
    psg_header_atitle = textFileRead("./view/passage/header_atitle.tpl");
    if(psg_header_atitle)
        psg_header_atitle_len=strlen(psg_header_atitle);
    else
        psg_header_atitle_len=0;
        
    psg_footer = textFileRead("./view/passage/footer.tpl");
    if(psg_footer)
        psg_footer_len=strlen(psg_footer);
    else
        psg_footer_len=0;
        
    psg_nofound = textFileRead("./view/passage/nofound.tpl");
    if(psg_nofound)
        psg_nofound_len=strlen(psg_nofound);
    else
        psg_nofound_len=0;
}

void controller_passage_destroy(){
    if(psg_header_btitle)free(psg_header_btitle);
    if(psg_header_atitle)free(psg_header_atitle);
    if(psg_footer)free(psg_footer);
    if(psg_nofound)free(psg_nofound);
}

LWAN_HANDLER(passage)
{
    
    response->mime_type = "text/html;charset=utf-8";
    
    int havep=0;
    const char * id=NULL;
    
    // url:/psg/{id}
    // id偏移量：url+5
    if(request->url.len>7){
        id=request->url.value+5;
    }
    
    if(psg_header_btitle>0)
        lwan_strbuf_append_str(response->buffer, psg_header_btitle, psg_header_btitle_len);
    
    static const char nofound[] = "Page No Found!";
    if(id){
        if(!model_passage_title_send(id,response)){
            lwan_strbuf_append_str(response->buffer, nofound, sizeof(nofound) - 1);
        }
    }else{
        lwan_strbuf_append_str(response->buffer, nofound, sizeof(nofound) - 1);
    }
    
    if(psg_header_atitle>0)
        lwan_strbuf_append_str(response->buffer, psg_header_atitle, psg_header_atitle_len);
    
    if(id){
        if(!model_passage_send(id,response)){
            if(psg_nofound_len>0)
                lwan_strbuf_append_str(response->buffer, psg_nofound, psg_nofound_len);
        }
    }else{
        if(psg_nofound_len>0)
            lwan_strbuf_append_str(response->buffer, psg_nofound, psg_nofound_len);
    }
    
    if(psg_footer_len>0)
        lwan_strbuf_append_str(response->buffer, psg_footer, psg_footer_len);
    
    return HTTP_OK;
}

#if defined(__cplusplus)
}
#endif

#endif