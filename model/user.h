#ifndef blog_model_user
#define blog_model_user
#include "passage.h"

class user:public passage{
    public:
        user(){
            leveldb::Options options;
            options.create_if_missing = true;
            
            assert(leveldb::DB::Open(options, "./data/users", &users).ok());
        }
        ~user(){
            delete users;
        }
        bool addUser(const std::string & name,const std::string & pwd){//添加用户
            std::string value;
            leveldb::Status s = users->Get(leveldb::ReadOptions(), std::string("pwd_")+name , &value);
            if(s.ok()){
                if(!value.empty())
                    return false;//用户已存在
            }
            setPwd(name,pwd);
            return true;
        }
        bool isLoged(const std::string & name,const std::string & token){//是否登录（检查token）
            std::string value;
            leveldb::Status s = users->Get(leveldb::ReadOptions(), std::string("token_")+name , &value);
            if(s.ok()){
                if(value==token)
                    return true;
            }
            return false;
        }
        bool checkPwd(const std::string & name,const std::string & pwd){//检查密码
            std::string value;
            leveldb::Status s = users->Get(leveldb::ReadOptions(), std::string("pwd_")+name , &value);
            if(s.ok()){
                if(value==sha256(pwd))
                    return true;
            }
            return false;
        }
        void setToken(const std::string & name,std::string & token){//设置token为随机数
            randString(32,token);
            users->Put(leveldb::WriteOptions(), std::string("token_")+name , token);
        }
        void setPwd(const std::string & name,const std::string & pwd){//设置密码
            users->Put(leveldb::WriteOptions(), std::string("pwd_")+name , sha256(pwd));
        }
        bool login(const std::string & name,const std::string & pwd,std::string & token){//登录
            if(checkPwd(name,pwd)){
                setToken(name,token);
                return true;
            }
            return false;
        }
    private:
        leveldb::DB * users;
};

#endif