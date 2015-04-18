#include "allegro_engine.h"

int AllegroEngine::screen_width;
int AllegroEngine::screen_height;

ALLEGRO_FONT* AllegroEngine::arial24;
ALLEGRO_FONT* AllegroEngine::arial16;
ALLEGRO_FONT* AllegroEngine::arial12;
ALLEGRO_FONT* AllegroEngine::arial8;

ALLEGRO_COLOR AllegroEngine::text_color;

ALLEGRO_DISPLAY* AllegroEngine::main_display;
ALLEGRO_BITMAP* AllegroEngine::main_bitmap;

void AllegroEngine::InitializeAllegro()
{
	if (!al_init())
	{
		std::cout<<"Al init failed."<<endl;
	}
	//allegro-5.0.10-monolith-md-debug.lib

	//Anti Aliasing
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

	//Initializing Addons
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	al_init_primitives_addon();

	al_reserve_samples(10);

	std::cout<<endl<<"Allegro initialized.";

	InitializeFonts();
};

void AllegroEngine::InitializeFonts()
{
	arial24 = al_load_font("arial.ttf",24,0);
	arial16 = al_load_font("arial.ttf",16,0);
	arial12 = al_load_font("arial.ttf",12,0);
	arial8 = al_load_font("arial.ttf",8,0);

	std::cout<<endl<<"Fonts loaded."<<endl;
};

void AllegroEngine::InitializeScreen(int myScreenWidth, int myScreenHeight)
{
	screen_width = myScreenWidth;
	screen_height = myScreenHeight;

	al_set_new_display_flags(ALLEGRO_WINDOWED | ALLEGRO_OPENGL);
	main_display = al_create_display(myScreenWidth,myScreenHeight);

	al_set_window_position(main_display,0,0);
	al_set_window_title(main_display,"World Simulation");
	text_color = al_map_rgb(
		atoi(Settings::GetSetting("color_text_r").c_str()),
		atoi(Settings::GetSetting("color_text_g").c_str()),
		atoi(Settings::GetSetting("color_text_b").c_str()));
};
void AllegroEngine::InitializeBitmap(int myScreenWidth, int myScreenHeight)
{
	screen_width = myScreenWidth;
	screen_height = myScreenHeight;

	main_bitmap = al_create_bitmap(screen_width,screen_height);
	al_set_target_bitmap(main_bitmap);

	text_color = al_map_rgb(
		atoi(Settings::GetSetting("color_text_r").c_str()),
		atoi(Settings::GetSetting("color_text_g").c_str()),
		atoi(Settings::GetSetting("color_text_b").c_str()));
};

void AllegroEngine::RenameSceen(std::string myName)
{
	al_set_window_title(main_display,myName.c_str());
};

void AllegroEngine::WriteText(std::string myText,
							  int myXDestination, int myYDestination, 
							  ALLEGRO_FONT* myFont, std::string myAlignment)
{
	const char * converted_text = myText.c_str();
	al_draw_text(myFont,text_color, myXDestination, myYDestination,0, converted_text);

};

int AllegroEngine::FlushScreenshot(std::string myWorldName,int myCurrentYear,int myCurrentDay, std::string myTags)
{
	ALLEGRO_PATH *path;
	char filename[80];
	const char *path_cstr;
	int char_index = 0;
	std::string output = Settings::GetSetting("picture_output_folder")+"/"+myWorldName+"/"+myTags+"/";
	const char* destination_path = output.c_str();

	//Testing the destination given
	if(!destination_path)
		path = al_get_standard_path(ALLEGRO_USER_DOCUMENTS_PATH);
	else
		path = al_create_path_for_directory(destination_path);

	if(!path)
		return -1;//Where we want it is bad

	al_set_standard_file_interface();

	//Creating the file name
	std::string name = myWorldName + "["+ std::to_string(myCurrentYear)+"-"+ std::to_string(myCurrentDay) + "]" + myTags;

	for (int c = 0; c < name.length(); c++)
	{
		filename[char_index] = name[c];
		char_index++;
	}
	filename[char_index] = '\0';

	//Setting up file specificiations
	al_set_path_filename(path, filename);//Where are we writing this
	al_set_path_extension(path, ".png");//We are creating a png
	path_cstr = al_path_cstr(path, ALLEGRO_NATIVE_PATH_SEP);

	if (!al_filename_exists(path_cstr))
	{
		//Saving the bitmap
		//std::cout<<"Screenshot saved at "<<path_cstr<<" named "<<filename<<"."<<endl;

		std::cout<<"Screenshot saved: "<<al_get_path_basename(path)<<endl;

		if(main_display)
		{
			if(al_save_bitmap(path_cstr,al_get_backbuffer(main_display)))
				return 1;
			else
				return -1;
		}
		else if (main_bitmap)
		{
			if(al_save_bitmap(path_cstr,main_bitmap))
				return 1;
			else
				return -1;
		}
		else
		{
			//No screen was initialized
			return -9;

		}
	}
	else
	{
		//We've already printed out this file
		delete(filename);
		al_destroy_path(path);
	}
	return -6;//We never placed a file
};

void AllegroEngine::FreeMemory()
{
	al_destroy_display(main_display);
	al_destroy_font(arial24);
	al_destroy_font(arial16);
	al_destroy_font(arial12);
	al_destroy_font(arial8);
};