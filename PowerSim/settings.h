#ifndef __WS_SETTINGS_MANAGER__
#define __WS_SETTINGS_MANAGER__

#include "utility.h"

class Settings
{
public:
	static int LoadConfigFile()
	{
		ifstream myfile ("config.txt");

		if (myfile.is_open())
		{
			//Let's read all the lines
			std::string line;
			while ( getline (myfile,line) )
			{
				if(line.length() >1)
				{
					//Skipping comments
					if(line[0] == '/' && line[1] == '/')
					{
						continue;
					}

					//Tokenizing 
					std::string index;
					std::string value;
					for (int c = 0; c < line.length(); c++)
					{
						if(line[c] = '=')
						{
							//Here's the value of our setting
							for (int q = c++; q < line.length(); q++)
							{
								if(line[q]!=';')
									value+=line[q];
							}
							break;
						}
						else
						{
							//We have yet to find the '=' denoting the stop of the index and the beginning of the value
							index+= line[c];
						}
					}
					settings[index] = value;
					cout<<"Setting : "<<index<<" : " << value << endl;
				}
			}
			myfile.close();
			cout<<"Config file read successfuly!"<<endl;
			return 0;
		}
		else
		{ 
			cout << "Config file read unsucessfully."<<endl;
			return -1;//Oh shit wheres the file
		}
	};

	static std::string GetSetting(std::string myKey)
	{
		return settings[myKey];
	};

private:
	static unordered_map<std::string,std::string> settings;
};

#endif