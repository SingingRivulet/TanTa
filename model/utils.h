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
std::string titlefilter(const std::string & str){
    std::string ret;
    for (size_t i = 0; i < str.size(); i++){
        char c = str[i];
        if(c=='\r' || c=='\n'){
            continue;
        }else
        if(c=='\0'){
            break;
        }else
        if(c=='>'){
            ret+="&gt;";
        }else
        if(c=='<'){
            ret+="&lt;";
        }else{
            ret+=c;
        }
    }
    return ret;
}
std::string utf8Cut(const std::string &src, int utf8Len) {
    std::string ret;
    int utf8LenCnt = 0;
    int srcIdx = 0;
    int srcLen = src.length();
    int cutLen = 0;
    unsigned char tmp;
    while (utf8LenCnt < utf8Len &&  srcIdx < srcLen) {
        tmp = (unsigned char)src[srcIdx];
        if (tmp >= 252)
            cutLen = 6;
        else if (tmp >= 248)
            cutLen = 5;
        else if (tmp >= 240)
            cutLen = 4;
        else if (tmp >= 224)
            cutLen = 3;
        else if (tmp >= 192)
            cutLen = 2;
        else if (tmp >= 65 && tmp <=90)
            cutLen = 1;
        else
            cutLen = 1;
        ret += src.substr(srcIdx, cutLen);
        srcIdx += cutLen;
        ++utf8LenCnt;
    }
    return ret;
}
void markdownRender(std::string & out,const std::string & data){
    out.clear();
    if(data.empty()){
        return;
    }
    
    int l=data.size()*5;
    auto ob = bufnew(l>65535?65535:l);
    
    struct sd_callbacks callbacks;
    struct html_renderopt options;
    struct sd_markdown *markdown;
    
    sdhtml_renderer(&callbacks, &options, 0);
    markdown = sd_markdown_new(0, 16, &callbacks, &options);
    
    sd_markdown_render(ob, (const unsigned char*)data.c_str(), data.size(), markdown);
    sd_markdown_free(markdown);
    
    if(ob->size > 0){
        if(ob->data[0]!='\0'){
            ob->data[ob->size-1]='\0';//封住底部
            out=(char*)ob->data;
        }
    }
    
    bufrelease(ob);
}
void displayRender(
    std::string & res,
    const std::string & id,
    const std::string & title,
    const std::string & content,
    const std::string & user,
    const std::string & time
){
    std::string cont = utf8Cut(content,64) + "  \n" + user + "\t" + time + "\n";
    std::string contHTML;
    
    markdownRender(contHTML , cont);
    
    res = std::string("<section><div class = 'psg-title'><a href='/psg/") + id + "'>" +
        titlefilter(title) +
        "</a></div>" + 
        cont + 
        "</section>\n";
}
void contentRender(
    std::string & res,
    const std::string & id,
    const std::string & title,
    const std::string & content,
    const std::string & user,
    const std::string & time
){
    std::string cont = content + "  \n" + user + "\t" + time + "\n";
    std::string contHTML;
    
    markdownRender(contHTML , cont);
    
    res = std::string("<div class = 'psg-title'><a href='/psg/") + id + "'>" +
        titlefilter(title) +
        "</a></div>" + 
        cont;
}
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