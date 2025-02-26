#!/bin/sh

filesdir=$1
searchstr=$2

if [ -z "$filesdir" ] || [ -z "$searchstr" ]
then
    echo "Both files directory and search string are required."
    exit 1
fi

if test ! -d "$filesdir"
then
    echo "Files directory does not exist."
    exit 1
fi

matchlines=$(grep -ro $searchstr $filesdir | wc -l)
numfiles=$(grep -rl $searchstr $filesdir | wc -l)

echo "The number of files are $numfiles and the number of matching lines are $matchlines"
