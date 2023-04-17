#include <iostream>
#include "avltree.hpp"






int main(int argc, char* argv[], char** env)
{	
	avlTree<int> tree;

	std::cout << tree.find(100) << '\n';


	while (true)
	{
		int value{};

		std::cout << "input value new node: ";
		std::cin >> value;

		if (value == -1) break;
		

		tree.insert(value);
		std::cout << tree.size() << '\n';
	}

	tree.print();

	avlTree<int> treeTwo(tree);

	while (true)
	{
		int value{};

		std::cout << "input value new node: ";
		std::cin >> value;

		if (value == -1) break;

		tree.insert(value);
	}

	treeTwo = tree;

	treeTwo.print();

	tree.clear();

	return 0;
}