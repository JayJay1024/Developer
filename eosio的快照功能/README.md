EOS Snapshot可以帮助Nodeos快速的恢复数据
## 一、配置(在config.ini中)
```
// 配置保存snapshot的路径
snapshots-dir = "snapshots"

// 插件
plugin = eosio::chain_api_plugin
plugin = eosio::chain_plugin
plugin = eosio::net_plugin
plugin = eosio::producer_api_plugin
```
## 二、重启nodeos
## 三、创建snapshot
```
// 下面将会在snapshots-dir下生成一个head_block_id的bin文件
$ curl http://127.0.0.1:8888/v1/producer/create_snapshot
```
## 从快照恢复数据
```
// 删除data下除blocks/blocks.log的其他文件
// 启动nodeos的时候添加--snapshot参数，指定snapshot文件路径
$ nodeos --snapshot + snapshot path
```
