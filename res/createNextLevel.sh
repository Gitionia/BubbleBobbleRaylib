#!/usr/bin/env bash

## generated using chatgpt

# copies the level template as the first new level that does not exist
# after the current level (passed as first argument)

if [[ -z "$1" ]]; then
    echo "Expected open file"
    exit 1
fi


input="$1"

# Extract directory, filename, and extension
dir="$(dirname "$input")"
file="$(basename "$input")"
echo "Creating new level after ${file}"

# Split into prefix, number, and extension
if [[ "$file" =~ ^(.*?)([0-9]{3})(\.[^.]+)$ ]]; then
    prefix="${BASH_REMATCH[1]}"
    num="${BASH_REMATCH[2]}"
    ext="${BASH_REMATCH[3]}"
else
    echo "Invalid filename format"
    exit 1
fi

# Convert number to integer (avoid leading zero issues)
start=$((10#$num))

# Loop from next number upward
for ((i = start + 1; i <= 999; i++)); do
    printf -v next "%03d" "$i"
    candidate="${dir}/${prefix}${next}${ext}"
    
    if [[ ! -e "$candidate" ]]; then
        cp 'LevelTemplate.json' "$candidate"
        echo "Created File $candidate"
        exit 0
    fi
done

echo "No free slot found"
exit 1