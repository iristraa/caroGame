#include "caroUI.hpp"

gameUI::gameUI() {

}

void gameUI::createDir(const std::filesystem::path& dir) 
{
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directory(dir);
    }
}

std::string gameUI::getTitleScreen()
{
    std::ifstream file("Resources/title.txt");
    if (!file) {
        std::cerr << "Error: Could not open title.txt\n";
        return std::filesystem::current_path().string();
    }

    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";  // Add explicit line break after each line
    }

    return content;
}

void gameUI::mainMenuLayer()
{
    int selected = 0;
    std::vector<std::string> menu_entries = {
        "Play",
        "Settings",
        "About",
        "Quit"
    };

    MenuOption option;
    option.on_enter = [&] {
        if (menu_entries[selected] == "About") {
            m_State = appState::ABOUT;
            m_Screen.Exit();
        }
        else if (menu_entries[selected] == "Play") {
            m_pCaroLogic->getBoardSize(m_RowNum, m_ColNum, m_kValue);
            m_pCaroLogic->initBoard();
            m_SaveNotif = "";
            m_State = appState::INGAME;
            m_Screen.Exit();
        }
        else if (menu_entries[selected] == "Settings") {
            m_State = appState::SETTINGS;
            m_Screen.Exit();
        }
        else if (menu_entries[selected] == "Quit")
            m_Running = false;
            m_Screen.Exit();
        };

    auto menu = Menu(&menu_entries, &selected, option);

    auto menuWrapped = Container::Vertical({
        menu,
        });

    std::string titleScreen = getTitleScreen();

    auto menu_renderer = Renderer(menuWrapped, [&] {
        Element content = vbox({
            // text("Use arrow keys to navigate, Enter to select"),
            // separator(),
            paragraph(titleScreen) | center | flex,
            menuWrapped->Render() | border | size(WIDTH, EQUAL, 25) | center | flex,
            separator(),
            text("Selected: " + menu_entries[selected])
            });
        return content | border;
        });

    m_Screen.Loop(menu_renderer);
}

void gameUI::aboutLayer()
{
    // auto screen_a_button = Button("Back to Main Menu", [&] {
    //     m_State = appState::MAIN_MENU;
    //     m_Screen.ExitLoopClosure()();
    // });

    int selected = 0;
    std::vector<std::string> menu_entries = {
        "Return"
    };

    MenuOption option;
    option.on_enter = [&] {
        if (menu_entries[selected] == "Return") {
            m_State = appState::MAIN_MENU;
            m_Screen.ExitLoopClosure()();
        }
        };

    auto menu = Menu(&menu_entries, &selected, option);

    // --- FIX: Put menu + button in a container ---
    auto container = Container::Vertical({
        menu,
        // screen_a_button
        });

    // --- FIX: Add event handling ---
    auto interactive = CatchEvent(container, [&](Event event) {
        return container->OnEvent(event);  // Forward events normally
        });

    // Element para = paragraph("Hold still!\nDon't go anywhere!");

    // Renderer that displays UI
    auto screen_a_renderer = Renderer(interactive, [&] {
        return vbox({
            text("About this game") | center,
            separator(),
            paragraph("Press the arrow keys to select the UI elements.\n"),
            text(""),
            paragraph("Press Enter to confirm selection.\n"),
            text(""),
            paragraph("Press \"q\" to exit the game menu, press \"r\" to restart the board.\n"),
            text("") | flex,
            separator(),
            menu->Render(),
            // text("") | flex,
            // separator(),
            // screen_a_button->Render(),
            }) | border;
        });

    m_Screen.Loop(screen_a_renderer);
}

void gameUI::settingsLayer()
{
    auto Wrap = [&](std::string name, Component component) {
        return Renderer(component, [name, component] {
            return hbox({
                       text(name) | size(WIDTH, GREATER_THAN, 30),
                       separator(),
                       component->Render() | xflex,
                }) |
                xflex;
            });
    };

    std::string lengthN;
    auto inputN = Input(&lengthN, std::to_string(m_RowNum));
    inputN = Wrap("Length M of graph", inputN);

    inputN |= CatchEvent([&](Event event) {
        if (event == Event::Return) return true;
        return event.is_character() && !std::isdigit(event.character()[0]);
        });

    std::string widthM;
    auto inputM = Input(&widthM, std::to_string(m_ColNum));
    inputM = Wrap("Width N of graph", inputM);

    inputM |= CatchEvent([&](Event event) {
        if (event == Event::Return) return true;
        return event.is_character() && !std::isdigit(event.character()[0]);
        });

    std::string valueK;
    auto inputK = Input(&valueK, std::to_string(m_kValue));
    inputK = Wrap("K in a row to win", inputK);

    inputK |= CatchEvent([&](Event event) {
        if (event == Event::Return) return true;
        return event.is_character() && !std::isdigit(event.character()[0]);
        });

    int selected = 0;
    std::vector<std::string> menu_entries = {
        "Save & Exit"
    };

    MenuOption option;
    option.on_enter = [&] {
        if (menu_entries[selected] == "Save & Exit") {
            if (!lengthN.empty()) m_RowNum = std::stoi(lengthN);
            if (!widthM.empty()) m_ColNum = std::stoi(widthM);
            if (!valueK.empty()) m_kValue = std::stoi(valueK);
            m_State = appState::MAIN_MENU;
            m_Screen.ExitLoopClosure()();
        }
        };

    auto menu = Menu(&menu_entries, &selected, option);

    auto layout = Container::Vertical({
        inputN,
        inputM,
        inputK,
        menu
    });

    auto interact = CatchEvent(layout, [&](Event event) {

        if (event == Event::Character('q') || event == Event::Character('Q')) {
            m_State = appState::MAIN_MENU;
            m_Screen.ExitLoopClosure()();
            return true;
        }

        return layout->OnEvent(event);  // Forward events normally
    });

    auto renderer = Renderer(interact, [&] {
        return vbox({
            inputN->Render(),
            separator(),
            inputM->Render(),
            separator(),
            inputK->Render(),
            separator(),
            text(" ") | flex,
            separator(),
            menu->Render()
            }) | border;
        });

    m_Screen.Loop(renderer);

}

