#pragma once

#include <ftxui/component/component.hpp>
#include <ftxui/component/component_base.hpp>
#include <ftxui/component/component_options.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>

#include "caroLogic.hpp"
#include "caroSound.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <functional>

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
	void createDir(const std::filesystem::path&);

	void mainMenuLayer();
	void aboutLayer();
	void playLayer();
	void settingsLayer();

	std::string getStringFromFile(std::filesystem::path&);
private:
	ftxui::ScreenInteractive m_Screen = ftxui::ScreenInteractive::Fullscreen();
	appState m_State = appState::MAIN_MENU;
	bool m_Running = true;

	int m_RowNum = 8;
	int m_ColNum = 8;
	int m_kValue = 4;

	int focusRow = 0;
	int focusCol = 0;

	std::string m_Message;

	int playerXpoints = 0;
	int playerYpoints = 0;

	bool m_IsSaving = false;
	bool m_IsLoading = false;
	std::string m_SaveFileName = "";
	std::string m_SaveNotif = "";

	const std::filesystem::path m_SaveDirectory = "saves/";

	std::unique_ptr<caroLogic> m_pCaroLogic = std::make_unique<caroLogic>();

	AudioSystem audio;
	int curSFXVol = 100;
	int curBGMVol = 100;
};