load(":platform.bzl", "platforms_transition")

def _host_binary_impl(ctx):
    """
    Transforms an host compiled binary into a shell script that can be run on the host.
    """

    tool = ctx.attr.tool[0]
    shell_script = "./%s" % tool.files_to_run.executable.short_path

    ctx.actions.write(
        output = ctx.outputs.executable,
        content = shell_script,
        is_executable = True,
    )

    return [
        DefaultInfo(
            executable = ctx.outputs.executable,
            runfiles = tool.default_runfiles,
        ),
    ]

"""
Creates a binary that can be run on the host using the host toolchain.
"""
host_binary = rule(
    implementation=_host_binary_impl,
    attrs={
        "_allowlist_function_transition": attr.label(
            default = "@bazel_tools//tools/allowlists/function_transition_allowlist",
        ),
        "tool": attr.label(cfg = platforms_transition, mandatory = True, executable = True, allow_single_file = True),
    },
    executable = True,
)