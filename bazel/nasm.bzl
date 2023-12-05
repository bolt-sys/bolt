def _nasm_compile_impl(ctx):
    output = ctx.outputs.obj_file
    src = ctx.file.src
    nasm = ctx.executable.nasm
    asmopts = ctx.attr.asmopts

    ctx.actions.run(
        outputs = [output],
        inputs = [src],
        executable = nasm,
        arguments = asmopts + [src.path, "-o", output.path],
        mnemonic = "NASM",
    )

    return DefaultInfo(
        files = depset([output]),
    )

nasm_compile = rule(
    implementation = _nasm_compile_impl,
    attrs = {
        "src": attr.label(allow_single_file = True),
        "nasm": attr.label(default = Label("@nasm"), cfg = "exec", executable = True, allow_files = True),
        "asmopts": attr.string_list(default = []),
    },
    outputs = {"obj_file": "nasm.%{name}"},
)

def nasm_cc_binary(name, srcs, deps = [], asmopts = "", **kwargs):
    """
    Macro to add nasm assembly compilation to a cc_binary rule.
    """
    asm_srcs = [src for src in srcs if src.endswith(".asm")]
    cc_srcs = [src for src in srcs if not src.endswith(".asm")]

    asm_objs = []
    for src in asm_srcs:
        obj = name + "_" + src[:-4] + ".o"
        nasm_compile(
            name = obj,
            src = src,
            asmopts = asmopts,
        )
        asm_objs.append(obj)

    native.cc_binary(
        name = name,
        srcs = cc_srcs + asm_objs,
        deps = deps,
        **kwargs
    )
