#ifndef __PS_UTILITY__
#define __PS_UTILITY__

#include <iostream>
#include <fstream>
using namespace std;

#include <cstdlib>
#include <cmath>
#include <cstring>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <stdexcept>
#include <sstream>

namespace ws_utility
{
	//Random Numbers
	int RandomNumberBetween(int myLowerIncusive, int myUpperExclusive);

	bool DoesFileExist(std::string myFileName);
	
	void GetFile(std::string myFileName);

	void CreateFile(std::string myFileName, std::string myContents);

	std::string CreateGibberishName(int myNumberLetters);

	//Config file management
}

#endif