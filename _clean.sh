# _clean.sh
# Clean build artifacts.

find . -type f -name '*.o' -exec rm {} +
find . -type f -name '*.out' -exec rm {} +
find . -type f -name '*.dylib' -exec rm {} +
find . -type f -name '*.so' -exec rm {} +