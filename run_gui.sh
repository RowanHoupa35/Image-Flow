#!/bin/bash
# Simple launcher for ImageFlow GUI with oneAPI/SYCL support

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Source oneAPI environment (suppress output)
if [ -f /opt/intel/oneapi/setvars.sh ]; then
    source /opt/intel/oneapi/setvars.sh > /dev/null 2>&1
else
    echo "Warning: oneAPI environment not found. GPU acceleration may not work."
fi

# Run the GUI
cd "$SCRIPT_DIR/build"
exec ./gui/ImageFlowGUI "$@"
