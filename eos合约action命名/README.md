## 概述：
EOS 中，合约的`action`对应合约 C++ 代码中的一个函数，如果你认为这个函数可以像传统 C++ 编码那样命名，那就错了。因为对应合约的`action`，所以有一些规则限制。

## 出错：
如果编译或执行合约的某个`action`，出现以下提示，说明`action`对应的函数命名不合规范：
```
Name should be less than 13 characters and only contains the following symbol .12345abcdefghijklmnopqrstuvwxyz
```

## 规范：
从`出错`提示知道，`action`对应的函数名，长度不能超过`12`个字符，并且只能由`.12345abcdefghijklmnopqrstuvwxyz`这些字符组成。
