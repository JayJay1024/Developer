## 背景
`eosio`的多索引表，有`code`和`scope`两个角色。一张表只能有一个`code`，但是可以有多个`scope`，当然`code`和`scope`可以是同一个账号。根据`code`和`scope`可以唯一的确定一张数据表。
如果`code`和`scope`同一个，我们很容易获得表的数据，但是如果不同一个呢？现在`eosjs`这个库还不支持获取`scope`，但是`eosio`软件已经实现了这个接口了，所以思路是先获取所有的`scope`，然后结合`code`确定表的数据。

## 方法一
使用`js`封装
```
// 1. 获取scope
fetchScopes = (upper = '') => {
    const data = {
        code: 'eosio.token',
        table: 'accounts',
        lower_bound: upper,  // 从 upper 开始
        // upper_bound: '',
        limit: 6,  // 表示每次获取6条记录
    };
    const url = 'http://api-kylin.eoshenzhen.io:8890/v1/chain/get_table_by_scope';

    fetch(url, {
        method: 'POST', // or 'PUT'
        body: JSON.stringify(data), // data can be `string` or {object}!
        headers: new Headers({
            'Content-Type': 'application/json'
        })
    })
        .then(response => {
            return response.json();
        })
        .then(myJson => {
            // miners: myJson.rows[0].scope, myJson.rows[1].scope, myJson.rows[2].scope, ...
            console.log( myJson );
            if ( myJson.more && myJson.more.length > 0 ) {
                this.fetchTest( myJson.more );
            }
        })
        .catch(e => {
            console.error(e);
        });
}

// 2. 拿到了scope，接下来按正常的用code和scope获取表就可以了
```

## 方法二
如果你有一个节点，可以改造插件
```
// 1. 在chain_api_plugin.cpp的void chain_api_plugin::plugin_startup中，添加一个接口
       CHAIN_RO_CALL(get_currency_balance, 200),
+      CHAIN_RO_CALL(get_table_scopes_rows, 200),
       CHAIN_RO_CALL(get_currency_stats, 200),

// 2. 在chain_plugin.hpp中，声明接口函数，定义参数、返回值类型
    vector<asset> get_currency_balance( const get_currency_balance_params& params )const;

+   struct get_table_scopes_rows_params {
+      bool             json = false;
+      name             code;
+      name             table = 0;
+      string           lower_bound;
+      string           upper_bound;
+      uint32_t         limit = 10;
+   };
+   struct get_table_scopes_rows_result_scope {
+      name                scope;
+      vector<fc::variant> rows;
+   };
+   struct get_table_scopes_rows_result {
+      vector<get_table_scopes_rows_result_scope>  scopes;
+      string           more;
+   };
+   get_table_scopes_rows_result get_table_scopes_rows( const get_table_scopes_rows_params& params )const;
+
    struct get_currency_stats_params {
       name           code;
       string         symbol;

// 按格式补充完整
 FC_REFLECT( eosio::chain_apis::read_only::get_currency_balance_params, (code)(account)(symbol));
+FC_REFLECT( eosio::chain_apis::read_only::get_table_scopes_rows_params, (json)(code)(table)(lower_bound)(upper_bound)(limit));
+FC_REFLECT( eosio::chain_apis::read_only::get_table_scopes_rows_result_scope, (scope)(rows));
+FC_REFLECT( eosio::chain_apis::read_only::get_table_scopes_rows_result, (scopes)(more));
 FC_REFLECT( eosio::chain_apis::read_only::get_currency_stats_params, (code)(symbol));


// 3. 在chain_plugin.cpp中添加方法的实现
    return results;
 }

+read_only::get_table_scopes_rows_result read_only::get_table_scopes_rows( const read_only::get_table_scopes_rows_params& params )const {
+   get_table_scopes_rows_result results;
+
+   // 构造获取table scope的参数
+   const get_table_by_scope_params table_scope_params = get_table_by_scope_params {
+      .code        = params.code,
+      .table       = params.table,
+      .lower_bound = params.lower_bound,
+      .upper_bound = params.upper_bound,
+      .limit       = params.limit,
+   };
+
+   // 获取table scope
+   const auto table_scope_result = get_table_by_scope( table_scope_params );
+
+   // 根据每一个scope，获取该scope所在的的table
+   for ( auto &table_scope : table_scope_result.rows ) {
+
+      // 构造获取table的参数
+      const get_table_rows_params table_rows_params = get_table_rows_params {
+         .json  = params.json,
+         .code  = table_scope.code,
+         .scope = table_scope.scope.to_string(),
+         .table = table_scope.table,
+      };
+
+      // 获取当前scope所在的table
+      const auto table_rows_result = get_table_rows( table_rows_params );
+
+      get_table_scopes_rows_result_scope scope;
+
+      for ( auto &row : table_rows_result.rows ) {
+         scope.rows.emplace_back( row );
+      }
+      scope.scope = table_scope.scope;
+
+      results.scopes.emplace_back( scope );
+   }
+
+   // 如果more不为空，调用get_table_scope_rows时设置lower_bound参数为该more数据，获取更多
+   results.more = table_scope_result.more;
+
+   return results;
+}
+
 fc::variant read_only::get_currency_stats( const read_only::get_currency_stats_params& p )const {
    fc::mutable_variant_object results;
```

改造完后，回到`eosio`源码根目录，执行`eosio_build`重新编译`nodeos`。开启`chain_api_plugin`插件，启动`nodeos`。

#### 测试
```
18:48:36 CryptoKylin $: curl http://127.0.0.1:8888/v1/chain/get_table_scopes_rows -X POST -d '{"code":"eosio.token","table":"accounts","json":true,"limit":5}'
{
    "scopes":[
        {
            "scope":"1234512345bb",
            "rows":[{"balance":"6.4945 EOS"}]
        },{
            "scope":"eosasia11111",
            "rows":[{"balance":"1999799.8758 EOS"}]
        },{
            "scope":"eosbixincool",
            "rows":[{"balance":"19999683.6871 EOS"}]
        },{
            "scope":"eosecoeoseco",
            "rows":[{"balance":"80001720.6698 EOS"}]
        },{
            "scope":"eoseekingapp",
            "rows":[{"balance":"1970.0000 EOS"}]
        }
    ],
    "more":"eosfaucet111"
}

18:48:58 CryptoKylin $: curl http://127.0.0.1:8888/v1/chain/get_table_scopes_rows -X POST -d '{"code":"eosio.token","table":"accounts","json":true,"limit":5,"lower_bound":"eosfaucet111"}'
{
    "scopes":[
        {
            "scope":"eosfaucet111",
            "rows":[{"balance":"99996798.0156 EOS"}]
        },{
            "scope":"eoshuobipool",
            "rows":[{"balance":"99981813.0324 EOS"}]
        },{
            "scope":"eosio.bpay",
            "rows":[{"balance":"3114.8928 EOS"}]
        },{
            "scope":"eosio.faucet",
            "rows":[{"balance":"199999730.0000 EOS"}]
        },{
            "scope":"eosio.names",
            "rows":[{"balance":"1.0000 EOS"}]
        }
    ],
    "more":"eosio.ram"
}
```
