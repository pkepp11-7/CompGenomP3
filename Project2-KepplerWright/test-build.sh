#create links to the two input files
cp src/InputFiles/$1 src/$1
cp src/InputFiles/$2 src/$2
#move to src directory
cd src
#(re)build the project and execute
g++ -g -o test-build -std=c++11 *.cpp FileReader/*.cpp SuffixTree/*.cpp
ddd -args test-build $1 $2 $3


#remove the links in src, they are no longer needed
rm $1 $2
