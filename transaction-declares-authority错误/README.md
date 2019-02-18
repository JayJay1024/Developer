## 概述：
push action 或者 transaction 的时候，出现类似以下错误：
```
transaction declares authority '{"actor":"myaccount111","permission":"active"}', but does not have signatures for it under a provided delay of 0 ms, provided permissions [{"actor":"myaccount111","permission":"eosio.code"}], and provided keys []
```

## 原因：
检查部署`myaccount111`的合约代码，看看是不是使用了内联 action 或 defer ，这需要你的`myaccount111`具有`eosio.code`权限

## 解决：
给`myaccount111`赋予`eosio.code`权限
```
// 这里 EOS5RZcioUTeXNWCUehqtSHpzcXEj4auyRQiD3qwsjCQjWqmH8Qgs 是你 myaccount111 的公钥
$: cleos set account permission myaccount111 active '{"threshold":1, "keys":[{"key":"EOS5RZcioUTeXNWCUehqtSHpzcXEj4auyRQiD3qwsjCQjWqmH8Qgs", "weight":1}], "accounts": [{"permission":{"actor":"myaccount111","permission":"eosio.code"},"weight":1}]}' owner -p myaccount111
```
