"""
Implementation of the iso_image rule.
"""

def _impl(ctx):
    """
    Create an ISO image from a list of files.
    """

    isorootfs = ctx.actions.declare_directory(ctx.attr.name + ".rootfs")

    ctx.actions.run_shell(
        inputs = ctx.files.srcs,
        outputs = [
            isorootfs,
        ],
        arguments = [isorootfs.path] +
                    [f.path for f in ctx.files.srcs],
        command = """
            ISOROOTFS_PATH=$1
            shift

            mkdir -p $ISOROOTFS_PATH/EFI/BOOT

            # Filter out .EFI files
            for file in $@; do
                if [[ $file == *.EFI ]]; then
                    cp $file $ISOROOTFS_PATH/EFI/BOOT/
                fi

                # limine.conf must be at the root
                if [[ $file == **/limine.conf ]]; then
                    cp $file $ISOROOTFS_PATH/
                fi

                # Copy everything else to the root
                if [[ $file != *.EFI && $file != **/limine.conf ]]; then
                    cp $file $ISOROOTFS_PATH/
                fi
            done
        """,
    )

    limine_tool = ctx.executable._limine_tool
    xorriso_tool = ctx.executable._xorriso_tool

    iso_file = ctx.actions.declare_file(ctx.attr.name + ".iso")

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

iso_image = rule(
    doc = "Create an ISO image from a list of files.",
    implementation = _impl,
    attrs = {
        "_limine_tool": attr.label(cfg = "exec", default = Label("@limine//:limine_deploy"), doc = "The limine tool to use to install limine on HDD images.", executable = True),
        "_xorriso_tool": attr.label(cfg = "exec", default = Label("@xorriso//:xorriso"), doc = "The xorriso tool to use to create ISO images.", executable = True),
        "srcs": attr.label_list(allow_files = True, mandatory = True, doc = "The files to include in the image."),
    },
)
