#ifndef _Asap_
#define _Asap_
#include "Module.h"
#include "Globals.h"


class Application;
class ImVec4;

class Asap : public Module
{
public:
	Asap(Application* app, bool start_enabled = true);

	bool Init();
	bool CleanUp();

	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);

private:

};


#endif // !_Asap_
