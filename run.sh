#!/usr/bin/env bash

# -e [exit on nonzero status]
# -u [unreferenced variables crash]
# -o pipefail [specify -option 'pipefail' causing no masking]
set -eu -o pipefail

cmake --build build -j $(nproc)

text_to_encode="$1"
output_filename="$2"
output_image_size="$3"

./build/qr-scan "$text_to_encode" "$output_filename" "$output_image_size"
