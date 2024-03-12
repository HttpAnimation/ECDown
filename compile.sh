#!/bin/bash

# Create Builds folder if it doesn't exist
mkdir -p Builds

# Get current date and time
current_datetime=$(date +'%Y-%m-%d_%H-%M-%S')

# Create a folder with the current date and time
build_folder="Builds/$current_datetime"
mkdir -p "$build_folder"

# Compile c scripts into the build folder
# gcc FILE.c -o "$build_folder/COMPILENAME"
gcc ECDown.c -o "$build_folder/ECDown" `pkg-config --cflags --libs gtk+-3.0` -lcurl
gcc req.c -o "$build_folder/req"

# Copy files into the build folder
# cp FILE "$build_folder/FILE"
excluded_files=("README.md")
for file in *; do
    if [[ ! " ${excluded_files[@]} " =~ " $file " ]]; then
        mv "$file" "$build_folder"
    fi
done

# Create a Version.ini file with the current date and time
echo "$current_datetime" > "$build_folder/Version.ini"

# Create README file
readme_file="$build_folder/README.md"
echo "# B_ID: $current_datetime" > "$readme_file"
echo "Build created at: $current_datetime" >> "$readme_file"
echo "This folder contains builds for the server and client programs." >> "$readme_file"
echo "This is auto message and might work or might not." >> "$readme_file"
echo "## Programs" >> "$readme_file"
echo "1) **req** | Install needed things to compile." >> "$readme_file"

# Print the path to the build folder
echo "Builds created at: $build_folder"
cd $build_folder