#include "ThreadedBST.h"

///-----------------------------------------------
/// Erases all nodes in the tree
/// 
void ThreadedBST::eraseTreeNodes(BSTNode* node) {
	if (node == NULL) return;
	if (node->leftLinkType == CHILD)
	{
		eraseTreeNodes(node->left);				//I changed this function because it gives error.
	}
	if (node->rightLinkType == CHILD)				// this function was written according to BST.
	{
		eraseTreeNodes(node->right);
	}

	delete node;


} //end-eraseTreeNodes

///-----------------------------------------------
/// Adds a given key to the BST
/// 
void ThreadedBST::add(int key) {
	BSTNode* q = NULL;
	BSTNode* p = root;
	while (p != NULL)					//traversing
	{
		q = p;
		if (key == p->key)
		{
			throw std::exception("Duplicate Key !\n");
			return;
		}
		else if (key < p->key) // if key < root moving left subtree
		{
			if (p->leftLinkType == CHILD)
			{
				p = p->left;
			}
			else
			{
				break;
			}
		}
		else				// if key > right moving right subtree
		{
			if (p->rightLinkType == CHILD)
			{
				p = p->right;
			}
			else
			{
				break;
			}
		}
	}// end-while
	BSTNode* node = new BSTNode(key);
	if (root == NULL)  // if tree is null, gives root the key
	{
		root = node;
	}
	else if (key < q->key)
	{
		node->left = q->left;		// establish the linear connection
		node->right = q;
		q->leftLinkType = CHILD;	// q now has a real child
		q->left = node;				// adding node to tree
	}
	else
	{
		node->left = q;				// establish the linear connection
		node->right = q->right;
		q->rightLinkType = CHILD;	// q now has a real child
		q->right = node;			// adding node to tree
	}

} // end-add

///-----------------------------------------------
/// Removes a given key from the BST (if it exists)
/// 
void ThreadedBST::remove(int key) {
	BSTNode* tmp = find(key);

	if (tmp == NULL)
	{
		return;								// if key not foumd, breaking functiom
	}
	if (tmp->leftLinkType == CHILD && tmp->rightLinkType == CHILD)	// if key has 2 real child
	{
		BSTNode* qp = tmp;
		BSTNode* q = tmp->left;

		while (q->rightLinkType == CHILD)	// traversing to find max value in left side
		{									//I could use q.max() but this way is more easy with 2 pointer.
			qp = q;
			q = q->right;
		}
		tmp->key = q->key;

		if (q->leftLinkType == CHILD)		//if max value has a real child in left node
		{
			qp->right = q->left;
			q->left->right = q->right;
		}
		else if (q->rightLinkType == CHILD)	// if max value has a real child in right node
		{
			qp->right = q->right;
			q->right->left = q->left;
		}
		else								//if max value has no a real child
		{
			qp->right = q->right;
		}
		tmp = q;
	}
	else if (tmp->leftLinkType == CHILD && tmp->rightLinkType == THREAD)	//operations to rotate nodes if key has a real children in left side
	{
		if (tmp->right != NULL && tmp->right->left == tmp)
		{
			tmp->right->left = tmp->left;
			tmp->left->left = tmp->left;
		}
		else
		{
			tmp->left->left->right = tmp->left;
			tmp->left->right = tmp->right;
		}
	}
	else if (tmp->leftLinkType == THREAD && tmp->rightLinkType == CHILD)	//operations to rotate nodes if key has a real children in right side
	{
		if (tmp->left != NULL && tmp->left->right == tmp)
		{
			tmp->left->right = tmp->right;
			tmp->right->left = tmp->left;
		}
		else
		{
			tmp->right->right->left = tmp->right;
			tmp->right->left = tmp->left;
		}
	}
	else															//operations to rotate nodes if key have no real children
	{
		if (tmp->left != NULL && tmp->left->right == tmp)
		{
			tmp->left->right = tmp->right;
			tmp->left->rightLinkType = THREAD;
		}
		else if (tmp->right != NULL && tmp->right->left == tmp)
		{
			tmp->right->left = tmp->left;
			tmp->right->leftLinkType = THREAD;
		}
	}
	delete tmp;
} // end-remove

///-----------------------------------------------
/// Searches a given key in the ThreadedBST
/// Return a pointer to the node that holds the key
/// If the key is not found, return NULL
/// 
BSTNode* ThreadedBST::find(int key) {
	BSTNode* tmp = root;
	while (tmp)
	{
		if (key == tmp->key)
		{
			return tmp;							//retun pointer when found
		}
		else if (key < tmp->key)
		{
			if (tmp->leftLinkType == CHILD)		// if key < root and root has a real child, traverse left side
			{
				tmp = tmp->left;
			}
			else
			{
				break;
			}
		}
		else
		{
			if (tmp->rightLinkType == CHILD)	// if key > root and root has a real child, traverse right side
			{
				tmp = tmp->right;
			}
			else
			{
				break;
			}
		}
	}
	return NULL;							// return null when not found
} // end-find

///-----------------------------------------------
/// Returns the minimum key in the ThreadedBST
/// Return a pointer to the node that holds the key
/// If the key is not found, return NULL
/// 
BSTNode* ThreadedBST::min() {
	BSTNode* tmp = root;
	while (tmp->left != NULL)
	{
		tmp = tmp->left;
	}
	return tmp;
} // end-min

///-----------------------------------------------
/// Returns the maximum key in the ThreadedBST
/// Return a pointer to the node that holds the key
/// If the key is not found, return NULL
/// 
BSTNode* ThreadedBST::max() {
	BSTNode* tmp = root;
	while (tmp->right != NULL)
	{
		tmp = tmp->right;
	}
	return tmp;
} // end-max

///-----------------------------------------------
/// Given a valid pointer to a node in ThreadedBST,
/// returns a pointer to the node that contains the inorder predecessor
/// If the inorder predecessor does not exist, returns NULL
/// 
BSTNode* ThreadedBST::previous(BSTNode* node) {
	BSTNode* tmp = node;
	if (node->leftLinkType == THREAD)
	{
		tmp = node->left;						//traversing it to the left while node  has no a real child
	}
	else
	{
		tmp = tmp->left;
		while (tmp->rightLinkType != THREAD)	// if node has a real childe, traverse it ovce to the left, 
		{										//after than traverse it to right until has no a real child.
			tmp = tmp->right;
		}
	}
	return tmp;
} // end-previous

///-----------------------------------------------
/// Given a valid pointer to a node in the ThreadedBST,
/// returns a pointer to the node that contains the inorder successor
/// If the inorder successor does not exist, returns NULL
/// 
BSTNode* ThreadedBST::next(BSTNode* node) {
	BSTNode* tmp = node;

	if (node->rightLinkType == THREAD)
	{
		tmp = node->right;   //traversing it to the right while node  has no a real child
	}
	else
	{
		tmp = tmp->right;
		while (tmp->leftLinkType != THREAD)	// if node has a real childe, traverse it ovce to the right, 
		{									//after than traverse it to left until has no a real child.
			tmp = tmp->left;
		}
	}
	return tmp;
} // end-next