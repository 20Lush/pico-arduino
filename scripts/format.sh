#!/bin/bash -e

VERIFY_MODE=false

# Lets see if the dependency folder exists with the format rules file before starting.
if ! test -f ./../.Dependencies/.clang-format; then
  echo "[ERROR] .clang-format not found!! have you ran get_dependencies.sh yet?"
  exit 1
fi

# Move to the base repo directory and copy the rule file from dependencies.
cd ..
cp ./.Dependencies/.clang-format ./

# Handle command line arguments
while getopts 'v:h' opt; do
    case "$opt" in
        v)
            $VERIFY_MODE=true
            ;;
        h)
            echo "====== HELP DIALOGUE ===================================================================="
            echo "  Usage: $(basename $0) [-h help] [-v verify formatting in project]  "
            echo "========================================================================================="
            exit 1
            ;;
        :)
            echo -e "[ERROR] Option flag required when passing in an argument. Stop."
            exit 1
            ;;
    esac
done

# TBD verify mode to check if commits follow my rules
if $VERIFY_MODE; then

    echo "[INFO] Verifying formatting in source/headers... "
    exit 0
fi

echo "[INFO] Formatting code! This could take a while..."

# pipe in the source files we found and run em thru the formatter
find ./ -iname *.hpp -o -iname *.cpp  -o -iname *.h | xargs clang-format -style=file:./.clang-format -i

echo "[INFO] Cleaning up."

#Clean up
rm ./.clang-format

echo "[INFO] ...Done!"