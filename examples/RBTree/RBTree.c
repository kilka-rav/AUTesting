/****************************************************************************************
 *
 *   RBTree.c
 *
 *   Created by dmitry
 *   06.03.2021
 *
 ***/



//
/// RBTree
///======================================================================================
/// A red-black tree is a binary search tree in which each node has a color attribute. At
/// the same time:
///
/// 1. A node can be either red or black and has two children;
/// 2. The root is usually black. This rule has little effect on the performance of the
///    model, since the color of the root can always be changed from red to black;
/// 3. All leaves that do not contain data are black.
/// 4. Both descendants of each red node are black.
/// 5. Any simple path from an ancestor node to a leaf descendant node contains the same
///    number of black nodes.
///
/// Thanks to these restrictions, the path from the root to the farthest leaf is no more
/// than twice as long as to the nearest one, and the tree is roughly balanced. The
/// insert, delete, and search operations require a time proportional to the length of
/// the tree in the worst case, which allows red-black trees to be more efficient in the
/// worst case than normal binary search trees.
///======================================================================================
///======================================================================================
//
#include "RBTree.h"

/****************************************************************************************
 *
 *   prototypes for helper functions
 *
 ***/
static void foreach_   (rbNode tree, void (*act)(rbPair*, void*), void* data);

static void printTree_ (rbNode tree, int indents);
static void printNode_ (rbNode node, int indents);

static rbNode find_top_            (rbNode node);
static rbNode find_parent_         (rbNode tree, rb_key_type key);
static rbNode find_grandparent_    (rbNode node);
static rbNode find_uncle_          (rbNode node);
static rbNode find_brother_        (rbNode node);
static rbNode find_node_with_key_(rbTree tree, rb_key_type key);


static void leftRotation  (rbNode node);
static void rightRotation (rbNode node);


static void replaceWithChild  (rbNode node, rbNode child);

static void   deleteTree      (rbNode tree);
static rbNode deleteNode      (rbNode node);
static void   delete_one_child(rbNode node);
static void   delete_case1    (rbNode node);
static void   delete_case2    (rbNode node);
static void   delete_case3    (rbNode node);
static void   delete_case4    (rbNode node);
static void   delete_case5    (rbNode node);
static void   delete_case6    (rbNode node);


static void   insert       (rbNode parent, rbNode node);
static void   insert_case1 (rbNode node);
static void   insert_case2 (rbNode node);
static void   insert_case3 (rbNode node);
static void   insert_case4 (rbNode node);
static void   insert_case5 (rbNode node);
/***
 *
 *   end of prototypes for helper functions
 *
 ****************************************************************************************/




/****************************************************************************************
 *
 *   interface functions
 *
 ***/
rbResult rbCreate (const rbPair* data, size_t size, rbTree* tree)
{
    if (tree == NULL) {
        return RB_INVALID_ARGS;
    }

    *tree = (struct rbTree_t*) calloc(1, sizeof(struct rbTree_t));

    if (*tree == NULL) {
        return RB_LACK_OF_MEMORY;
    }

    (*tree)->treeRoot = NULL;

    if (data == NULL || size == 0) {
        return RB_SUCCESS;
    }

    for (size_t i = 0; i < size; ++i) {
        rbResult res = rbInsert(*tree, data[i]);
        if (res != RB_SUCCESS) {
            rbDestroy(*tree);
            return res;
        }
    }

    return RB_SUCCESS;
}


rbResult rbDestroy (rbTree tree)
{
    deleteTree(tree->treeRoot);
    free (tree);
    return RB_SUCCESS;
}


rbResult rbClear (rbTree map) {

    if (map == NULL)
        return RB_INVALID_ARGS;

    deleteTree(map->treeRoot);
    map->treeRoot = NULL;
    return RB_SUCCESS;
}


rbPair* rbFind (rbTree tree, rb_key_type key)
{
    rbNode res = find_node_with_key_(tree, key);
    if (res == NULL)
        return NULL;

    return &res->pair;
}


