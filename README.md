# Camera Telemetry Qt Application

A Qt 6 C++ desktop application that displays an OpenCV camera feed and simulated vehicle telemetry.

## Features

- Live OpenCV camera capture
- Play, pause, resume, forward, and rewind controls
- 640x480, 1280x720, and 1920x1080 resolution selection
- Temperature, speed, GPS position, and motor RPM telemetry every two seconds
- Camera and telemetry implemented in separate classes
- CMake build configuration

## Requirements

- Qt 6.5 or newer (`Widgets`)
- OpenCV 4.x
- CMake 3.16 or newer
- A C++17 compiler

## Windows Build

Configure Qt and OpenCV with CMake. Adjust the paths for your installation:

```powershell
cmake -S . -B build -G "Ninja" `
  -DCMAKE_PREFIX_PATH="D:/Qt/6.8.3/mingw_64" `
  -DOpenCV_DIR="D:/opencv/build"
-DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```

Run the executable from `build/CameraTelemetryApp.exe`. Ensure the Qt and OpenCV DLL directories are in `PATH`, or use Qt's deployment tool.
