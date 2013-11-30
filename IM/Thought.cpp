
// Headers
#include <iostream>
#include <fstream>
#include <time.h>
#include "Thought.h"
using namespace std;

Thought::Thought()
{
	sIP_ = "";
	rIP_ = "";
	idea_ = "";
	len_ = 0;
}

Thought::Thought(char* s, int length, bool outgoing)
{
	idea_ = s;
	len_ = length;
	outgoing_ = outgoing;

	// Increment appropriate message count tracker
	if (outgoing_)
	{
		outThoughtCount++;
	}
	else
	{
		inThoughtCount++;
	}
	//\\ Might need arguments for SIP/RIP
}

Thought::~Thought()
{

}

bool Thought::Getoutgoing()
{
	return outgoing_;
}

string Thought::GetSIP()
{
	return sIP_;
}

string Thought::GetRIP()
{
	return rIP_;
}

string Thought::GetIdea()
{
	return idea_;
}

int Thought::GetLen()
{
	return len_;
}

void Thought::SetSIP(string sip)
{
	sIP_ = sip;
}

void Thought::SetRIP(string rip)
{
	sIP_ = rip;
}

void Thought::SetIdea(string idea)
{
	idea_ = idea;
}

void Thought::AppendToFile()
{
	// Init func vars
	ofstream out;
	time_t now = time(0);		// Records current time
	struct tm tstruct;			// Struct to handle time formattig 
	char fileTime[100];			// Stores current time in convenient character array

	// Store time in fileTime in appropriate format
	tstruct = *localtime(&now);
	strftime(fileTime, sizeof(fileTime), " -%H:%M:%S", &tstruct);

	//cout << endl << filePath_;
	//system("pause");
	// Open file, set to append mode
	out.open(filePath_, ios::app);
	if (!out)
	{
		cout << endl << "File not open. :(";
	}

	// Write message to file, followed w/ \n
	if (outgoing_)
	{
		out << "outgoing: " << idea_ << fileTime << endl;
	}
	else
	{
		out << "incoming: " << idea_ << fileTime << endl;
	}

	// Close file, reset flags
	out.close();
	out.clear(std::ios_base::goodbit);
}
