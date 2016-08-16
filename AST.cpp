#include "AST.h"

template<typename ValueType>
void AST<ValueType>::addNode(std::unique_ptr<Node<ValueType>> &node)
{
	node->right = std::move(root);
	root = std::move(node->parent);
}