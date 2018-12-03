## MongoDB简介
MongoDB是一个基于分布式文件存储，介于关系型和非关系型的数据库。
今天我们通过第三方平台（如Google等）可以很轻易地访问和抓取用户数据。用户的个人信息、社交网络、地理位置，用户生成的数据和用户操作日子已经成倍的增加。我们如果要对这些数据挖掘，那SQL数据库已经不适合这些应用了，NoSQL(即No Only SQL，MongoDB就是这种类型的数据库之一)数据库的发展却能很好的处理这些大数据。
MongoDB将数据存储为一个文档，数据结构有键值对(key=>value)组成，类似于JSON对象，MongoDB中叫BSON(即序列化的二进制JSON)。
## MacOS平台安装MongoDB
官网提供了安装包：[https://www.mongodb.com/download-center#community](https://www.mongodb.com/download-center#community)；
也可以通过brew进行安装:
```
$ sudo brew install mongodb

// 安装支持 TLS/SSL 命令
$ sudo brew install mongodb --with-openssl

// 安装最新开发版本
$sudo brew install mongodb --devel
```
## MongoDB操作
#### 运行
```
// 创建一个数据库存储目录
$ sudo mkdir -p /data/db

// 启动 mongodb，默认数据库目录即为 /data/db
$ sudo mongod

// 打开另一个终端连接刚启动的mongod服务
$ mongo
```
#### 操作(启动mongo连接数据库服务后)
##### 显示数据库
```
// 显示所有数据库列表
> show dbs

// 显示当前的数据库或集合
> db
```
##### 使用mytest数据库，如果不存在则新建
`> use mytest`
##### 删除mytest数据库
```
> use mytest
> db.dropDatabase()
```
##### 创建集合
```
// 在test数据库上创建一个叫kk的集合
> use test
> db.createCollection("kk")

// 或者插入数据的时候，也会自动创建集合
> db.kk.insert({"name" : "hhily"})
```
##### 查看集合
`> show collections`
##### 删除集合
```
// 删除test数据库中叫kk的集合
> use test
> db.kk.drop()
```
##### 插入文档
```
// 在runoob数据库col集合中插入一条文档
> use runoob
> db.col.insert({title: 'MongoDB 教程', 
    description: 'MongoDB 是一个 Nosql 数据库',
    by: '菜鸟教程',
    url: 'http://www.runoob.com',
    tags: ['mongodb', 'database', 'NoSQL'],
    likes: 100
})
// 上面的插入中，如果col集合不存在，将新建
```
##### 查询文档
```
> db.col.find().pretty()
// or
> db.col.findOne().pretty()
```
##### 更新文档
`> db.col.updateOne({'title':'MongoDB 教程'},{$set:{'title':'MongoDB'}})`
##### 删除文档
`db.col.deleteMany({'title':'MongoDB 教程'})`
