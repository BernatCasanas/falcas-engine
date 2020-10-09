#include "Console.h"
#include "Application.h"

Console::Console()
{
}

Console::~Console()
{

	while (logs.empty() == false) {
		free(logs.back());
		logs.pop_back();
	}
	App->console_active = false;
}

void Console::AddLog(const char* text )
{
	if(App->console_active ==true)
		logs.push_back(strdup(text));
}

std::vector<char*> Console::GetLogs()
{
	if (App->console_active == true)
		return logs;
}
