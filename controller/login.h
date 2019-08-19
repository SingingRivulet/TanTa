#ifndef blog_login
#define blog_login

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdlib.h>
#include <lwan/lwan.h>
#include <lwan/lwan-mod-serve-files.h>
LWAN_HANDLER(login)
{
    response->mime_type = "text/html";
    
    const char * user       = lwan_request_get_post_param(request,"user");
    const char * pwd        = lwan_request_get_post_param(request,"pwd");
    if(user==NULL || pwd==NULL){
        static const char noarg[] = "noarg!";
        lwan_strbuf_set_static(response->buffer, noarg, sizeof(noarg) - 1);
        return HTTP_OK;
    }
    
    char tokenbuf[128];
    if(model_user_login(user,pwd,tokenbuf,128)){
        lwan_strbuf_set(response->buffer,tokenbuf,strlen(tokenbuf));
    }else{
        static const char loginFail[] = "fail!";
        lwan_strbuf_set_static(response->buffer, loginFail, sizeof(loginFail) - 1);
    }

    return HTTP_OK;
}

#if defined(__cplusplus)
}
#endif

#endif