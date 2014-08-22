#include "utility.h"

int RandomNumberBetween(int myLowerInclusive, int myUpperExclusive)
{
	return myLowerInclusive + rand()%(myUpperExclusive-myLowerInclusive);

};

int RandomNumberBelow(int myUpperExclusive)
{
	return (rand()%myUpperExclusive);
};

std::string CreateGibberishName(int myNumberLetters)
{
	std::string name ("");
	for (int i = 0; i < myNumberLetters; i++)
	{
		if(i%2==0)//We want a consonant
		{
			int index = 0;

			//If you found a vowel keep looking
			while (index==0 || index ==4 || index==8 || index==14 || index ==20)
			{
				index = RandomNumberBetween(0,26);
			}

			//We found our consonant
			char next_letter = index;

			//If its the first letter make it capital
			if(i==0)
				toupper(next_letter);

			//Add the character
			name+= next_letter;

			//If its a q add the u
			if(next_letter == 'q')
			{
				name+="u";
			}
		}
		else//We are looking for a vowel
		{
			int index = 1;

			//If you found a vowel keep looking
			while (index!=0 && index!=4 && index!=8 && index!=14 && index!=20)
			{
				index = RandomNumberBetween(0,26);
			}

			//We found our consonant
			char next_letter = index;

			//Add the character
			name+= next_letter;
		}
	}
	return name;
};