rbResult rbInsert (rbTree tree, rbPair pair) {

    if (tree == NULL)
        return RB_INVALID_ARGS;

    rbNode rNode = tree->treeRoot;

    rbNode node = find_node_with_key_(tree, pair.key);
    if (node != NULL) {
        node->pair.value = pair.value;
        return RB_SUCCESS;
    }
    else {
        node = (rbNode) calloc(1, sizeof(struct rbNode_t));
    }

    if (node == NULL) {
        return RB_LACK_OF_MEMORY;
    }

    *((int*)&node->pair.key) = pair.key;

    node->pair.value = pair.value;
    node->color = RED;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;

    rbNode parent = find_parent_(rNode, node->pair.key);

    insert (parent, node);

    tree->treeRoot = find_top_(node);

    return RB_SUCCESS;
}


rbResult rbErase (struct rbTree_t* tree, int key) {

    if (tree == NULL)
        return RB_INVALID_ARGS;

    rbNode node = find_node_with_key_(tree, key);

    if (node != NULL)
        tree->treeRoot = deleteNode(node);

    return RB_SUCCESS;
}


rbResult rbEmpty (rbTree tree) {

    if (tree == NULL)
        return RB_INVALID_ARGS;

    return tree->treeRoot == NULL;
}
/***
 *
 *   end of interface functions
 *
 ****************************************************************************************/




/****************************************************************************************
 *
 *   Find functions
 *
 ***/
static rbNode find_node_with_key_(rbTree tree, rb_key_type key)
{
    if (tree->treeRoot == NULL)
        return NULL;

    rbNode node = tree->treeRoot;

    rbNode tmp = node;

    while (tmp) {
        if (tmp->pair.key > key)
            tmp = tmp->left;
        else if (tmp->pair.key < key)
            tmp = tmp->right;
        else
            return tmp;
    }

    return NULL;
}


/// Looking for a node that has no parent (root node).
/// There is only one such node in the whole tree.
/// \param node - current node
/// \return pointer to root node.
rbNode find_top_ (rbNode node) {

    if (node == NULL)
        return NULL;

    rbNode res = node;

    while (res->parent)
        res = res->parent;

    return res;
}


///  Looks for a place where to attach a new node with the given key.
/// \param tree - root node
/// \param key  - the key to look for the parent
/// \return a pointer to a node to which a new node with the given key can be attached.
rbNode find_parent_ (rbNode  tree, int key) {

    if (tree == NULL)
        return NULL;

    if (tree->pair.key > key) {
        if (tree->left)
            return find_parent_(tree->left, key);
        else
            return tree;
    } else {
        if (tree->right)
            return find_parent_(tree->right, key);
        else
            return tree;
    }
}

static rbNode find_grandparent_(rbNode node) {

    return node->parent->parent;
}

static rbNode find_uncle_ (rbNode node) {

    rbNode  grandpa = find_grandparent_(node);

    if (node->parent == grandpa->left)
        return grandpa->right;
    else
        return grandpa->left;
}

static rbNode find_brother_ (rbNode node) {

    if (node == NULL || node->parent == NULL)
        return NULL;

    if (node == node->parent->left)
        return node->parent->right;
    else
        return node->parent->left;
}

rbNode findMax (rbNode tree) {

    while (tree->right)
        tree = tree->right;

    return tree;
}

rbNode findMin (rbNode tree) {

    while (tree->left)
        tree = tree->left;

    return tree;
}
/***
 *
 *   end of Rotate functions
 *
 ****************************************************************************************/




/****************************************************************************************
 *
 *   Rotate functions
 *
 ***/
static void leftRotation (rbNode  node) {

    rbNode  pivot = node->right;

    pivot->parent = node->parent; // and pivot can become the root of tree
    if (node->parent != NULL) {
        if (node->parent->left == node)
            node->parent->left = pivot;
        else
            node->parent->right = pivot;
    }

    node->right = pivot->left;
    if (pivot->left != NULL)
        pivot->left->parent = node;

    node->parent = pivot;
    pivot->left = node;
}

