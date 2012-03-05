/**
 * This function will simulate the getch() function from conio.h.
 * Note that this does not work for special keys like F1-F12, arrow keys,
 * even the ESC key does not seem to work with this function.
 * @return TCHAR read from input buffer.
 */
TCHAR win_getch() {
	DWORD mode;
	TCHAR theChar = 0;
	DWORD count;
	//get a handle to stdin
	HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	//save the current input mode...
	GetConsoleMode(hStdIn,&mode);
	// Clear the mode, turn off ENABLE_ECHO_INPUT and ENABLE_LINE_INPUT
	// so that the output will not be echoed and will not pause until the end of
	// a line for input.
	SetConsoleMode(hStdIn,0);
	// Read in 1 char from the input buffer.
	ReadConsole(hStdIn,&theChar,sizeof(TCHAR),&count,NULL);
	//restore the current input mode.
	SetConsoleMode(hStdIn, mode);
	return theChar;
}