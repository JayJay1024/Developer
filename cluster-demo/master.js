function exuteFibo() {
    return (new Promise(
        (reslove, reject) => {
            let result = [];
            let workerID = [];
            let cluster = require('cluster');
            let numCPUs = require('os').cpus().length;

            cluster.setupMaster({
                exec: 'worker.js',
                slient: true,
            });

            cluster.on('fork', (worker) => {
                if (workerID.indexOf(worker.id) !== -1) {
                    console.log(`[master ${process.pid}] : fork worker ${worker.id}`);
                }
            });
            cluster.on('exit', (worker, code, signal) => {
                console.log(`[master] : worker ${worker.id} died`);
            });
            
            let st = Date.now();
            let numOfCompelete = 0;
            let collection = [44, 42, 42, 43];
            
            for (let i = 0; i < Math.min(numCPUs, collection.length); i++) {
                wk = cluster.fork();
                workerID.push(wk.id);
                wk.send(collection[i]);
            }

            workerID.forEach((id) => {
                cluster.workers[id].on('message', (msg) => {
                    console.log(`[master] receive message from [worker${id}]: ${msg}`);
                    result.push(msg);

                    numOfCompelete++;
                    if (numOfCompelete === collection.length) {
                        console.log(`[master] finish all work and using ${Date.now() - st} ms`);
                        workerID.forEach((id) => {
                            if (!cluster.workers[id].suicide) {
                                cluster.workers[id].disconnect();
                            }
                        });
                        reslove(result);
                    }
                });
            });
        }
    ));
}

module.exports = exuteFibo;
