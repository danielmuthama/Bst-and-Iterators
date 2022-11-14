#include "equal-paths.h"
#include <algorithm> 
using namespace std;


// You may add any prototypes of helper functions here
int pathLength(Node* node); //prototype

int pathLength(Node* node)
{
    if(node->left == nullptr && node->right == nullptr) //if node has no children 
    {
        return 0; //return 0 (Halting condition)
    }
    else if (node->left && node->right == nullptr) //if node has just left subtree
    {
        return 1 + pathLength(node->left); //return 1 + recursive length of left subtree 
    }
    else if (node->left == nullptr && node->right) //if node has just right subtree
    {
        return 1 + pathLength(node->right); 
    }
    else if (node->left && node->right) //if node has both left and right subtrees
    {
        return 1 + max(pathLength(node->left), pathLength(node->right)); //returns max for true depth
    }
    return 0; 
}

bool equalPaths(Node * root)
{
    if (root->left && root->right)
    {
        return (pathLength(root->left) == pathLength(root->right)); //call to helper, true if lengths equal 
    }
    else if (root->left == nullptr && root->right == nullptr)
    {
        return true; //if just 1 node return true  
    }
    else
    {
        return false; //all other options reveal false 
    }
}