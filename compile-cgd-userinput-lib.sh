#!/bin/sh
error(){
    printf "\033[31;1mERROR\033[0;0m: $1\n"
    exit $2
}
FT2SDL_DIR="$(pwd)"
CGD_DIR="$1"
LIB_NAME='libcgdengine_user-input.a'
[ -d "$1" ] || {
    echo "usage: ./compile-cgd-userinput-lib.sh <cgd dir>" >&2
    exit 1
}

# Copy over header files
echo ">> Copying headers..."
headers="$(find "$CGD_DIR/user-input/" -name "*.h" | xargs echo)"

rm -rf "$FT2SDL_DIR/include/cgd/user-input"
mkdir -p "$FT2SDL_DIR/include/cgd/user-input" &&
cp -vf $headers -t "$FT2SDL_DIR/include/cgd/user-input" || 
    error "Failed to copy over the required header files." 2

# Remove and recompile the needed obj files
echo ">> Recompiling object files..."
object_files="$(echo "$headers" | sed -e "s/user-input\//obj\//g" -e "s/\.h/.o/g")"
rm -vf $object_files
make compile -C "$CGD_DIR" -j$(nproc --all) || error "Compilation failed." 3

# Create the .a archive
echo ">> Archiving"
printf "AR 	%-20s %-20s\n" "$LIB_NAME" "<= $object_files"
ar rvcs "$FT2SDL_DIR/lib/$LIB_NAME" $object_files ||
    error "Failed to create the library archive." 4

printf '\033[32;1mOK\033[0;0m\n'
exit 0
