#!/usr/bin/env bash
source "$(dirname "$0")/_helper.sh"

rm -rf "${REPO_ROOT}"/build-* "${REPO_ROOT}/compile_commands.json"
echo "Cleaned build directories."
