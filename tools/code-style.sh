#!/bin/bash
set -e

###########################
# Check tools

# If any C files are formatted
if [[ "$*" == *.c* ]]; then
    if ! command -v clang-format >/dev/null 2>&1; then
        echo "$0: requires clang-format." >&2
        exit 127
    fi
fi

if [[ "$#" == 0 ]]; then
    echo "usage: $0 <source.c> ..."
    exit 1
fi

script_dir="$(dirname "$0")"

formatter_blacklist=(
)

for source_file in "$@"; do
    # Check if file is actually one which we want to check
    rel_path="$(realpath --relative-to="$script_dir/.." "$source_file")"
    format_item=true
    for blacklist_item in "${formatter_blacklist[@]}"; do
        if [[ "$rel_path" == "$blacklist_item"* ]]; then
            format_item=false
            break
        fi
    done
    if [[ "$format_item" == false ]]; then
        echo -e "\tIGN\t\t$source_file"
        continue
    fi

    if [[ "$source_file" == *.c* || "$source_file" == *.h* ]]; then
        echo -e "\tCLANG-FORMAT\t$source_file"
        clang-format -i "$source_file"
    else
        echo -e "\tIGN\t\t$source_file"
	fi
done
