#ifndef blog_newuser
#define blog_newuser


#if defined(__cplusplus)
extern "C" {
#endif

#include <stdlib.h>
#include <lwan/lwan.h>
#include <lwan/lwan-mod-serve-files.h>
LWAN_HANDLER(newuser)
{//此函数请在conf里面配置，且使用完马上删除！
    response->mime_type = "text/html";
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