#include "allegro_engine.h"

void AllegroEngine::InitializeScreen(int myScreenWidth, int myScreenHeight)
{
	screen_width = myScreenWidth;
	screen_height = myScreenHeight;

	//Creating screen
	ALLEGRO_DISPLAY* display = al_create_display(myScreenWidth, myScreenWidth);
	al_set_window_position(display,0,0);
	al_set_window_title(display,"World Simulation");//CreateName(7).c_str());

	main_display = display;
};

void AllegroEngine::DrawTextC(std::string myText,
							 int myXDestination, int myYDestination, 
							 ALLEGRO_FONT* myFont, std::string myAlignment)
{
	const char * converted_text = myText.c_str();
	al_draw_text(myFont,al_map_rgb(100,100,100), myXDestination, myYDestination,0, converted_text);

};

int AllegroEngine::FlushScreenshot(const char *destination_path, const char *myWorldName,int myCurrentYear,int myCurrentMonth, int myCurrentDay)
{
	ALLEGRO_PATH *path;
	char *filename, *filename_wp;
	const char *path_cstr;

	//Testing the destination given
	if(!destination_path)
		path = al_get_standard_path(ALLEGRO_USER_DOCUMENTS_PATH);
	else
		path = al_create_path_for_directory(destination_path);

	if(!path)
		return -1;//Where we want it is bad

	if(!myWorldName) {
		if( !(myWorldName = al_get_app_name()) ) {
			al_destroy_path(path);
			return -2;//We cant use app name or the given name
		}
	}

	//Length of gamename + length of ":YYYY-MM-DD" + NULL terminator
	if ( !(filename_wp = filename = (char *)malloc(strlen(myWorldName)+ 1 + 8 + 1 + 1 + 1)) ) {
		al_destroy_path(path);
		return -4;//We don't have enough space?
	}

	//Creating the file name
	std::string date = myCurrentYear+"/"+myCurrentMonth;
	date += "/" + myCurrentDay;

	//strcpy(filename, myWorldName);

	for(; *filename != '.' && *filename != 0; ++filename);
	*filename = ':';

	//strcpy(filename++, date.c_str());


	//Setting up file specificiations
	al_set_path_filename(path, filename);//Where are we writing this
	al_set_path_extension(path, ".png");//We are creating a png
	path_cstr = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

	//We've already printed out this file
	if (al_filename_exists(path_cstr))
	{
		//Saving the bitmap
		al_save_bitmap(path_cstr, al_get_target_bitmap());
		return 0;//It worked
	}
	free(filename);
	al_destroy_path(path);

	return -6;//We never placed a file
}
