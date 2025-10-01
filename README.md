Automatic video player for Windows 10/11.

English|[简体中文](README_zh-CN.md)

## Features

- [x] Able to configure.
- [x] Close after running the task.

## Installing

### Automatically

Scan the release page and install the .msi file.

### Manually

1. Make sure a compiler has been installed. If not, please install gcc for your machine.

2. Clone this repository.

```bash
git clone --depth 1 https://github.com/ilasie/video_auto_player.git path/to/your/install_dir/video_auto_player
```

3. Build.

```bash
cd path/to/install_dir/video_auto_player
g++ -static -O2 -std=c++17 -I. -o "Video Auto Player.exe" src/*.cpp
```

4. Run by clicking the `Video Auto Player.exe` file generated.

5. Add this executable to startup. (Optional)

```bash
AddToStartup.bat
```

## Third-Party Libraries

This project uses [JSON for Modern C++](https://github.com/nlohmann/json) by Niels Lohmann, licensed under the MIT License.