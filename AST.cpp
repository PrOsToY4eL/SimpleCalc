#include "AST.h"

void AST::addNode(std::shared_ptr<Node> &node)
{
	node->right = std::move(root);
	root = std::move(node->parent);
}