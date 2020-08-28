
## 1 文件说明

ble_3435_sdk_ext_39_0F0E —— 官方SDK（Beken，上海博通，BK3431Q和BK3435内核一样，所以SDK也一样）    

tuya_ble_sdk_demo —— 涂鸦SDK例程（内含Tuya_ble_sdk源码）    

bk3435.uvprojx —— Keil工程文件快捷方式（相对路径，下载后可直接双击打开）    

clear_git.bat —— Project清理及Git上传脚本     


## 2 相关文档

[SDK开发指南](https://docs.tuya.com/zh/iot/smart-product-solution/outdoor/lost/user-guide?id=K9qhcltt0zmdn)  

[DP点协议](https://docs.tuya.com/zh/iot/smart-product-solution/outdoor/lost/dpprotocol?id=K9qhc1o8v9b9f)  

产测资料包请产品经理联系工艺同学（水门）获取最新版本      

   

​      

​     

## ---------- Change Log ----------

## [版本1.1] - 2020-08-29

### Added

- Nothing

### Changed

- 内部晶振切换到外部晶振（内部晶振容易导致断开连接）
- OTA结束后本地主动断开连接

## [版本1.0] - 2020-08-28

### Added

- Nothing

### Changed

- 修复授权后，上电到开机时间段内功耗偏高的问题
- 优化Flash操作逻辑，增加notify缓存空间
- 外部晶振切换到内部晶振（本地无计时功能，内部晶振够用）


## [版本0.9] - 2020-08-17

### Added

- Nothing

### Changed

- 优化授权、产测流程
- 修复双击防丢器的同时手机连接防丢器导致ibeacon未停止的问题
- 修复设备复位时按键卡死的问题


## [版本0.8] - 2020-07-08

### Added

- 初始版本

### Changed

- 初始版本











