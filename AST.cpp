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

auto AST::copy(const std::shared_ptr<Node> &node)-> std::shared_ptr<Node>const
{
	auto nodeCopy{ node };
	if (node->left != nullptr)
		nodeCopy->left = copy(node->left);
	if (node->right != nullptr)
		nodeCopy->right = copy(node->right);
	return nodeCopy;
}

void AST::simplify(std::shared_ptr<Node> &root)
{
	if (root->value->priority() > root->left->value->priority())
	{
		std::shared_ptr<Node> newRoot{ std::move(root->right) };
		newRoot->parent = nullptr;
		std::shared_ptr<Node> newLeft{ copy(root) };
		std::shared_ptr<Node> newRight{ copy(root) };
		newLeft->right = std::move(newRoot->left);
		newRight->right = std::move(newRoot->right);
		newRoot->left = std::move(newLeft);
		newRoot->right = std::move(newRight);
		this->root = std::move(newRoot);
	}

}
