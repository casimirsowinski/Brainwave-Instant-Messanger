// Can recommend creating two linked lists, copied from previous homeworks. One for
// received messages and one for outgoing messages. Store each message in a linked    
// list and when a new one comes in can delete the oldest and reprint all of them
// to give the effect of scrolling. Can print each list in a different color,
// preceded by user name, in different sections of the console window
// (left/right, top/bottom, etc.)


/*
//HWND hWnd = GetConsoleWindow();
//MoveWindow(hWnd, 100, 100, 2000, 500, TRUE);

// Create a small console
SetUpConsoleWindowSize(26, 30);
LeftRightDemo();
system("pause");

// Clear the console window
system("cls");

// Create a larger console
SetUpConsoleWindowSize(56, 60);
LeftRightDemo();
system("pause");
*/

/* crap_02
if (cnt < 33)
{
lineNum = 0;
}
else if (cnt >= 33 && cnt < 65)
{
lineNum = 1;
}
else
{
lineNum = 2;
}
*/

/*	// Old order of cursor handling, from getThought()
// Prepare cursor, print character
if (fwd && inProgress)
{
curX = w_ip_min_x + p;
curY = w_ip_min_y + l;
inputPos = { curX, curY };
SetConsoleCursorPosition(g_consoleHndl, inputPos);
s[x] = sChar;
std::cout << s[x];
}

// Handle x
if (fwd)
{
if (x < (MAX_CHAR - 1)) // Moves cursor fwd if not at the last spot
{
x++;
p++;
}
}
else
{
if (x > 0)	// Moves cursor back if not at first spot
{
x--;
p--;
}
}

if (p > (MAX_CHAR_PER_LINE - 1))
{
l++;
p = 0;
}

if (p < 0)
{
l--;
p = MAX_CHAR_PER_LINE - 1;
}

if (!fwd)
{
//cout << ' ';
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

}

*/

/*
bool getInput(string &s)
{
// Init func vars
char c;
s = "";
int cnt = 0;
bool swap = true;
COORD inputPos;
int curX = 7;
int curY = 26;

while (true)
{
c = getch();

if (c == ESCAPE)
{
s = "Good bye!";
return false;
}

if (c == RETURN)
{
//s = "(ready for sending)";
//sendMessage(s);
return true;
}

if (c == TAB)
{
cnt += 3;
curX += 3;

}

if (c == BACKSPACE)
{

}

// word wrap
if (cnt == 32 && swap)
{
inputPos = { 7, 27 };
SetConsoleCursorPosition(g_consoleHndl, inputPos);
swap = false;
}
if (cnt == 64 && !swap)
{
inputPos = { 7, 28 };
SetConsoleCursorPosition(g_consoleHndl, inputPos);
swap = true;
}

std::cout.put(c) << flush;

// Keep the message w/i 95 characters
if (cnt < 95)
{
cnt++;

s += c;
}
else // If 95th char, reset last char w/ c
{
inputPos = { 38, 28 };
SetConsoleCursorPosition(g_consoleHndl, inputPos);
s[94] = c;
}

}
}
*/


/*
bool getInput(string &s)
{
// Init func vars
char c;
s = "";
int cnt = 0;
bool swap = true;
COORD inputPos;
int curX = 7;
int curY = 26;

while (true)
{
c = getch();

if (c == ESCAPE)
{
s = "Good bye!";
return false;
}

if (c == RETURN)
{
//s = "(ready for sending)";
//sendMessage(s);
return true;
}

if (c == TAB)
{
cnt += 3;
curX += 3;

}

if (c == BACKSPACE)
{

}

// word wrap
if (cnt == 32 && swap)
{
inputPos = { 7, 27 };
SetConsoleCursorPosition(g_consoleHndl, inputPos);
swap = false;
}
if (cnt == 64 && !swap)
{
inputPos = { 7, 28 };
SetConsoleCursorPosition(g_consoleHndl, inputPos);
swap = true;
}

std::cout.put(c) << flush;

// Keep the message w/i 95 characters
if (cnt < 95)
{
cnt++;

s += c;
}
else // If 95th char, reset last char w/ c
{
inputPos = { 38, 28 };
SetConsoleCursorPosition(g_consoleHndl, inputPos);
s[94] = c;
}

}
}
*/


//void SetUpConsoleWindowSize(int xSz, int ySz)
//{
//	// To get rid of the scroll bars on the console window, first set the
//	// size very small, then set it to the desired, larger size.
//	SettingCustomScreenAndBuffer(4, 4);
//	SettingCustomScreenAndBuffer(xSz, ySz);
//}
//
//void LeftRightDemo()
//{
//	// Declare needed variables
//	CONSOLE_SCREEN_BUFFER_INFO screenBuff;
//
//	// Set the cursor position (x, y) or (col, row). Note that these are not
//	// pixel positions but character positions.
//	COORD pos1 = { 1, g_consoleSzX - 8 };
//	COORD pos2 = { g_consoleSzX / 2, g_consoleSzX / 4 };
//
//	// Set the color to bright white. Can use different combinations of these three
//	// enums to get different colors.
//	SetConsoleTextAttribute(g_consoleHndl, FOREGROUND_RED | FOREGROUND_GREEN |
//		FOREGROUND_BLUE | FOREGROUND_INTENSITY);
//
//	// Get and store the current cursor position so can restore later
//	GetConsoleScreenBufferInfo(g_consoleHndl, &screenBuff);
//
//	// Print text to specified position, using current color
//	SetConsoleCursorPosition(g_consoleHndl, pos1);
//	cout << "(" << pos1.X << "," << pos1.Y << ")";
//
//	// Now put the cursor back where it was
//	SetConsoleCursorPosition(g_consoleHndl, screenBuff.dwCursorPosition);
//
//	// Set the color to bright green
//	SetConsoleTextAttribute(g_consoleHndl, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
//
//	SetConsoleCursorPosition(g_consoleHndl, pos2);
//	cout << "(" << pos2.X << "," << pos2.Y << ")";
//
//	// Writes a character string to a console screen buffer beginning at
//	// the current cursor location.
//	//WriteConsole
//
//	// Sets the title for the current console window.
//	//SetConsoleTitle
//
//	// Set the cursor position back to the beginning (top-left)
//	COORD pos3 = { 0, 0 };
//	SetConsoleCursorPosition(g_consoleHndl, pos3);
//	// Set the color to bright purple (blue and red)
//	SetConsoleTextAttribute(g_consoleHndl, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
//}
//
//void SettingCustomScreenAndBuffer(int xSz, int ySz)
//{
//	// Set up a custom console window - window must be at least one less
//	// than buffer size in both x and y dimensions (measured in characters,
//	// not pixels).
//	// 1. Make screen buffer very big
//	// 2. Set window size to desired values
//	// 3. Set buffer size to one more than window x and y values
//
//	g_consoleSzX = xSz;
//	g_consoleSzY = ySz;
//	BOOL result;
//	COORD sz = { g_consoleSzX + 1, g_consoleSzY + 1 }; // (width, height)
//	SMALL_RECT rect = { 0, 0, g_consoleSzX, g_consoleSzY }; // (left, top, right, bottom)
//
//	result = SetConsoleScreenBufferSize(g_consoleHndl, sz);
//	result = SetConsoleWindowInfo(g_consoleHndl, true, &rect);
//}
