#!/usr/bin/env bash

set -e

IMAGE_NAME="bolt"

function build-iso() {
  rm -rf build/iso_root
  mkdir -p build/iso_root

  make -C external/limine -j"$(nproc)"

  pushd build
    #ninja -C cmake
    ls ../
    cp -v cmake/kernel/kernel ../limine.cfg ../external/limine/limine-bios.sys ../external/limine/limine-bios-cd.bin ../external/limine/limine-uefi-cd.bin iso_root/

    mkdir -p iso_root/EFI/BOOT
    cp -v ../external/limine/BOOTX64.EFI iso_root/EFI/BOOT/
    cp -v ../external/limine/BOOTIA32.EFI iso_root/EFI/BOOT/

    xorriso -as mkisofs -b limine-bios-cd.bin \
      -no-emul-boot -boot-load-size 4 -boot-info-table \
      --efi-boot limine-uefi-cd.bin \
      -efi-boot-part --efi-boot-image --protective-msdos-label \
      iso_root -o "$IMAGE_NAME.iso"

    ../external/limine/limine bios-install "$IMAGE_NAME.iso"
    rm -rf iso_root
  popd
}

build-iso
