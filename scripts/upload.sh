#!/usr/bin/env bash
source "$(dirname "$0")/_helper.sh"

: "${TARGET_HOST:?set TARGET_HOST in config.env}"
: "${TARGET_DIR:?set TARGET_DIR in config.env}"

if [ "${ARCH}" != "arm64" ]; then
  echo "warning: ARCH=${ARCH}, but the Wombat is arm64 — this binary will not run there." >&2
fi

if [ ! -x "${BIN_PATH}" ]; then
  echo "error: ${BIN_PATH} not found — run scripts/build.sh first." >&2
  exit 1
fi

prepare_ssh

ssh ${SSH_BASE_OPTS} "${TARGET_HOST}" "mkdir -p '${TARGET_DIR}'"
scp ${SSH_BASE_OPTS} "${BIN_PATH}" "${TARGET_HOST}:${TARGET_DIR}/"
echo "Uploaded ${PROGRAM_NAME} to ${TARGET_HOST}:${TARGET_DIR}/"
