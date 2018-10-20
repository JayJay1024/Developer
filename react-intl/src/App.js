import React, { Component } from 'react';
// import logo from './logo.svg';
// import './App.css';

import Intl from './Intl/Intl';

// for locale
import {addLocaleData, IntlProvider} from 'react-intl';
import en from 'react-intl/locale-data/en';
import zh from 'react-intl/locale-data/zh';
import zh_CN from './locale/zh_CN';
import en_US from './locale/en_US';

// for locale
addLocaleData([...en, ...zh]);
const  arrLang = [];
arrLang['zh'] = zh_CN;
arrLang['en'] = en_US;

class App extends Component {
  constructor(props) {
    super(props);

    this.state = {
      locale: 'zh',
    }
  }

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

  render() {
    return (
      // <div className="App">
      //   <header className="App-header">
      //     <img src={logo} className="App-logo" alt="logo" />
      //     <p>
      //       Edit <code>src/App.js</code> and save to reload.
      //     </p>
      //     <a
      //       className="App-link"
      //       href="https://reactjs.org"
      //       target="_blank"
      //       rel="noopener noreferrer"
      //     >
      //       Learn React
      //     </a>
      //   </header>
      // </div>
      <div>
        <IntlProvider locale={this.state.locale} messages={arrLang[this.state.locale]}>
            <Intl handleSetLocale={this.setLocale.bind(this)}/>
        </IntlProvider>
      </div>
    );
  }
}

export default App;
