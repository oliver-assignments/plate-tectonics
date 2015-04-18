#include "settings.h"

unordered_map<std::string,std::string> Settings::settings;

int Settings::LoadConfigFile()
{
	ifstream myfile ("config.txt");

	int setting_number = 1;

	std::cout<<"Reading the config file."<<endl;

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
					char character= line[c];
					if(line[c] == '=')
					{
						//Here's the value of our setting
						for (int q = ++c; q < line.length(); q++)
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
				/*std::cout<<setting_number<<" : "<<index<<" = " << value << endl;
				if(setting_number%5==0 && setting_number!=0)
					std::cout<<endl;
				setting_number++;*/
				
			}
		}
		myfile.close();
		std::cout<<"Config file read successfuly!"<<endl;
		return 0;
	}
	else
	{ 
		std::cout << endl << "Config file read unsucessfully.";
		return -1;//Oh shit wheres the file
	}
};

std::string Settings::GetSetting(std::string myKey)
{
	return settings[myKey];
};