#!/usr/bin/env bash
# Remove all build directories and the clangd symlink.
# shellcheck source=scripts/_common.sh
source "$(dirname "$0")/_common.sh"

rm -rf "${REPO_ROOT}"/build-* "${REPO_ROOT}/compile_commands.json"
echo "Cleaned build directories."
