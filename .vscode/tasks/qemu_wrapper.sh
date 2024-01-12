#!/usr/bin/env bash

set -euo pipefail

# This script is a wrapper for qemu-system-x86_64 that sets up the
# environment for the VM to run in. It is called by the VSCode task
# "launch_qemu-x64" (Ctrl+Shift+B).

# This is required for the VSCode debugger to work.
echo "Starting QEMU VM..."

QEMU_ARGS=(
    -s     "-S" # Start the VM in a paused state (Wait for debugger to connect)
    -m     "2G"
    -smp   "cores=2,threads=1,sockets=1" # 2 cores
    -drive "file=bazel-bin/image/cd.iso,format=raw,if=ide,index=0,media=cdrom" # Boot from the OS ISO

    -serial "mon:stdio" # Redirect serial output to the terminal
)

CUSTOM_ARGS=$(cat .vscode/qemu_args.txt)

qemu-system-x86_64 "${QEMU_ARGS[@]}" $CUSTOM_ARGS

echo "QEMU VM stopped. ($?)"
