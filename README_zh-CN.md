Windows 10/11 自动视频播放器

[English](README.md)|简体中文

## 功能特性

- [x] 支持配置。
- [x] 完成任务后自动关闭。

## 安装方式

### 自动

浏览发布页面并安装 .msi 文件。

### 手动

1. 确保已安装编译器，否则请安装 gcc 。

2. 克隆此仓库。

```bash
git clone --depth 1 https://github.com/ilasie/video_auto_player.git path/to/your/install_dir/video_auto_player
```

3. 构建。

```bash
cd path/to/install_dir/video_auto_player
g++ -static -O2 -std=c++17 -I. -o "Video Auto Player.exe" src/*.cpp
```

4. 点击生成的 `Video Auto Player.exe` 文件以运行。

5. 将此可执行文件添加到开机启动项。（可选）

```bash
AddToStartup.bat
```

## 第三方库

本项目使用了 Niels Lohmann 开发的 [JSON for Modern C++](https://github.com/nlohmann/json)，该库基于 MIT 许可证授权。