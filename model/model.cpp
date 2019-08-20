#include "utils.h"
#include "user.h"
user * m;
extern "C" {
#include "model.h"
void model_init(){
    m=new user;
}
void model_close(){
    delete m;
}

int  model_user_add     (const char * name,const char * pwd){
    return m->addUser(name,pwd)?1:0;
}
int  model_user_login   (const char * name,const char * pwd,char * token,int len){
    std::string t;
    if(m->login(name,pwd,t)){
        snprintf(token , len , "%s" , t.c_str());
        return 1;
    }
    return 0;
}
int  model_user_isLoged (const char * name,const char * token){
    return m->isLoged(name,token)?1:0;
}
int  model_user_checkPwd(const char * name,const char * pwd){
    return m->checkPwd(name,pwd)?1:0;
}
void model_user_setPwd  (const char * name,const char * pwd){
    m->setPwd(name,pwd);
}
int  model_passage_send         (const char * id,struct lwan_response * response){
    std::string val;
    if(m->getContentRender(id,val)){
        int len=val.size();
        if(len>0)
            lwan_strbuf_append_str(response->buffer, val.c_str(), len);
        return 1;
    }
    return 0;
}
int  model_passage_display_send (const char * id,struct lwan_response * response){
    std::string val;
    if(m->getDisplayRender(id,val)){
        int len=val.size();
        if(len>0)
            lwan_strbuf_append_str(response->buffer, val.c_str(), len);
        return 1;
    }
    return 0;
}
int  model_passage_index_send   (const char * page,struct lwan_response * response){
    std::list<std::string> ids;
    bool haveNext;
    if(page){
        haveNext=m->getIndex(ids,20,page);
    }else{
        haveNext=m->getIndex(ids,20);
    }
    for(auto it:ids){
        model_passage_display_send(it.c_str(),response);
    }
    if(haveNext){
        auto rid=ids.rbegin();
        if(rid!=ids.rend()){
            std::string np=std::string("\n<br/><a class='next-page' href='/")+*rid+"'>下一页</a></br/>";
            const char * ptr = np.c_str();
            int len = np.size();
            lwan_strbuf_append_str(response->buffer, ptr, len);
        }
    }
    
}
int  model_passage_title_send   (const char * id,struct lwan_response * response){
    std::string val;
    if(m->getTitleRender(id,val)){
        int len=val.size();
        if(len>0)
            lwan_strbuf_append_str(response->buffer, val.c_str(), len);
        return 1;
    }
    return 0;
}
void model_passage_add          (char * outid,int len,const char * title,const char * cont,const char * user){
    std::string id;
    m->addPassage(id,title,cont,user);
    snprintf(outid,len,"%s",id.c_str());
}
int  model_passage_set          (const char * id,const char * title,const char * cont,const char * user){
    return m->setPassage(id,title,cont,user)?1:0;
}
int  model_passage_del          (const char * id){
    return m->delPassage(id)?1:0;
}
}//end extern