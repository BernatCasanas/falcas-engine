#include "Console.h"

Console::Console()
{
	logs.push_back("HELLO");
	logs.push_back("IT'S ME");
}

Console::~Console()
{
	while (logs.empty() == false) {
		logs.pop_back();
	}
}

void Console::AddLog(char* text)
{
	logs.push_back(text);
}

std::vector<char*> Console::GetLogs()
{
	return logs;
}
