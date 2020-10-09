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
	int total = sizeof(text);
	if (logs.size() < 3) {
		text2 = text;
		logs.push_back(text);
		console = false;
	}
}

std::vector<char*> Console::GetLogs()
{
	return logs;
}
