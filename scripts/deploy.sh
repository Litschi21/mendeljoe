#!/usr/bin/env bash
source "$(dirname "$0")/_helper.sh"

"${SCRIPT_DIR}/format.sh"
"${SCRIPT_DIR}/build.sh"
"${SCRIPT_DIR}/upload.sh"
"${SCRIPT_DIR}/run.sh"
