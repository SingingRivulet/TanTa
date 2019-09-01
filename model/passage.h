#ifndef blog_model_passage
#define blog_model_passage
#include <leveldb/db.h>
#include <atomic>
#include "utils.h"
class passage{
    public:
        passage(){
            leveldb::Options options;
            options.create_if_missing = true;
            
            assert(leveldb::DB::Open(options, "./data/passages", &passages).ok());
            assert(leveldb::DB::Open(options, "./data/psindexer", &indexer).ok());
            assert(leveldb::DB::Open(options, "./data/pscache", &cache).ok());
        }
        
        ~passage(){
            delete passages;
            delete indexer;
            delete cache;
        }
        
        //获取索引
        bool getIndex(std::list<std::string> & res,int num){//返回：是否有下一页
            int i=0;
            leveldb::Iterator * it = indexer->NewIterator(leveldb::ReadOptions());
            
            for(it->SeekToFirst(); it->Valid(); it->Next()) {
                
                res.push_back(it->value().ToString());
                
                if(i>=num){//有下一页
                    delete it;
                    return true;
                }else
                    ++i;
                
            }
            delete it;
            return false;
        }
        //同上，带分页
        bool getIndex(std::list<std::string> & res,int num,const char * startAt){
            int i=0;
            leveldb::Iterator * it = indexer->NewIterator(leveldb::ReadOptions());
            
            for(it->Seek(startAt); it->Valid(); it->Next()) {
                
                res.push_back(it->value().ToString());
                
                if(i>=num){//没有下一页
                    delete it;
                    return true;
                }else
                    ++i;
                
            }
            delete it;
            return false;
        }
        
        bool getPassage(//获取
            const std::string & id , 
            std::string & title , 
            std::string & content , 
            std::string & user , 
            std::string & tm
        ){
            std::string value;
            leveldb::Status s = passages->Get(leveldb::ReadOptions(), id, &value);
            if (!s.ok() || value.empty())
                return false;
            
            cJSON * json=cJSON_Parse(value.c_str());
            if(json->type!=cJSON_Object){
                cJSON_Delete(json);
                return false;
            }
            
            //cjson的查找是遍历，故不应使用cJSON_GetObjectItem
            cJSON *c=json->child;
            while (c){
                if(strcmp(c->string,"title")==0     && c->type==cJSON_String){
                    title   = c->valuestring;
                    //printf("title:%s\n",c->valuestring);
                }else
                if(strcmp(c->string,"content")==0   && c->type==cJSON_String){
                    content = c->valuestring;
                    //printf("content:%s\n",c->valuestring);
                }else
                if(strcmp(c->string,"user")==0      && c->type==cJSON_String){
                    user    = c->valuestring;
                    //printf("user:%s\n",c->valuestring);
                }else
                if(strcmp(c->string,"time")==0      && c->type==cJSON_String){
                    tm      = c->valuestring;
                    //printf("time:%s\n",c->valuestring);
                }
                c=c->next;
            }
        
            cJSON_Delete(json);
            return true;
        }
        void getPassage(const std::string & id , std::string & res){
            std::string title;
            std::string content;
            std::string user;
            std::string tm;
            //重组数据，保证安全
            if(!getPassage(id , title , content , user , tm)){
                res="notexist!";
                return;
            }
            
            cJSON *json=cJSON_CreateObject();
            cJSON_AddStringToObject(json,"title"    ,title.c_str());
            cJSON_AddStringToObject(json,"content"  ,content.c_str());
            cJSON_AddStringToObject(json,"user"     ,user.c_str());
            cJSON_AddStringToObject(json,"time"     ,tm.c_str());
            char * out=cJSON_PrintUnformatted(json);
            res = out;
            free(out);
            cJSON_Delete(json);
        }
        void addPassage(//添加文章
            std::string & id , 
            const std::string & title , 
            const std::string & content , 
            const std::string & user
        ){
            std::string sindex;
            getTimeIndexer(sindex);//索引
            std::string tm;
            getTime(tm);
            
            {
                long itm = time(NULL);
                int sunq = sendTimes++;
                char idstr[64];
                snprintf(idstr,sizeof(idstr),"%lx-%x",itm,sunq);
                id=idstr;//id为时间（暂定）
            }
            
            indexer->Put(leveldb::WriteOptions(), sindex , id);
            
            cJSON *json=cJSON_CreateObject();
            
            cJSON_AddStringToObject(json,"title"    ,title.c_str());
            cJSON_AddStringToObject(json,"content"  ,content.c_str());
            cJSON_AddStringToObject(json,"user"     ,user.c_str());
            cJSON_AddStringToObject(json,"time"     ,tm.c_str());
            cJSON_AddStringToObject(json,"indexer"  ,sindex.c_str());
            
            char * out=cJSON_PrintUnformatted(json);
            passages->Put(leveldb::WriteOptions(), id, out);
            free(out);
            
            cJSON_Delete(json);
            render(id,title,content,user,tm);
        }
        bool setPassage(//设置文章
            const std::string & id , 
            const std::string & title , 
            const std::string & content , 
            const std::string & user
        ){
            std::string value;
            leveldb::Status s = passages->Get(leveldb::ReadOptions(), id, &value);
            if (!s.ok())
                return false;
            
            cJSON * json=cJSON_Parse(value.c_str());
            if(json->type!=cJSON_Object){
                cJSON_Delete(json);
                return false;
            }
            
            std::string tm;
            getTime(tm);
            
            cJSON_ReplaceItemInObject(json , "title"   , cJSON_CreateString(title.c_str()));
            cJSON_ReplaceItemInObject(json , "content" , cJSON_CreateString(content.c_str()));
            cJSON_ReplaceItemInObject(json , "user"    , cJSON_CreateString(user.c_str()));
            cJSON_ReplaceItemInObject(json , "time"    , cJSON_CreateString(tm.c_str()));
            
            char * out=cJSON_PrintUnformatted(json);
            passages->Put(leveldb::WriteOptions(), id, out);
            free(out);
            
            cJSON_Delete(json);
            
            render(id,title,content,user,tm);
            
            return true;
        }
        bool delPassage(const std::string & id){//删除
            std::string value;
            leveldb::Status s = passages->Get(leveldb::ReadOptions(), id, &value);
            if (!s.ok())
                return false;
            
            bool succ = false;
            
            cJSON * json=cJSON_Parse(value.c_str());
            if(json->type!=cJSON_Object){
                cJSON_Delete(json);
                return false;
            }
            
            cJSON * item;
            //cjson的查找是遍历，故不应使用cJSON_GetObjectItem
            cJSON *c=json->child;
            while (c){
                if(strcmp(c->string,"indexer")==0 && c->type==cJSON_String){
                    indexer->Delete(leveldb::WriteOptions(), c->valuestring);
                }
                c=c->next;
            }
        
            cJSON_Delete(json);
            
            passages->Delete(leveldb::WriteOptions(), id);
            delRender(id);
            
            return succ;
        }
        
