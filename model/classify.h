#ifndef blog_model_classify
#define blog_model_classify
#include "utils.h"
#include <mutex>
#include <unordered_set>
#include <leveldb/write_batch.h>
class classify{
    public:
        classify(){
            leveldb::Options options;
            options.create_if_missing = true;
            
            assert(leveldb::DB::Open(options, "./data/classifies", &classifies).ok());
        }
        ~classify(){
            delete classifies;
        }
        void add(const std::string & cla , const std::string & name){
            leveldb::WriteBatch batch;
            std::string prefix = std::string("class\n")+cla+"\n";
            
            batch.Put(std::string("classes\n") , "@begin");
            batch.Put(std::string("classes\n")+cla , cla);
            batch.Put(prefix , "@begin");
            batch.Put(prefix+"\n"+name , name);
            
            classifies->Write(leveldb::WriteOptions(), &batch);
        }
        bool isPrefix(const std::string & pfx , const std::string & str){
            auto p=pfx.c_str();
            auto s=str.c_str();
            int len = pfx.size();
            for(int i=0;i<len;i++){
                if(p[i]!=s[i])
                    return false;
            }
            return true;
        }
        
        bool searchPrefix(
            const std::string & prefix , 
            const std::string & startAt , 
            std::list<std::string> & res , 
            int num=10
        ){
            int i=0;
            leveldb::Iterator * it = classifies->NewIterator(leveldb::ReadOptions());
            
            for(it->Seek(startAt); it->Valid(); it->Next()) {
                
                auto k = it->key().ToString();
                auto v = it->value().ToString();
                
                if(!isPrefix(prefix,k))
                    break;//前缀不匹配，说明搜索完了
                
                if(v!="@begin"){//不是起始键
                    res.push_back(v);
                }
                
                if(i>=num){//没有下一页
                    delete it;
                    return true;
                }else
                    ++i;
                
            }
            delete it;
            return false;
        }
        
        bool get(const std::string & cla , std::list<std::string> & res , int num=10){
            std::string prefix = std::string("class\n")+cla+"\n";
            
            return searchPrefix(prefix , prefix , res , num);
        }
        
        bool get(
            const std::string & cla , 
            const std::string & startAt ,
            std::list<std::string> & res , 
            int num=10
        ){
            std::string prefix = std::string("class\n")+cla+"\n\n";
            
            return searchPrefix(prefix , prefix+startAt , res , num);
        }
        
        void del(const std::string & cla , const std::string & name){
            std::string prefix = std::string("class\n")+cla+"\n\n";
            
            classifies->Delete(leveldb::WriteOptions(), prefix+name);
        }
        
        bool getClasses(const std::string & cla , std::list<std::string> & res , int num=10){
            std::string prefix = std::string("classes\n");
            return searchPrefix(prefix , prefix , res , num);
        }
        
        bool getClasses(
            const std::string & cla , 
            const std::string & startAt ,
            std::list<std::string> & res , 
            int num=10
        ){
            std::string prefix = std::string("classes\n");
            return searchPrefix(prefix , prefix+startAt , res , num);
        }
        
    private:
        leveldb::DB * classifies;
};
#endif