static void rightRotation(rbNode node) {

    rbNode  pivot = node->left;

    pivot->parent = node->parent; // and pivot can become the root of tree
    if (node->parent != NULL) {
        if (node->parent->left == node)
            node->parent->left = pivot;
        else
            node->parent->right = pivot;
    }

    node->left = pivot->right;

    if (pivot->right != NULL)
        pivot->right->parent = node;

    node->parent = pivot;
    pivot->right = node;
}
/***
 *
 *   end of Rotate functions
 *
 ****************************************************************************************/




/****************************************************************************************
 *
 *   Insertion functions
 *
 ***/

//
/// Insertion
///======================================================================================
/// A new node in the red-black tree is added to the place of one of the leaves, colored
/// red, and two leaves are attached to it (since the leaves are an abstraction that does
/// not contain data, adding them does not require an additional operation). What happens
/// next depends on the color of the nearby nodes. Note that:
///
/// Property 3 (All leaves are black) is always executed.
/// Property 4 (Both descendants of any red node are black) can only be broken when a red
/// node is added, when a black node is recolored to red, or when it is rotated.
/// Property 5 (All paths from any node to leaf nodes contain the same number of black
/// nodes) can only be broken when adding a black node, repainting a red node to black
/// (or vice versa), or when turning.
///======================================================================================
///======================================================================================
//


/// Attaches a new node to an existing one without violating the red-black tree
/// invariants.
/// \param parent - parent of the new node
/// \param node   - new node
static void insert (rbNode  parent, rbNode  node) {

    node->parent = parent;

    if (parent != NULL)
    {
        if (parent->pair.key > node->pair.key)
            parent->left = node;
        else
            parent->right = node;
    }
    insert_case1(node);
}

/// Case 1: The current node N at the root of the tree. In this case, it is repainted
/// black to keep Property 2 true (Root is black). Since this action adds one black node
/// to each path, Property 5 (All paths from any given node to leaf nodes contain the
/// same number of black nodes) is not violated.
/// \param node - insert node
static void insert_case1(rbNode  node) {

    if (node->parent == NULL)
        node->color = BLACK;
    else
        insert_case2(node);
}

/// Case 2: The ancestor P of the current node is black, that is, Property 4 (Both
/// children of each red node are black) is not violated. In this case, the tree remains
/// correct. Property 5 (All paths from any given node to leaf nodes contain the same
/// number of black nodes) is not violated, because the current node N has two black leaf
/// children, but since N is red, the path to each of these children contains the same
/// number of black ones. nodes as the path to the black sheet, which was replaced by the
/// current node, so the property remains true.
/// \param node - insert node
static void insert_case2(rbNode  node) {

    if (node->parent->color == BLACK)
        return; /* Tree is still valid */
    else
        insert_case3(node);
}

/// Case 3: If both parent P and uncle U are red, then they can both be repainted black,
/// and grandpa G will turn red (to preserve property 5 (All paths from any given node to
/// leaf nodes contain the same number of black nodes)) ... The current red node N now
/// has a black parent. Since any path through the parent or uncle must pass through the
/// grandfather, the number of black nodes in these paths will not change. However, the
/// grandfather G can now violate properties 2 (Root is black) or 4 (Both children of
/// each red node are black) (property 4 can be violated, since the parent of G may be
/// red). To fix this, the whole procedure is recursively executed on G from case 1.
/// \param node - insert node
static void insert_case3(rbNode node) {

    rbNode uncle = find_uncle_(node), grandpa;

    if ((uncle != NULL) && (uncle->color == RED)) {

        node->parent->color = BLACK;
        uncle->color = BLACK;

        grandpa = find_grandparent_(node);
        grandpa->color = RED;

        insert_case1(grandpa);
    } else {
        insert_case4(node);
    }
}

