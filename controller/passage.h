#ifndef blog_passage
#define blog_passage

#if defined(__cplusplus)
extern "C" {
#endif

#include <lwan/lwan.h>
#include <lwan/lwan-mod-serve-files.h>
#include "utils.h"
#include "model/model.h"

char * psg_header=NULL;
int psg_header_len=0;

char * psg_footer=NULL;
int psg_footer_len=0;

char * psg_nofound=NULL;
int psg_nofound_len=0;

void controller_passage_init(){
    psg_header = textFileRead("./view/passage/header.tpl");
    if(psg_header)
        psg_header_len=strlen(psg_header);
    else
        psg_header_len=0;
        
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
    if(psg_header)free(psg_header);
    if(psg_footer)free(psg_footer);
    if(psg_nofound)free(psg_nofound);
}

LWAN_HANDLER(passage)
{
    
    response->mime_type = "text/html";
    
    if(psg_header_len>0)
        lwan_strbuf_append_str(response->buffer, psg_header, psg_header_len);
    
    // url:/psg/{id}
    // id偏移量：url+5
    if(request->url.len>7){
        const char * id=request->url.value+5;
        if(!model_passage_send(id,response)){
            if(psg_nofound_len>0)
                lwan_strbuf_append_str(response->buffer, psg_nofound, psg_nofound_len);
        }
    }
    
    if(psg_footer_len>0)
        lwan_strbuf_append_str(response->buffer, psg_footer, psg_footer_len);
    
    return HTTP_OK;
}

#if defined(__cplusplus)
}
#endif

#endif