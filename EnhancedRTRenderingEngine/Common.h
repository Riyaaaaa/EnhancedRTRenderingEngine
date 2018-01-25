#pragma once
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#include <string> 

std::string getEnvironmentVariable(const char* environment_name);