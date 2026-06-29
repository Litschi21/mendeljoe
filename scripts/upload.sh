#!/usr/bin/env bash
# Copy the built binary to the Wombat via scp.
# shellcheck source=scripts/_common.sh
source "$(dirname "$0")/_common.sh"

: "${TARGET_HOST:?set TARGET_HOST in config.env}"
: "${TARGET_DIR:?set TARGET_DIR in config.env}"

if [ "${ARCH}" != "arm64" ]; then
    echo "warning: ARCH=${ARCH}, but the Wombat is arm64 — this binary will not run there." >&2
fi

if [ ! -x "${BIN_PATH}" ]; then
    echo "error: ${BIN_PATH} not found — run scripts/build.sh first." >&2
    exit 1
fi

# SSH_OPTS is intentionally unquoted so multi-word options (e.g. -i <key>) split.
# shellcheck disable=SC2086
ssh ${SSH_OPTS:-} "${TARGET_HOST}" "mkdir -p '${TARGET_DIR}'"
# shellcheck disable=SC2086
scp ${SSH_OPTS:-} "${BIN_PATH}" "${TARGET_HOST}:${TARGET_DIR}/"
echo "Uploaded ${PROGRAM_NAME} to ${TARGET_HOST}:${TARGET_DIR}/"
