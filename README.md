# FDumpDex-Android脱壳

> 插件需要在xposed环境中使用,支持市面上大多数加密壳,软件仅供学习用，请勿用于其他用途,项目不是成品，可能会引起软件崩溃

### 编译环境

Android Studio 3.0

### 支持设备

大多数xposed环境的手机

### apk 下载

[https://github.com/ljf1239848066/FDumpDex/releases](https://github.com/ljf1239848066/FDumpDex/releases)

### 分支

[develop](https://github.com/ljf1239848066/FDumpDex/tree/develop) 开发分支


[master](https://github.com/ljf1239848066/FDumpDex/tree/master) 稳定分支

### 使用方式

下载源码编译或者下载apk包并安装，应用xposed模块（推荐使用[VirtualXposed](https://github.com/android-hacker/VirtualXposed)），在 VXP 中打开 dumpDex 选择待脱壳的应用后重启，运行加固的应用后，插件会自动将dex文件dump到 **/sdcard/dumpDex/包名/dex/source-** 目录

**apk文件不会实时更新，获取最新apk请自行编译源码**


### 源码编译

将源码下载或者clone到本地，使用android studio打开，编译成功后，安装apk，

配置完成后激活xposed重启即可

### TODO

当前通过 A64InlineHook 在 Android P 与 Q 上有很多兼容性问题，修复需要花费较多精力与时间，当前未兼容 arm64 环境，希望有兴趣的大佬可以自行兼容。

### 更多精彩内容请关注博客

[https://blog.lxzh123.com/](https://blog.lxzh123.com/)

### 相关文章(网友分享)

[dumpDex 脱壳原理](http://liteng1220.com/blog/articles/dumpdex-principle/)

[Android逆向之路---脱壳360加固原理解析](https://juejin.im/post/5c1934226fb9a04a0b221c3c)

### 支持开发，欢迎打赏

![](https://lxzh.oss-cn-hangzhou.aliyuncs.com/pay.png)


