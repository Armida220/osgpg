#pragma once

class Console
{
public:
	virtual ~Console(void);
	static void Destroy();
	static Console* Instance(bool noCreate=false);

	FILE *fstdout, *fstdin, *fstderr;

	void ShowConsole(bool show);

private:
	static Console* _instance;
	Console(void);
	void Attach(SHORT ConsoleHeight, SHORT ConsoleWidth);
};
