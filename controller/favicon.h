#ifndef blog_favicon
#define blog_favicon

#if defined(__cplusplus)
extern "C" {
#endif

void controller_favicon_init(){
    lwan_determine_mime_type_for_file_name()
}

LWAN_HANDLER(favicon)
{
    
}

#if defined(__cplusplus)
}
#endif

#endif
