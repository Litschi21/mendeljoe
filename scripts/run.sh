#!/usr/bin/env bash
source "$(dirname "$0")/_helper.sh"

: "${TARGET_HOST:?set TARGET_HOST in config.env}"
: "${TARGET_DIR:?set TARGET_DIR in config.env}"

prepare_ssh

remote_bin="${TARGET_DIR}/${PROGRAM_NAME}"
echo "Running ${remote_bin} on ${TARGET_HOST}..."
ssh ${SSH_BASE_OPTS} -t "${TARGET_HOST}" "${remote_bin}"
