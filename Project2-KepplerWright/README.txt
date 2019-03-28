Instructions for running the Suffix Tree program:

There are two scripts that can run this program, the clean-build, and the pre-build.
Both have the same execution format:

./<script name> <fasta file name> <alphabet file name> <testMode>

The two files must always be provided for execution, and testMode will default to 0 if not provided.

the other testMode codes are:
1 - run DFS
3 - display BWT
5 - display other data, but not BWT
7 - display BWT, then other data
8 - display time only
9 - DFS + time
11 - BWT + time
13 - other data + time
15 - BWT + other data + time
17 - Longest Repeating String (LR)
19 - BWT + LR
21 - other data + LR
23 - BWT + other data + LR
25 - LR + time
27 - BWT + LR + time
29 - other data + LR + time
31 - BWT + other data + LR + time
32 - with pause (for space complexity analysis)
