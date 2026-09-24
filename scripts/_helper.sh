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

DEPLOY_DIR="${REPO_ROOT}/.ssh-deploy"
DEPLOY_KEY="${DEPLOY_DIR}/id_ed25519"

prepare_ssh() {
  if [ ! -f "${DEPLOY_KEY}" ]; then
    echo "error: no deploy key at .ssh-deploy/id_ed25519" >&2
    echo "generate one, then put .ssh-deploy/id_ed25519.pub into ~/.ssh/authorized_keys on the Wombat:" >&2
    echo "  ssh-keygen -t ed25519 -N \"\" -f .ssh-deploy/id_ed25519" >&2
    exit 1
  fi

  staged_key="${HOME}/.ssh/wombat-deploy"
  mkdir -p "${HOME}/.ssh"
  chmod 700 "${HOME}/.ssh"
  cp "${DEPLOY_KEY}" "${staged_key}"
  chmod 600 "${staged_key}"

  SSH_BASE_OPTS="-i ${staged_key} -o IdentitiesOnly=yes"
  SSH_BASE_OPTS="${SSH_BASE_OPTS} -o UserKnownHostsFile=${DEPLOY_DIR}/known_hosts"
  SSH_BASE_OPTS="${SSH_BASE_OPTS} -o StrictHostKeyChecking=accept-new"
}
