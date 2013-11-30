// Headers
#include <iostream>
#include "LinkedList.h"
using namespace std;

// Constructor
LinkedList::LinkedList()
{
	first_	 = 1;
	last_	 = 0;
	listLen_ = 0;
}

// Destructor
LinkedList::~LinkedList()
{

}

// Accessors
Node* LinkedList::GetFirstNode() const
{
	return first_;
}

Node* LinkedList::GetLastNode() const
{
	return last_;
}

long LinkedList::GetListLength() const
{
	return listLen_;
}

// Mutators
void LinkedList::AddLinkToBack(void* ptr)
{
	// Create new Node, populate its data
	Node * newNode = new Node;
	newNode->data_ = ptr;

	// If first Node, point first_/last_ to it
	if(!listLen_)
	{
		first_ = newNode;
		last_ = newNode;
	}
	else // Not the first
	{
		last_->next_ = newNode;
		newNode->prev_ = last_;
		last_ = newNode;
	}	
	listLen_++;
}

void* LinkedList::RemoveThisLink(Node* node)
{	
	// Init func vars
	void* retDataNode = node->data_;
	Node* tempNode = node;
	Node* tempPrev = tempNode->prev_;
	Node* tempNext = tempNode->next_;

	if (listLen_ > 1)
	{
		// If first node set next node to first_, nullify prev_
		if (tempPrev == NULL)
		{
			first_ = first_->next_;	// Set first_ to new first
			first_->prev_ = NULL;
		}
		else if (tempNext == NULL)// If last node set previous node to last_, Nullify next_
		{
			last_ = last_->prev_;
			last_->next_ = NULL;
		}
		else // Stitch together both sides around Node to be removed
		{
			tempPrev->next_ = tempNext;
			tempNext->prev_ = tempPrev;
		}
	}
	else // If only element, set everything to NULL
	{
		first_ = NULL;
		last_ = NULL;
		tempPrev = NULL;
		tempNext = NULL;
	}
		
	// Delete node, dec listLen_ and return Song pointer for deletion
	delete node;
	listLen_--;
	return retDataNode;	
}

void* LinkedList::RemoveLinkFromFront()
{
	// Store pointer to second Node
	Node * tempNode = first_->next_;
	
	// Delete current first Node
	delete first_;

	// Set first_ to previous second Node
	first_ = tempNode;

	// Dec listLen_
	listLen_--;

	return first_;
}

