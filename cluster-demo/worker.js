// 这里是master fork的worker
cluster = require('cluster');

function fibo(n) {
    return n == 0 ? 0 : n > 1 ? fibo(n - 1) + fibo(n - 2) : 1;
}

console.log(`[worker ${cluster.worker.id}] start ...` );

process.on('message', (msg) => {
    let st = Date.now();
    console.log(`[worker ${cluster.worker.id}] start to work`);
    let result = fibo(msg);
    console.log(`[worker ${cluster.worker.id}] worker finish work and using ${Date.now() - st} ms`);
    process.send(result);  // master订阅了该worker得message，所以master将收到该result
});
