#include "utils.h"
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
    const std::string & classify,
    const std::string & user,
    const std::string & time
){
    std::string cont = utf8Cut(content,64);
    std::string contHTML;
    
    markdownRender(contHTML , cont);
    
    res = std::string("<section class='psg-display'><div class = 'psg-title'><a href='/psg/") + id + "'>" +
        titlefilter(title) +
        "</a></div><div class='psg-display'>" + 
        contHTML +
        "</div>";
    
    if(!classify.empty())
        res+="<div name='classify' class='classify'>"+titlefilter(classify)+"</div>";
    
    res+="<div class = 'psg-user'>" + user + "</div><time class = 'psg-time'>" + time + "</time>"
        "</section>\n";
}
void contentRender(
    std::string & res,
    const std::string & id,
    const std::string & title,
    const std::string & content,
    const std::string & classify,
    const std::string & user,
    const std::string & time
){
    std::string cont = content + "  \n";
    std::string contHTML;
    
    markdownRender(contHTML , cont);
    
    res = std::string("<div class = 'psg-title'><a href='/psg/") + id + "'>" +
        titlefilter(title) +
        "</a></div>" + "<div class = 'psg-content'>" +
        contHTML + "</div><div class = 'psg-user'>" + user + "</div><time class = 'psg-time'>" + time + "</time>";
    
    if(!classify.empty())
        res+="<div name='classify' class='classify'>"+titlefilter(classify)+"</div>";
}