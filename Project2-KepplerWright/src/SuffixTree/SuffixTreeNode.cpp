//Patrick Keppler and Connor Wright, 2019
#include "SuffixTreeNode.h"


//********constructors*************

//default constructor for root node
SuffixTreeNode::SuffixTreeNode()
{
  id = 0;
  depth = 0;
  parent = this;
  childrenPointer = nullptr;
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
  childrenPointer = childPtr;
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

SuffixTreeNode * SuffixTreeNode::getChildPointer()
{
  return childrenPointer;
}

SuffixTreeNode * SuffixTreeNode::getChild(char firstLabel)
{
  int compare = 0;
  SuffixTreeNode * indexer = childrenPointer;
  assert(indexer != nullptr);

  while(true)
  {
    compare = Alphabet::compare(firstLabel, indexer->getLabel()[0]);
    if(compare == 0)
    {
      //found child
      return indexer;
    }
    if(indexer->getSibling() == nullptr)
    {
      //unable to find child
      return nullptr;
    }
    //increment search
    indexer = indexer->getSibling();
  }
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

void SuffixTreeNode::setSibling(SuffixTreeNode * newSibling)
{
  sibling = newSibling;
}

void SuffixTreeNode::setParent(SuffixTreeNode * newParent)
{
  parent = newParent;
}

void SuffixTreeNode::setChildrenPointer(SuffixTreeNode * newChildPointer)
{
  childrenPointer = newChildPointer;
}

void SuffixTreeNode::calculateDepth()
{
  assert(parent != nullptr);
  depth = parent->getDepth() + strlen(label);
  //propagate changes throgh tree
  if(sibling != nullptr)
  {
    sibling->calculateDepth();
  }
  if(childrenPointer != nullptr)
  {
    childrenPointer->calculateDepth();
  }
}

void SuffixTreeNode::setDepth(int newDepth)
{
  depth = newDepth;
}

void SuffixTreeNode::setLabel(char * newLabel)
{
  label = newLabel;
}

//mutators

//set the suffix link of the node to the input parameter.
void SuffixTreeNode::setSL(SuffixTreeNode * v)
{
  sl = v;
}

SuffixTreeNode * SuffixTreeNode::addChild(SuffixTreeNode * newChild)
{
  //check the child is valid
  assert(newChild->getLabel() != nullptr);

  //check if the parent has a child already
  if(childrenPointer != nullptr)
  {
    //easy insertion
    childrenPointer = newChild;
    newChild->setSibling(nullptr);
    newChild->setParent(this);
    newChild->calculateDepth();
  }
  else
  {
    addSibling(newChild, childrenPointer);
  }

  return newChild;
}

//inserts a sibling in lexographic order
//called within parent node
SuffixTreeNode * SuffixTreeNode::addSibling(SuffixTreeNode * newSibling,
                                            SuffixTreeNode * firstSibling)
{
  SuffixTreeNode * currentSibling = firstSibling;
  SuffixTreeNode * previousSibling = nullptr;

  bool inserted = false;
  int ordering = 0;

  newSibling->setParent(this);
  newSibling->calculateDepth();

  while(!inserted)
  {
    ordering = Alphabet::compare(newSibling->getLabel()[0], currentSibling->getLabel()[0]);
    //we should not be inserting the same characters
    assert(ordering != 0);
    
    if(ordering > 0)
    {
      //we insert before
      newSibling->setSibling(currentSibling);
      
      if(currentSibling == firstSibling)
      {
        //first in the list
        this->setChildrenPointer(newSibling);
        inserted = true;
      }
      else
      {
        //inserting in middle or end
        previousSibling->setSibling(newSibling);
        inserted = true;
      }
    }
    else
    {
      //we keep looking
      if(currentSibling->getSibling() == nullptr)
      {
        //insert at the end
        currentSibling->setSibling(newSibling);
        newSibling->setSibling(nullptr);
        inserted = true;
      }
      else
      {
        //increment for search
        previousSibling = currentSibling;
        currentSibling = currentSibling->getSibling();
      } 
    }
  }

  return newSibling;
}

char * SuffixTreeNode::copyNewLabel(string * stringLabel)
{
  const char * constCstrLabel = stringLabel->c_str();
  char * cstrLabel = new char[stringLabel->length() + 1];
  strcpy(cstrLabel, constCstrLabel);

  return cstrLabel;
}

//removes a child and returns it
SuffixTreeNode * SuffixTreeNode::removeChild(SuffixTreeNode * childToRemove)
{
  SuffixTreeNode * currentSibling = childrenPointer; 
  SuffixTreeNode * previousSibling = nullptr;

  while(true)
  {
    if(childToRemove == currentSibling)
    {
      //found child
      currentSibling->setParent(nullptr);
      currentSibling->setDepth(-1);

      if(childToRemove == childrenPointer)
      {
        //removal from begining of list
        childrenPointer = currentSibling->getSibling();
        currentSibling->setSibling(nullptr);
        return currentSibling;
      }
      else
      {
        //removal from middle or end
        previousSibling->setSibling(currentSibling->getSibling());
        currentSibling->setSibling(nullptr);
        return currentSibling;
      }
    }
    else
    {
      //we continue searching

      if(currentSibling->getSibling() == nullptr)
      {
        //couldnt find child
        return nullptr;
      }
      else
      {
        previousSibling = currentSibling;
        currentSibling = currentSibling->getSibling();
      }
    }
  }
}


SuffixTreeNode * SuffixTreeNode::addInternalNode(char firstLabel, int indexBreak)
{
  SuffixTreeNode * childAlongEdge = getChild(firstLabel);
  assert(childAlongEdge != nullptr);

  //remove the child of the edge we want to break
  childAlongEdge = removeChild(childAlongEdge);
  assert(childAlongEdge != nullptr);

  //convert label to string for easy conversion
  string * oldLabelString = new string(childAlongEdge->getLabel());
  string * newInternalNodeLabelString = new string(oldLabelString->substr(0, indexBreak));
  string * childAlongEdgeLabelString = new string(indexBreak, oldLabelString->length());

  char * newInternalNodeLabel = copyNewLabel(newInternalNodeLabelString);
  char * childAlongEdgeLabel = copyNewLabel(childAlongEdgeLabelString); 
  
  //free memory
  delete oldLabelString;
  delete newInternalNodeLabelString;
  delete childAlongEdgeLabelString;

  //edge is now broken
  childAlongEdge->setLabel(childAlongEdgeLabel);
  
  //create new internal node
  //TODO: set id to something meaningful
  SuffixTreeNode * newInternalNode = new SuffixTreeNode(0, newInternalNodeLabel, nullptr);
  
  //new internal node added
  addChild(newInternalNode);
  //old child added
  newInternalNode->addChild(childAlongEdge);


  return newInternalNode;
}