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
        clang-format \
        git \
        openssh-client \
        ca-certificates \
        curl \
        gnupg \
    && rm -rf /var/lib/apt/lists/*

RUN printf '#!/bin/sh\nexport PKG_CONFIG_LIBDIR=/usr/lib/aarch64-linux-gnu/pkgconfig:/usr/share/pkgconfig\nexport PKG_CONFIG_SYSROOT_DIR=/\nexec pkgconf "$@"\n' \
        > /usr/local/bin/aarch64-linux-gnu-pkg-config \
    && chmod +x /usr/local/bin/aarch64-linux-gnu-pkg-config

COPY cross/apt-setup.sh /tmp/apt-setup.sh
RUN chmod +x /tmp/apt-setup.sh && /tmp/apt-setup.sh && rm /tmp/apt-setup.sh

ARG USERNAME=dev
RUN useradd --create-home --shell /bin/bash "${USERNAME}"
USER ${USERNAME}

WORKDIR /workspace
