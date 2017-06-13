#!/bin/bash
for name in $@; do
    perl -pe 's/\@param ([a-zA-Z0-9_]+) - /\@param \1 /g' < $name > $name.tmp
    mv $name.tmp $name
done
