#include "AST.h"

void AST::preOrder(const std::shared_ptr<Node> &node)const
{
	this->print(node);
	if (node->left != nullptr) preOrder(node->left);
	if (node->right != nullptr) preOrder(node->right);
}

void AST::inOrder(const std::shared_ptr<Node> &node)const
{
	if (node->left != nullptr) inOrder(node->left);
	this->print(node);
	if (node->right != nullptr) inOrder(node->right);
}
void AST::postOrder(const std::shared_ptr<Node> &node)const
{
	if (node->left != nullptr) postOrder(node->left);
	if (node->right != nullptr) postOrder(node->right);
	this->print(node);
}
void AST::print(const std::shared_ptr<Node> &node)const
{
	if (node->value->isSymbol())
		std::cout << node->value->getSymbolValue();
	else std::cout << node->value->getNumberValue();
}
