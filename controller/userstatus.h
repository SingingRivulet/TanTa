#ifndef blog_userstatus
#define blog_userstatus

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <lwan/lwan.h>
#include <lwan/lwan-mod-serve-files.h>

LWAN_HANDLER(isloged)
{
    response->mime_type = "text/html;charset=utf-8";
    
    const char * user       = lwan_request_get_post_param(request,"user");
    const char * token      = lwan_request_get_post_param(request,"token");
    
    if(user==NULL || token==NULL){
        static const char noarg[] = "noarg!";
        lwan_strbuf_set_static(response->buffer, noarg, sizeof(noarg) - 1);
        return HTTP_OK;
    }
    
    if(model_user_isLoged(user,token)==0){
        static const char nologin[] = "0";
        lwan_strbuf_set_static(response->buffer, nologin, sizeof(nologin) - 1);
    }else{
        static const char loged[] = "1";
        lwan_strbuf_set_static(response->buffer, loged, sizeof(loged) - 1);
    }
    
    return HTTP_OK;
}

#if defined(__cplusplus)
}
#endif


#endif