# Camera Telemetry Qt Quick Application

A Qt 6 C++ application with a QML dashboard, OpenCV camera capture, playback controls, and simulated vehicle telemetry.

## Features

- Live OpenCV camera capture
- Play, pause, resume, forward, and rewind controls
- 640x480, 1280x720, and 1920x1080 resolution selection
- Temperature, speed, GPS position, and motor RPM telemetry every two seconds
- QML-based dashboard with reusable components
- OpenCV frames rendered in a custom `QQuickPaintedItem`
- Camera and telemetry implemented in separate C++ classes
- CMake build configuration

## Requirements

- Qt 6.5 or newer (`Quick`, `Qml`, and `QuickControls2`)
- OpenCV 4.x
- CMake 3.16 or newer
- A C++17 compiler

## Controls

- **Play** opens camera index 0 and starts capture.
- **Pause** freezes the most recently displayed frame.
- **Resume** restarts capture after pausing.
- **Forward** captures and displays subsequent frames.
- **Rewind** moves backward through frames captured during the current session.
- The resolution selector applies 640 x 480, 1280 x 720, or 1920 x 1080.

Camera errors are shown in a dialog. Resolution warnings appear in the status banner and a popup.

## Project structure

```text
Components/
  qmldir
  Theme.qml
  SplashScreen.qml
  MainScreen.qml
  Camerascreen.qml
  Controls.qml
  Telemetrydata.qml
include/
  CameraController.hpp
  TelemetryGenerator.hpp
  VideoItem.hpp
src/
  main.cpp
  CameraController.cpp
  TelemetryGenerator.cpp
  VideoItem.cpp
main.qml
resources.qrc
CMakeLists.txt
```

## Windows Build

Install Visual Studio 2022 Build Tools with **Desktop development with C++** and a
Windows SDK. Use an MSVC-built Qt and OpenCV installation; mixing MinGW Qt with
MSVC OpenCV libraries will not work.

```powershell
cmake -S . -B build-msvc -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_PREFIX_PATH="D:\Qt\6.8.3\msvc2022_64" `
  -DOpenCV_DIR="D:\opencv\build\x64\vc16\lib"
cmake --build build-msvc --config Release --parallel
```

For a local run, add the Qt and OpenCV runtime directories to `PATH`:

```powershell
$env:PATH = "D:\Qt\6.8.3\msvc2022_64\bin;D:\opencv\build\x64\vc16\bin;$env:PATH"
& ".\build-msvc\Release\CameraTelemetryApp.exe"
```

In the app, the splash screen transitions to the telemetry dashboard. Click **Play** to open the default camera. Telemetry updates every two seconds.
If the camera cannot be opened, close apps that may be using it and check Windows
camera permissions.
