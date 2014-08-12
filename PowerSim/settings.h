#ifndef __WS_SETTINGS_MANAGER__
#define __WS_SETTINGS_MANAGER__

#include "utility.h"

class Settings
{
public:
	static int LoadConfigFile();

	static std::string GetSetting(std::string myKey);

private:
	static unordered_map<std::string,std::string> settings;
};

#endif