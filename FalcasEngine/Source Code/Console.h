#ifndef __Console_H__
#define __Console_H__

#include <vector>

class Console {
public:
	Console();
	~Console();
	void AddLog(const char*);
	std::vector<char*> GetLogs();


private:
	std::vector<char*> logs;

};
#endif //__Console_H__ 