#ifndef blog_newuser
#define blog_newuser


#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <lwan/lwan.h>
#include <lwan/lwan-mod-serve-files.h>
LWAN_HANDLER(newuser)
{
    response->mime_type = "text/html;charset=utf-8";
    
    FILE * fp = fopen("ALLOW_CREATE_USER","r");
    if(fp==NULL){
        static const char forbid[] = "强制新建用户功能已被禁用！";
        lwan_strbuf_set_static(response->buffer, forbid, sizeof(forbid) - 1);
        return HTTP_OK;
    }
    
    fclose(fp);
    
    const char * user       = lwan_request_get_post_param(request,"user");
    const char * pwd        = lwan_request_get_post_param(request,"pwd");
    
    if(user==NULL || pwd==NULL){
        static const char noarg[] = "noarg!";
        lwan_strbuf_set_static(response->buffer, noarg, sizeof(noarg) - 1);
        return HTTP_OK;
    }
    
    if(model_user_add(user,pwd)){
        static const char success[] = "success!";
        lwan_strbuf_set_static(response->buffer, success, sizeof(success) - 1);
    }else{
        static const char fail[] = "fail!";
        lwan_strbuf_set_static(response->buffer, fail, sizeof(fail) - 1);
    }
    return HTTP_OK;
}

#if defined(__cplusplus)
}
#endif


#endif