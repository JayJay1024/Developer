var exuteFibo = require('./master');
console.log('=====Start========');
var st = Date.now();
exuteFibo().then(result => {
    console.log(`Finish all work and using ${Date.now() - st} ms`);
    console.log('####Get result from multiple-processes: '+ result);
});

// st = Date.now();
// exuteFibo().then(function(result){
//     console.log(`Finish all work and using ${Date.now() - st} ms`);
//     console.log('####Get result1 from mutliple-processes: '+ result);
// });
