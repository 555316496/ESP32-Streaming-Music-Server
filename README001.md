# ESP32 Streaming Music Server

基于 ESP32 的本地音乐流媒体服务器，通过 Web 界面播放 SD 卡中的音乐。

## 功能特性

- 🎵 **SD 卡音乐库自动扫描** — 支持 mp3/wav/flac/aac/ogg/m4a
- 📱 **响应式 Web 播放器** — 手机/电脑浏览器直接访问，无需安装 App
- 📂 **播放列表管理** — 创建/删除/添加歌曲
- 🕐 **最近播放记录** — 自动记录播放历史
- 🔍 **歌曲搜索** — 实时搜索本地音乐
- 🌐 **WiFi 配网** — STA/AP 双模式，支持 mDNS (esp32streaming.local)
- 🎨 **多主题切换** — 蓝/绿/粉三种主题
- ⚡ **流畅加载** — 分页加载 + 虚拟列表，171 首歌曲秒开

## 硬件要求

| 组件 | 型号/规格 | 接线 |
|------|----------|------|
| 主控 | ESP32-DevKitC | — |
| SD 模块 | SPI 接口 | SCK=14, MISO=19, MOSI=13, CS=15 |
| SD 卡 | FAT32 格式 | ≤32GB 推荐 |

## 软件依赖

- [PlatformIO](https://platformio.org/) (VS Code 插件)
- 必需库：
  - `ESPAsyncWebServer` @ ^3.0.0
  - `ArduinoJson` @ ^7.0.0
  - `AsyncTCP` @ ^1.0.0

## 快速开始

### 1. 克隆项目
```bash
git clone https://github.com/你的用户名/ESP32-Streaming-Music-Server.git
cd ESP32-Streaming-Music-Server
