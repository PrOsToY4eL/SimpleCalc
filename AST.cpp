#include "AST.h"

void AST::preOrder(const std::shared_ptr<Node> &node)const
{
	this->print(node);
	if (node->left != nullptr) preOrder(node->left);
	if (node->right != nullptr) preOrder(node->right);
}

void AST::inOrder(const std::shared_ptr<Node> &node)const
{
	if (node == nullptr)
		return;
	else
	{
		if (node->left != nullptr)
		{
			if (node->value->isSymbol() && node->left->value->isSymbol() && (node->value->priority() > node->left->value->priority()))
			{
				std::cout << "(";
				inOrder(node->left);
				std::cout << ")";
			}
			else
				inOrder(node->left);
		}
		this->print(node);
		if (node->right != nullptr)
		{
			if (node->value->isSymbol() && node->right->value->isSymbol() && (node->value->priority() > node->right->value->priority()))
			{
				std::cout << "(";
				inOrder(node->right);
				std::cout << ")";
			}
			else
				inOrder(node->right);
		}
	}
}
void AST::postOrder(const std::shared_ptr<Node> &node)const
{
	if (node == nullptr)
		return;
	if (node->left != nullptr) postOrder(node->left);
	if (node->right != nullptr) postOrder(node->right);
	this->print(node);
}
void AST::print(const std::shared_ptr<Node> &node)const
{
	if (node == nullptr)
		return;
	if (node->value->isSymbol())
		std::cout << node->value->getSymbolValue();
	else std::cout << node->value->getNumberValue();
}

auto AST::copy(const std::shared_ptr<Node> &node)-> std::shared_ptr<Node>const
{
	if (node != nullptr)
	{
		std::shared_ptr<Node> nodeCopy{ node };
		if (node->left != nullptr)
			nodeCopy->left = copy(node->left);
		if (node->right != nullptr)
			nodeCopy->right = copy(node->right);
		return nodeCopy;
	}
	else
		return nullptr;
}


void AST::simplify(std::shared_ptr<Node> &root)
{
	if (root->value->isSymbol() && root->left->value->isSymbol())
		if (root->value->priority() > root->left->value->priority())
		{
			std::shared_ptr<Node> newRoot{ copy(root->left) };
			newRoot->parent.reset();
			root->left.reset();
			std::shared_ptr<Node> newLeft{ std::make_shared<Node>(*copy(root)) };
			std::shared_ptr<Node> newRight{ std::make_shared<Node>(*copy(root)) };

			newLeft->left = copy(newRoot->left);
			newRoot->left.reset();

			newRight->left = copy(newRoot->right);
			newRoot->right.reset();

			newRoot->left = newLeft;
			newRoot->right = newRight;
			this->root = newRoot;
		}

	if (root->value->isSymbol() && root->right->value->isSymbol())
		if (root->value->priority() > root->right->value->priority())
		{
			std::shared_ptr<Node> newRoot{ copy(root->right) };
			newRoot->parent.reset();//improve
			root->right.reset();
			std::shared_ptr<Node> newLeft{ std::make_shared<Node>(*copy(root)) };
			std::shared_ptr<Node> newRight{ std::make_shared<Node>(*copy(root)) };

			newLeft->right = copy(newRoot->left);
			newRoot->left.reset();

			newRight->right = copy(newRoot->right);
			newRoot->right.reset();

			newRoot->left = newLeft;
			newRoot->right = newRight;
			this->root = newRoot;
		}

}
