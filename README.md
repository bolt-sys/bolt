# Bolt

A NT Like Operating System.

**NOTE: This project is still in early development and is not ready for use.**

This is a hobby project of mine to create a simple operating system from scratch. \
The goal is to create a simple operating system that can be used to run simple programs.

## Table of Contents

- [Bolt](#Bolt)
  - [Table of Contents](#table-of-contents)
  - [Getting Started](#getting-started)
    - [Prerequisites](#prerequisites)
    - [How to Build](#how-to-build)
    - [How to run / test](#how-to-run--test)
  - [Contributing](#contributing)
  - [LICENSE](#license)

## Getting Started

a lot of documentation is missing, but the build process is pretty simple:

### Prerequisites

- [Git](https://git-scm.com/downloads)
- [QEMU](https://www.qemu.org/download/)
- [Any Linux Distribution](https://www.linux.org/pages/download/)
- [Any C compiler](https://gcc.gnu.org/)
- [NASM](https://www.nasm.us/)
- [xorriso](https://www.gnu.org/software/xorriso/)

### How to Build

Building is straight forward. Just run the following commands:

```sh
~$ mkdir -p build/cmake && cd build/cmake
~$ cmake ../.. -GNinja
~$ ninja
~$ ./build-iso.sh
```

### How to run / test

Running / testing is only supported using QEMU and only under Linux.

You can run the following command to run the OS in QEMU:
```
TODO
```


## Contributing

Contributions are welcome. Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on how to contribute.

## Screenshots

    We aren't there yet

## Contributing

Contributions are welcome. Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on how to contribute.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details \
TL;DR: Do whatever you want with it. Just don't blame me if it breaks your computer but credit me if you use it in your project.
