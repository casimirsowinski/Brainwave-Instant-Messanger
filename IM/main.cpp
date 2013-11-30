/*************************************************************************************
Name: Casimir Sowinski, Ben Pritchard
Date: 
Assignment: IM Application
Description: 
Source: cplusplus.com, microsoft msdn, stackoverflow.com, textbook, wikipedia.org
Notes: 

@	Chat log recording and asking for remote IP are currently disabled for testing

@	Need ability to ESCAPE while asking for remote IP

@	Need to make a default.txt file to store previously connected to remote IP's and
	chat log file path. Ask whether or not to use defaults, update, etc.

@	Need auto capitalization on first letter, to be set off after one erase (maybe)

@	Need message display code for forground/background color, boundary box, position

@	Need to update start screen

*************************************************************************************/

// Headers
#include <windows.h>
#include <iostream>
#include <time.h>
#include <conio.h>
#include <string>
#include <fstream>
#include "LinkedList.h"
#include "Thought.h"
#include "SocketUDPComm.h"
using namespace std;

/*******************************************INTI GLOBAL VARIABLES******************************************************/
// Console 
HANDLE	g_consoleHndl;									// Main console handle
int		g_consoleSzX;									// Console width
int		g_consoleSzY;									// Console height
COORD inputPos = {0, 0};								// global cursor position variable
// LinkedList/Thought 
LinkedList llObj;										// Linked list object
char Thought::filePath_[ENTRY_SZ];						// Init static class vars
int Thought::inThoughtCount;							// Init static class vars
int Thought::outThoughtCount;							// Init static class vars
// Printing
const int MAX_LINES = 3;								// Defines input box height (in # of characters)
const int MAX_CHAR_PER_LINE = 32;						// Defines input box width (in # of characters)
const int MAX_CHAR = MAX_LINES * MAX_CHAR_PER_LINE;		// Maximum message length
// UDP 
const int IPADDR_BUFFER_SIZE = 16;
const int SEND_BUFFER_SIZE = 1024;
int g_remoteSendPort = 55551;
// Chat log
bool g_record = false;									// Tracks whether to generate chat log

/*************************************************PROTOTYPES***********************************************************/
// Console
void SettingCustomScreenAndBuffer(int xSz, int ySz);	//\\ Probably getting rid of this
void SetUpConsoleWindowSize(int xSz, int ySz);			//\\ Probably getting rid of this
// LinkedList/Thought functions
void linkThought(char* s, bool record, bool outgoing);	//\\ GET RID OF record, Creates node and Thought object
// Printing
void SetUpWindow();										// Sets size, prints main interface
void ClearConversationBox();							// Clears the conversation box for printing messages
void printThoughts(const LinkedList* obj);				// Prints the last n messages in LinkedList
void exitMessage();										// Prints goodbye w/ formatting
// UDP
void aquireIP(char[IPADDR_BUFFER_SIZE]);				// Stores remote IP for starting two way comms
int StartTwoWayComm(char remIP[]);						// Processes input/output/error handling
void RcvdDataCallBack(const char* data, const char* remIPAddr);	// Handles incoming messages
// Chat log
bool setUpChatLog();									// Returns true if log is to be kept, records thoughts in timestamped file
void terminateLog();
// Process input
bool getThought(char * s);								// Handles outgoing messages 
char scrubbedChar(char c);								// Sanitizes input characters

/*************************************************FUNCTIONS************************************************************/
void main()
{
	// Init func vars
	g_consoleHndl = GetStdHandle(STD_OUTPUT_HANDLE);	// Main console handle
	char remIP[IPADDR_BUFFER_SIZE];						// Stores remote IP
	
	////------Disabled for easy testing, auto sets recording to no, sets remote IP to value below
	//// Handle recording, if applicable
	//g_record = setUpChatLog();
	//if (g_record)
	//{
	//	cout << "Recording";
	//}
	//else
	//{
	//	cout << "Not recording";
	//}
	//
	//// Collect remote 	
	//aquireIP(remIP);

	strcpy(remIP, "192.168.1.89");	//--------------Replace w/ applicable IP for easy testing

	// Set up Interface		
	SetUpWindow();		
	ClearConversationBox();
	//std::cout << "\a"; // Bell sound
	
	// Main input/output loop
	StartTwoWayComm(remIP);

	// Exit Mesage
	SetUpWindow();
	exitMessage();

	// Pause and exit
	//system("pause");
}

