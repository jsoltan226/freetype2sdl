#!/bin/sh
_start_time="$(date +%s.%N)"

error(){
    printf "\033[31;1mERROR\033[0;0m: $1\n"
    exit $2
}
FT2SDL_DIR="$(pwd)"
CGD_DIR="$1"
LIB_NAME='libcgdengine_user-input.a'
OK_MSG='\033[32;1mOK\033[0;0m\n'

[ -d "$1" ] || {
    echo "usage: ./compile-cgd-userinput-lib.sh <cgd dir> [-v|--verbose]" >&2
    exit 1
}

OUT_STREAM=/dev/null
ERR_STREAM=/dev/null
[ "$2" = '-v' ] || [ "$2" = '--verbose' ] && {
    VERBOSE=1
	OUT_STREAM=/dev/stdout
	ERR_STREAM=/dev/stderr
}

# Copy over header files
echo -n ">> Updating header files... "
[ $VERBOSE ] && echo

headers="$(find "$CGD_DIR/user-input/" -name "*.h" | xargs echo)"

rm -vfr   "$FT2SDL_DIR/include/cgd/user-input" >$OUT_STREAM 2>$ERR_STREAM
mkdir -vp "$FT2SDL_DIR/include/cgd/user-input" >$OUT_STREAM 2>$ERR_STREAM &&
cp -vf     $headers -t "$FT2SDL_DIR/include/cgd/user-input" >$OUT_STREAM 2>$ERR_STREAM || 
    error "Failed to copy over the required header files." 2
printf $OK_MSG

# Remove and recompile the needed obj files
echo -n ">> Recompiling object files...   "
[ $VERBOSE ] && echo
object_files="$(echo "$headers" | sed -e "s/user-input\//obj\//g" -e "s/\.h/.c.o/g")"
rm -vf $object_files >$OUT_STREAM
make compile -C "$CGD_DIR" -j$(nproc --all) >$OUT_STREAM 2>$ERR_STREAM
    #|| error "Compilation failed." 3

printf $OK_MSG

# Create the .a archive
echo -n ">> Archiving... "
[ $VERBOSE ] && printf "\nAR 	%-30s %-30s\n" "lib/$LIB_NAME" "<= $object_files"
ar $VERBOSE_FLAGS rcs "$FT2SDL_DIR/lib/$LIB_NAME" $object_files >$OUT_STREAM ||
    error "Failed to create the library archive." 4

printf $OK_MSG

_elapsed_time="$(echo "$(date +%s.%N) - $_start_time" | bc)"
echo ">> Done, took $(printf "%.4f %s" "$_elapsed_time" "seconds")"
exit 0
