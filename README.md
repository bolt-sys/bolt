# Bolt

An operating system completely written from scratch in C, C++, Assembly and various other languages, depending on the needs of the project.

**NOTE: This project is still in early development and is not ready for use.**

This is a hobby project of mine to create a simple operating system from scratch. The goal is to create a simple operating system that can be used to run simple programs.

## Table of Contents

- [Bolt](#Bolt)
    - [Table of Contents](#table-of-contents)
    - [Project Overview](#project-overview)
    - [Current State](#current-state)
    - [Goals](#goals)
    - [Getting Started](#getting-started)
        - [Prerequisites](#prerequisites)
        - [How to Build](#how-to-build)
        - [How to run / test](#how-to-run--test)
    - [Contributing](#contributing)
    - [Screenshots](#screenshots)
    - [Community](#community)
    - [LICENSE](#license)

## Project Overview

Bolt is an operating system project that aims to create a simple and efficient OS from scratch. It is written in C, C++, Assembly, and other languages as needed. The project is a hobby endeavor with the goal of learning and experimenting with OS development concepts.

## Current State

The project is still in its early stages of development. Basic functionalities such as bootstrapping, memory management, and simple kernel operations are being implemented. The project is not yet ready for production use and is primarily intended for educational purposes.

## Goals

The primary goal of Bolt is to create a simple operating system that can run basic programs. The project aims to provide a learning platform for OS development and to experiment with various OS concepts and techniques. Future goals include implementing more advanced features such as file systems, networking, and user interfaces.

## Getting Started

The following sections provide detailed instructions for setting up the development environment, building, and running the project.

### Prerequisites

- [git](https://git-scm.com/downloads)
- [QEMU](https://www.qemu.org/download/)
- [Any Linux Distribution](https://www.linux.org/pages/download/)
- [Bazel](https://bazel.build/)
- [xorriso](https://www.gnu.org/software/xorriso/) - This is a temporary requirement and will soon cease to be required.

### How to Build

Building the project has been simplified with the introduction of Bazel. You can build the project by running the following command:

```sh
# For a CD image
bazel build //image:cd

# The CD image will be located at bazel-bin/image/cd.iso
```

### How to run / test

Running the project is done through QEMU, a virtual machine. To run the project, use the following command:

```sh
qemu-system-x86_64 -cdrom bazel-bin/image/cd.iso -no-reboot -no-shutdown -s -S -serial stdio -d int -m 2G -vga std
```

Testing is done on the host machine and can be performed by running the following command:

```sh
bazel test //...
```

## Contributing

Contributions are welcome. Please read [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines on how to contribute to the project.

## Screenshots

We aren't there yet.

## Community

Join our community to stay updated and get involved:


## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details. \
TL;DR: Do whatever you want with it. Just don't blame me if it breaks your computer but credit me if you use it in your project.
