There are two build scripts that can be used to see the final submission:
-clean-build.sh
-pre-build.sh

clean-build.sh will do a fresh compile of the code
pre-build.sh will use an existing build instead of compiling

both have the same parameters for running:
(clean/pre)-build.sh <input genome file> <input read file> <input alphabet file>

This solution was tested on Ubuntu 14.04 with compiler g++ version 8.