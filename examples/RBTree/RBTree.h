/****************************************************************************************
 *
 *   RBTree.h
 *
 *   Created by dmitry
 *   06.03.2021
 *
 ***/



//
/// RBTree
///======================================================================================
/// This file presents the implementation of the red-black tree.
/// (https://en.wikipedia.org/wiki/Red%E2%80%93black_tree)
///
/// All names (of types, functions, etc.) start with the 'rb' prefix. The C version of
/// the namespace.
///
/// Due to encapsulation, the user has access only to a pointer to the tree itself and,
/// through functions, to a pointer to a key-value pair.
///======================================================================================
///======================================================================================
//
#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>



/****************************************************************************************
 *
 *   defining the types available to the user
 *
 ***/
struct rbTree_t;
struct rbPair_t;


enum rbResult_enum_t {
    RB_SUCCESS = 0,

    RB_LACK_OF_MEMORY = -10,
    RB_INVALID_ARGS   = -11
};


typedef struct rbTree_t*      rbTree;

typedef enum rbResult_enum_t rbResult;

typedef struct rbPair_t      rbPair;
typedef int                  rb_key_type;
typedef int                  rb_val_type;


/// Structure that defines the data in the container node.
/// \note Changing the key value will violate the container invariant.
///       At the same time, you can change the value field.
struct rbPair_t {
    const rb_key_type key;
    rb_val_type       value;
};
/***
 *
 *   end of defining the types
 *
 ****************************************************************************************/

/****************************************************************************************
 *
 *   defining structures
 *
 ***/

/// An attribute of every node in the tree. Used for balancing of tree.
enum color_t { BLACK, RED };

struct rbNode_t {
  struct rbNode_t *parent;
  struct rbNode_t *left;
  struct rbNode_t *right;

  enum color_t color;
  rbPair pair;
};
typedef struct rbNode_t *rbNode;

struct rbTree_t {
  rbNode treeRoot;
};
/***
 *
 *   end of defining structures
 *
 ****************************************************************************************/

/****************************************************************************************
 *
 *   interface functions
 *
 ***/


/// Creates an instance of red-black tree
/// \param data - an array of elements to be placed in the tree
/// \param size - the number of elements in the 'data' array
/// \param tree - if successful, a pointer to a variable where to place the created
///               container
/// \return an enum member from rbResult
rbResult rbCreate (const rbPair* data, size_t size, rbTree* tree);

/// Removes the container instance
/// \param tree - the container instance to be deleted.
/// \return an enum member from rbResult
rbResult rbDestroy (rbTree tree);


/// Function to perform some kind of action on all elements of the container
/// \param tree - container
/// \param act  - pointer to a function that is called for each element of the container.
///               Prototype: void (* act) (rbPair *, void *)
///               First parameter is a pointer to a key-value pair, where the key is
///               a constant field (do not violate constancy!), and the value can be changed.
///               For the second parameter, see the following.
/// \param data - passed as the second parameter when calling the 'act' function
/// \return an enum member from rbResult
rbResult rbForeach (rbTree tree, void (*act)(rbPair*, void*), void* data);


/// Tries to find a value in the tree with the given key.
/// \param map - container to search in
/// \param key - required key
/// \return if a key is found, a pointer to a key-value pair is returned.
///         If the key is not found or if an invalid pointer to the container is passed,
///         then NULL is returned.
rbPair* rbFind (rbTree tree, rb_key_type key);


/// Adds a new unique key to the container. If such a key already exists,
/// then the value that is stored under this key is simply replaced.
/// \param tree - container
/// \param pair - insert element
/// \return an enum member from rbResult
rbResult rbInsert (rbTree tree, rbPair pair);

/// Removes a pair with the given key from the container
/// \param tree - container
/// \param key  - key of the pair to be removed
/// \return an enum member from rbResult
rbResult rbErase (rbTree tree, rb_key_type key);

/// Checks if the container is empty.
/// \param map - container
/// \return 'true' if empty and 'false' if there is at least one element
rbResult rbEmpty (rbTree tree);

/// Removes all items from the container.
/// After the call to rbEmpty will return true.
/// \param map - container
/// \return an enum member from rbResult
rbResult rbClear (rbTree tree);


/// Prints a tree to standard output.
/// \param tree - container
/// \return an enum member from rbResult
rbResult rbDump (rbTree tree);
/***
 *
 *   end of interface functions
 *
 ****************************************************************************************/