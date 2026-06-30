#!/usr/bin/env bash
set -euo pipefail

REPO_URL="https://git.robo4you.at/api/packages/unified-robot-control/debian"
KEYRING="/etc/apt/keyrings/forgejo-unified-robot-control.asc"

export DEBIAN_FRONTEND=noninteractive

apt-get update
apt-get install -y --no-install-recommends ca-certificates curl gnupg

install -d /etc/apt/keyrings
curl -fsSL "${REPO_URL}/repository.key" -o "${KEYRING}"

dpkg --add-architecture arm64

echo "deb [arch=amd64,arm64 signed-by=${KEYRING}] ${REPO_URL} trixie main" \
    > /etc/apt/sources.list.d/forgejo.list

apt-get update

apt-get install -y --no-install-recommends \
    liburc-wombat-api-dev \
    liburc-common-dev \
    libprotobuf-dev

apt-get install -y --no-install-recommends \
    liburc-wombat-api-dev:arm64 \
    liburc-common-dev:arm64 \
    libprotobuf-dev:arm64

rm -rf /var/lib/apt/lists/*
