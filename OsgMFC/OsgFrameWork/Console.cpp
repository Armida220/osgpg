#include "StdAfx.h"
#include "Console.h"

#include < process.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>
#include <Windows.h>


Console* Console::_instance = 0;

Console::Console(void)
{
	fstdin = fstdout = fstderr = 0;
	AllocConsole();
	Attach(300,80);
}

Console::~Console(void)
{
	FreeConsole();
}

void Console::Attach(SHORT ConsoleHeight, SHORT ConsoleWidth)
{
	HANDLE  g_hConsoleOut;                   // Handle to debug console
	int                        hConHandle;
	long                       lStdHandle;
	FILE                       *fp;

	// redirect unbuffered STDOUT to the console
	g_hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fstdout = fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

	// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fstdout = fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fstderr = fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );

	::SetConsoleTitle("OsgFrameWork Console");
}

void Console::ShowConsole(bool show)
{
	::ShowWindow(::GetConsoleWindow(), show);
}

Console* Console::Instance(bool noCreate)
{
	if(_instance==0 && !noCreate) {
		_instance = new Console;
	}
	return _instance;
}

void Console::Destroy()
{
	if(_instance) {
		delete _instance;
	}
	_instance = 0;
}