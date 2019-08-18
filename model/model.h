#ifndef blog_model
#define blog_model
#include <lwan/lwan.h>
#include <lwan/lwan-mod-serve-files.h>
void model_init();
void model_close();

int  model_user_add     (const char * name,const char * pwd);
int  model_user_login   (const char * name,const char * pwd,char * token,int len);
int  model_user_isLoged (const char * name,const char * token);
int  model_user_checkPwd(const char * name,const char * pwd);
void model_user_setPwd  (const char * name,const char * pwd);

void model_passage_add          (char * id,int len,const char * title,const char * cont,const char * user);
int  model_passage_set          (const char * id,const char * title,const char * cont,const char * user);
int  model_passage_del          (const char * id);

int  model_passage_send         (const char * id,struct lwan_response * response);
int  model_passage_display_send (const char * id,struct lwan_response * response);

int  model_passage_index_send   (const char * page,struct lwan_response * response);
#endif
