#pragma once

#include "game/application.h"
#include "game/selectionstate.h"
#include "game/optionsstate.h"

#include "entity/cursor.h"

#include "gfx/texture.h"

#include "sfx/music.h"

#include "utils/gui/button.h"

class Menu : public GameState
{
	public:
		Menu(bool playBgm);
		~Menu();

		void render();
		void update(float deltaTime_f);
		void handleEvents();

	private:
		Cursor* m_cursor;

		std::vector<Button*> m_buttons;

		Texture m_background;

		Bgm m_menuTheme;
};