#!/usr/bin/env bash

set -e

IMAGE_NAME="bolt"

function build-iso() {
  rm -rf build/iso_root
  mkdir -p build/iso_root

  make -C External/limine -j"$(nproc)"

  pushd build
    #ninja -C cmake
    ls ../
    cp -v cmake/kernel/Kernel ../limine.cfg ../External/limine/limine-bios.sys ../External/limine/limine-bios-cd.bin ../External/limine/limine-uefi-cd.bin iso_root/

    mkdir -p iso_root/EFI/BOOT
    cp -v ../External/limine/BOOTX64.EFI iso_root/EFI/BOOT/
    cp -v ../External/limine/BOOTIA32.EFI iso_root/EFI/BOOT/

    xorriso -as mkisofs -b limine-bios-cd.bin \
      -no-emul-boot -boot-load-size 4 -boot-info-table \
      --efi-boot limine-uefi-cd.bin \
      -efi-boot-part --efi-boot-image --protective-msdos-label \
      iso_root -o "$IMAGE_NAME.iso"

    ../External/limine/limine bios-install "$IMAGE_NAME.iso"
    rm -rf iso_root
  popd
}

build-iso
