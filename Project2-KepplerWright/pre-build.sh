#create links to the two input files
ln $1 src/$1
ln $2 src/$2
#move to src directory
cd src
#run the pre-built executable with the necessary parameters
./output-build $1 $2 $3
#remove the links in src, they are no longer needed
rm $1 $2
