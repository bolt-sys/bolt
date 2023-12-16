FROM mcr.microsoft.com/devcontainers/base:ubuntu

ARG BAZELISK_VERSION="v1.19.0"
ARG BAZELISK_BINARY_URL="https://github.com/bazelbuild/bazelisk/releases/download/${BAZELISK_VERSION}/bazelisk-linux-amd64"

ARG BUILDTOOLS_VERSION="v6.4.0"
ARG BUILDTOOLS_BUILDIFIER_URL="https://github.com/bazelbuild/buildtools/releases/download/${BUILDTOOLS_VERSION}/buildifier-linux-amd64"
ARG BUILDTOOLS_BUILDOZER_URL="https://github.com/bazelbuild/buildtools/releases/download/${BUILDTOOLS_VERSION}/buildozer-linux-amd64"
ARG BUILDTOOLS_UNUSED_DEPS_URL="https://github.com/bazelbuild/buildtools/releases/download/${BUILDTOOLS_VERSION}/unused_deps-linux-amd64"

RUN apt-get update && apt-get upgrade -y && apt-get dist-upgrade -y

#
# Bazel
#
RUN wget ${BAZELISK_BINARY_URL} -O /bin/bazelisk && \
    chmod +x /bin/bazelisk && \
    ln -sf /bin/bazelisk /bin/bazel

#
# BuildTools
#
RUN wget ${BUILDTOOLS_BUILDIFIER_URL} -O /bin/buildifier && \
    wget ${BUILDTOOLS_BUILDOZER_URL} -O /bin/buildozer && \
    wget ${BUILDTOOLS_UNUSED_DEPS_URL} -O /bin/unused_deps && \
    chmod +x /bin/buildifier /bin/buildozer /bin/unused_deps

#
# Packages
#
RUN apt-get install -y \
    build-essential \
    bash-completion \
    gdb

#
# Bash Completion
#
RUN curl -fsSL https://raw.githubusercontent.com/bazelbuild/bazel/7.0.0/scripts/bazel-complete-header.bash >> /etc/bash_completion.d/bazel && \
    curl -fsSL https://raw.githubusercontent.com/bazelbuild/bazel/7.0.0/scripts/bazel-complete-template.bash >> /etc/bash_completion.d/bazel && \
    bazel help completion >> /etc/bash_completion.d/bazel

#
# Cleanup
#
RUN apt-get autoremove -y && \
    apt-get clean -y && \
    rm -rf /var/lib/apt/lists/* && \
    rm -rf /root/.cache/
