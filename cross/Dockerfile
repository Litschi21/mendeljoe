# Build container for the Wombat API template.
#
# A single amd64 image carrying BOTH toolchains:
#   - native g++           -> ARCH=amd64 builds
#   - aarch64 cross g++    -> ARCH=arm64 builds (for the Wombat)
#
# The editor / clangd runs in this container natively (amd64), so IntelliSense
# stays fast; only the produced binary is arm64. The build target is selected at
# build time from config.env, not by rebuilding this image.
FROM debian:trixie

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
        build-essential \
        crossbuild-essential-arm64 \
        meson \
        ninja-build \
        cmake \
        pkgconf \
        clangd \
        git \
        openssh-client \
        ca-certificates \
        curl \
        gnupg \
    && rm -rf /var/lib/apt/lists/*

# aarch64 pkg-config wrapper: points pkgconf at the arm64 .pc files so the meson
# cross-file can resolve urc-wombat-api / urc-common for the arm64 target.
RUN printf '#!/bin/sh\nexport PKG_CONFIG_LIBDIR=/usr/lib/aarch64-linux-gnu/pkgconfig:/usr/share/pkgconfig\nexport PKG_CONFIG_SYSROOT_DIR=/\nexec pkgconf "$@"\n' \
        > /usr/local/bin/aarch64-linux-gnu-pkg-config \
    && chmod +x /usr/local/bin/aarch64-linux-gnu-pkg-config

# Add the URC apt repo and install native + cross build dependencies.
COPY cross/apt-setup.sh /tmp/apt-setup.sh
RUN chmod +x /tmp/apt-setup.sh && /tmp/apt-setup.sh && rm /tmp/apt-setup.sh

# Non-root user so files created in the bind-mounted workspace are owned by you,
# not root. VS Code Dev Containers remaps this UID to the host user automatically.
ARG USERNAME=dev
RUN useradd --create-home --shell /bin/bash "${USERNAME}"
USER ${USERNAME}

WORKDIR /workspace
