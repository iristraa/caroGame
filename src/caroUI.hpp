#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include "miniaudio.h"
#include "caroLogic.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

using namespace ftxui;

enum class appState {
	MAIN_MENU,
	ABOUT,
	SETTINGS,
	INGAME
};

class gameUI {
public:
	gameUI();

	void runGame();
private:
	void mainMenuLayer();
	void aboutLayer();
	void playLayer();
	// ftxui::Component aboutLayer();

	std::string getTitleScreen();
private:
	ftxui::ScreenInteractive m_Screen = ftxui::ScreenInteractive::Fullscreen();
	appState m_State = appState::MAIN_MENU;
	bool m_Running = true;

	int m_RowNum = 8;
	int m_ColNum = 8;

	int focusRow = 0;
	int focusCol = 0;

	std::string m_Message;

	bool m_IsSaving = false;
	std::string m_SaveFileName = "";
	std::string m_SaveNotif = "";

	std::unique_ptr<caroLogic> m_pCaroLogic = std::make_unique<caroLogic>();
};