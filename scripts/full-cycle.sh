#!/usr/bin/env bash
# Build -> upload -> run, in one go.
# shellcheck source=scripts/_common.sh
source "$(dirname "$0")/_common.sh"

"${SCRIPT_DIR}/build.sh"
"${SCRIPT_DIR}/upload.sh"
"${SCRIPT_DIR}/run.sh"
