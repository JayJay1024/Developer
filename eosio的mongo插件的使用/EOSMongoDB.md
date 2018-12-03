## eosio提供了mongo插件，实现把数据导到mongo中，从而可以更加灵活的操作数据。
## 配置（config.ini）
```
plugin = eosio::mongo_db_plugin
mongodb-uri = mongodb://localhost:27017
```
## 本地启动mongo
```
$ sudo mongod
```
## 重启nodeos
```
$ nodeos --replay-blockchain --hard-replay-blockchain --mongodb-wipe
```
## mongo中查询
```
$ mongo
> show dbs
EOS     0.372GB
...

> use EOS
switched to db EOS

> show tables
account_controls
accounts
action_traces
block_states
blocks
pub_keys
transaction_traces
transactions

> db.accounts.find().pretty()
{
	"_id" : ObjectId("5bf3cfbf4f6371c75cdc8890"),
	"name" : "superoneiobp",
	"createdAt" : ISODate("2018-11-20T09:11:27.309Z")
}
...
```