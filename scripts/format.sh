#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/utils.sh" || exit 1

show_help() {
    cat << 'EOF'
Usage: format.sh [OPTIONS]

Format C++ source code using clang-format.

Formats files in:
    - include/
    - src/
    - tests/
EOF
    help_common_options
}

parse_common_flags "$@"

format_directory() {
    local dir=$1
    local label=$2

    step "Formatting $label"
    if ! find "$dir" -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i -style=file; then
        die "Failed to format $label"
    fi
}

header "Formatting C++ Code"

format_directory "include/" "headers"
format_directory "src/" "source"
format_directory "tests/" "tests"

success "All files formatted"
