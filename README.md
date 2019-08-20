# TanTa  
基于Lwan + leveldb的超高性能轻量级博客，为压榨主机性能而生。  
本项目专治共享服务器，超售服务器，轻量级应用服务器，阿里云t5，土豆服务器等等。  
目前还是个半成品
## 使用方法：  
### 1.安装Lwan  
```
sudo apt-get update && apt-get install git cmake zlib1g-dev pkg-config
git clone git://github.com/lpereira/lwan
cd lwan
mkdir build
cd build
cmake ../
make
sudo make install
```
  
### 2.安装leveldb  
```
sudo apt-get install libleveldb-dev
```
  
### 3.下载编译  
```
git clone https://github.com/cgoxopx/TanTa
cd TanTa
make
```

### 4.配置  
```
make createUserOn
```
然后用浏览器配置  
配置完一定记得执行 ` make createUserOff ` 关闭强制创建功能！！！  
