# Iris Flow Driver

Linux kernel driver for the NVIDIA Jetson AGX Orin's camera and ISP pipeline.
Part of the Iris Flow vision pipelining ecosystem.

## Features
- Multi-stream camera capture (up to 6 streams)
- ISP tuning (brightness, contrast, saturation, white balance)
- Frame buffer management
- HDR support
- V4L2 integration

## Quick Start
```bash
make lib test    # Build userspace library and tests
make module      # Build kernel module (requires Jetson kernel headers)
```

## License: GPL-2.0-only

## 🌐 Ecosystem Website
Visit the [Jetson AGX Orin Capability Showcase](https://github.com/soccentric-jetson-oss/soccentric-jetson-oss) for an overview of all projects.
