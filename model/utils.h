#ifndef blog_utils
#define blog_utils
extern "C" {
#include "markdown.h"
#include "html.h"
#include "buffer.h"
}
#include "cJSON.h"
#include <string>
#include <list>
#include <time.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

std::string sha256(const std::string & str);
std::string titlefilter(const std::string & str);
std::string utf8Cut(const std::string &src, int utf8Len);
void markdownRender(std::string & out,const std::string & data);
void displayRender(
    std::string & res,
    const std::string & id,
    const std::string & title,
    const std::string & content,
    const std::string & user,
    const std::string & time
);
void contentRender(
    std::string & res,
    const std::string & id,
    const std::string & title,
    const std::string & content,
    const std::string & user,
    const std::string & time
);
inline void randString(int len , std::string & res){
    res.clear();
    static const char mapping[] =
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int maxl=sizeof(mapping)-1;
    
    class random{
        public:
            inline unsigned int rand(){
                next = next * 1103515245 + 12345;
                return((unsigned)((next/65536)&0x7fffffff) % 32768);
            }
            inline void setSeed(unsigned long seed){
                next=seed;
            }
        private:
            unsigned long next=1;
    }rd;
    rd.setSeed(time(NULL));
    
    for(int i=0;i<len;i++){
        res+=mapping[(rd.rand()%maxl)];
    }
}
#endif