// UDP vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void aquireIP(char ipAddr[IPADDR_BUFFER_SIZE])
{
	// Init cursor
	inputPos = {7, 8};
	SetConsoleCursorPosition(g_consoleHndl, inputPos);

	// Prompt user for, collect, IP address
	std::cout << "Enter an IP address of the machine to which to connect: ";
	std::cin >> ipAddr;
}

int StartTwoWayComm(char remIP[])
{
	// Init func vars	
	CSocketUDPComm senderSocket;						// Object to handle comms
	char str[MAX_CHAR + 1];								// Stores text input to send to remote host
	int retVal = 0;										// Tracks errors
	bool running = true;								// Tracks whether to loop or escape

	try
	{
		// Start listener thread to receive messages from remote host
		senderSocket.StartListenerThread(g_remoteSendPort, RcvdDataCallBack);

		// Use custom socket class to start loop or throw exception	
		if (senderSocket.InitializeSenderSocket(remIP, g_remoteSendPort) != 0)
		{
			// Can't work without this so bypass loop
			throw;
		}
		
		// Main loop for handling input, sending to remote host, printing, exiting
		while (!senderSocket.ShallTerminateNow() && running)
		{
			// Handle input text, returns false for ESCAPE
			running = getThought(str);

			// Store message, add to LinkedList, send message
			if (running)
			{
				linkThought(str, g_record, true);				
				senderSocket.SendData(str);
			}
			
			// Send disconnection message, teminate log if applicable
			if (!running)
			{
				senderSocket.SendData("<remote host disconnected>");

				if (g_record)
				{
					terminateLog();
				}				
			}

			// Update shown messages
			ClearConversationBox();
			printThoughts(&llObj);
		}
	}
	catch (...)
	{

		senderSocket.SetTerminateNow(true);
		std::cout << "Error: exiting" << endl;
		retVal = -1;				
	}

	return retVal;
}

void RcvdDataCallBack(const char* data, const char* remIPAddr)
{
	// Init func vars	
	char message[MAX_CHAR + 1];

	// Grab local copy of message data
	strcpy(message, data);

	// Link thought	
	linkThought(message, g_record, false);
	
	// Update screen and print
	SetUpWindow();
	printThoughts(&llObj);
}
// UDP ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


// Process input vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void linkThought(char* s, bool record, bool outgoing)
{
	// Init func vars
	int length = (int)s[CHAR_MAX];	//\\ Extracts length, maybe replace w/ sizeof()
	
	// Dynamically create new Thought object, link to list
	Thought * newThought = new Thought(s, length, outgoing);	
	llObj.AddLinkToBack(newThought);

	// If recording desired, append to chat log
	if (g_record)
	{
		newThought->AppendToFile();
	}	
}

