![robot-arm-monitor title banner](./docs/assets/robot-arm-monitor_1.png)

<div align="center">

[![CI](https://github.com/niaBaldoni/robot-arm-monitor/actions/workflows/ci.yml/badge.svg?branch=main)](https://github.com/niaBaldoni/robot-arm-monitor/actions/workflows/ci.yml) ![C++20](https://img.shields.io/badge/C%2B%2B-20-blue) ![Linux](https://img.shields.io/badge/platform-Linux-orange) ![License: MIT](https://img.shields.io/badge/License-MIT-yellow)

</div>

---
## Architecture

We have five total threads:

- `encoderThread`, `temperatureThread` and `forceThread` write their values to the shared struct `sensorData`
- `displayThread` reads the shared struct and prints a status line once a second
- `monitorThread` reads the shared struct, evaluates danger conditions, prints alerts

![Architecture diagram](./docs/assets/diagram1.png)

The shared struct `sensorData` is protected by one mutex, `dataMutex`. When `monitorThread` locks `dataMutex` and reads `angle`, `temperature`, and `force` together, it is guaranteed that none of those values can change mid-read: it gets a fully consistent snapshot of all three at once. This project is focused on catching combinations of dangerous values, so we need them to be consistent at the same instant.

## How to build and run

Clone the repo, navigate to your repo folder, then execute these commands:

```bash
mkdir build
cd build
cmake ..
make
./robot_arm_monitor
```

---

Unit tested with Google Test, CI/CD via GitHub Actions

---
Photo by [Simon Kadula](https://unsplash.com/photos/a-factory-filled-with-lots-of-orange-machines-8gr6bObQLOI) on [Unsplash](https://unsplash.com).