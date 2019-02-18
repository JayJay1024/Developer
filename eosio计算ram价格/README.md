## 首先获取多索引表`rammarket`的数据：
```
$: cleos -u https://nodes.get-scatter.com:443 get table eosio eosio rammarket
{
  "rows": [{
      "supply": "10000000000.0000 RAMCORE",
      "base": {
        "balance": "70910488246 RAM",
        "weight": "0.50000000000000000"
      },
      "quote": {
        "balance": "1722420.0326 EOS",
        "weight": "0.50000000000000000"
      }
    }
  ],
  "more": false
}
```

## 那么`n KB`的内存价格为：
```
n KB内存需要的EOS = ( n * quote.balance ) / ( n + base.balance / 1024 )