bool getThought(char * s)
{
	// Init func vars
	char sChar;					// Stores sanitized character input from keyboard
	int curX;					// Cursor x position
	int curY;					// Cursor y position
	COORD inputPos;				// Cursor position
	const int ESCAPE	= 1;	// 1 is outputted from scrubbedChar()
	const int RETURN	= 13;
	const int TAB		= 9;
	const int BACKSPACE = 8;
	int w_ip_min_x = 7;			// Define input field position 
	int w_ip_min_y = 26;		// Define input field position
	bool fwd = true;			// false for backspace, true for all others
	int x = 0;					// Overall position in string
	int l = 0;					// line number
	int p = 0;					// position in line	
	bool inProgress = true;		// Tracks when to send message and when to accept input
	bool retValue = true;		// Tracks whether exiting or sending message, false means exit

	// Flush s[]
	fill(s, s + sizeof(s), ' ');

	// Clear input box	
	for (int iy = 0; iy < MAX_LINES; iy++)
	{
		for (int ix = 0; ix < MAX_CHAR_PER_LINE; ix++)
		{
			inputPos = { w_ip_min_x + ix, w_ip_min_y + iy};
			SetConsoleCursorPosition(g_consoleHndl, inputPos);
			std::cout << " ";
		}
	}

	// Accept input loop
	while (inProgress && x < MAX_CHAR)
	{		
		// Position cursor at beginning of input box
		curX = w_ip_min_x + p;
		curY = w_ip_min_y + l;
		inputPos = { curX, curY };
		SetConsoleCursorPosition(g_consoleHndl, inputPos);
		
		// Wait for input
		while (_kbhit() == 0) {} 

		// Grab input and filter out unwanted characters
		_flushall();		
		sChar = scrubbedChar(_getch());

		// Handle control characters and backspace		
		switch (sChar)
		{
		case ESCAPE:
			inProgress = false;
			retValue = false;			
		case RETURN:
			inProgress = false;			
		case BACKSPACE:
			fwd = false;
			break;	
		default:						
			break;
		} // end switch		
	
		// Handle storing and displaying characters
		if (fwd && inProgress)	// Handles non-backspace characters
		{
			// Update cursor position
			curX = w_ip_min_x + p;
			curY = w_ip_min_y + l;
			inputPos = { curX, curY };
			SetConsoleCursorPosition(g_consoleHndl, inputPos);

			// Store and display captured character
			s[x] = sChar;
			std::cout << s[x];

			// Handle cursor position and character array index			
			if (x < (MAX_CHAR - 1)) // Moves cursor fwd if not at the last spot
			{
				x++;
				p++;
			}

			// Set cursor at beginning of next line
			if (p > (MAX_CHAR_PER_LINE - 1))
			{
				l++;
				p = 0;
			}
		}

		if(!fwd && inProgress) // Handles backspace character
		{
			// Moves cursor back if not at first spot
			if (x > 0)	
			{
				x--;
				p--;
			}

			// Set cursor at end of previous line
			if (p < 0)
			{
				l--;
				p = MAX_CHAR_PER_LINE - 1;
			}
						
			// Set character, update cursor, store character
			s[x] = ' ';
			curX = w_ip_min_x + p;
			curY = w_ip_min_y + l;
			inputPos = { curX, curY };
			SetConsoleCursorPosition(g_consoleHndl, inputPos);
			cout << s[x];

			// Crappy hack to delete bottom rigt character
			if (x > (MAX_CHAR - 5))
			{
				curX = w_ip_min_x + MAX_CHAR_PER_LINE - 1;
				curY = w_ip_min_y + MAX_LINES - 1;
				inputPos = { curX, curY };
				SetConsoleCursorPosition(g_consoleHndl, inputPos);
				cout << ' ';
			}					
		}// endif
				
		// Null terminate string
		s[x + 1] = '\0';
		// Record character count in string
		s[MAX_CHAR] = x;
		// Reset loop vars
		fwd = true;

	} // end while

	return retValue;
}

char scrubbedChar(char c)
{
	// Init func vars
	char sChar = ' ';

	// Backspace
	if (c == 8)
	{
		sChar = c;
	}

	// Tab
	if (c == 9)
	{
		sChar = 33;
	}

	// Return/Enter
	if (c == 13)
	{
		sChar = c;
	}

	// Escape
	if (c == 27)
	{
		sChar = 1;
	}

	// Other printable characters
	if (c >= 32 && c <= 125)
	{
		sChar = c;
	}

	return sChar;
}
// Process input ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


