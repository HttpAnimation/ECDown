#!/bin/bash

# Create Builds folder if it doesn't exist
mkdir -p Builds

# Get current date and time
current_datetime=$(date +'%Y-%m-%d_%H-%M-%S')

# Create a folder with the current date and time
build_folder="Builds/$current_datetime"
mkdir -p "$build_folder"

# Compile c scripts into the build folder
gcc Server-EXP.c -o "$build_folder/webserver" -lmicrohttpd

# Copy files into the build folder
cp Server-EXP.c "$build_folder/Server-EXP.c"
cp README.md "$build_folder/README.md"
cp LICENSE "$build_folder/LICENSE"
cp Server-EXP.jcl "$build_folder/Server-EXP.jcl"
cp goup.sh "$build_folder/goup.sh"

# Create a Version.ini file with the current date and time
echo "$current_datetime" > "$build_folder/Version.ini"

# Create README file
readme_file="$build_folder/README.txt"
echo "Build created at: $current_datetime" > "$readme_file"
echo "This folder contains builds for the server and client programs." >> "$readme_file"
echo "This is auto message and might work or might not."

# Print the path to the build folder
echo "Builds created at: $build_folder"
cd $build_folder