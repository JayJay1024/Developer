import React, { Component } from 'react';

class Dfuse extends Component {
    constructor(props) {
        super(props);

        this.state = {
            ws_state: null,
        }

        this.ws_url = "wss://mainnet.eos.dfuse.io/v1/stream?token=eyJhbGciOiJLTVNFUzI1NiIsInR5cCI6IkpXVCJ9.eyJhdWQiOiJiZXRhLXVzZXJzLmVvc3dzLmlvIiwiZXhwIjoxNTQyMDgyNjk5LCJqdGkiOiI5MGYxNGI2Yy0xMGZhLTQ3NzEtYTNlYS0xZDMxOWFkMzFiZWQiLCJpYXQiOjE1Mzk0OTA2OTksImlzcyI6ImVvc3dzLmlvIiwic3ViIjoiMTM3MDQyOTUyQHFxLmNvbSJ9.bbz_udBFm9eJ3PCbS1NNX4a3EPQhjo0vpqNPRmko7ZIu4BdnYc3OEWPKc8FqVt3bnNsswt13XtIrH0nCVjjkWg";
    
        this.restartWS = this.restartWS.bind(this);
    }

    componentDidMount = () => {
        this.restartWS();

        setInterval(() => {
            switch (this.ws.readyState) {
                case WebSocket.CONNECTING:
                    this.setState({ ws_state: "connecting" });
                    break;
                case WebSocket.OPEN:
                    this.setState({ ws_state: "open" });
                    break;
                case WebSocket.CLOSING:
                    this.setState({ ws_state: "closing" });
                    break;
                case WebSocket.CLOSED:
                    this.setState({ ws_state: "cloed" });
                    break;
                default:
                    // this never happens
                    break;
              }
        }, 500);
    }

    closeWS = () => {
        console.log("close ws");
        this.ws.close();
    }

    sendWS = () => {
        console.log("send ws");

        let _send_data = {
            "type": "get_actions",
            "listen": true,
            "req_id": "123",
            // "start_block": 22266908,
            // "fetch": true,
            "data": {
                "account": "eosio.token",
                "action_name": "transfer",
                "receiver": "eosbetdice11",
            }
        };
        this.ws.send(JSON.stringify(_send_data));
    }

    unlistenWS = () => {
        console.log("unlisten ws");

        let _send_data = {
            "type": "unlisten",
            "data": {
                "req_id": "123"
            }
        };
        this.ws.send(JSON.stringify(_send_data));
    }

    restartWS = () => {
        console.log("restart ws");
        this.ws = new WebSocket(this.ws_url);
        this.ws.onopen = (evt) => {
            console.log("ws is opened");
        }
        this.ws.onmessage = (evt) => {
            console.log("ws rev data: ", evt.data);
        }
    }
    

    render() {
      return (
        <div style={{ textAlign: "center" }}>
            <div style={{ margin: "20px 0", fontSize: "2em" }}>This is dfuse demo</div>
            <button onClick={this.sendWS.bind(this)} style={{ margin: "0 10px", width: "120px" }}>
                ws send
            </button>
            <button onClick={this.closeWS.bind(this)} style={{ margin: "0 10px", width: "120px" }}>
                ws close
            </button>
            <button onClick={this.unlistenWS.bind(this)} style={{ margin: "0 10px", width: "120px" }}>
                ws unlisten
            </button>
            <button onClick={this.restartWS.bind(this)} style={{ margin: "0 10px", width: "120px" }}>
                ws restart
            </button>
            <div style={{ margin: "30px 0", fontSize: "2em" }}>
                ws.readyState: {this.state.ws_state}
            </div>
        </div>
      );
    }
  }
  
  export default Dfuse;