#!/usr/bin/env bash

UNCRUSTIFY=prebuilt/linux-x86/bin/uncrustify
UNCRUSTIFY_CONFIG=uncrustify.cfg

if [ ! -f $UNCRUSTIFY ]; then
    echo "uncrustify not found, this should be run from the root of the project"
    exit 1
fi

find . -name "*.h" -o -name "*.cpp" -o -name "*.c" -o -name "*.hpp" | xargs $UNCRUSTIFY -c $UNCRUSTIFY_CONFIG --replace --no-backup
