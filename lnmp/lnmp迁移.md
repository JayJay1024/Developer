## LNMP(Linux Nginx Mysql PHP)
### LNMP官网：[https://lnmp.org/](https://lnmp.org/)
### 这里记录一下LNMP部署的站点从云服务器A迁移到云服务器B的过程，服务器的系统为Ubuntu 16.04 64位，LNMP1.5
- 首先，使用LNMP搭建的网站根站点一般在`/home/wwwroot`下，默认站点目录是`default`(通过`/usr/local/nginx/conf/nginx.conf`可知)，`default`里面还有一个`phpmyadmin`目录，由此，大概你能猜到能做什么了(由于安全问题，`default`和`phpmyadmin`可能已被重命名，这样就能禁止通过网页访问`phpmyadmin`等)。
- 因为`Nginx`可以为同一`IP`绑定多个域名，所以`/home/wwwroot`下除了`default`应该还有别的站点目录。在对应站点目录找到数据库账号和密码。
- 通过浏览器使用`IP`访问，这时访问的应该是`/home/wwwroot/default`，页面上有`探针`、`phpinfo`、`phpMyadmin`等功能链接。
- 通过`phpMyadmin`导出数据库。
- 使用`探针`查看站点`Mysql`数据库、`PHP`模块、`LNMP`的版本。
- 到`LNMP`[官网](https://lnmp.org/)下载对应的版本。
- 按照官网[安装](https://lnmp.org/install.html)教程进行安装，安装过程中选择对应的`Mysql`、`PHP`版本。
- 安装完成后通过`IP`访问看是否出现`LNMP`页面。然后通过`phpMyadmin`创建数据库账号，并且导入数据库，设置数据库拥有者权限。
- 打包网站代码，并上传至新的服务器对应的站点目录下。
- 如果`Nginx`有绑定多域名，则打包原服务器`/usr/local/nginx/conf`下的`vhost`，并上传到新的站点服务器相应目录下。
- `sudo /etc/init.d/nginx restart`重启服务，然后通过域名访问网站。
### 踩坑
如果网站是基于`thinkphp`等一些框架的，这时候通过域名访问可能返回的是`500`错误。这个问题的原因很可能是由于`LNMP`的防跨域引起的。
根据[这里](https://lnmp.org/faq/lnmp-vhost-add-howto.html)的说法，在`thinkphp`、`codeigniter`、`laravel`等框架下，网站目录一般是在`public`下，但
是`public`下的程序要跨目录调用`public`上级目录下的文件，因为`LNMP`默认是不允许跨目录访问的，所以都是必须要将防跨目录访问的设置去掉：`LNMP 1.4`或以上将 `/usr/local/nginx/conf/fastcgi.conf`里面的`fastcgi_param PHP_ADMIN_VALUE "open_basedir=$document_root/:/tmp/:/proc/"`这一行删除或在行首添加
`#`号注释掉，然后`sudo /etc/init.d/nginx restart`重启`Nginx`服务。