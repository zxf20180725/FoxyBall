# FoxyBall（目前正在开发中，请勿用于实际项目）
## 简介
FoxyBall是用C语言开发的一款nosql内存数据库，并且支持Linux和Windows操作系统。  
FoxyBall服务采用C/S架构，目前有python的客户端和windows、linux的服务端。  

## 特点
  * 高性能，FoxyBall的数据总是存储在内存中的，并且核心数据结构是hash表。
  * 持久化，用户不用担心内存中数据丢失的问题，FoxyBall会定期持久化数据到磁盘中。
  * 分布式，FoxyBall是支持分布式部署的。
  * 简易性，FoxyBall不论是安装部署还是使用，都非常简单，命令一共就14个。
  
## 内部实现
### 数据结构
FoxyBall的数据存储使用的是hash表。hash冲突的解决策略是hash数组+链表的形式。  
FoxyBall的过期键采用双向链表存储。  

### 淘汰策略
FoxyBall采用主动删除和惰性删除两种策略。  
主动删除：FoxyBall将定期检查设置过expire的key，将过期的key主动删除。  
惰性删除：当用户访问一个key的时候，FoxyBall总会先检查该key是否过期，如果过期就删除。  
