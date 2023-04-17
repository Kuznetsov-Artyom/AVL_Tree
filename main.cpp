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

	std::cout << tree.find(17) << '\n';
	tree.remove(50);
	tree.remove(75);

	tree.clear();

	return 0;
}