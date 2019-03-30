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
  label = "";
}


//constructor with arguments. Requires an id, label string, and a pointer to the parent.
//Optional child and sibling pointers, default to null if not provided. Depth in tree can be passed in, or computed by default.
SuffixTreeNode::SuffixTreeNode(const int & newId, 
                              size_t labelIndex,
                              size_t labelLength, 
                              SuffixTreeNode * parentPtr, 
                              SuffixTreeNode * childPtr, 
                              SuffixTreeNode * siblingPtr, 
                              const int & treeDepth)
{
  id = newId;
  setLabel(labelIndex, labelLenght);
  parent = parentPtr;
  childrenPointer = childPtr;
  sibling = siblingPtr;
  sl = nullptr;

  //will initalize to zero if no parent provided
  if(parentPtr != nullptr)
  {
    calculateDepth();
  }
  else
  {
    depth = 0;
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

SuffixTreeNode * SuffixTreeNode::getChild(char firstLabel, const string & inputString)
{
  int compare = 0;
  SuffixTreeNode * indexer = childrenPointer;
  if(indexer == nullptr)
  {
    return nullptr;
  }

  while(true)
  {
    compare = Alphabet::compare(firstLabel, indexer->getLabel(inputString)[0]);
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

string SuffixTreeNode::getLabel(const string & inputString)const {

  assert(labelIndex + labelLenght <= inputString.size());
  return inputString.substr(labelIndex, labelLenght);
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
  depth = parent->getDepth() + label.size();
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

void SuffixTreeNode::setLabel(size_t _labelIndex, size_t _labelLength)
{
  labelIndex = _labelIndex;
  labelLenght = _labelLength;
}

//mutators

//set the suffix link of the node to the input parameter.
void SuffixTreeNode::setSL(SuffixTreeNode * v)
{
  sl = v;
}

SuffixTreeNode * SuffixTreeNode::addChild(SuffixTreeNode * newChild, const string& inputString)
{
  //check the child is valid
  assert(newChild->getLabel(inputString) != "");

  //check if the parent has a child already
  if(childrenPointer == nullptr)
  {
    //easy insertion
    childrenPointer = newChild;
    newChild->setSibling(nullptr);
    newChild->setParent(this);
    newChild->calculateDepth();
  }
  else
  {
    addSibling(newChild, childrenPointer, inputString);
  }

  return newChild;
}

//inserts a sibling in lexographic order
//called within parent node
SuffixTreeNode * SuffixTreeNode::addSibling(SuffixTreeNode* newSibling,
                                            SuffixTreeNode* firstSibling,
                                            const string& inputString)
{
  SuffixTreeNode * currentSibling = firstSibling;
  SuffixTreeNode * previousSibling = nullptr;

  bool inserted = false;
  int ordering = 0;

  newSibling->setParent(this);
  newSibling->calculateDepth();

  while(!inserted)
  {
    ordering = Alphabet::compare(newSibling->getLabel(inputString)[0]
                                , currentSibling->getLabel(inputString)[0]);
    //we should not be inserting the same characters
    assert(ordering != 0 || newSibling->getLabel(inputString)[0] != '$');
    
    if(ordering < 0)
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

//removes a child and returns it
SuffixTreeNode * SuffixTreeNode::removeChild(SuffixTreeNode * childToRemove, 
                                             const string& inputString)
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

SuffixTreeNode * SuffixTreeNode::addInternalNode(char firstLabel, 
                                                 int indexBreak, 
                                                 int id, 
                                                 const string& inputString)
{
  SuffixTreeNode * childAlongEdge = getChild(firstLabel, inputString);
  assert(childAlongEdge != nullptr);

  //remove the child of the edge we want to break
  childAlongEdge = removeChild(childAlongEdge, inputString);
  assert(childAlongEdge != nullptr);

  //edge is now broken
  childAlongEdge->setLabel(0, indexBreak);
  
  //create new internal node
  SuffixTreeNode * newInternalNode = new SuffixTreeNode(id, 0, indexBreak, nullptr);
  
  //new internal node added
  addChild(newInternalNode, inputString);
  //old child added
  newInternalNode->addChild(childAlongEdge, inputString);

  return newInternalNode;
}