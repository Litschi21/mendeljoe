#!/usr/bin/env bash
# Configure (if needed) and compile the project for the configured ARCH.
# shellcheck source=scripts/_common.sh
source "$(dirname "$0")/_common.sh"

if [ -d "${BUILD_DIR}" ]; then
    # Pick up a changed PROGRAM_NAME without a full reconfigure.
    meson configure "${BUILD_DIR}" "-Dprogram_name=${PROGRAM_NAME}" >/dev/null
else
    setup_args=("-Dprogram_name=${PROGRAM_NAME}")
    if [ "${ARCH}" = "arm64" ]; then
        setup_args+=(--cross-file "${REPO_ROOT}/cross/arm64-cross.ini")
    fi
    meson setup "${BUILD_DIR}" "${setup_args[@]}"
fi

meson compile -C "${BUILD_DIR}"

# Point clangd at the active build (relative target keeps the symlink portable).
ln -sf "build-${ARCH}/compile_commands.json" "${REPO_ROOT}/compile_commands.json"

echo "Built ${BIN_PATH} (${ARCH})"
