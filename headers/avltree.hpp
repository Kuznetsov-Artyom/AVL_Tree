#ifndef _AVLTREE_HPP_
#define _AVLTREE_HPP_

#include <cstdint>
#include <iostream>
#include <vector>

template <typename T>
struct Node {
  T key;
  size_t height;

  Node<T>* left;
  Node<T>* right;

  Node(T keyValue, size_t heightVal = 0)
      : key{keyValue}, height{heightVal}, left{nullptr}, right{nullptr} {}

  ~Node() {
    delete left;
    delete right;
  }
};

template <typename T>
class avlTree {
 private:
  Node<T>* mRoot;
  size_t mSize;

 public:
  avlTree() : mRoot{nullptr}, mSize{0} {}
  avlTree(const avlTree& other);
  avlTree(avlTree&& other) noexcept;

  ~avlTree() { delete mRoot; }

  void insert(const T& value);
  void remove(const T& value);

  size_t size() const noexcept { return mSize; }
  bool empty() const noexcept { return mRoot == nullptr; }

  bool find(const T& value) const { return finder(mRoot, value) != nullptr; }
  void clear();
  void print() const;

  avlTree& operator=(const avlTree& other);
  avlTree& operator=(avlTree&& other) noexcept;

 private:
  Node<T>* inserter(Node<T>*& node, const T& value);
  Node<T>* balancing(Node<T>* node);
  int8_t balanceFactor(Node<T>* node);
  Node<T>* rightRotate(Node<T>* node);
  Node<T>* leftRotate(Node<T>* node);
  void updHeight(Node<T>* node);
  int8_t heightNode(Node<T>* node);

  Node<T>* remover(Node<T>*& node, const T& value);
  Node<T>* findMinNode(Node<T>* node);
  Node<T>* removeMin(Node<T>* node);

  const Node<T>* finder(const Node<T>* node, const T& value) const;
  Node<T>* copyOther(const Node<T>* otherRoot);
  void printer(Node<T>* node) const;
};

template <typename T>
inline avlTree<T>::avlTree(const avlTree& other) : avlTree() {
  if (!other.empty()) {
    mRoot = copyOther(other.mRoot);
    mSize = other.mSize;
  }
}

template <typename T>
inline avlTree<T>::avlTree(avlTree&& other) noexcept
    : mRoot{other.mRoot}, mSize{other.mSize} {
  other.mRoot = nullptr;
}

////////////////////// PUBLIC METHODS //////////////////////

template <typename T>
inline void avlTree<T>::insert(const T& value) {
  mRoot = inserter(mRoot, value);
}

template <typename T>
inline void avlTree<T>::remove(const T& value) {
  if (empty()) return;
  mRoot = remover(mRoot, value);
}

template <typename T>
inline void avlTree<T>::clear() {
  delete mRoot;
  mRoot = nullptr;
  mSize = 0;
}

template <typename T>
inline void avlTree<T>::print() const {
  if (empty()) {
    std::cout << "Tree is empty\n";
    return;
  }

  std::cout << "Current size: " << mSize << '\n';
  printer(mRoot);
  std::cout << '\n';
}

template <typename T>
inline avlTree<T>& avlTree<T>::operator=(const avlTree& other) {
  if (&other == this) return *this;

  clear();

  if (!other.empty()) {
    mRoot = copyOther(other.mRoot);
    mSize = other.mSize;
  }

  return *this;
}

template <typename T>
inline avlTree<T>& avlTree<T>::operator=(avlTree&& other) noexcept {
  if (&other == this) return *this;

  clear();

  mSize = other.mSize;
  mRoot = other.mRoot;
  other.mRoot = nullptr;

  return *this;
}

////////////////////// PRIVATE METHODS //////////////////////

template <typename T>
inline Node<T>* avlTree<T>::inserter(Node<T>*& node, const T& value) {
  if (node == nullptr) {
    ++mSize;
    return new Node<T>(value, 0);
  }

  if (value < node->key)
    node->left = inserter(node->left, value);
  else if (value > node->key)
    node->right = inserter(node->right, value);
  else
    return node;

  return balancing(node);
}

template <typename T>
inline Node<T>* avlTree<T>::balancing(Node<T>* node) {
  updHeight(node);
  int8_t balance = balanceFactor(node);

  if (balance == -2) {
    if (balanceFactor(node->left) > 0) node->left = leftRotate(node->left);
    return rightRotate(node);
  }
  if (balance == 2) {
    if (balanceFactor(node->right) < 0) node->right = rightRotate(node->right);
    return leftRotate(node);
  }

  return node;
}

template <typename T>
inline int8_t avlTree<T>::balanceFactor(Node<T>* node) {
  return (heightNode(node->right) + 1) - (heightNode(node->left) + 1);
}

template <typename T>
inline Node<T>* avlTree<T>::rightRotate(Node<T>* node) {
  Node<T>* A = node;
  Node<T>* B = node->left;

  A->left = B->right;
  B->right = A;

  updHeight(A);
  updHeight(B);

  return B;
}

template <typename T>
inline Node<T>* avlTree<T>::leftRotate(Node<T>* node) {
  Node<T>* A = node;
  Node<T>* B = node->right;

  A->right = B->left;
  B->left = A;

  updHeight(A);
  updHeight(B);

  return B;
}

template <typename T>
inline void avlTree<T>::updHeight(Node<T>* node) {
  int8_t leftHeight(heightNode(node->left));
  int8_t rightHeight(heightNode(node->right));

  node->height = ((leftHeight > rightHeight) ? leftHeight : rightHeight) + 1;
}

template <typename T>
inline int8_t avlTree<T>::heightNode(Node<T>* node) {
  return (node != nullptr) ? node->height : -1;
}

template <typename T>
inline Node<T>* avlTree<T>::remover(Node<T>*& node, const T& value) {
  if (node == nullptr) return nullptr;

  if (value > node->key)
    node->right = remover(node->right, value);

  else if (value < node->key)
    node->left = remover(node->left, value);

  else {
    Node<T>* leftNode = node->left;
    Node<T>* rightNode = node->right;

    node->left = nullptr;
    node->right = nullptr;
    delete node;
    --mSize;

    if (rightNode == nullptr) return leftNode;

    Node<T>* minNode = findMinNode(rightNode);

    minNode->right = removeMin(rightNode);
    minNode->left = leftNode;

    return balancing(minNode);
  }

  return balancing(node);
}

template <typename T>
inline Node<T>* avlTree<T>::findMinNode(Node<T>* node) {
  return (node->left) ? findMinNode(node->left) : node;
}

template <typename T>
inline Node<T>* avlTree<T>::removeMin(Node<T>* node) {
  if (node->left == nullptr) return node->right;

  node->left = removeMin(node->left);

  return balancing(node);
}

template <typename T>
inline const Node<T>* avlTree<T>::finder(const Node<T>* node,
                                         const T& value) const {
  if (node == nullptr) return nullptr;

  if (value > node->key) return finder(node->right, value);
  if (value < node->key) return finder(node->left, value);

  return node;
}

template <typename T>
inline Node<T>* avlTree<T>::copyOther(const Node<T>* otherRoot) {
  if (otherRoot == nullptr) return nullptr;

  Node<T>* node =
      new Node<T>(otherRoot->key, otherRoot->height);

  node->left = copyOther(otherRoot->left);
  node->right = copyOther(otherRoot->right);

  return node;
}

template <typename T>
inline void avlTree<T>::printer(Node<T>* node) const {
  if (node == nullptr) return;

  printer(node->left);
  std::cout << node->key << ' ';
  printer(node->right);
}

#endif  // _AVLTREE_HPP_
