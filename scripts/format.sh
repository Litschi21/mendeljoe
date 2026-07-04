#!/usr/bin/env bash
source "$(dirname "$0")/_helper.sh"

if ! command -v clang-format >/dev/null; then
    echo "error: clang-format not found — run this inside the dev container." >&2
    exit 1
fi

mapfile -t files < <(find "${REPO_ROOT}/src" "${REPO_ROOT}/include" -type f \( -name '*.cpp' -o -name '*.hpp' \) 2>/dev/null)
if [ "${#files[@]}" -eq 0 ]; then
    echo "No source files to format."
    exit 0
fi

clang-format -i "${files[@]}"
echo "Formatted ${#files[@]} files."
