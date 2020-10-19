#pragma once
#ifndef _Asap_
#define _Asap_
#include "Module.h"
#include "Globals.h"


class Application;
class ImVec4;

class Mesh
{
public:
	Mesh();
	
	uint id_index = 0; 
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; 
	uint num_vertex = 0;
	float* vertex = nullptr;

};


#endif // !_Asap_