/// Case 4: P's parent is red, but U's uncle is black. Also, the current node N is the
/// right child of P, and P, in turn, is the left child of its ancestor G. In this case,
/// a tree rotation can be performed, which changes the roles of the current node N and
/// its ancestor P. Then, for the former parent of P in the updated structure, we use
/// case 5, because Property 4 (Both children of any red node are black) is still broken.
/// Rotation leads to the fact that some paths (in the subtree marked "1" in the diagram)
/// go through the node N, which was not before. This also causes some paths (in the
/// subtree denoted by "3") not to go through node P. However, both of these nodes are
/// red, so Property 5 (All paths from any given node to leaf nodes contain the same
/// number of black knots) is not broken during rotation. However Property 4 is still
/// violated, but now the problem is reduced to Case 5.
/// \param node - insert node
static void insert_case4(rbNode node) {

    rbNode  grandpa = find_grandparent_(node);

    if ((node == node->parent->right) && (node->parent == grandpa->left)) {

        leftRotation(node->parent);
        node = node->left;
    } else if ((node == node->parent->left) && (node->parent == grandpa->right)) {

        rightRotation(node->parent);
        node = node->right;
    }

    insert_case5(node);
}


/// Case 5: P's parent is red, but U's uncle is black, the current node N is the left
/// child of P and P is the left child of G. In this case, the tree rotates by G. The
/// result is a tree in which the former parent of P is now the parent and the current
/// node N and the former grandfather G. It is known that G is black, since its former
/// child P could not otherwise be red (without violating Property 4). Then the colors
/// of P and G change and as a result the tree satisfies Property 4 (Both children of
/// any red node are black). Property 5 (All paths from any given node to leaf nodes
/// contain the same number of black nodes) also remains true, since all paths that pass
/// through any of these three nodes previously went through G, so now they all go
/// through P. B in each case, of the three nodes, only one is colored black.
/// \param node - insert node
static void insert_case5(rbNode  node) {

    rbNode  grandpa = find_grandparent_(node);

    node->parent->color = BLACK;
    grandpa->color = RED;

    if ((node == node->parent->left) && (node->parent == grandpa->left)) {
        rightRotation(grandpa);
    } else {
        leftRotation(grandpa);
    }
}
/***
 *
 *   end of Insertion functions
 *
 ****************************************************************************************/




/****************************************************************************************
 *
 *   Removal functions
 *
 ***/


rbNode  deleteNode (rbNode  node) {

    rbNode  M;
    rbNode  tmp = node;

    if (node->right)
        M = findMin (node->right);
    else if (node->left)
        M = findMax (node->left);
    else {
        tmp = node->parent;
        M = node;
    }

    node->pair.value = M->pair.value;

    *((int*)&node->pair.key) = M->pair.key;

    delete_one_child(M);

    return find_top_(tmp);
}


static void delete_one_child(rbNode  node) {

    assert (node->left == NULL || node->right == NULL);

    rbNode child;

    if (node->left == NULL && node->right == NULL) {

        if (node->color == BLACK)
            delete_case1(node);

        if (node->parent == NULL) {
            free(node);
            return;
        }

        if (node == node->parent->left)
            node->parent->left = NULL;
        else
            node->parent->right = NULL;

        free(node);
        return;
    }

    child = node->right;

    replaceWithChild (node, child);

    if (node->color == BLACK)//Cause node has only one child, child->color can be only RED
        child->color = BLACK;

    free(node);
}


static void replaceWithChild (rbNode  node, rbNode  child) {

    assert (child && node);

    child->parent = node->parent;

    if (node == node->parent->left)
        node->parent->left = child;
    else
        node->parent->right = child;
}


static void delete_case1 (rbNode  node)
{
    if (node->parent != NULL)
        delete_case2(node);
}


static void delete_case2 (rbNode  node) {

    rbNode  brother = find_brother_(node);

    if (brother->color == RED) {
        node->parent->color = RED;
        brother->color = BLACK;

        if (node == node->parent->left)
            leftRotation (node->parent);
        else
            rightRotation (node->parent);
    }

    delete_case3 (node);
}


