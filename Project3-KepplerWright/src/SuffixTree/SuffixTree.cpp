//Patrick Keppler and Connor Wright, 2019
#include "SuffixTree.h"



//******private methods***********

//findPath navigates the (sub)tree from the starting node parameter. Returns the created leaf node that was inserted
//called by basicInsert or slInsert
SuffixTreeNode * SuffixTree::findPath(SuffixTreeNode * start, unsigned int suffix)
{

  SuffixTreeNode * child = nullptr;
  SuffixTreeNode * current = start;
  //size_t suffixLen = strlen(suffix);
  size_t labelIndex = 0;
  size_t suffixIndex = 0;
  unsigned int id = lastInserted->getId() + 1;

  while(suffixIndex < fullString->length() - suffix)
  {
    child = current->getChild((*fullString)[suffix + suffixIndex], fullString);

    if(child == nullptr)
    {
      //could not find child add a new branch
      //char * newLabel = new char[suffixLen - suffixIndex + 1];
      //newLabel = strcpy(newLabel, &suffix[suffixIndex]);
      Label newLabel = { suffixIndex + suffix, fullString->length() };
      SuffixTreeNode * newLeaf = new SuffixTreeNode(id, newLabel, nullptr);
      current->addChild(newLeaf, fullString);

      return newLeaf;
    }
    else
    {
      //found child need to walk down
      labelIndex = 0;
      int comparison = 0;
      Label childLabel = child->getLabel();
      //string childString = fullString->substr(childLabel.startIndex, childLabel.endIndex - childLabel.startIndex);

      //while(labelIndex < childString.length() && suffixIndex < fullString->length() - suffix)
      while(labelIndex < (childLabel.endIndex - childLabel.startIndex))
      {
        //comparison = Alphabet::compare((*fullString)[suffix + suffixIndex], childString[labelIndex]);
        comparison = Alphabet::compare((*fullString)[suffix + suffixIndex], (*fullString)[childLabel.startIndex + labelIndex]);

        if(comparison != 0)
        {
          //path no longer matches

          //add new internal node
          SuffixTreeNode * newInternalNode =
            current->addInternalNode((*fullString)[childLabel.startIndex], fullString, labelIndex, ++lastInternalId);
           //current->addInternalNode(childString[0], fullString, labelIndex, ++lastInternalId);

          //add new leaf to the end of new internal node
          //char * newLabel = new char[suffixLen - suffixIndex + 1];
          //newLabel = strcpy(newLabel, &suffix[suffixIndex]);
          Label newLabel = {suffix + suffixIndex, fullString->length() };
          SuffixTreeNode * newLeafNode = new SuffixTreeNode(id, newLabel, nullptr);

          newInternalNode->addChild(newLeafNode, fullString);

          return newLeafNode;
        }

        labelIndex++;
        suffixIndex++;
      }

      if(suffixIndex > fullString->length() - suffix)
      {
        //we have reached the end of the suffix
        //add a new internal node and leaf with label $

        SuffixTreeNode * newInternalNode =
          current->addInternalNode((*fullString)[childLabel.startIndex], fullString, labelIndex, ++lastInternalId);
          //current->addInternalNode(childString[0], fullString, labelIndex, ++lastInternalId);

        Label endLabel = {fullString->length() -1, fullString->length()};

        SuffixTreeNode * newLeafNode = new SuffixTreeNode(id, endLabel, nullptr);
        newInternalNode->addChild(newLeafNode, fullString);

        return newLeafNode;
      }
      current = child;
    }
  }
}

