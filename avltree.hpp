#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP


#include <cstdint>
#include <stack>



template<typename T>
struct Node
{
	T key;
	int8_t height;

	Node<T>* left;
	Node<T>* right;

	Node(T keyValue, T height = 0) : key{ keyValue }, height{ 0 }, left{ nullptr }, right{ nullptr } {}
};





template<typename T>
class avlTree
{
private:
	Node<T>* mRoot;
	size_t mSize;

public:
	avlTree() : mRoot{ nullptr }, mSize{ 0 } {}
	avlTree(const avlTree& other) : avlTree()
	{
		if (other.empty()) return;


		// TODO


	}


	~avlTree() { clear(); }



	void insert(const T& value)
	{
		std::stack<Node<T>*> way;
		Node<T>* newNode = inserter(mRoot, value, way);

		if (newNode == nullptr) return;

		while (!way.empty())
		{
			Node<T>* node = way.top();
			way.pop();

			if (!way.empty())
			{
				Node<T>* parentNode = way.top();

				if (node->key > parentNode->key)
					parentNode->right = balancing(node);
				else parentNode->left = balancing(node);
			}
			else mRoot = balancing(node);
		}
	}
	void remove(const T& value)
	{
		if (empty()) return;
		mRoot = remover(mRoot, value);
	}


	size_t size() const noexcept { return mSize; }
	bool empty() const noexcept { return mRoot == nullptr; }
	bool find(const T& value) const
	{
		return finder(mRoot, value) != nullptr;
	}
	void clear()
	{
		if (empty()) return;

		std::stack<Node<T>*> way;

		clearAll(mRoot, way);
		mSize = 0;
	}


	avlTree& operator = (const avlTree& other)
	{
		// TODO
	}





private:
	Node<T>* inserter(Node<T>*& node, const T& value, std::stack<Node<T>*>& way)
	{
		if (node == nullptr)
		{
			node = new Node<T>(value);
			++mSize;
			return node;
		}

		way.push(node);

		if (value < node->key) return inserter(node->left, value, way);
		if (value > node->key) return inserter(node->right, value, way);

		return nullptr;
	}
	Node<T>* balancing(Node<T>* node)
	{
		updHeight(node);
		int8_t balance = balanceFactor(node);

		if (balance == -2)
		{
			if (balanceFactor(node->left) > 0)
				node->left = leftRotate(node->left);
			return rightRotate(node);
		}
		if (balance == 2)
		{
			if (balanceFactor(node->right) < 0)
				node->right = rightRotate(node->right);
			return leftRotate(node);
		}

		return node;
	}
	int8_t balanceFactor(Node<T>* node)
	{
		return (heightNode(node->right) + 1) - (heightNode(node->left) + 1);
	}
	Node<T>* rightRotate(Node<T>* node)
	{
		Node<T>* A = node;
		Node<T>* B = node->left;

		A->left = B->right;
		B->right = A;

		updHeight(A);
		updHeight(B);

		return B;
	}
	Node<T>* leftRotate(Node<T>* node)
	{
		Node<T>* A = node;
		Node<T>* B = node->right;

		A->right = B->left;
		B->left = A;

		updHeight(A);
		updHeight(B);

		return B;
	}
	void updHeight(Node<T>* node)
	{
		int8_t leftHeight(heightNode(node->left));
		int8_t rightHeight(heightNode(node->right));

		node->height = ((leftHeight > rightHeight) ? leftHeight : rightHeight) + 1;
	}
	int8_t heightNode(Node<T>* node)
	{
		return (node != nullptr) ? node->height : -1;
	}


	Node<T>* remover(Node<T>*& node, const T& value)
	{
		if (node == nullptr)
			return nullptr;

		if (value > node->key)
			node->right = remover(node->right, value);

		else if (value < node->key)
			node->left = remover(node->left, value);

		else
		{
			Node<T>* leftNode = node->left;
			Node<T>* rightNode = node->right;

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
	Node<T>* findMinNode(Node<T>* node)
	{
		return (node->left) ? findMinNode(node->left) : node;
	}
	Node<T>* removeMin(Node<T>* node)
	{
		if (node->left == nullptr)
			return node->right;

		node->left = removeMin(node->left);

		return balancing(node);
	}


	void clearAll(Node<T>*& node, std::stack<Node<T>*>& way)
	{
		if (node->left != nullptr)
		{
			way.push(node);
			clearAll(node->left, way);
			return;
		}
		if (node->right != nullptr)
		{
			way.push(node);
			clearAll(node->right, way);
			return;
		}
		if (node == mRoot)
		{
			delete mRoot;
			mRoot = nullptr;
			return;
		}

		Node<T>* lastNode = way.top();
		way.pop();

		if (lastNode->left == node)
		{
			delete node;
			lastNode->left = nullptr;
		}
		else
		{
			delete node;
			lastNode->right = nullptr;
		}

		clearAll(lastNode, way);
	}
	Node<T>* finder(Node<T>* node, const T& value)
	{
		if (node == nullptr)
			return nullptr;

		if (value > node->key)
			return finder(node->right, value);

		if (value < node->key)
			return finder(node->left, value);

		return node;
	}
	Node<T>* copyOther(Node<T>*& thisRoot,const Node<T>*& otherRoot)
	{
		// TODO
	}
};

#endif // AVL_TREE_HPP
