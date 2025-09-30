Automatical video player for Windows.

## Featrues

- [x] Able to configure.
- [x] Close after running the task.

## Installing

### Auto

Scan the release page and install the .msi file, then click it in your machine. 

### Manually

1. Make sure complier has been installed. If not, please install gcc for your machine.

2. Clone this repository.

```bash
git clone --depth 1 https://github.com/ilasie/video_auto_player.git video_auto_player
```

3. Build.

```bash
cd video_auto_player
g++ -static -O2 -std=c++17 -I. -o "Video Auto Player.exe" src/*.cpp
```

4. Run by clicking `Video Auto Player.exe` generated.

5. Add this execute to startup. (Optional)

```bash
./AddToStartup.bat
```