        void getTime(std::string & tm){
            time_t timep;
            struct tm *p;
            time (&timep);
            p=gmtime(&timep);
            char buf[64];
            snprintf(buf,64,
                "%d-%d-%d:%d:%d:%d",
                1900+p->tm_year,
                1+p->tm_mon,
                p->tm_mday,
                8+p->tm_hour,
                p->tm_min,
                p->tm_sec
            );
            tm = buf;
        }
        
        void getTimeIndexer(std::string & idx){
            time_t myt=time(NULL);
            long long idxn = 9223372036854775807;
            idxn -= myt;//目的是让它倒序排列
            char buf[64];
            snprintf(buf,64,"%lld",idxn);
            idx = buf;
        }
        
        bool getDisplayRender(const std::string & id,std::string & out){//主页展示渲染
            std::string value;
            leveldb::Status s = cache->Get(leveldb::ReadOptions(), std::string("display_")+id, &value);
            if (!s.ok())
                return false;
            out = value;
            return true;
        }
        bool getContentRender(const std::string & id,std::string & out){//文章渲染
            std::string value;
            leveldb::Status s = cache->Get(leveldb::ReadOptions(), std::string("content_")+id, &value);
            if (!s.ok())
                return false;
            out = value;
            return true;
        }
        bool getTitleRender(const std::string & id,std::string & out){//文章渲染
            std::string value;
            leveldb::Status s = cache->Get(leveldb::ReadOptions(), std::string("title_")+id, &value);
            if (!s.ok())
                return false;
            out = value;
            return true;
        }
        void delRender(const std::string & id){
            cache->Delete(leveldb::WriteOptions(), std::string("display_")+id);
            cache->Delete(leveldb::WriteOptions(), std::string("content_")+id);
            cache->Delete(leveldb::WriteOptions(), std::string("title_")+id);
        }
        void render(
            const std::string & id , 
            const std::string & title , 
            const std::string & content , 
            const std::string & user , 
            const std::string & tm
        ){
            std::string out;
            displayRender(out,id,title,content,user,tm);
            cache->Put(leveldb::WriteOptions(), std::string("display_")+id, out);
            
            contentRender(out,id,title,content,user,tm);
            cache->Put(leveldb::WriteOptions(), std::string("content_")+id, out);
            
            out=titlefilter(title);
            cache->Put(leveldb::WriteOptions(), std::string("title_")+id, out);
        }
        
    private:
        
        leveldb::DB * passages;
        leveldb::DB * indexer;
        leveldb::DB * cache;
        
        std::atomic<int> sendTimes;
};
#endif