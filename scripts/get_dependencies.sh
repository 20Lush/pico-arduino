#!/bin/bash -e

# Put links to any project dependencies you need in here.
# You should only be using release artifacts as dependency, we shouldn't be compiling other people's code../
# If the archive is not xz... well we might have to deal with that later.
# 
# Do .../releases/latest/download/<file>.tar.xz to always get the latest artifact without have to manually update the string here
declare -a dependency_links=(
    "https://github.com/20Lush/DevOps/releases/latest/download/clang-format.tar.xz"
)

# Make the dependencies directory if it doesn't exist...
if [ ! -d ./../.Dependencies ]; then
  echo "[INFO] Creating dependencies directory..."
  mkdir ./../.Dependencies
fi

# Go to the dependencies directory
cd ./../.Dependencies

# Loop through the dependency link array and extract their juicy file.
# stdout the archive so we dont uneccesarily write it to disk.
for i in "${dependency_links[@]}"
do
    echo "[INFO] Pulling dependency from: " "$i" 
    wget -qO- "$i" | tar Jxf -
    echo "[INFO] Done"
done

echo "[INFO] All dependencies pulled!"