//slInsert handles the 4 cases for inserting a suffix using suffix links. Ultimately calls findPath.
SuffixTreeNode * SuffixTree::slInsert(SuffixTreeNode * last, unsigned int suffix)
{
  SuffixTreeNode * u, * v, * uPrime, * vPrime;
  Label beta;
  //assert that the parameter node exists
  assert(last != nullptr);
  //assert that the parent to the last inserted node exists
  assert(last->getParent() != nullptr);

  u = last->getParent();

  //case I: suffix link is known
  if(u->getSL())
  {
    //case A: u is not the root
    if(u->getId() != 0)
    {
      v = u->getSL();
      return findPath(v, suffix + v->getDepth());
    }

    //case B: u is the root
    else {
      return findPath(u, suffix);
    }
  }
  //case II: suffix link is not known
  else {
    //assert that u's parent exists
    assert(u->getParent() != nullptr);

    //save u', beta
    uPrime = u->getParent();
    beta = u->getLabel();

    //case A: u' is not the root
    if(uPrime->getId() != 0)
    {
      //save vPrime
      vPrime = uPrime->getSL();
      //assert the suffix link from u' to v' exists
      assert(vPrime != nullptr);

      //use nodeHop to get v
      v = nodeHop(vPrime, beta);
      //assert nodeHop was successful
      assert(v != nullptr);
      //set suffix link of u to v
      u->setSL(v);
      return findPath(v,  suffix + v->getDepth());
    }
    //case B: u' is root
    else {
      //node hop from root to get v
      if(beta.endIndex - beta.startIndex > 1)
      {
        beta.startIndex++;
        v = nodeHop(uPrime, beta);
      }
      else
      {
        v = root;
      }
      //assert nodeHop was successful
      assert (v != nullptr);
      //set suffix link of u to v
      u->setSL(v);
      return findPath(v,  suffix + v->getDepth());
    }
  }
  //default: return nullptr;
  return nullptr;
}

//helper function for the node hop operation. Called by slInsert.
//Either locates the correct node (v) that already exists, or creates it.
//Returns the node v
SuffixTreeNode * SuffixTree::nodeHop(SuffixTreeNode * start, const Label & beta)
{
  size_t betaIndex = 0;
  unsigned int startDepth = start->getDepth();
  //string betaString = fullString->substr(beta.startIndex, beta.endIndex - beta.startIndex);
  SuffixTreeNode * current = start;
  SuffixTreeNode * child = nullptr;

  while(true)
  {
    //child = current->getChild(betaString[betaIndex], fullString);
    child = current->getChild((*fullString)[betaIndex + beta.startIndex], fullString);
    assert(child != nullptr);

    //if(betaString.length() + startDepth < child->getDepth())
    if((beta.endIndex - beta.startIndex) + startDepth < child->getDepth())
    {
      //we overshot and need to add an internal node
      SuffixTreeNode * newInternalNode
      = current->addInternalNode((*fullString)[child->getLabel().startIndex], fullString,
                                   (beta.endIndex-beta.startIndex) + startDepth - current->getDepth(),
                                   ++lastInternalId);
      /*
        = current->addInternalNode((*fullString)[child->getLabel().startIndex], fullString,
                                   betaString.length() + startDepth - current->getDepth(),
                                   ++lastInternalId);
      */
      return newInternalNode;
    }
    //else if(betaString.length() + startDepth == child->getDepth())
    else if((beta.endIndex - beta.startIndex) + startDepth == child->getDepth())
    {
      //we have found our lost child V!
      return child;
    }
    else
    {
      //we need to keep go deeper
      betaIndex = child->getDepth() - startDepth;
      current = child;
    }
  }
}


void SuffixTree::DFS(SuffixTreeNode * currentNode, int &nextIndex)
{
  //need to replace this with a global x
  int x = 0;

  if(currentNode != nullptr)
  {
    //base case
    if(currentNode == nullptr) {return; }

    //if it is a leaf
    if(currentNode->getId() <= lastInserted->getId() && currentNode->getId() != 0)
    {
      STData::incrementLeafNodes();
      STData::pushBwt(currentNode->getId());

      LeafArray[nextIndex] = currentNode->getId();
      if(currentNode->getDepth() >= x)
      {
        currentNode->set_start_leaf_index(nextIndex);
        currentNode->set_end_leaf_index(nextIndex);
      }
      nextIndex++;
      return;
    }

    //if it is an internal node
    STData::incrementInternalNodes();
    STData::findLongestRepeat(currentNode);

    //recurse through all children
    SuffixTreeNode* temp = currentNode->getChildPointer();
    while(temp != nullptr)
    {
      DFS(temp, nextIndex);
      temp = temp->getSibling();
    }

    //set leaf intervul for internal node
    //only calculate for deep enough nodes
    if(currentNode->getDepth() >= x)
    {
      SuffixTreeNode* left_child = currentNode->getChildPointer();
      SuffixTreeNode* right_child = currentNode->getRightmostChildPointer();

      assert(left_child != nullptr && right_child != nullptr);

      currentNode->set_start_leaf_index(left_child->get_start_leaf_index());
      currentNode->set_end_leaf_index(right_child->get_end_leaf_index());
    }
  }
}


