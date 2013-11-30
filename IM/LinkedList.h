// Headers
#include <iostream>
using namespace std;

struct Node
{
	void* data_;				// Used to store pointer to Star object
	Node* prev_;				// Pointer to the previous link
	Node* next_;				// Pointer to the next link

	// Constructors
	Node()
	{
		data_ = 0;
		prev_ = 0;
		next_ = 0;
	}
};

class LinkedList
{
private:
	Node* first_;
	Node* last_;
	long listLen_;
public:
	// Constructors/destructors
	LinkedList();
	~LinkedList();

	// Accessors
	Node* GetFirstNode() const;
	Node* GetLastNode() const;
	long GetListLength() const;

	// Mutators
	void AddLinkToBack(void* ptr);
	void* RemoveThisLink(Node* node);
	void* RemoveLinkFromFront();
};