#!/bin/bash

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$SCRIPT_DIR/utils.sh" || exit 1

show_help() {
    cat << 'EOF'
Usage: test.sh [OPTIONS]

Build and run neuron tests.

Options:
    -b, --build          Rebuild tests from scratch before running
    -o, --output-failure Rerun failed tests with output visibility
    -p, --plugin-support Enable NEO_PLUGIN_SUPPORT for atomic parameter testing
EOF
    help_common_options
}

# Defaults
BUILD_TESTS=false
OUTPUT_FAILURE=false
PLUGIN_SUPPORT=false

parse_common_flags "$@"
set -- "${ARGS[@]}"

for arg in "$@"; do
    case "$arg" in
        -b|--build)
            BUILD_TESTS=true
            ;;
        -o|--output-failure)
            OUTPUT_FAILURE=true
            ;;
        -p|--plugin-support)
            PLUGIN_SUPPORT=true
            ;;
        *)
            die "Unknown option: $arg"
            ;;
    esac
done

TARGET_DIR="$PWD/target/test"

if [[ "$BUILD_TESTS" == true ]]; then
    START_TIME=$(date +%s)
    header "Building Tests"

    if [[ -d "$TARGET_DIR" ]]; then
        step "Removing previous build"
        rm -rf "$TARGET_DIR"
    fi

    mkdir -p "$TARGET_DIR"
    cd "$TARGET_DIR" || die "Failed to change to build directory"

    # Configure CMake flags
    CMAKE_FLAGS="-DCMAKE_BUILD_TYPE=Release -DNEO_BUILD_TESTS=ON"
    if [[ "$PLUGIN_SUPPORT" == true ]]; then
        CMAKE_FLAGS="$CMAKE_FLAGS -DNEO_PLUGIN_SUPPORT=ON"
    fi

    step "Generating build files"
    if ! run cmake $CMAKE_FLAGS ../../; then
        die "Failed to generate build files"
    fi

    step "Compiling"
    if ! run make; then
        die "Failed to compile"
    fi

    success "Build complete"
    print_elapsed "$START_TIME"
else
    cd "$TARGET_DIR" || die "Test build directory not found. Run with -b to build first."
fi

header "Running Tests"

# Build ctest command
CTEST_ARGS=()
if [[ "$OUTPUT_FAILURE" == true ]]; then
    CTEST_ARGS+=("--rerun-failed" "--output-on-failure")
fi

step "Executing tests"
if ! run ctest "${CTEST_ARGS[@]}"; then
    die "Tests failed"
fi

success "All tests passed"
