# Bolt

An operating completely written from scratch in C, C++, Assembly and various other languages.
depending on the needs of the project.

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

- [git](https://git-scm.com/downloads)
- [QEMU](https://www.qemu.org/download/)
- [Any Linux Distribution](https://www.linux.org/pages/download/)
- [Bazel](https://bazel.build/)
- [xorriso](https://www.gnu.org/software/xorriso/) - This is a temporary requirement and will soon cease to be required.

### How to Build

Building over the project has been gotten simpler and easier with the introduction of Bazel. \
You can build the project by running the following command:

```sh
# For an CD image
bazel build //image:cd

# The CD image will be located at bazel-bin/image/cd.iso
```

### How to run / test

Running is done through QEMU, a virtual machine. \
to run the project, run the following command:

```sh
qemu-system-x86_64 -cdrom bazel-bin/image/cd.iso -no-reboot -no-shutdown -s -S -serial stdio -d int -m 2G -vga std
```

Testing however is done on the host machine, and can be done by running the following command:

```sh
bazel test //...
```

## Contributing

Contributions are welcome. Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on how to contribute.

## Screenshots

    We aren't there yet

## Contributing

Contributions are welcome. Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on how to contribute.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details \
TL;DR: Do whatever you want with it. Just don't blame me if it breaks your computer but credit me if you use it in your
project.