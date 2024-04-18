#!/bin/bash -e

# Default directory to build into
BUILD_DIRECTORY="$PWD/../build"
BUILD_TYPE="DEBUG"

# Default directory of cmake boilerplate scripts
CMAKE_BOILERPLATE="$PWD/../.CMake-Boilerplate"

# Default directory to install package to
INSTALL_DIRECTORY="${BUILD_DIRECTORY}/install"

# Build with unit tests
TESTING=false

# Handle command line arguments
while getopts 'b:th' opt; do
    case "$opt" in
        b)
            BUILD_TYPE="$OPTARG"
            ;;
        t)
            TESTING=true
            ;;
        h)
            echo "====== HELP DIALOGUE ===================================================================="
            echo "  Usage: $(basename $0) [-h help] [-b built type(DEBUG or RELEASE)] [-t enable testing]  "
            echo "========================================================================================="
            exit 1
            ;;
        :)
            echo -e "Option flag required when passing in an argument. Stop."
            exit 1
            ;;
    esac
done

shift "$(($OPTIND -1))"

echo "====== BUILD DIALOGUE ====================================================================================="
echo "      !!! Building in $BUILD_DIRECTORY !!!                                                                 "
echo "      !!! Configured for $BUILD_TYPE   !!!                                                                 "
echo "==========================================================================================================="

# make our build directory and move into it
mkdir -p ${BUILD_DIRECTORY}
cd ${BUILD_DIRECTORY}

# cmake command line calls that define some build/install environment variables like install directory and if we want unit testing
cmake -DCMAKE_BOILERPLATE_PATH=${CMAKE_BOILERPLATES} -DCMAKE_INSTALL_PREFIX=${INSTALL_DIRECTORY} -DENABLE_TESTING=${TESTING} -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..
cmake --build . -j 8
cmake --install .