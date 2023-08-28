# Bolt

An experimental Linux Distribution.

## Getting Started

a lot of documentation is missing, but the build process is pretty simple:

### Prerequisites
* A working Linux system (tested on Arch Linux)
* A working assembler (tested with nasm)
* You need `libisoburn` for xorriso
* Qemu for testing and running
* Ninja for building
### Installing
1. Clone the repository
2. Download the submodules `git submodule update --init --recursive`
3. Create the buid directory `mkdir -p build/cmake`
### Building
1. Compile the bootloader with
```bash
   mkdir -p build/cmake && cd build/cmake
    cmake ../.. -GNinja
    ninja
```

### Running

    TODO

## Contributing

    TODO

## Screenshots

    We aren't there yet

## License

```
MIT License

Copyright (c) 2023 Robin Alexander Plate
Copyright (c) 2023 Emil Glenn Karpawitz

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.```