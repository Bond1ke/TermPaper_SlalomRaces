#include "AboutGame.h"

AboutGame::AboutGame()
{
	about_Game_flag = true;
	countlines = 0;
	//read info from file
	//MainMenuData[i].loadFromRenderedText(MenuWords[i].c_str(), defaultColor);
	countlines = CountLinesInFile((char*)"Assets/TextAboutGame.txt");
	lines = nullptr; // ������ ����� ���� string

	// 2. �������� ������ ����� ���� string*
	GetStringsFromFileS((char*)"Assets / TextAboutGame.txt", &lines, countlines);

	// 3. ��������, ������� �� ������
	if (countlines < 0)
	{
		std::cout << "Error" << std::endl;
		return;
	}
}

bool AboutGame::about_Game_getter()
{
	return about_Game_flag;
}

void AboutGame::handle_Menu(SDL_Event& e, Menu& menu)
{
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_RETURN:
			about_Game_flag = false;
			gameAboutGame = false;
			//menu.about_game_setter(aboutGameMenu.about_Game_getter());
			menu.about_game_setter(about_Game_flag);
			if (gameIsStarted == false)
				menu.show_menu_setter(true);
			Mix_PlayChannel(-1, ButtonPressed, 0);
			break;
		}
	}
	else if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
	{
		switch (e.type)
		{
		case SDL_MOUSEBUTTONDOWN:
			about_Game_flag = false;
			gameAboutGame = false;
			if (gameIsStarted == false)
				menu.show_menu_setter(true);
			menu.about_game_setter(about_Game_flag);
			Mix_PlayChannel(-1, ButtonPressed, 0);
			break;
		}
	}
}

void AboutGame::render()
{
	//Clear screen
	SDL_SetRenderDrawColor(Renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(Renderer);

	//text_info = read_from_file(text_info);
	for (int i = 0; i < countlines; ++i)
	{
		if (lines[i] == "") lines[i] = " ";
		info.loadFromRenderedText(lines[i].c_str(), defaultColor);
		info.render((SCREEN_WIDTH - info.getWidth()) / 2, info.getHeight() * 2 * i + SCREEN_HEIGHT / 16);
	}

	//Update screen
	SDL_RenderPresent(Renderer);
}

int AboutGame::CountLinesInFile(char* filename)
{
	// 1. �������� ��������� F, ������� ������ � ������ filename.
	// ���� ����������� ��� ������ � ��������� �������.
	std::ifstream F(filename, std::ios::in);

	// 2. ��������, ���������� �� ����
	if (!F)
	{
		return -1;
	}

	// 3. ��������� ���������� ����� � �����
	// 3.1. �������� �������������� ����������
	int count = 0;
	char buffer[1000]; // ����� ��� ���������� ����� ������

	// 3.2. ���� ������ �����.
	// ������ ������ ����� �������� �������� getline().
	// �������� ����� ����� �������������� �������� eof().
	while (!F.eof())
	{
		// ��������� ������� �����
		count++;

		// ������� ���� ������ � �����
		F.getline(buffer, 1000);
	}

	// 4. ������� ���� F
	F.close();

	// 5. ������� ���������
	return count;
}



int AboutGame::GetStringsFromFileS(char* filename, std::string** _lines, int n)
{
	// 1. �������������� ����������
	std::string* lines; // ��������� ������ �����

	// 2. ��������, ��������� �� ��������� ������ �� �����
	if (n == -1) return -1;

	// 3. �������� �������� ���������� � ������� ���� filename ��� ������
	std::ifstream F("Assets/TextAboutGame.txt");

	// 4. ��������, ������ �� ����
	if (!F) return -1;

	// 5. ������� �������� ������ ��� n ����� ���� string
	try
	{
		lines = new std::string[n];
	}
	catch (std::bad_alloc e)
	{
		std::cout << e.what() << std::endl; // ������� ��������� �� ������
		F.close();
		return -2; // ������� � ����� -2
	}

	// 6. ������ ����� �� ����� � ������ ����� � ������ lines
	char buffer[1000]; // ����� ��� ������ ������

	for (int i = 0; i < n; i++)
	{
		// 6.1. ��������� ������ �� ����� � ����� buffer
		F.getline(buffer, 1000);

		// 6.2. ��������� ����� ����������� ������
		int len;
		for (len = 0; buffer[len] != '\0'; len++);

		// 6.3. �������� buffer => lines[i], ������������ ����� assign().
		// ����������� len ���� �� buffer � lines[i].
		lines[i].assign(buffer, len);
	}

	// 7. ������� ����
	F.close();

	// 8. ������� ���������
	*_lines = lines;
	return n;
}

