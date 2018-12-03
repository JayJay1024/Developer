## 通过mongo_db_plugin存储账号余额到mongodb

我们这里以存储`eosio.token`的`EOS`余额为例，思路是在发生`transfer`的时候，我们捕获该`action`，然后更新双方的余额。

首先，在`class mongo_db_plugin_impl`中声明我们需要的变量和函数
```
    static const std::string accounts_col;
    static const std::string pub_keys_col;
    static const std::string account_controls_col;
+
+   // for get currency balance
+   mongocxx::collection _currency_balance;  // mongo集合的handle
+   static const std::string currency_balance_col;  // 将存放mongo集合的名称
+   chain_plugin* chain_plug_handle;  // chain_plugin 中 get_currency_balance 可以帮助我们获取余额信息
+   void update_currency_balance( const chain::action_trace& );  // 我们的操作放到这里进行
```

给集合取名为`currency_balance`
```
 const std::string mongo_db_plugin_impl::account_controls_col = "account_controls";
+const std::string mongo_db_plugin_impl::currency_balance_col = "currency_balance";  // mongo中集合将显示的是这个
```

在`mongo_db_plugin_impl::consume_blocks`中初始化`handle`
```
       _account_controls = mongo_conn[db_name][account_controls_col];
+      _currency_balance = mongo_conn[db_name][currency_balance_col];
```

在`mongo_db_plugin_impl::init()`中给集合定义一个索引
```
             account_controls.create_index( bsoncxx::from_json( R"xxx({ "controlling_account" : 1 })xxx" ));

+            // currency_balance index
+            auto currency_balance = mongo_conn[db_name][currency_balance_col];
+            currency_balance.create_index( bsoncxx::from_json( R"xxx({ "name" : 1 })xxx" ));  // name字段索引为升序
+            currency_balance.create_index( bsoncxx::from_json( R"xxx({ "balance" : -1 })xxx" ));  // balance字段索引为降序
+
          } catch (...) {
             handle_mongo_exception( "create indexes", __LINE__ );
```

在`mongo_db_plugin::plugin_initialize`中实例化`chain_plug_handle`
```
          auto& chain = chain_plug->chain();
          my->chain_id.emplace( chain.get_chain_id());

+         // 给 chain_plug_handle 实例化
+         my->chain_plug_handle = chain_plug;
+
          my->accepted_block_connection.emplace( chain.accepted_block.connect( [&]( const chain::block_state_ptr& bs ) {
             my->accepted_block( bs );
          } ));
```

定义我们的`update_currency_balance`函数
```
+void mongo_db_plugin_impl::update_currency_balance( const chain::action_trace& atrace ) {
+   using namespace bsoncxx::types;
+   using bsoncxx::builder::basic::kvp;
+   using bsoncxx::builder::basic::make_document;
+
+   auto receiver = atrace.receipt.receiver;
+   if ( atrace.act.account == name("eosio.token") && atrace.act.name == name("transfer") ) {
+      chain_apis::read_only::get_currency_balance_params params = chain_apis::read_only::get_currency_balance_params {
+         .code    = name("eosio.token"),
+         .account = receiver,
+         .symbol  = "EOS",
+      };
+
+      chain_apis::read_only ro_api = chain_plug_handle->get_read_only_api();
+      vector<asset> asserts = ro_api.get_currency_balance( params );
+      if ( !asserts.empty() ) {
+         asset balance = asserts.at(0);
+         auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
+               std::chrono::microseconds{fc::time_point::now().time_since_epoch().count()} );
+
+         // ilog( "${a}'s balance: ${b}", ("a", atrace.receipt.receiver.to_string())("b", balance.to_string()) );
+
+         mongocxx::options::update update_opts{};
+         update_opts.upsert( true );
+
+         const double balance_real = balance.to_real();
+         const string receiver_str = receiver.to_string();
+
+         auto update = make_document(
+            kvp( "$set", make_document( kvp( "name", receiver_str),
+                                        kvp( "balance", balance_real),
+                                        kvp( "createdAt", b_date{now} ))
+            )
+         );
+
+         try {
+            if( !_currency_balance.update_one( make_document( kvp( "name", receiver_str )), update.view(), update_opts )) {
+               EOS_ASSERT( false, chain::mongo_db_update_fail, "Failed to insert account ${n}", ("n", receiver));
+            }
+         } catch (...) {
+            handle_mongo_exception( "update_currency", __LINE__ );
+         }
+      }
+   }
+}
+
 mongo_db_plugin_impl::mongo_db_plugin_impl()
 {
 }
```

`mongodb-wipe`的时候，应该删除集合，在`mongo_db_plugin_impl::wipe_database`添加如下
```
    auto pub_keys = mongo_conn[db_name][pub_keys_col];
    auto account_controls = mongo_conn[db_name][account_controls_col];
+   auto currency_balance = mongo_conn[db_name][currency_balance_col];

    pub_keys.drop();
    account_controls.drop();
+   currency_balance.drop();
    ilog("done wipe_database");
```

准备好了之后，重新`./eosio_build.sh`，然后使用该`nodeos`重启节点(当然重启节点前需要配置好`mongo`插件，并且`sudo mongod`启动了`mongo`服务)。
通过`shell`查看`mongo`结果：
```
$ mongo

> show dbs
EOS     0.093GB
...

> use EOS
switched to db EOS

> show collections
account_controls
accounts
action_traces
block_states
blocks
currency_balance
pub_keys
transaction_traces
transactions
// 通过上面show collections可以看到我们自定义的currency_balance集合了

// 按照balance降序显示
> db.currency_balance.find().sort({balance:-1}).pretty()
{
	"_id" : ObjectId("5bf684f44549fee6d16a0888"),
	"name" : "eosio.stake",
	"balance" : 240020654.8,
	"createdAt" : ISODate("2018-11-22T10:52:39.140Z")
}
{
	"_id" : ObjectId("5bf684f64549fee6d16a2964"),
	"name" : "eosio.faucet",
	"balance" : 199999730,
	"createdAt" : ISODate("2018-11-22T10:51:18.982Z")
}
...
```