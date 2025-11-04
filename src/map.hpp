/**
* implement a container like std::map
*/
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
   class Key,
   class T,
   class Compare = std::less <Key>
   > class map {
  public:
   typedef pair<const Key, T> value_type;

  private:
   enum Color { RED, BLACK };

   struct Node {
       value_type *data;
       Node *left, *right, *parent;
       Color color;

       Node(const value_type &val, Node *p = nullptr)
           : data(new value_type(val)), left(nullptr), right(nullptr), parent(p), color(RED) {}

       // Constructor for sentinel node (no data)
       Node(Node *p, bool sentinel)
           : data(nullptr), left(nullptr), right(nullptr), parent(p), color(BLACK) {}

       ~Node() {
           if (data) delete data;
       }
   };

   Node *root;
   Node *endNode;  // sentinel node for end()
   size_t nodeCount;
   Compare comp;

   // Helper function to compare keys
   bool keyEqual(const Key &a, const Key &b) const {
       return !comp(a, b) && !comp(b, a);
   }

   bool keyLess(const Key &a, const Key &b) const {
       return comp(a, b);
   }

   // Find minimum node in subtree
   Node* minimum(Node *node) const {
       if (!node) return nullptr;
       while (node->left) node = node->left;
       return node;
   }

   // Find maximum node in subtree
   Node* maximum(Node *node) const {
       if (!node) return nullptr;
       while (node->right) node = node->right;
       return node;
   }

   // Find successor of a node
   Node* successor(Node *node) const {
       if (!node) return endNode;
       if (node->right) {
           return minimum(node->right);
       }
       Node *p = node->parent;
       while (p && node == p->right) {
           node = p;
           p = p->parent;
       }
       return p ? p : endNode;
   }

   // Find predecessor of a node
   Node* predecessor(Node *node) const {
       if (node == endNode) {
           return maximum(root);
       }
       if (!node) return nullptr;
       if (node->left) {
           return maximum(node->left);
       }
       Node *p = node->parent;
       while (p && node == p->left) {
           node = p;
           p = p->parent;
       }
       return p;
   }

   // Left rotation
   void rotateLeft(Node *x) {
       Node *y = x->right;
       x->right = y->left;
       if (y->left) y->left->parent = x;
       y->parent = x->parent;

       if (!x->parent) {
           root = y;
       } else if (x == x->parent->left) {
           x->parent->left = y;
       } else {
           x->parent->right = y;
       }
       y->left = x;
       x->parent = y;
   }

   // Right rotation
   void rotateRight(Node *y) {
       Node *x = y->left;
       y->left = x->right;
       if (x->right) x->right->parent = y;
       x->parent = y->parent;

       if (!y->parent) {
           root = x;
       } else if (y == y->parent->left) {
           y->parent->left = x;
       } else {
           y->parent->right = x;
       }
       x->right = y;
       y->parent = x;
   }

   // Fix tree after insertion
   void insertFixup(Node *z) {
       while (z->parent && z->parent->color == RED) {
           if (z->parent == z->parent->parent->left) {
               Node *y = z->parent->parent->right;
               if (y && y->color == RED) {
                   z->parent->color = BLACK;
                   y->color = BLACK;
                   z->parent->parent->color = RED;
                   z = z->parent->parent;
               } else {
                   if (z == z->parent->right) {
                       z = z->parent;
                       rotateLeft(z);
                   }
                   z->parent->color = BLACK;
                   z->parent->parent->color = RED;
                   rotateRight(z->parent->parent);
               }
           } else {
               Node *y = z->parent->parent->left;
               if (y && y->color == RED) {
                   z->parent->color = BLACK;
                   y->color = BLACK;
                   z->parent->parent->color = RED;
                   z = z->parent->parent;
               } else {
                   if (z == z->parent->left) {
                       z = z->parent;
                       rotateRight(z);
                   }
                   z->parent->color = BLACK;
                   z->parent->parent->color = RED;
                   rotateLeft(z->parent->parent);
               }
           }
       }
       root->color = BLACK;
   }

   // Transplant for deletion
   void transplant(Node *u, Node *v) {
       if (!u->parent) {
           root = v;
       } else if (u == u->parent->left) {
           u->parent->left = v;
       } else {
           u->parent->right = v;
       }
       if (v) v->parent = u->parent;
   }

   // Fix tree after deletion
   void deleteFixup(Node *x, Node *xParent) {
       while (x != root && (!x || x->color == BLACK)) {
           if (!xParent) break;
           if (x == xParent->left) {
               Node *w = xParent->right;
               if (!w) break;
               if (w->color == RED) {
                   w->color = BLACK;
                   xParent->color = RED;
                   rotateLeft(xParent);
                   w = xParent->right;
                   if (!w) break;
               }
               if ((!w->left || w->left->color == BLACK) &&
                   (!w->right || w->right->color == BLACK)) {
                   w->color = RED;
                   x = xParent;
                   xParent = x->parent;
               } else {
                   if (!w->right || w->right->color == BLACK) {
                       if (w->left) w->left->color = BLACK;
                       w->color = RED;
                       rotateRight(w);
                       w = xParent->right;
                       if (!w) break;
                   }
                   w->color = xParent->color;
                   xParent->color = BLACK;
                   if (w->right) w->right->color = BLACK;
                   rotateLeft(xParent);
                   x = root;
               }
           } else {
               Node *w = xParent->left;
               if (!w) break;
               if (w->color == RED) {
                   w->color = BLACK;
                   xParent->color = RED;
                   rotateRight(xParent);
                   w = xParent->left;
                   if (!w) break;
               }
               if ((!w->right || w->right->color == BLACK) &&
                   (!w->left || w->left->color == BLACK)) {
                   w->color = RED;
                   x = xParent;
                   xParent = x->parent;
               } else {
                   if (!w->left || w->left->color == BLACK) {
                       if (w->right) w->right->color = BLACK;
                       w->color = RED;
                       rotateLeft(w);
                       w = xParent->left;
                       if (!w) break;
                   }
                   w->color = xParent->color;
                   xParent->color = BLACK;
                   if (w->left) w->left->color = BLACK;
                   rotateRight(xParent);
                   x = root;
               }
           }
       }
       if (x) x->color = BLACK;
   }

   // Find node by key
   Node* findNode(const Key &key) const {
       Node *current = root;
       while (current) {
           if (keyEqual(key, current->data->first)) {
               return current;
           } else if (keyLess(key, current->data->first)) {
               current = current->left;
           } else {
               current = current->right;
           }
       }
       return nullptr;
   }

   // Copy tree recursively
   Node* copyTree(Node *other, Node *parent) {
       if (!other) return nullptr;
       Node *newNode = new Node(*(other->data), parent);
       newNode->color = other->color;
       newNode->left = copyTree(other->left, newNode);
       newNode->right = copyTree(other->right, newNode);
       return newNode;
   }

   // Delete tree recursively
   void deleteTree(Node *node) {
       if (!node) return;
       deleteTree(node->left);
       deleteTree(node->right);
       delete node;
   }

  public:
   class const_iterator;
   class iterator {
      private:
       const map *mapPtr;
       Node *nodePtr;

       friend class map;
       friend class const_iterator;

      public:
       iterator(const map *m = nullptr, Node *n = nullptr) : mapPtr(m), nodePtr(n) {}

       iterator(const iterator &other) : mapPtr(other.mapPtr), nodePtr(other.nodePtr) {}

       iterator operator++(int) {
           if (!nodePtr || nodePtr == mapPtr->endNode) {
               throw invalid_iterator();
           }
           iterator temp = *this;
           nodePtr = mapPtr->successor(nodePtr);
           return temp;
       }

       iterator &operator++() {
           if (!nodePtr || nodePtr == mapPtr->endNode) {
               throw invalid_iterator();
           }
           nodePtr = mapPtr->successor(nodePtr);
           return *this;
       }

       iterator operator--(int) {
           if (nodePtr == mapPtr->endNode) {
               nodePtr = mapPtr->maximum(mapPtr->root);
               if (!nodePtr) throw invalid_iterator();
               return iterator(mapPtr, mapPtr->endNode);
           }
           Node *pred = mapPtr->predecessor(nodePtr);
           if (!pred) throw invalid_iterator();
           iterator temp = *this;
           nodePtr = pred;
           return temp;
       }

       iterator &operator--() {
           if (nodePtr == mapPtr->endNode) {
               nodePtr = mapPtr->maximum(mapPtr->root);
               if (!nodePtr) throw invalid_iterator();
               return *this;
           }
           Node *pred = mapPtr->predecessor(nodePtr);
           if (!pred) throw invalid_iterator();
           nodePtr = pred;
           return *this;
       }

       value_type &operator*() const {
           if (!nodePtr || nodePtr == mapPtr->endNode) {
               throw invalid_iterator();
           }
           return *(nodePtr->data);
       }

       bool operator==(const iterator &rhs) const {
           return mapPtr == rhs.mapPtr && nodePtr == rhs.nodePtr;
       }

       bool operator==(const const_iterator &rhs) const {
           return mapPtr == rhs.mapPtr && nodePtr == rhs.nodePtr;
       }

       bool operator!=(const iterator &rhs) const {
           return !(*this == rhs);
       }

       bool operator!=(const const_iterator &rhs) const {
           return !(*this == rhs);
       }

       value_type *operator->() const noexcept {
           return nodePtr->data;
       }
   };

   class const_iterator {
      private:
       const map *mapPtr;
       Node *nodePtr;

       friend class map;
       friend class iterator;

      public:
       const_iterator(const map *m = nullptr, Node *n = nullptr) : mapPtr(m), nodePtr(n) {}

       const_iterator(const const_iterator &other) : mapPtr(other.mapPtr), nodePtr(other.nodePtr) {}

       const_iterator(const iterator &other) : mapPtr(other.mapPtr), nodePtr(other.nodePtr) {}

       const_iterator operator++(int) {
           if (!nodePtr || nodePtr == mapPtr->endNode) {
               throw invalid_iterator();
           }
           const_iterator temp = *this;
           nodePtr = mapPtr->successor(nodePtr);
           return temp;
       }

       const_iterator &operator++() {
           if (!nodePtr || nodePtr == mapPtr->endNode) {
               throw invalid_iterator();
           }
           nodePtr = mapPtr->successor(nodePtr);
           return *this;
       }

       const_iterator operator--(int) {
           if (nodePtr == mapPtr->endNode) {
               nodePtr = mapPtr->maximum(mapPtr->root);
               if (!nodePtr) throw invalid_iterator();
               return const_iterator(mapPtr, mapPtr->endNode);
           }
           Node *pred = mapPtr->predecessor(nodePtr);
           if (!pred) throw invalid_iterator();
           const_iterator temp = *this;
           nodePtr = pred;
           return temp;
       }

       const_iterator &operator--() {
           if (nodePtr == mapPtr->endNode) {
               nodePtr = mapPtr->maximum(mapPtr->root);
               if (!nodePtr) throw invalid_iterator();
               return *this;
           }
           Node *pred = mapPtr->predecessor(nodePtr);
           if (!pred) throw invalid_iterator();
           nodePtr = pred;
           return *this;
       }

       const value_type &operator*() const {
           if (!nodePtr || nodePtr == mapPtr->endNode) {
               throw invalid_iterator();
           }
           return *(nodePtr->data);
       }

       bool operator==(const iterator &rhs) const {
           return mapPtr == rhs.mapPtr && nodePtr == rhs.nodePtr;
       }

       bool operator==(const const_iterator &rhs) const {
           return mapPtr == rhs.mapPtr && nodePtr == rhs.nodePtr;
       }

       bool operator!=(const iterator &rhs) const {
           return !(*this == rhs);
       }

       bool operator!=(const const_iterator &rhs) const {
           return !(*this == rhs);
       }

       const value_type *operator->() const noexcept {
           return nodePtr->data;
       }
   };

   map() : root(nullptr), endNode(new Node(nullptr, true)), nodeCount(0) {}

   map(const map &other) : root(nullptr), endNode(new Node(nullptr, true)), nodeCount(other.nodeCount), comp(other.comp) {
       root = copyTree(other.root, nullptr);
   }

   map &operator=(const map &other) {
       if (this == &other) return *this;
       clear();
       root = copyTree(other.root, nullptr);
       nodeCount = other.nodeCount;
       comp = other.comp;
       return *this;
   }

   ~map() {
       clear();
       delete endNode;
   }

   T &at(const Key &key) {
       Node *node = findNode(key);
       if (!node) throw index_out_of_bound();
       return node->data->second;
   }

   const T &at(const Key &key) const {
       Node *node = findNode(key);
       if (!node) throw index_out_of_bound();
       return node->data->second;
   }

   T &operator[](const Key &key) {
       Node *node = findNode(key);
       if (node) return node->data->second;

       // Insert new element with default value
       pair<iterator, bool> result = insert(value_type(key, T()));
       return result.first.nodePtr->data->second;
   }

   const T &operator[](const Key &key) const {
       Node *node = findNode(key);
       if (!node) throw index_out_of_bound();
       return node->data->second;
   }

   iterator begin() {
       Node *min = minimum(root);
       return iterator(this, min ? min : endNode);
   }

   const_iterator cbegin() const {
       Node *min = minimum(root);
       return const_iterator(this, min ? min : endNode);
   }

   iterator end() {
       return iterator(this, endNode);
   }

   const_iterator cend() const {
       return const_iterator(this, endNode);
   }

   bool empty() const {
       return nodeCount == 0;
   }

   size_t size() const {
       return nodeCount;
   }

   void clear() {
       deleteTree(root);
       root = nullptr;
       nodeCount = 0;
   }

   pair<iterator, bool> insert(const value_type &value) {
       // Find position to insert
       Node *parent = nullptr;
       Node *current = root;

       while (current) {
           parent = current;
           if (keyEqual(value.first, current->data->first)) {
               // Key already exists
               return pair<iterator, bool>(iterator(this, current), false);
           } else if (keyLess(value.first, current->data->first)) {
               current = current->left;
           } else {
               current = current->right;
           }
       }

       // Create new node
       Node *newNode = new Node(value, parent);
       nodeCount++;

       if (!parent) {
           root = newNode;
       } else if (keyLess(value.first, parent->data->first)) {
           parent->left = newNode;
       } else {
           parent->right = newNode;
       }

       insertFixup(newNode);
       return pair<iterator, bool>(iterator(this, newNode), true);
   }

   void erase(iterator pos) {
       if (!pos.nodePtr || pos.nodePtr == endNode || pos.mapPtr != this) {
           throw invalid_iterator();
       }

       Node *z = pos.nodePtr;
       Node *y = z;
       Node *x;
       Node *xParent;
       Color yOriginalColor = y->color;

       if (!z->left) {
           x = z->right;
           xParent = z->parent;
           transplant(z, z->right);
       } else if (!z->right) {
           x = z->left;
           xParent = z->parent;
           transplant(z, z->left);
       } else {
           y = minimum(z->right);
           yOriginalColor = y->color;
           x = y->right;

           if (y->parent == z) {
               xParent = y;
               if (x) x->parent = y;
           } else {
               xParent = y->parent;
               transplant(y, y->right);
               y->right = z->right;
               y->right->parent = y;
           }
           transplant(z, y);
           y->left = z->left;
           y->left->parent = y;
           y->color = z->color;
       }

       delete z;
       nodeCount--;

       if (yOriginalColor == BLACK) {
           deleteFixup(x, xParent);
       }
   }

   size_t count(const Key &key) const {
       return findNode(key) ? 1 : 0;
   }

   iterator find(const Key &key) {
       Node *node = findNode(key);
       return node ? iterator(this, node) : end();
   }

   const_iterator find(const Key &key) const {
       Node *node = findNode(key);
       return node ? const_iterator(this, node) : cend();
   }
};

}

#endif
