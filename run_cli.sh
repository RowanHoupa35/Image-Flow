#!/bin/bash

# ImageFlow CLI Launcher Script
# Automatically sources Intel oneAPI environment and runs the CLI

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Source Intel oneAPI environment if available
if [ -f /opt/intel/oneapi/setvars.sh ]; then
    source /opt/intel/oneapi/setvars.sh > /dev/null 2>&1
fi

# Navigate to build directory
cd "$SCRIPT_DIR/build"

# Execute CLI with all arguments passed through
exec ./imageflow_cli "$@"
