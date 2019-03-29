//Patrick Keppler and Connor Wright, 2019
#include "SuffixTree.h"



//******private methods***********

//findPath navigates the (sub)tree from the starting node parameter. Returns the created leaf node that was inserted
//called by basicInsert or slInsert
SuffixTreeNode * SuffixTree::findPath(SuffixTreeNode * start, char * suffix)
{

  SuffixTreeNode * child = nullptr;
  SuffixTreeNode * current = start;
  size_t labelIndex = 0;
  size_t suffixIndex = 0;
  unsigned int id = lastInserted->getId() + 1;

  while(suffixIndex < strlen(suffix))
  {
    child = current->getChild(suffix[suffixIndex]);

    if(child == nullptr)
    {
      //could not find child add a new branch
      char * newLabel = new char[strlen(suffix) - suffixIndex + 1];
      newLabel = strcpy(newLabel, &suffix[suffixIndex]);
      SuffixTreeNode * newLeaf = new SuffixTreeNode(id, newLabel, nullptr);
      current->addChild(newLeaf);
      STData::incrementLeafNodes();

      return newLeaf;
    }
    else
    {
      //found child need to walk down
      labelIndex = 0;
      int comparison = 0;
      char * childLabel = child->getLabel();

      while(labelIndex < strlen(childLabel) && suffixIndex < strlen(suffix))
      {
        comparison = Alphabet::compare(suffix[suffixIndex], childLabel[labelIndex]);
        
        if(comparison != 0)
        {
          //path no longer matches

          //add new internal node
          SuffixTreeNode * newInternalNode =
           current->addInternalNode(childLabel[0], labelIndex, ++lastInternalId);
          STData::incrementInternalNodes();

          //add new leaf to the end of new internal node
          char * newLabel = new char[strlen(suffix) - suffixIndex + 1];
          newLabel = strcpy(newLabel, &suffix[suffixIndex]);
          SuffixTreeNode * newLeafNode = new SuffixTreeNode(id, newLabel, nullptr);

          newInternalNode->addChild(newLeafNode);
          STData::incrementLeafNodes();

          return newLeafNode;
        }

        labelIndex++;
        suffixIndex++;
      }

      if(suffixIndex > strlen(suffix))
      {
        //we have reached the end of the suffix
        //add a new internal node and leaf with label $
        
        SuffixTreeNode * newInternalNode =
          current->addInternalNode(childLabel[0], labelIndex, ++lastInternalId);
        STData::incrementInternalNodes();

        SuffixTreeNode * newLeafNode = new SuffixTreeNode(id, "$", nullptr);
        newInternalNode->addChild(newLeafNode);
        STData::incrementLeafNodes();

        return newLeafNode;
      }
      current = child;
    }
  }
}

//slInsert handles the 4 cases for inserting a suffix using suffix links. Ultimately calls findPath.
SuffixTreeNode * SuffixTree::slInsert(SuffixTreeNode * last, char * suffix)
{
  SuffixTreeNode * u, * v, * uPrime, * vPrime;
  char * beta;
  //assert that the parameter node exists
  assert(last != nullptr);
  //assert that the parent to the last inserted node exists
  assert(last->getParent() != nullptr);

  u = last->getParent();

  //case I: uffix link is known
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
      v = nodeHop(uPrime, beta);
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
SuffixTreeNode * SuffixTree::nodeHop(SuffixTreeNode * start, char * beta)
{
  size_t betaIndex = 0;
  unsigned int startDepth = start->getDepth();

  SuffixTreeNode * current = start;
  SuffixTreeNode * child = nullptr;

  while(true)
  {
    child = current->getChild(beta[betaIndex]);
    assert(child != nullptr);

    if(strlen(beta) + startDepth < child->getDepth())
    {
      //we overshot and need to add an internal node
      SuffixTreeNode * newInternalNode 
        = current->addInternalNode(child->getLabel()[0],
                                   strlen(beta) - current->getDepth(), 
                                   ++lastInternalId);
      STData::incrementInternalNodes();
      return newInternalNode;
    }
    else if(strlen(beta) + startDepth == child->getDepth())
    {
      //we have found our lost child V!
      return child;
    }
    else
    {
      //we need to keep go deeper
      betaIndex = child->getDepth() - startDepth;
    }
  }
}


void SuffixTree::DFS(SuffixTreeNode * currentNode)
{
  if(currentNode != nullptr)
  {
    DFS(currentNode->getChildPointer());
    DFS(currentNode->getSibling());
    //if current node is an internal node
    if(currentNode->getId() > lastInserted->getId())
    {
      STData::incrementInternalNodes();
      STData::findLongestRepeat(currentNode);
    }
    else {
      STData::incrementLeafNodes();
      STData::pushBwt(currentNode->getId());
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
bool SuffixTree::basicInsert(char * str, unsigned int length)
{
  bool success = false;
  unsigned int i;

  //internal nodes always have ids created after the leaf nodes for a given string
  lastInternalId += length;

  //for each suffix in the string
  for(i = 0; i < length; i++)
  {
    //do findpath from root
    lastInserted = findPath(root, str + i);
  }
  //all suffixes added, success.
  success = true;

  return success;
}

//McCreight's suffix tree insertion.
//Invokes FindPath on every suffix of the parameter string, and uses suffix links
//returns true if all suffixes inserted successfully
bool SuffixTree::McCreightInsert(char * str, unsigned int length)
{
  bool success = false;
  unsigned int i;


  //internal nodes always have ids created after the leaf nodes for a given string
  lastInternalId += length;

  //for each suffix in the string
  for(i = 0; i < length; i++)
  {
    //call slInsert to handle insert cases
    lastInserted = slInsert(lastInserted, str + i);
  }
  //all suffixes added, success.
  success = true;

  return success;

}

//Depth-first search. Used for gathering data and constructing the BWT
void SuffixTree::DFS()
{
  //Calls recursive form of DFS
  DFS(root);
}
