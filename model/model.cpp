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
int  model_passage_index_send   (const char * page,const char * classify,struct lwan_response * response){
    std::list<std::string> ids;
    bool haveNext;
    if(classify){
        if(page){
            haveNext=m->classes.get(classify,page,ids,20);
        }else{
            haveNext=m->classes.get(classify,ids,20);
        }
    }else{
        if(page){
            haveNext=m->getIndex(ids,20,page);
        }else{
            haveNext=m->getIndex(ids,20);
        }
    }
    if(ids.empty()){
        if(!page)
            return 1;
        else{
            static const char nofound[] = "<h1>Page No Found!</h1>";
            lwan_strbuf_append_str(response->buffer, nofound, sizeof(nofound) - 1);
            return 0;
        }
    }
    for(auto it:ids){
        model_passage_display_send(it.c_str(),response);
    }
    if(haveNext){
        auto rid=ids.rbegin();
        if(rid!=ids.rend()){
            std::string np=std::string("\n<br/><a class='next-page' href='/");
            
            if(classify){
                std::string fcla;
                const char * p = classify;
                while(1){
                    unsigned char c = *p;
                    if((c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9')){
                        fcla+=c;
                    }else
                    if(c!='\0'){
                        
                        const static char mapping[] = "0123456789ABCDEF";
                        
                        char buf[4];
                        
                        unsigned char low = c & 0x0f;//低4位
                        unsigned char hei = (c >> 4) & 0x0f;//高4位
                        
                        buf[0]='%';
                        buf[1]=mapping[hei];
                        buf[2]=mapping[low];
                        buf[3]='\0';
                        
                        fcla+=buf;
                        
                    }
                    
                    if(c=='\0')
                        break;
                    else
                        ++p;
                }
                np+=*rid+"?classify="+fcla;
            }else{
                std::string indexer;
                m->getPassageIndexer(*rid , indexer);
                np+=indexer;
            }
            
            np+="'>下一页</a></br/>";
            const char * ptr = np.c_str();
            int len = np.size();
            lwan_strbuf_append_str(response->buffer, ptr, len);
        }
    }
    return 1;
}
int  model_passage_title_send   (const char * id,struct lwan_response * response){
    std::string val;
    if(m->getTitleRender(id,val)){
        int len=val.size();
        if(len>0){
            lwan_strbuf_append_str(response->buffer, val.c_str(), len);
            return 1;
        }
    }
    return 0;
}
int  model_passage_send_json    (const char * id,struct lwan_response * response){
    std::string val;
    m->getPassage(id,val);
    if(val.empty()){
        static const char notexist[] = "notexist!";
        lwan_strbuf_set_static(response->buffer, notexist, sizeof(notexist) - 1);
    }else{
        lwan_strbuf_set(response->buffer,val.c_str(),val.size());
    }
}
void model_passage_add          (char * outid,int len,const char * title,const char * cont,const char * classify,const char * user){
    std::string id;
    m->addPassage(id,title,cont,(classify?classify:std::string()),user);
    snprintf(outid,len,"%s",id.c_str());
}
void model_passage_preview      (const char * data,struct lwan_response * response){
    if(data==NULL)
        return;
    int inlen = strlen(data);
    if(inlen<=0)
        return;
    
    int l=inlen*5;
    auto ob = bufnew(l>65535?65535:l);
    
    struct sd_callbacks callbacks;
    struct html_renderopt options;
    struct sd_markdown *markdown;
    
    sdhtml_renderer(&callbacks, &options, 0);
    markdown = sd_markdown_new(0, 16, &callbacks, &options);
    
    sd_markdown_render(ob, (const unsigned char*)data, inlen, markdown);
    
    if(ob->size > 0){
        if(ob->data[0]!='\0'){
            ob->data[ob->size-1]='\0';//封住底部
            auto outd = (const char *)ob->data;
            lwan_strbuf_set(response->buffer,outd,strlen(outd));
        }
    }
    
    sd_markdown_free(markdown);
    
    bufrelease(ob);
}
void model_passage_getClasses   (const char * startAt,struct lwan_response * response){
    std::string res;
    m->getClasses((startAt?startAt:std::string()),res);
    lwan_strbuf_set(response->buffer,res.c_str(),res.size());
}
void model_passage_getClassify  (const char * id,const char * startAt,struct lwan_response * response){
    std::string res;
    m->getPassageInClassify(id,(startAt?startAt:std::string()),res);
    lwan_strbuf_set(response->buffer,res.c_str(),res.size());
}
int  model_passage_set          (const char * id,const char * title,const char * cont,const char * classify,const char * user){
    return m->setPassage(id,title,cont,(classify?classify:std::string()),user)?1:0;
}
int  model_passage_del          (const char * id){
    return m->delPassage(id)?1:0;
}
}//end extern