### react-intl 是一个语言本地化的库
*我们可以通过配置 `json` 文件或 `js` 文件实现多语言切换，可以还在用这么 `low` 的方法真的有钱途嘛~*
*比如中文的货币符号是 '￥'，英文是 '$'，这些通过配置 `react-intl` 都能自动帮我们处理好。*
### 仓库地址：[https://github.com/yahoo/react-intl](https://github.com/yahoo/react-intl)
`react-intl` 可以使用组件的方式，或 `api` 的方式进行语言本地化。推荐使用组件的方式，只有在组件方式不可用时（比如使用 `antd` 的`message`输出提示消息时），才选择使用 `api` 的方式。
### 使用示例
```
// 准备语言文件 locale/zh_CN.js 和 locale/en_US.js
const zh_CN = {
    "intl.say_hello": "您好，{name} !",
}
export default zh_CN;
const en_US = {
    "intl.say_hello": "Hello,  {name} !",
}
export default en_US;

// 初始化
import {addLocaleData, IntlProvider} from 'react-intl';
import en from 'react-intl/locale-data/en';
import zh from 'react-intl/locale-data/zh';
import zh_CN from './locale/zh_CN';
import en_US from './locale/en_US';

// 初始化
addLocaleData([...en, ...zh]);
const  arrLang = [];
arrLang['zh'] = zh_CN;
arrLang['en'] = en_US;

// 父组件进行包裹
<IntlProvider locale={this.state.locale} messages={arrLang[this.state.locale]}>
    <Intl handleSetLocale={this.setLocale.bind(this)}/>
</IntlProvider>

// 切换
setLocale = (lang) => {
    switch (lang) {
          case 'zh': {
            this.setState({ locale: 'zh' });
            break;
          }
          case 'en': {
            this.setState({ locale: 'en' });
            break;
          }
          default: {
            this.setState({ locale: 'zh' });
          }
    }
}

// 用于 api 方式的文件
import {defineMessages} from 'react-intl';
const Intlapi = defineMessages({
    say_hello: {
        id: 'intl.say_hello',
        defaultMessage: '你好呀',
    },
});
export default Intlapi;

// 子组件（被包裹）
import React, { Component } from 'react';
import { FormattedMessage, injectIntl } from 'react-intl';
import Intlapi from './Intlapi';

class Intl extends Component {

    chooseZH = () => {
        this.props.handleSetLocale('zh');
    }

    chooseEN = () => {
        this.props.handleSetLocale('en');
    }

    render() {
        return (
            <div style={{ textAlign: "center", padding: "20px 0" }}>
                <button onClick={this.chooseZH.bind(this)} style={{ margin: "0 10px", width: "80px", height: "30px" }}>
                    中文
                </button>
                <button onClick={this.chooseEN.bind(this)} style={{ margin: "0 10px", width: "80px", height: "30px" }}>
                    English
                </button>
                <div style={{ margin: "20px 0" }}>
                    <span>使用组件的方式：</span>
                    <FormattedMessage
                        id="intl.say_hello"
                        defaultMessage="你好！"
                        values={{
                            name: "Jay",
                        }}
                    />
                </div>
                <div style={{ margin: "20px 0" }}>
                    <span>使用API的方式：</span>
                    {this.props.intl.formatMessage(Intlapi.say_hello, {name: "Jay Lau"})}
                </div>
            </div>
        );
    }
}

// 使用 injectIntl 注入，然后 this.props会得到一个 intl 对象，用于 api 方式
export default injectIntl(Intl);
```
### Demo
```
$ yarn install
$ yarn start
```