static void delete_case3 (rbNode  node) {

    rbNode brother = find_brother_(node);

    if ((node->parent->color == BLACK) &&
        (brother->color == BLACK) &&
        (brother->left == NULL || brother->left->color == BLACK) &&
        (brother->right == NULL || brother->right->color == BLACK)) {

        brother->color = RED;
        delete_case1(node->parent);
    } else
        delete_case4(node);
}


static void delete_case4 (rbNode  node) {

    rbNode brother = find_brother_(node);

    if ((node->parent->color == RED) &&
        (brother->color == BLACK) &&
        (brother->left == NULL  || brother->left->color == BLACK) &&
        (brother->right == NULL || brother->right->color == BLACK))  {

        brother->color = RED;
        node->parent->color = BLACK;
    } else
        delete_case5(node);
}


static void delete_case5 (rbNode  node) {

    rbNode brother = find_brother_(node);

    if  (brother->color == BLACK) {

        if ((node == node->parent->left) &&
            (brother->right == NULL || brother->right->color == BLACK) &&
            (brother->left && brother->left->color == RED)) { /* this last test is trivial too due to cases 2-4. */

            brother->color = RED;
            brother->left->color = BLACK;
            rightRotation(brother);

        } else if ((node == node->parent->right) &&
                   (brother->left == NULL || brother->left->color == BLACK) &&
                   (brother->right && brother->right->color == RED)) { /* this last test is trivial too due to cases 2-4. */

            brother->color = RED;
            brother->right->color = BLACK;
            leftRotation(brother);
        }
    }

    delete_case6(node);
}


static void delete_case6 (rbNode  node) {

    rbNode  brother = find_brother_(node);

    brother->color = node->parent->color;
    node->parent->color = BLACK;

    if (node == node->parent->left) {
        brother->right->color = BLACK;
        leftRotation (node->parent);
    } else {
        brother->left->color = BLACK;
        rightRotation (node->parent);
    }
}


static void deleteTree (rbNode  tree) {

    if (tree == NULL)
        return;

    if (tree->left)
        deleteTree(tree->left);
    if (tree->right)
        deleteTree(tree->right);

    free (tree);
}
/***
 *
 *   end of Removal functions
 *
 ****************************************************************************************/




/****************************************************************************************
 *
 *   foreach functions
 *
 ***/
rbResult rbForeach (struct rbTree_t* tree, void (*act)(rbPair*, void*), void* data) {

    if (tree == NULL || act == NULL)
        return RB_INVALID_ARGS;

    foreach_(tree->treeRoot, act, data);
    return RB_SUCCESS;
}


static void foreach_ (rbNode  tree, void (*act)(rbPair*, void*), void* data) {

    if (tree == NULL)
        return;

    foreach_(tree->left, act, data);
    foreach_(tree->right, act, data);
    act (&tree->pair, data);
}
/***
 *
 *   end of foreach functions
 *
 ****************************************************************************************/




/****************************************************************************************
 *
 *   dump functions
 *
 ***/
rbResult rbDump (rbTree tree) {

    if (tree == NULL)
        return RB_INVALID_ARGS;

    printTree_(tree->treeRoot, 0);

    return RB_SUCCESS;
}


/// prints the current node and then the children indented one more.
/// \param tree - current node
/// \param indents - отступ
static void printTree_ (rbNode tree, int indents) {

    printNode_(tree, indents);

    if (tree == NULL)
        return;

    printTree_(tree->left, indents + 1);
    printTree_(tree->right, indents + 1);
}


/// prints the contents of the node indented appropriately.
/// \param node    - print node
/// \param indents - отступ
static void printNode_ (rbNode node, int indents)
{
    for (int i = 0; i < indents; ++i)
        printf("    |");

    if (node == NULL) {
        printf("NULL(B)\n");
    }
    else{
        printf("[key: %d, value: %d]", node->pair.key, node->pair.value);

        if (node->color == RED)
            printf("(R)\n");
        else
            printf("(B)\n");
    }
}
/***
 *
 *   end of dump functions
 *
 ****************************************************************************************/