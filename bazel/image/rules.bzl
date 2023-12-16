"""
*TODO*: make this fully hermetic. it still requires system packages right now.

### Packages
  * ubuntu: `sudo apt install xorriso`
  * macos: `brew install xorriso`
"""

def _image_impl(ctx):
    srcs = ctx.files.srcs
    type = ctx.attr.type

    limine_tool = ctx.executable._limine_tool
    xorriso_tool = ctx.executable._xorriso_tool

    filename = ctx.attr.name + "." + type

    # TODO: add more complicated logic, like different folders.

    if type == "iso":
        # Create an isorootfs
        isorootfs = ctx.actions.declare_directory(filename + ".rootfs")
        ctx.actions.run_shell(
            inputs = srcs,
            outputs = [
                isorootfs,
            ],
            arguments = [isorootfs.path] +
                        [f.path for f in srcs],
            command = """
                ISOROOTFS_PATH=$1
                shift
                echo $@

                mkdir -p $ISOROOTFS_PATH/EFI/BOOT

                # Filter out .EFI files
                for file in $@; do
                    if [[ $file == *.EFI ]]; then
                        cp $file $ISOROOTFS_PATH/EFI/BOOT/
                    fi

                    # limine.cfg must be at the root
                    if [[ $file == **/limine.cfg ]]; then
                        cp $file $ISOROOTFS_PATH/
                    fi

                    # Copy everything else to the root
                    if [[ $file != *.EFI && $file != **/limine.cfg ]]; then
                        cp $file $ISOROOTFS_PATH/
                    fi
                done
            """,
        )

        iso_file = ctx.actions.declare_file(filename)

        # Create the actual iso
        ctx.actions.run_shell(
            inputs = [isorootfs, limine_tool, xorriso_tool],
            outputs = [iso_file],
            arguments = [
                limine_tool.path,
                xorriso_tool.path,
                isorootfs.path,
                iso_file.path,
            ],
            command = """
                LIMINE_TOOL=$1
                XORRISO_TOOL=$2
                ROOTFS_PATH=$3
                ISO_PATH=$4

                echo $LIMINE_TOOL
                echo $XORRISO_TOOL
                echo $ROOTFS_PATH
                echo $ISO_PATH
                
                set -euxo pipefail

                # xorriso, for some reason re-uses the symlinks from bazel, thus we need to copy them to a temp folder.
                mkdir -p .tmp
                cp -Lr $ROOTFS_PATH .tmp/rootfs

                $XORRISO_TOOL -as mkisofs -b limine-bios-cd.bin              \
                    -no-emul-boot -boot-load-size 4 -boot-info-table         \
                    --efi-boot limine-uefi-cd.bin                            \
                    -efi-boot-part --efi-boot-image --protective-msdos-label \
                    .tmp/rootfs -o "$ISO_PATH"

                rm -rf .tmp

                $LIMINE_TOOL bios-install "$ISO_PATH"
            """,
        )

        return [
            DefaultInfo(
                files = depset([
                    iso_file,
                ]),
            ),
        ]
    elif type == "hdd":
        fail("hdd images are not implemented yet")

    return []

image = rule(
    implementation = _image_impl,
    attrs = {
        "_limine_tool": attr.label(cfg = "exec", default = Label("@limine//:limine_deploy"), doc = "The limine tool to use to install limine on HDD images.", executable = True),
        "_xorriso_tool": attr.label(cfg = "exec", default = Label("@xorriso//:xorriso"), doc = "The xorriso tool to use to create ISO images.", executable = True),
        "srcs": attr.label_list(allow_files = True, mandatory = True, doc = "The files to include in the image."),
        "type": attr.string(mandatory = True, default = "iso", values = ["iso", "hdd"], doc = "The type of image to create."),
    },
    doc = "Create an Image from a list of files.",
)
