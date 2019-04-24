#create links to the two input files
ln src/InputFiles/$1 src/$1
ln src/InputFiles/$2 src/$2
ln src/InputFiles/$3 src/$3
#move to src directory
cd src
#(re)build the project and execute
g++ -g -o test-build *.cpp FileReader/*.cpp SuffixTree/*.cpp Alignment/Alignment.*
ddd -args test-build $1 $2 $3


#remove the links in src, they are no longer needed
rm $1 $2 $3
