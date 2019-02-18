## 概述：
你通过`config.ini`配置了`http-server-address = 0.0.0.0:8888`，并且配置了插件`plugin = eosio::http_plugin`，假设你的 server 的公开 ip 为`12.34.56.78`，氮素`cleos -u http://12.34.56.78:8888`仍然无法访问你的节点 rpc。

## 一些问题：
- 看看你的 server 安全组有没有允许`8888`端口的访问
- 在`config.ini`中配置`http-alias = 12.34.56.78:8888`试试看
- 然后重启 nodeos