// Chat log vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
bool setUpChatLog()
{
	// Init func vars
	char cRecord;				// Accepts user input on whether or not to record conversation
	bool bRecord;				// Boolean output version of cRecord
	bool haveAnswer = false;	// Whether to loop for an valid responce from user
	time_t now = time(0);		// Records current time
	struct tm tstruct;			// Struct to handle time formatting 
	char fileTime[100];			// Stores current time in convenient character array
	char headerTime[100];		// Stores time in HH:MM:SS
	char filePath[512];			// Stores path for creating log file

	// Store time in fileTime in appropriate format
	tstruct = *localtime(&now);
	strftime(fileTime, sizeof(fileTime), "-%Y-%m-%d, %H_%M_%S", &tstruct);
	strftime(headerTime, sizeof(fileTime), "%A, %d %B %Y, %H:%M:%S", &tstruct);

	// Prompt and accept whether to record/file path
	while (!haveAnswer)
	{
		// Set up cursor, prompt
		inputPos = { 7, 7 };
		SetConsoleCursorPosition(g_consoleHndl, inputPos);
		std::cout << "                  ";
		SetConsoleCursorPosition(g_consoleHndl, inputPos);
		std::cout << "Save chat log? (y/n)";

		// Accept input, convert to lower case
		_flushall();
		cin >> cRecord;
		cRecord = tolower(cRecord);

		// Handle choices
		switch (cRecord)
		{
		case 'y':
			bRecord = true;
			haveAnswer = true;
			break;
		case 'n':
			bRecord = false;
			haveAnswer = true;
			break;
		default:
			haveAnswer = false;
			break;
		}
	}

	// If recording is desired, prompt for path, set filePath_ accordingly
	if (bRecord)
	{
		// Set up cursor clear old invalid responses, prompt, 
		inputPos = { 7, 7 };
		SetConsoleCursorPosition(g_consoleHndl, inputPos);
		std::cout << "                      ";
		SetConsoleCursorPosition(g_consoleHndl, inputPos);
		std::cout << "Enter path to save to:";

		// Accept input
		_flushall();
		cin.getline(filePath, 256);

		// Construct whole string w/ file path, name, and time stamp
		strcpy(Thought::filePath_, filePath);
		strcat(Thought::filePath_, "\\Chat Log");
		strcat(Thought::filePath_, fileTime);
		strcat(Thought::filePath_, ".txt");

		// Write header to file
		ofstream out;
		out.open(Thought::filePath_, ios::app);

		// Check that file opened properly
		if (!out)
		{
			cout << endl << "File header failed";
		}

		// Write header
		out << "\t\tBrainwave Chat Log " << endl;
		out << "Chat began: " << headerTime << endl;
		out << "-------------------------------------------------------------" << endl;

		// Close file, reset flags
		out.close();
		out.clear(std::ios_base::goodbit);
	}

	return bRecord;
}

void terminateLog()	// Write file footer
{
	// Init func vars
	ofstream out;
	time_t now = time(0);		// Records current time
	struct tm tstruct;			// Struct to handle time formattig 
	char fileTime[100];			// Stores current time in convenient character array

	// Store time in fileTime in appropriate format
	tstruct = *localtime(&now);
	strftime(fileTime, sizeof(fileTime), "%A, %d %B %Y, %H:%M:%S", &tstruct);

	// Open file, check that it worked
	out.open(Thought::filePath_, ios::app);

	if (!out)
	{
		std::cout << endl << "File footer failed. :(";
	}

	// Write header
	out << "-------------------------------------------------------------" << endl;
	out << "Chat ended: " << fileTime << endl;

	// Close file, reset flags
	out.close();
	out.clear(std::ios_base::goodbit);
}
// Chat log ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


// Printing vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void exitMessage()
{
	// Print message box/exit message
	inputPos = {14, 12};
	SetConsoleCursorPosition(g_consoleHndl, inputPos);
	std::cout << "ษอออออออออออป";
	inputPos = {14, 13};
	SetConsoleCursorPosition(g_consoleHndl, inputPos);
	std::cout << "บ Good bye! บ";
	inputPos = {14, 14};
	SetConsoleCursorPosition(g_consoleHndl, inputPos);
	std::cout << "ศอออออออออออผ";
	inputPos = {7, 26};
	SetConsoleCursorPosition(g_consoleHndl, inputPos);
}

