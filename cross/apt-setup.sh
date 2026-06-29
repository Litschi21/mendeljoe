#!/usr/bin/env bash
# Configures the URC Forgejo apt repository and installs the Wombat API build
# dependencies for BOTH amd64 (native) and arm64 (cross) targets.
#
# Run inside the container build (see cross/Dockerfile). It is self-contained, so
# it can also be run on a fresh Debian trixie system to reproduce the toolchain.
set -euo pipefail

REPO_URL="https://git.robo4you.at/api/packages/unified-robot-control/debian"
KEYRING="/etc/apt/keyrings/forgejo-unified-robot-control.asc"

export DEBIAN_FRONTEND=noninteractive

apt-get update
apt-get install -y --no-install-recommends ca-certificates curl gnupg

install -d /etc/apt/keyrings
curl -fsSL "${REPO_URL}/repository.key" -o "${KEYRING}"

# Enable arm64 so the cross sysroot packages (libc6:arm64, the :arm64 -dev
# packages, ...) can be fetched from the multiarch-capable Debian mirrors.
dpkg --add-architecture arm64

echo "deb [arch=amd64,arm64 signed-by=${KEYRING}] ${REPO_URL} trixie main" \
    > /etc/apt/sources.list.d/forgejo.list

apt-get update

# Native amd64 build deps (ARCH=amd64). The -dev packages pull in most transitive
# deps (quill, magic_enum, tomlplusplus, libstdc++, ...). libprotobuf-dev is added
# explicitly: urc-wombat-api.pc lists protobuf under Requires.private, so pkg-config
# needs protobuf.pc even though protobuf is otherwise internal to the API.
apt-get install -y --no-install-recommends \
    liburc-wombat-api-dev \
    liburc-common-dev \
    libprotobuf-dev

# arm64 cross deps (ARCH=arm64). Multi-Arch: same makes them co-installable
# alongside the amd64 variants; headers are shared, only the arch-specific
# .so symlinks differ.
apt-get install -y --no-install-recommends \
    liburc-wombat-api-dev:arm64 \
    liburc-common-dev:arm64 \
    libprotobuf-dev:arm64

rm -rf /var/lib/apt/lists/*
