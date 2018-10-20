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
