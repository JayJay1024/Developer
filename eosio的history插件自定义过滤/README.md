## history插件自定义filter

#### 以只过滤`eosio.token`的`transfer`为例

在history_plugin.cpp文件中

```
void on_action_trace( const action_trace& at ) {
    // if( filter( at ) ) {  // 先注释掉原来的
    if( 1 ) {  // if 1 测试，使每次都进入这里
        //idump((fc::json::to_pretty_string(at)));
        auto& chain = chain_plug->chain();
        chainbase::database& db = const_cast<chainbase::database&>( chain.db() ); // Override read-only access to state DB (highly unrecommended practice!)

        db.create<action_history_object>( [&]( auto& aho ) {
            auto ps = fc::raw::pack_size( at );
            aho.packed_action_trace.resize(ps);
            datastream<char*> ds( aho.packed_action_trace.data(), ps );
            fc::raw::pack( ds, at );
            aho.action_sequence_num = at.receipt.global_sequence;
            aho.block_num = chain.pending_block_state()->block_num;
            aho.block_time = chain.pending_block_time();
            aho.trx_id     = at.trx_id;
        });

        // 先注释掉原来的
        // auto aset = account_set( at );
        // for( auto a : aset ) {
        //    record_account_action( a, at );
        // }

        // 添加自定义的过滤
        auto rev = at.receipt.receiver;
        auto acc = at.act.account;
        auto nam = at.act.name;
        if ( acc == N(eosio.token) && nam == N(transfer) ) {
            record_account_action( rev, at );
        }
    }
    if( at.receipt.receiver == chain::config::system_account_name )
        on_system_action( at );
    for( const auto& iline : at.inline_traces ) {
        on_action_trace( iline );
    }
}
```

修改完后回到`eosio`源码根目录，执行`./eosio_build.sh`。使用该`build`出来的`nodeos`启动，启动后通过`cleos get actions eosio.token`查看。
