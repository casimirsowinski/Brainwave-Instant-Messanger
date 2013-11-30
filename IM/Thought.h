

// Headers
#include <iostream>
#include <string>
using namespace std;

// Init class vars
const int ENTRY_SZ = 512;

class Thought
{
private:
	string	sIP_;				// Sender IP
	string	rIP_;				// Receiver IP
	string	idea_;				// Message text
	int		len_;				// Message length
	bool	outgoing_;			// true: outgoing message, false, incoming message
public:
	// Store the path name of the star cataloge file
	static char filePath_[ENTRY_SZ];
	static int inThoughtCount;
	static int outThoughtCount;

	// Constructors
	Thought();
	Thought(char* s, int length, bool outgoing);
	~Thought();

	// Accessors
	string	GetSIP();
	string	GetRIP();
	string	GetIdea();
	int		GetLen();
	bool	Getoutgoing();

	// Mutators
	void SetSIP(string sender);
	void SetRIP(string receiver);
	void SetIdea(string blurb);			// Sets idea, and calculates numLines too

	void AppendToFile();
};
