#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/utils.sh" || exit 1

show_help() {
    cat << 'EOF'
Usage: build.sh [OPTIONS] [CONFIG]

Build the neuron library using CMake.

Arguments:
    CONFIG               Build configuration: release, debug, or test (default: release)

Options:
    -r, --remove         Remove previous build directory before building
EOF
    help_common_options
}

# Defaults
CONFIG="release"
REMOVE_PREV=false

parse_common_flags "$@"
set -- "${ARGS[@]}"

for arg in "$@"; do
    case "$arg" in
        -r|--remove)
            REMOVE_PREV=true
            ;;
        release|debug|test)
            CONFIG="$arg"
            ;;
        *)
            die "Unknown option: $arg"
            ;;
    esac
done

# Validate config
if [[ "$CONFIG" != "debug" ]] && [[ "$CONFIG" != "release" ]] && [[ "$CONFIG" != "test" ]]; then
    die "Invalid build configuration: $CONFIG (must be release, debug, or test)"
fi

START_TIME=$(date +%s)

header "Building Neuron ($CONFIG)"

# Setup build directory
TARGET_DIR="$PWD/target/$CONFIG"

if [[ "$REMOVE_PREV" == true ]] && [[ -d "$TARGET_DIR" ]]; then
    step "Removing previous build"
    rm -rf "$TARGET_DIR"
fi

mkdir -p "$TARGET_DIR"
cd "$TARGET_DIR" || die "Failed to change to build directory"

# Configure CMake flags
CMAKE_FLAGS=""
if [[ "$CONFIG" == "test" ]]; then
    CMAKE_FLAGS="-DNEO_BUILD_TESTS=ON"
fi

# Generate build files
step "Generating build files"
if ! run cmake $CMAKE_FLAGS ../../; then
    die "Failed to generate build files"
fi

# Compile
step "Compiling"
if ! run make; then
    die "Failed to compile"
fi

success "Build complete"
print_elapsed "$START_TIME"
