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
