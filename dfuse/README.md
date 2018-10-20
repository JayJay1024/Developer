### Dfuse是EOS Canada为EOS开发打造的流式API，可以让开发者更轻松流畅的获取链上信息。使用Websocket协议，可以主动推送数据至客户端。
### 官网: [https://dfuse.io/zh/](https://dfuse.io/zh/)
### 使用示例
```
// token字段后面的是key，目前属于beta版本，可以在官网免费申请
this.ws_url = "wss://mainnet.eos.dfuse.io/v1/stream?token=eyJhbGciOiJLTVNFUzI1NiIsInR5cCI6IkpXVCJ9.eyJhdWQiOiJiZXRhLXVzZXJzLmVvc3dzLmlvIiwiZXhwIjoxNTQyMDgyNjk5LCJqdGkiOiI5MGYxNGI2Yy0xMGZhLTQ3NzEtYTNlYS0xZDMxOWFkMzFiZWQiLCJpYXQiOjE1Mzk0OTA2OTksImlzcyI6ImVvc3dzLmlvIiwic3ViIjoiMTM3MDQyOTUyQHFxLmNvbSJ9.bbz_udBFm9eJ3PCbS1NNX4a3EPQhjo0vpqNPRmko7ZIu4BdnYc3OEWPKc8FqVt3bnNsswt13XtIrH0nCVjjkWg";

// 初始化WebSocket
this.ws = new WebSocket(this.ws_url);

// 设置客户端接收到服务端推送的回调
this.ws.onopen = (evt) => {
    console.log("ws is opened");
}
this.ws.onmessage = (evt) => {
    console.log("ws rev data: ", evt.data);
}

// 根据官网的发送格式，侦听eosbetdice11收到转账的action
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
```
### Demo
```
$ yarn install
$ yarn start
```