void gameUI::playLayer()
{
    caroState curGamestate = m_pCaroLogic->gameState();

    if (curGamestate != caroState::ONGOING) audioResult = ma_engine_play_sound(&audioEngine, "Resources/achievement.wav", NULL);

    m_pCaroLogic->getBoardSize(m_RowNum, m_ColNum, m_kValue);

    std::vector<std::vector<Component>> grid(m_RowNum, std::vector<Component>(m_ColNum));

    for (int i = 0; i < m_RowNum; ++i) {
        for (int j = 0; j < m_ColNum; ++j) {
            int curCell = m_pCaroLogic->getCell(i, j);
            std::string labelCell;
            if (curCell == 0) labelCell = "   ";
            else if (curCell == 1) labelCell = " X ";
            else if (curCell == 2) labelCell = " O ";
            grid[i][j] = Button(labelCell, [&] {
                // m_State = appState::MAIN_MENU;
                
                // m_Message = "Message: Row " + std::to_string(focusRow + 1) + " Column " + std::to_string(focusCol + 1) + " Selected!";
                m_SaveNotif = "";
                m_pCaroLogic->setCell(focusRow, focusCol);
                m_Screen.ExitLoopClosure()();
            });
        }
    }

    Component mainContainer = Renderer([&] {
        std::vector<Element> rows;

        for (int i = 0; i < m_RowNum; ++i) {
            std::vector<Element> rowElements;
            for (int j = 0; j < m_ColNum; ++j) {
                bool focused = (i == focusRow && j == focusCol);

                Element renderE = grid[i][j]->Render() | inverted;

                int curCell = m_pCaroLogic->getCell(i, j);
                if (curCell == 1) renderE |= color(Color::Blue);
                else if (curCell == 2) renderE |= color(Color::Red);
                else if (!focused) renderE |= dim;

                if (focused) {
                    renderE = renderE | bold;
                }

                rowElements.push_back(renderE);
            }
            rows.push_back(hbox(std::move(rowElements)));
        }
        return vbox(std::move(rows)) | border;
        });

    auto saveInput = Input(&m_SaveFileName, "Please type the save file name...");

    auto logicLayer = Container::Vertical({
        mainContainer,
        saveInput
        });

    // maintain row position
    logicLayer = CatchEvent(logicLayer, [&](Event e) {

        if (m_IsSaving) {
            if (e == Event::Return) {
                if (m_SaveFileName.empty()) m_SaveNotif = "Error: Filename cannot be empty!";
                else {
                    if (m_pCaroLogic->saveState(m_SaveDirectory / m_SaveFileName) == -1) {
                        m_SaveNotif = "Error: Cannot open file!";
                        m_IsSaving = false;
                        m_SaveFileName = "";
                    }
                    else {
                        m_SaveNotif = "Game saved to \"" + m_SaveFileName + "\" successfully!";
                        m_IsSaving = false;
                        m_SaveFileName = "";
                    }
                }
                return true;
            }
            
            if (e == Event::Escape) {
                m_IsSaving = false;
                m_SaveNotif = "Save cancelled.";
                m_SaveFileName = "";
                return true;
            }

            return saveInput->OnEvent(e);
        }

        if (e == Event::Character('l') || e == Event::Character('L')) {
            m_IsSaving = true;
            m_SaveNotif = "";
            return true;
        }

        if (m_IsLoading) {
            if (e == Event::Return) {
                if (m_SaveFileName.empty()) m_SaveNotif = "Error: Filename cannot be empty!";
                else {
                    if (m_pCaroLogic->loadState(m_SaveDirectory / m_SaveFileName) == -1) {
                        m_SaveNotif = "Error: Cannot open file!";
                        m_IsLoading = false;
                        m_SaveFileName = "";
                    }
                    else {
                        m_SaveNotif = "Game loaded from \"" + m_SaveFileName + "\" successfully!";
                        m_IsLoading = false;
                        m_SaveFileName = "";
                        m_RowNum = m_pCaroLogic->returnBoardRow();
                        m_ColNum = m_pCaroLogic->returnBoardCol();
                        m_kValue = m_pCaroLogic->returnBoardKVal();
                        m_Screen.ExitLoopClosure()();
                    }
                }
                return true;
            }

            if (e == Event::Escape) {
                m_IsLoading = false;
                m_SaveNotif = "Load cancelled.";
                m_SaveFileName = "";
                return true;
            }

            return saveInput->OnEvent(e);
        }

        if (e == Event::Character('t') || e == Event::Character('T')) {
            m_IsLoading = true;
            m_SaveNotif = "";
            return true;
        }

        if (curGamestate == caroState::ONGOING) {
            if (e == Event::ArrowRight) {
                focusCol = (focusCol + 1) % m_ColNum;
                return true;
            }
            if (e == Event::ArrowLeft) {
                focusCol = (focusCol - 1 + m_ColNum) % m_ColNum;
                return true;
            }
            if (e == Event::ArrowDown) {
                focusRow = (focusRow + 1) % m_RowNum;
                return true;
            }
            if (e == Event::ArrowUp) {
                focusRow = (focusRow - 1 + m_RowNum) % m_RowNum;
                return true;
            }

            // Forward Enter to the selected button
            if (e == Event::Return || e == Event::Character(' ')) {
                grid[focusRow][focusCol]->OnEvent(Event::Return);
                return true;
            }
        }

        if (e == Event::Character('q') || e == Event::Character('Q')) {
            m_State = appState::MAIN_MENU;
            m_Screen.ExitLoopClosure()();
            return true;
        }

        if (e == Event::Character('r') || e == Event::Character('R')) {
            m_pCaroLogic->initBoard();
            m_Screen.ExitLoopClosure()();
            return true;
        }

        return false;
    });

    auto gameMessageFormat = [&](const std::string& message) {
        auto textBefore = text(message);

        if (m_pCaroLogic->getTurn() == playerState::PLAYER_ONE && curGamestate == caroState::ONGOING) textBefore |= color(Color::Blue);
        else if (m_pCaroLogic->getTurn() == playerState::PLAYER_TWO && curGamestate == caroState::ONGOING) textBefore |= color(Color::Red);
        else textBefore |= color(Color::Green);

        auto container = hbox({
            text("Turn: " + std::to_string(m_pCaroLogic->getTurnsTaken())),
            separator(),
            textBefore,
            });

        return container;
    };

    if (curGamestate == caroState::PLAYER_ONE_WINS) m_Message = "Player 1 (X) wins! Press \"r\" to reset board";
    else if (curGamestate == caroState::PLAYER_TWO_WINS) m_Message = "Player 2 (O) wins! Press \"r\" to reset board";
    else if (curGamestate == caroState::DRAW) m_Message = "Both players draw!";
    else {
        if (m_pCaroLogic->getTurn() == playerState::PLAYER_ONE) m_Message = "Player 1's turn! Please make a move";
        else if (m_pCaroLogic->getTurn() == playerState::PLAYER_TWO) m_Message = "Player 2's turn! Please make a move";
    }

    auto playScreenRender = Renderer(logicLayer, [&] {
        Element saveArea;
        if (m_IsSaving) {
            saveArea = hbox({
                text("Save file: "),
                saveInput->Render() | flex
            }) | border | color(Color::Yellow);
        }
        else if (m_IsLoading) {
            saveArea = hbox({
                text("Load from file: "),
                saveInput->Render() | flex
            }) | border | color(Color::Yellow);
        }
        else {
            if (!m_SaveNotif.empty()) {
                saveArea = text(m_SaveNotif) | center | color(Color::Green);
            }
            else {
                saveArea = hbox({
                    text(" Press \"l\" to save game "),
                    separator() | dim,
                    text(" Press \"t\" to load from saved games "),
                    separator() | dim,
                    text(" Press \"q\" to exit ")
                    }) | center | dim;
            }
        }

        return vbox({
            gameMessageFormat(m_Message) | hcenter,
            mainContainer->Render() | center | flex,
            saveArea
            }) | borderHeavy;
        });

    m_Screen.Loop(playScreenRender);
}

void gameUI::runGame() {
    createDir(m_SaveDirectory);
    audioResult = ma_engine_init(NULL, &audioEngine);
    if (audioResult != MA_SUCCESS) {
        std::cerr << "Failed to initialize audio engine" << std::endl;
        return;
    }
	while (m_Running) {
		switch (m_State) {
		case appState::MAIN_MENU: mainMenuLayer(); break;
        case appState::ABOUT: aboutLayer(); break;
        case appState::SETTINGS: settingsLayer(); break;
        case appState::INGAME: playLayer(); break;
		}
	}
    ma_engine_uninit(&audioEngine);
}