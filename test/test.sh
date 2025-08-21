set -Eeuxo pipefail

CFLAGS="-Wall -Wextra -Werror"
LFLAGS="-framework Cocoa"
FILES="test.c ../native_menu_bar.c"

clang $CFLAGS $LFLAGS $FILES -std=c99 -ObjC 
clang $CFLAGS $LFLAGS $FILES -std=c17 -ObjC
clang $CFLAGS $LFLAGS $FILES -std=c++98 -ObjC++
clang $CFLAGS $LFLAGS $FILES -std=c++14 -ObjC++
