## eosio.cdt test
### 官方仓库: [https://github.com/EOSIO/eosio.cdt](https://github.com/EOSIO/eosio.cdt)
- 在`Mac OS X`用`Brew`安装完之后，可以`brew link eosio.cdt`一下，这样就会在`/usr/local/bin/`下创建符号链接
- 如果之前有`make install eosio`，`brew link`可能会提示需要删除一些`/usr/local/bin/`下已存在的文件，比如`/usr/local/bin/eosio-abigen`，按照提示使用`rm`删除即可
### 编译和部署
```
$ cd contracttest/build
$ cmake ..
$ make
$ ./deploy.sh
```