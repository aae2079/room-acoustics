#!/bin/bash

# Resolve the path to this script, even if sourced from elsewhere
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export PROJECT_DIR

export LD_LIBRARY_PATH="$PROJECT_DIR/lib:$LD_LIBRARY_PATH"

echo "PROJECT_DIR set to $PROJECT_DIR"




