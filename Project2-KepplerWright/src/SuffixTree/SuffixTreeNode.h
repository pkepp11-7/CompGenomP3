//Patrick Keppler and Connor Wright, 2019
#ifndef SUFFIXTREENODE_H
#define SUFFIXTREENODE_H

#include <cstring>
#include <cassert>

//Nodes for use by a Suffix Tree
class SuffixTreeNode {


private:
//***********data*****************
unsigned int id, depth;
SuffixTreeNode * child, * sibling, * parent, * sl;
char * label;


//******private methods***********

//TODO: Write any internal helper methods here.
//      Will be called by public methods and constructors.

public:

//********constructors*************

//default constructor for root node
SuffixTreeNode();

//constructor with arguments. Requires an id, label string, and a pointer to the parent.
//Optional child and sibling pointers, default to null if not provided. Depth in tree can be passed in, or computed by default.
SuffixTreeNode(const int & newId, char * labelStr, SuffixTreeNode * parentPtr, SuffixTreeNode * childPtr = nullptr, SuffixTreeNode * siblingPtr = nullptr, const int & treeDepth = 0);


//*******public methods*************

//accessors
unsigned int getDepth() const;
unsigned int getId() const;
SuffixTreeNode * getParent();
SuffixTreeNode * getChild();
SuffixTreeNode * getSibling();
SuffixTreeNode * getSL();
char * getLabel();

//mutators
void setSL(SuffixTreeNode * v);

//TODO: write additional accessors/API methods as needed.

};

#endif
