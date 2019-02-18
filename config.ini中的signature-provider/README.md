## 概述：
如果你要部署一个 EOSIO 的出块节点，你可能会关心这个问题。如果不是，或者你只是部署一个普通节点，那你不用care这里说的。

## `signature-provider`应该填哪对秘钥：
在节点的配置文件`config.ini`中，`signature-provider`应该填哪一对秘钥呢？可能你会和刚开始的我一样，觉得应该填的是出块账号对应的 owner 或 active 的 key ，但其实这是不建议的，因为涉及到安全问题。
其实你完全可以通过 `cleos create key --to-console` 出来一对`key` ，只要你注册出块节点的时候使用这个`key`，那么`signature-provider`你就可以使用这对`key`。
即，只要保证`signature-provider`用的是注册时的就行，至于注册你用哪个，你爱用哪个用哪个。
