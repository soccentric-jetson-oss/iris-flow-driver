# Iris Flow Driver — Jetson AGX Orin Camera & ISP Kernel Module

The Iris Flow Driver is a Linux kernel module that provides direct access to the NVIDIA Jetson AGX Orin's camera and Image Signal Processor (ISP) hardware. It supports up to 6 simultaneous camera streams over the 16-lane MIPI CSI-2 interface, handling raw Bayer sensor data up to 24 megapixels. The driver exposes a character device interface with ioctls for stream start/stop, frame capture, and ISP parameter tuning including brightness, contrast, saturation, white balance, and HDR mode. A V4L2 integration path enables compatibility with standard video4linux applications. The driver uses devres-managed resource allocation for leak-free operation, regmap for register access, and supports concurrent access from multiple processes. A userspace C library wraps the ioctl interface, and a test suite validates all operations including error paths and edge cases.

## Features

- Multi-stream
- camera
- capture
- (up
- to
- 6
- streams)

## Quick Start

### Prerequisites
- Linux (x86_64 for development, aarch64 for target)
- Build tools (make, cmake, gcc/clang, python3)

### Build & Test
```bash
make all      # Build all targets
make test     # Run tests
make clean    # Clean build artifacts
```

## Architecture

```
Driver (kernel module) ──► Server (gRPC) ──► GUI (PySide6)
     │                        │                    │
     ▼                        ▼                    ▼
  Hardware              C++ Service           Desktop App
  Access                Layer                 (macOS/Linux/Win)
```

## Repository Structure

| Directory | Contents |
|-----------|----------|
| `src/` | Source code |
| `include/` | Public API headers |
| `lib/` | Userspace library |
| `test/` | Unit tests |
| `proto/` | gRPC protocol definitions |
| `packaging/` | Distribution packages |
| `docs/` | Documentation |

## Project Status

**Version:** 0.1.0 — Initial release
**License:** 16-lane MIPI CSI-2 interface support
**Audit Score:** 90/100

## 🌐 Ecosystem

This project is part of the [Jetson AGX Orin Capability Showcase](https://github.com/soccentric-jetson-oss/soccentric-jetson-oss) — five open-source projects demonstrating full exploitation of NVIDIA's flagship edge AI platform.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines. All contributions welcome!

## License

16-lane MIPI CSI-2 interface support. See [LICENSE](LICENSE) for details.
