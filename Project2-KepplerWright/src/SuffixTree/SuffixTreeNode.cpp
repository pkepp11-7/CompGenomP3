//Patrick Keppler and Connor Wright, 2019
#include "SuffixTreeNode.h"


//********constructors*************

//default constructor for root node
SuffixTreeNode::SuffixTreeNode()
{
  id = 0;
  depth = 0;
  parent = this;
  child = nullptr;
  sibling = nullptr;
  sl = this;
  label = nullptr;
}


//constructor with arguments. Requires an id, label string, and a pointer to the parent.
//Optional child and sibling pointers, default to null if not provided. Depth in tree can be passed in, or computed by default.
SuffixTreeNode::SuffixTreeNode(const int & newId, char * labelStr, SuffixTreeNode * parentPtr, SuffixTreeNode * childPtr , SuffixTreeNode * siblingPtr, const int & treeDepth)
{
  id = newId;
  strcpy(label, labelStr);
  parent = parentPtr;
  child = childPtr;
  sibling = siblingPtr;
  sl = nullptr;

  //use asserts to
  assert(parent != nullptr);

  //if treeDepth is 0, compute depth based on label length
  if(!treeDepth)
  {
    depth = parent->getDepth() + strlen(label);
  }
  //else use input value
  else
  {
    depth = treeDepth;
  }

}




//*******public methods*************

//returns the depth of a node
unsigned int SuffixTreeNode::getDepth() const
{
  return depth;
}

//returns a node's id
unsigned int SuffixTreeNode::getId() const
{
  return id;
}

//return a pointer to this node's parent
SuffixTreeNode * SuffixTreeNode::getParent()
{
  return parent;
}

SuffixTreeNode * SuffixTreeNode::getChild()
{
  return child;
}

SuffixTreeNode * SuffixTreeNode::getSibling()
{
  return sibling;
}

SuffixTreeNode * SuffixTreeNode::getSL()
{
  return sl;
}

char * SuffixTreeNode::getLabel() {
  return label;
}


//set the suffix link of the node to the input parameter.
void SuffixTreeNode::setSL(SuffixTreeNode * v)
{
  sl = v;
}
