# Shared setup for all scripts. Source this; do not execute directly.
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

if [ ! -f "${REPO_ROOT}/config.env" ]; then
    echo "error: ${REPO_ROOT}/config.env not found" >&2
    exit 1
fi

# Sourced as shell, so config.env supports inline comments and references to
# earlier values (e.g. TARGET_DIR=/home/urc/${PROGRAM_NAME}/bin).
set -a
# shellcheck disable=SC1091
source "${REPO_ROOT}/config.env"
set +a

: "${ARCH:?set ARCH in config.env (arm64 or amd64)}"
: "${PROGRAM_NAME:?set PROGRAM_NAME in config.env}"

case "${ARCH}" in
    arm64 | amd64) ;;
    *)
        echo "error: ARCH must be 'arm64' or 'amd64', got '${ARCH}'" >&2
        exit 1
        ;;
esac

BUILD_DIR="${REPO_ROOT}/build-${ARCH}"
BIN_PATH="${BUILD_DIR}/${PROGRAM_NAME}"