void SetUpWindow()
{	
	// Prints main interface
	SettingCustomScreenAndBuffer(4, 4);
	SettingCustomScreenAndBuffer(39, 30);
	inputPos = {0, 0};
	SetConsoleCursorPosition(g_consoleHndl, inputPos);
	cout << "ษออออออออออออออออออออออออออออออออออออออป";
	cout << "บ((((((((((((((Brain Wave))))))))))))))บ";
	cout << "บ))))))))))))))))))IM((((((((((((((((((บ";
	cout << "ฬออออออออออออออออออออออออออออออออออออออน";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "ฬออออออออออออออออออออออออออออออออออออออน";
	cout << "บ๎nterณ                                บ";
	cout << "บÿÿÿณÿÿ                              บ";
	cout << "บThinkณ                                บ";
	cout << "ศออออออออออออออออออออออออออออออออออออออผ";		
}

void ClearConversationBox()
{
	inputPos = { 0, 4 };
	SetConsoleCursorPosition(g_consoleHndl, inputPos);
	
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
	cout << "บ                                      บ";
}

void printThoughts(const LinkedList* obj)
{
	// Init func vars
	int numDisplayed = 0;
	Node * tempNode = obj->GetLastNode();
	int yOffset = 21;
	int xPosIncoming = 17;
	int xPosOutgoing = 2;
	int numWantPrint = 5;
	int maxNumtoPrint = 5;
	//char messageBuff[MAX_CHAR + 1];
	string smessageBuff;

	// Determine number of messages to print
	maxNumtoPrint = obj->GetListLength();
	if (numWantPrint > maxNumtoPrint)
	{
		numWantPrint = maxNumtoPrint;
	}

	// Cycle through printing last 
	for (int idx = 0; idx < numWantPrint; idx++)
	{
		// Check if outgoing or incoming
		if (((Thought*)(tempNode->data_))->Getoutgoing()) // true: outgoing
		{
			// Position cursor
			inputPos = { xPosOutgoing, yOffset - 4 * idx };
			SetConsoleCursorPosition(g_consoleHndl, inputPos);
			smessageBuff = ((Thought*)(tempNode->data_))->GetIdea();
			cout << smessageBuff;

		}
		else // False: incoming
		{
			// Position cursor
			inputPos = { xPosIncoming, yOffset - 4 * idx };
			SetConsoleCursorPosition(g_consoleHndl, inputPos);
			smessageBuff = ((Thought*)(tempNode->data_))->GetIdea();
			cout << smessageBuff;
		}

		// Step through list
		tempNode = tempNode->prev_;
	}

	// Reset cursor
	inputPos = { 7, 26 };									// 7 and 26 are magic numbers, maybe replace somehow
	SetConsoleCursorPosition(g_consoleHndl, inputPos);
}
// Printing ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


// Console vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
void SetUpConsoleWindowSize(int xSz, int ySz)
{
    // To get rid of the scroll bars on the console window, first set the
    // size very small, then set it to the desired, larger size.
    SettingCustomScreenAndBuffer(4, 4);
    SettingCustomScreenAndBuffer(xSz, ySz);
}

void SettingCustomScreenAndBuffer(int xSz, int ySz)
{
    // Set up a custom console window - window must be at least one less
    // than buffer size in both x and y dimensions (measured in characters,
    // not pixels).
    // 1. Make screen buffer very big
    // 2. Set window size to desired values
    // 3. Set buffer size to one more than window x and y values

    g_consoleSzX = xSz;
    g_consoleSzY = ySz;
    BOOL result;
    COORD sz = {g_consoleSzX + 1, g_consoleSzY + 1}; // (width, height)
    SMALL_RECT rect = {0, 0, g_consoleSzX, g_consoleSzY}; // (left, top, right, bottom)

    result = SetConsoleScreenBufferSize(g_consoleHndl, sz);
    result = SetConsoleWindowInfo(g_consoleHndl, true, &rect);

	SetConsoleTextAttribute(g_consoleHndl, 128);
}
// Console ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^


