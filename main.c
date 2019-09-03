#include <lwan/lwan.h>
#include <lwan/lwan-mod-serve-files.h>
#include "controller/index.h"
#include "controller/passage.h"
#include "controller/sendpsg.h"
#include "controller/login.h"
#include "controller/newuser.h"
#include "controller/userstatus.h"

SEND_FILE_HANDLER_CREATE(favicon,"./view/favicon.ico","image/x-icon");
SEND_FILE_HANDLER_CREATE(robots_txt,"./view/robots.txt","text/txt");

int
main(void)
{
    
    const struct lwan_url_map default_map[] = {
        { .prefix = "/", .handler = LWAN_HANDLER_REF(index) },
        { .prefix = "/psg/", .handler = LWAN_HANDLER_REF(passage) },
        
        { .prefix = "/api/sendpsg", .handler = LWAN_HANDLER_REF(sendpsg) },
        { .prefix = "/api/getpsg", .handler = LWAN_HANDLER_REF(getpsg) },
        { .prefix = "/api/delpsg", .handler = LWAN_HANDLER_REF(delpsg) },
        { .prefix = "/api/editpsg", .handler = LWAN_HANDLER_REF(editpsg) },
        { .prefix = "/api/preview", .handler = LWAN_HANDLER_REF(preview) },
        
        { .prefix = "/api/getClass", .handler = LWAN_HANDLER_REF(getClass) },
        
        { .prefix = "/api/login", .handler = LWAN_HANDLER_REF(login) },
        { .prefix = "/api/newuser", .handler = LWAN_HANDLER_REF(newuser) },
        { .prefix = "/api/isloged", .handler = LWAN_HANDLER_REF(isloged) },
        { .prefix = "/api/changepwd", .handler = LWAN_HANDLER_REF(changepwd) },
        
        { .prefix = "/favicon.ico", .handler = SEND_FILE_HANDLER(favicon) },
        { .prefix = "/robots.txt", .handler = SEND_FILE_HANDLER(robots_txt) },
        
        { .prefix = "/static", SERVE_FILES("./static") },
        { .prefix = NULL }
    };
    struct lwan l;

    lwan_init(&l);
    model_init();
    
    SEND_FILE_HANDLER_INIT(favicon);
    SEND_FILE_HANDLER_INIT(robots_txt);
    
    controller_passage_init();
    controller_index_init();

    lwan_set_url_map(&l, default_map);
    lwan_main_loop(&l);

    lwan_shutdown(&l);
    
    controller_passage_destroy();
    controller_index_destroy();
    
    SEND_FILE_HANDLER_CLOSE(favicon);
    SEND_FILE_HANDLER_CLOSE(robots_txt);
    
    model_close();

    return 0;
}
