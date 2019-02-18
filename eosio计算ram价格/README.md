## 概述：
有时候我们使用别人的 rpc 地址，`get actions`的时候返回的是空：
```
$: cleos -u https://别人的rpc.io get actions myaccount111
#  seq  when                              contract::action => receiver      trx id...   args
================================================================================================================
```

这是因为，你使用的这个别人的 rpc 地址，他的 nodeos 没有打开`filter-on`配置，或者他只开了他需要监听的账号和 action ，这个时候，你就要另找 rpc 地址了。

## 配置自己的节点：
如果你有自己的节点，你想用自己节点的 rpc 查找 actions ，应该怎么配置呢？
当然，你可以在 `config.ini` 配置文件中，设置 `filter-on = *` ，这样会记录全网的所有账号，以及所有的 actions ，但是你要认识到这需要很大的服务器物理内存。
如果你只需要记录账号`myaccount111`的`transfer`，你只用这样配置就好了：`filter-on = myaccount111::transfer`。
