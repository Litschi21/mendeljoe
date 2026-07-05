#!/usr/bin/env bash
set -euo pipefail

REPO_HOST="git.robo4you.at"
REPO_PATH="api/packages/unified-robot-control/debian"
KEYRING="/etc/apt/keyrings/forgejo-unified-robot-control.asc"
SOURCE_LIST="/etc/apt/sources.list.d/forgejo.list"

export DEBIAN_FRONTEND=noninteractive

TOKEN="$(cat /run/secrets/pkgtoken 2>/dev/null || true)"
auth=""
[ -n "${TOKEN}" ] && auth="oauth2:${TOKEN}@"

apt-get update
apt-get install -y --no-install-recommends ca-certificates curl gnupg

install -d /etc/apt/keyrings
curl -fsSL "https://${auth}${REPO_HOST}/${REPO_PATH}/repository.key" -o "${KEYRING}"

dpkg --add-architecture arm64
echo "deb [arch=amd64,arm64 signed-by=${KEYRING}] https://${auth}${REPO_HOST}/${REPO_PATH} trixie main" \
  >"${SOURCE_LIST}"

apt-get update

apt-get install -y --no-install-recommends \
  liburc-wombat-api-dev \
  liburc-common-dev \
  libprotobuf-dev \
  liburc-wombat-api-dev:arm64 \
  liburc-common-dev:arm64 \
  libprotobuf-dev:arm64

rm -f "${SOURCE_LIST}"
rm -rf /var/lib/apt/lists/*
