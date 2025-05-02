"""
Bazel transition for setting the platform.
"""

def _platforms_transition_impl():
    """
    Transition implementation for setting the platform.
    """

    # Effectively the same as calling `bazel build --platforms=...`
    return {"//command_line_option:platforms": "@local_config_platform//:host"}

platforms_transition = transition(
    implementation = _platforms_transition_impl,
    inputs = [],
    outputs = ["//command_line_option:platforms"],
)
