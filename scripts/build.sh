#!/usr/bin/env bash
source "$(dirname "$0")/_helper.sh"

if [ ! -d "${BUILD_DIR}" ]; then
  setup_args=()
  if [ "${ARCH}" = "arm64" ]; then
    setup_args+=(--cross-file "${REPO_ROOT}/cross/arm64-cross.ini")
  fi
  meson setup "${BUILD_DIR}" "${setup_args[@]}"
fi

meson compile -C "${BUILD_DIR}"

ln -sf "build-${ARCH}/compile_commands.json" "${REPO_ROOT}/compile_commands.json"

echo "Built ${BIN_PATH} (${ARCH})"
