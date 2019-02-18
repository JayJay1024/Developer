## 概述：
使用`scatter js sdk`发起一笔交易，请求`scatter`确认时，出现如下错误提示：
```
Missing required accounts, repull the identity
```

## 解决：
需要添加权限选项`{ authorization: [account.name@account.authority] }`，比如 EOS 转账交易：
```
eos.contract('eosio.token').then(contract => {
    contract.transfer(
        'fromaccount1',
        'toaccount111',
        '1.0000 EOS',
        'test',
        { authorization: ['fromaccount1@active'] }
    );
});
```
