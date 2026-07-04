set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
REPO_ROOT="$(cd "${SCRIPT_DIR}/.." && pwd)"

if [ ! -f "${REPO_ROOT}/config.env" ]; then
    echo "error: ${REPO_ROOT}/config.env not found" >&2
    exit 1
fi

set -a
source "${REPO_ROOT}/config.env"
set +a

# On the host, re-run the calling script inside the prebuilt dev image. Inside
# the VS Code dev container (or any container) /.dockerenv exists, so we run
# natively and skip this.
if [ ! -f /.dockerenv ] && [ -z "${WOMBAT_IN_CONTAINER:-}" ]; then
    : "${IMAGE:?set IMAGE in config.env}"
    if ! docker image inspect "${IMAGE}" >/dev/null 2>&1 && ! docker pull "${IMAGE}"; then
        echo "error: could not pull ${IMAGE}" >&2
        echo "log in once:  docker login git.robo4you.at   (read:package token from Forgejo -> Settings -> Applications)" >&2
        exit 1
    fi
    docker_tty=(-i)
    [ -t 0 ] && docker_tty=(-it)
    exec docker run --rm "${docker_tty[@]}" \
        -e WOMBAT_IN_CONTAINER=1 \
        -v "${REPO_ROOT}:/workspace" -w /workspace \
        -v "${HOME}/.ssh:/home/dev/.ssh:ro" \
        "${IMAGE}" bash "scripts/$(basename "${0}")" "$@"
fi

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
