#!/bin/bash
# utils.sh - Shared utilities for neuron build scripts

#==============================================================================
# GLOBAL STATE
#==============================================================================

VERBOSE=false
ARGS=()

#==============================================================================
# COLORS
#==============================================================================

# Only use colors if terminal supports them
if [[ -t 1 ]] && [[ -n "$TERM" ]] && [[ "$TERM" != "dumb" ]]; then
    COLOR_RESET='\033[0m'
    COLOR_RED='\033[0;31m'
    COLOR_GREEN='\033[0;32m'
    COLOR_YELLOW='\033[0;33m'
    COLOR_BLUE='\033[0;34m'
    COLOR_CYAN='\033[0;36m'
    COLOR_BOLD='\033[1m'
    COLOR_DIM='\033[2m'
else
    COLOR_RESET=""
    COLOR_RED=""
    COLOR_GREEN=""
    COLOR_YELLOW=""
    COLOR_BLUE=""
    COLOR_CYAN=""
    COLOR_BOLD=""
    COLOR_DIM=""
fi

#==============================================================================
# OUTPUT FUNCTIONS
#==============================================================================

# Header with decorative lines
header() {
    local line="━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
    printf "\n${COLOR_BOLD}${COLOR_BLUE}%s${COLOR_RESET}\n" "$line"
    printf "${COLOR_BOLD}${COLOR_BLUE}  %s${COLOR_RESET}\n" "$1"
    printf "${COLOR_BOLD}${COLOR_BLUE}%s${COLOR_RESET}\n" "$line"
}

# Step indicator: ▸ message
step() {
    printf "${COLOR_CYAN}▸${COLOR_RESET} %s\n" "$1"
}

# Success message: ✓ message
success() {
    printf "${COLOR_GREEN}✓${COLOR_RESET} %s\n" "$1"
}

# Warning message: ⚠ message
warn() {
    printf "${COLOR_YELLOW}⚠${COLOR_RESET} %s\n" "$1"
}

# Error message and exit: ✗ message
die() {
    printf "\n${COLOR_RED}✗ %s${COLOR_RESET}\n" "$1" >&2
    exit 1
}

#==============================================================================
# VERBOSE MODE
#==============================================================================

# Run command with optional verbose output
run() {
    if [[ "$VERBOSE" == true ]]; then
        printf "${COLOR_DIM}   \$ %s${COLOR_RESET}\n" "$*"
    fi
    "$@"
}

# Run command quietly (suppress output unless verbose)
run_quiet() {
    if [[ "$VERBOSE" == true ]]; then
        printf "${COLOR_DIM}   \$ %s${COLOR_RESET}\n" "$*"
        "$@"
    else
        "$@" > /dev/null 2>&1
    fi
}

#==============================================================================
# ARGUMENT PARSING HELPERS
#==============================================================================

# Parse common flags (call at start of each script)
# Handles --help and --verbose, stores remaining args in ARGS array
# Requires show_help() to be defined before calling
parse_common_flags() {
    ARGS=()
    for arg in "$@"; do
        case "$arg" in
            -h|--help)
                show_help
                exit 0
                ;;
            -v|--verbose)
                VERBOSE=true
                ;;
            *)
                ARGS+=("$arg")
                ;;
        esac
    done
}

# Standard help footer (common options)
help_common_options() {
    cat << 'EOF'

Common Options:
    -h, --help           Show this help message
    -v, --verbose        Show commands as they execute
EOF
}

#==============================================================================
# TIMING UTILITIES
#==============================================================================

# Format seconds as Xm Xs
format_duration() {
    local seconds=$1
    local mins=$((seconds / 60))
    local secs=$((seconds % 60))
    printf "%dm %02ds" "$mins" "$secs"
}

# Print elapsed time (call with start time)
print_elapsed() {
    local start_time=$1
    local end_time
    end_time=$(date +%s)
    local elapsed=$((end_time - start_time))
    printf "\n${COLOR_DIM}Completed in %s${COLOR_RESET}\n" "$(format_duration $elapsed)"
}
