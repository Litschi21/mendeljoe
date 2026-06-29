#!/usr/bin/env bash
# Run the uploaded binary on the Wombat over ssh.
# shellcheck source=scripts/_common.sh
source "$(dirname "$0")/_common.sh"

: "${TARGET_HOST:?set TARGET_HOST in config.env}"
: "${TARGET_DIR:?set TARGET_DIR in config.env}"

remote_bin="${TARGET_DIR}/${PROGRAM_NAME}"
echo "Running ${remote_bin} on ${TARGET_HOST}..."
# SSH_OPTS is intentionally unquoted so multi-word options (e.g. -i <key>) split.
# shellcheck disable=SC2086
ssh ${SSH_OPTS:-} -t "${TARGET_HOST}" "${remote_bin}"