//********constructors*************
SuffixTree::SuffixTree()
{
  root = new SuffixTreeNode();
  lastInserted = root;
  lastInternalId = 0;
}


//naive suffix tree insertion.
//Invokes FindPath on every suffix of the parameter string, but only uses root
//returns true if all suffixes inserted successfully
bool SuffixTree::basicInsert(string * str)
{
  bool success = false;
  unsigned int i;
  fullString = str;
  //internal nodes always have ids created after the leaf nodes for a given string
  lastInternalId += str->length();

  //for each suffix in the string
  for(i = 0; i < str->length(); i++)
  {
    //do findpath from root
    lastInserted = findPath(root, i);
  }
  //all suffixes added, success.
  success = true;

  return success;
}

//McCreight's suffix tree insertion.
//Invokes FindPath on every suffix of the parameter string, and uses suffix links
//returns true if all suffixes inserted successfully
bool SuffixTree::McCreightInsert(string * str)
{
  bool success = false;
  unsigned int i;

  fullString = str;

  //internal nodes always have ids created after the leaf nodes for a given string
  lastInternalId += str->length();

  //for each suffix in the string
  for(i = 0; i < str->length(); i++)
  {
    //call slInsert to handle insert cases
    lastInserted = slInsert(lastInserted, i);
  }
  //all suffixes added, success.
  success = true;

  return success;

}

//Depth-first search. Used for gathering data and constructing the BWT
void SuffixTree::DFS()
{
  //Calls recursive form of DFS
  LeafArray = new int[fullString->size()];
  for(int i = 0; i < fullString->size(); i++)
  {
    LeafArray[i] = -1;
  }

  int nextIndex = 0;
  DFS(root, nextIndex);
}

int* SuffixTree::getSuffixTreeLeafArray()
{
  return LeafArray;
}

SuffixTreeNode* SuffixTree::findLocationPrivate(int &readIndex, const string *read)
{
//modify later to use a global x
  int x = 0;
  int setCurrentToSL = 0;
  
  SuffixTreeNode *current = root;
  SuffixTreeNode *child = nullptr;
  SuffixTreeNode *temp = nullptr;
  SuffixTreeNode *deepest = new SuffixTreeNode();

  while(true)
  {
    child = current->getChild((*read)[readIndex], fullString);
    //base case we keep progressing
    //nodes scope
    if(child != nullptr)
    {
      Label childLabel = child->getLabel();
      int labelIndex = 0;
      int comparison = 0;

      while(labelIndex < (childLabel.endIndex - childLabel.startIndex) &&
       readIndex < read->size())
      {
        comparison 
          = Alphabet::compare((*read)[readIndex], 
                              (*fullString)[childLabel.startIndex + labelIndex]);

        if(comparison != 0)
        {
          //path no longer matches case B in instructions
          
          if(child->getDepth() >= x && deepest->getDepth() < child->getDepth())
          {
            //set as deepest
            deepest = child;
          }
          readIndex -= labelIndex;
          setCurrentToSL = 1;
          break;
        }

        //matched keep walking
        readIndex++;
        labelIndex++;
      }
      if(readIndex >= read->size())
      {
        if(child->getDepth() >= x && deepest->getDepth() < child->getDepth())
        {
          //set as deepest
          deepest = child;
        }
        readIndex -= labelIndex;
        return deepest;
      }
    }
    else
    {
      //case a in instructions
      if(current->getDepth() >= x && deepest->getDepth() < current->getDepth())
      {
        //set as deepest
        deepest = current;
        setCurrentToSL = 1;
      }
    }
    if(setCurrentToSL == 1)
    {
      //circle ignore
      if(current == current->getSL())
      {
        return deepest;
      }
      current = current->getSL();
      setCurrentToSL = 0;
    }
    else
    {
      current = child;
    }
  }
}


vector<int> SuffixTree::findLocation(int &readIndex, const string *read)
{
  readIndex = 0;
  SuffixTreeNode* deepest = findLocationPrivate(readIndex, read);
  
  if(deepest->get_end_leaf_index() == -1)
  {
    //could not find a location
    return vector<int>();
  }
  else
  {
    //found deepest now need to find indexes in array
    vector<int> returnVector;

    for(int i = deepest->get_start_leaf_index(); i <= deepest->get_end_leaf_index(); i++)
    {
      returnVector.push_back(LeafArray[i]);
    }

    return returnVector;
  }
}
