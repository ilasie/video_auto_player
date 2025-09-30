Automatical video player for windows

## Featrues

- [x] Able to configure.
- [x] Close after running the task.

## Installing

### Install graphically

Scan the release page and install the .msi file, then click it in your machine. 

### Install manaully

1. Make sure complier has been installed.

```bash
g++ --version
```

If no output, please install gcc for your machine.

2. Clone this repository.

```bash
git clone --depth 1 https://github.com/ilasie/video_auto_player.git video_auto_player
```

3. Build.

```bash
cd video_auto_player
g++ -static -O2 -std=c++17 -I. -o "Video Auto Player.exe" src/*.cpp
```

4. Add this execute to startup (Optional)

```bash
./AddToStartup.bat
```