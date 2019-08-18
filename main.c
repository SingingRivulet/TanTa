#include <lwan/lwan.h>
#include <lwan/lwan-mod-serve-files.h>
#include "controller/index.h"
#include "controller/passage.h"
#include "controller/sendpsg.h"

int
main(void)
{
    const struct lwan_url_map default_map[] = {
        { .prefix = "/", .handler = LWAN_HANDLER_REF(index) },
        { .prefix = "/psg/", .handler = LWAN_HANDLER_REF(passage) },
        { .prefix = "/sendpsg", .handler = LWAN_HANDLER_REF(sendpsg) },
        { .prefix = "/static/", SERVE_FILES("./static") },
        { .prefix = NULL }
    };
    struct lwan l;

    lwan_init(&l);
    model_init();
    controller_passage_init();
    controller_index_init();

    lwan_set_url_map(&l, default_map);
    lwan_main_loop(&l);

    lwan_shutdown(&l);
    
    controller_passage_destroy();
    controller_index_destroy();
    model_close();

    return 0;
}
