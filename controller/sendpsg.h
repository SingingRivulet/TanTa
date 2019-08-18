#ifndef blog_admin
#define blog_admin

#if defined(__cplusplus)
extern "C" {
#endif

#include <lwan/lwan.h>
#include <lwan/lwan-mod-serve-files.h>
LWAN_HANDLER(sendpsg)
{
    static const char message[] = "<br>";

    response->mime_type = "text/html";
    //lwan_strbuf_set_static(response->buffer, message, sizeof(message) - 1);
    lwan_strbuf_append_str(response->buffer, message, sizeof(message) - 1);
    if(request->url.len!=0)
        lwan_strbuf_append_str(response->buffer, request->url.value, request->url.len - 1);
    lwan_strbuf_append_str(response->buffer, message, sizeof(message) - 1);
    lwan_strbuf_append_str(response->buffer, request->original_url.value, request->original_url.len - 1);

    return HTTP_OK;
}

#if defined(__cplusplus)
}
#endif

#endif