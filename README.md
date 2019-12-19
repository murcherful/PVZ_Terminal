# PVZ_Terminal
This is a game for AP(Adanced Program) Project2.

# 游戏简介

- 本游戏为控制台版的植物大战僵尸，无尽模式，玩家需要通过种植不同植物来抵挡僵尸，分数由抵挡时间和击杀僵尸数决定。

- 项目在Ubuntu16.04、CMAKE3.5.1、g++5.4.0环境下开发。`包含CMakeList.txt`。但由于使用了Linux系统特有的函数和库，因此不支持在Windows或其他系统下编译。编译成功后，生成可执行文件`Game`。

- 编译：进入`PVZ`文件夹

  ```
  mkdir build
  cd build
  cmake ..
  make
  ```

- 运行：为了游戏正常运行，需要全屏控制台。

  ```
  ./Game
  ```
- 植物和僵尸的特性基本同原植物大战僵尸。操作上，游戏分为三种状态`NORMAL`、`PLANT`、`REMOVE`，通过`f`切换。`NORMAL`状态用于收集太阳，`PALNT`状态用于种植植物，`REMOVE`状态用于移除植物。`w`、`a`、`s`、`d`移动选择框进行选择种植或收集的区域。`Enter`种植、收集或移除。通过`q`、`w`选择种植的植物。`p`暂停，再按继续。暂停状态下，`ESC`退出。

