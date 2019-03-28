#create links to the two input files
ln $1 src/InputFiles/$1
ln $2 src/InputFiles/$2
#move to src directory
cd src
#(re)build the project and execute
g++ -g -ggdb -o test-build -std=c++11 *.cpp FileReader/*.cpp SuffixTree/*.cpp
ddd -args test-build $1 $2 $3

#remove the links in src, they are no longer needed
rm $1 $2
