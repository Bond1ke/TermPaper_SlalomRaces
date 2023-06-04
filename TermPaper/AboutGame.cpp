#include "AboutGame.h"

AboutGame::AboutGame()
{
	about_Game_flag = true;
	countlines = 0;
	//read info from file
	//MainMenuData[i].loadFromRenderedText(MenuWords[i].c_str(), defaultColor);
	countlines = CountLinesInFile((char*)"Assets/TextAboutGame.txt");
	lines = nullptr; // список строк типа string

	// 2. Получить список строк типа string*
	GetStringsFromFileS((char*)"Assets / TextAboutGame.txt", &lines, countlines);

	// 3. Проверка, получен ли список
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
	// 1. Объявить экземпляр F, который связан с файлом filename.
	// Файл открывается для чтения в текстовом формате.
	std::ifstream F(filename, std::ios::in);

	// 2. Проверка, существует ли файл
	if (!F)
	{
		return -1;
	}

	// 3. Вычислить количество строк в файле
	// 3.1. Объявить дополнительные переменные
	int count = 0;
	char buffer[1000]; // буфер для сохранения одной строки

	// 3.2. Цикл чтения строк.
	// Каждая строка файла читается функцией getline().
	// Проверка конца файла осуществляется функцией eof().
	while (!F.eof())
	{
		// Увеличить счетчик строк
		count++;

		// Считать одну строку в буфер
		F.getline(buffer, 1000);
	}

	// 4. Закрыть файл F
	F.close();

	// 5. Вернуть результат
	return count;
}



int AboutGame::GetStringsFromFileS(char* filename, std::string** _lines, int n)
{
	// 1. Дополнительные переменные
	std::string* lines; // временный список строк

	// 2. Проверка, правильно ли прочитаны строки из файла
	if (n == -1) return -1;

	// 3. Объявить файловую переменную и открыть файл filename для чтения
	std::ifstream F("Assets/TextAboutGame.txt");

	// 4. Проверка, открыт ли файл
	if (!F) return -1;

	// 5. Попытка выделить память для n строк типа string
	try
	{
		lines = new std::string[n];
	}
	catch (std::bad_alloc e)
	{
		std::cout << e.what() << std::endl; // вывести сообщение об ошибке
		F.close();
		return -2; // возврат с кодом -2
	}

	// 6. Чтение строк из файла и запись строк в список lines
	char buffer[1000]; // буфер для чтения строки

	for (int i = 0; i < n; i++)
	{
		// 6.1. Прочитать строку из файла в буфер buffer
		F.getline(buffer, 1000);

		// 6.2. Вычислить длину прочитанной строки
		int len;
		for (len = 0; buffer[len] != '\0'; len++);

		// 6.3. Записать buffer => lines[i], использовать метод assign().
		// Скопировать len байт из buffer в lines[i].
		lines[i].assign(buffer, len);
	}

	// 7. Закрыть файл
	F.close();

	// 8. Вернуть результат
	*_lines = lines;
	return n;
}

