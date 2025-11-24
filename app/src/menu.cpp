#include "menu.hpp"

// FTXUI Library
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>

#include "utils.hpp"


/**
 * @brief Construct a new Menu:: Menu object
 * 
 * @param title Title of the menu
 * @param options All options of the menu (must be ordered)
 */
Menu::Menu(const std::string& title, const std::vector<std::string>& options) : title(title), options(options) {}

/**
 * @brief Show the menu and wait for input
 * 
 * @return int Button chosen, starting on 0 and up to the number of options - 1
 */
int Menu::show(bool clearTerminal) {
    if (clearTerminal) Utils::clear();

    int selected = 0;

    auto screen = ftxui::ScreenInteractive::TerminalOutput();
    auto menu = ftxui::Menu(&options, &selected);

    auto component = ftxui::Renderer(menu, [&] {
        return ftxui::vbox({
            ftxui::text(title) | ftxui::bold | ftxui::center,
            ftxui::separator(),
            menu->Render(),
        }) | ftxui::border;
    });

    // Capture Esc and Enter
    component = component | ftxui::CatchEvent([&](ftxui::Event e) {
        if (e == ftxui::Event::Escape) {
            // If it's a submenu, Esc quits (last option)
            selected = options.size() - 1;
            screen.Exit();
            return true;
        }
        if (e == ftxui::Event::Return) { // Return is Enter in ftxui
            screen.Exit();
            return true;
        }
        return false;
    });

    // Show the menu
    screen.Loop(component);
    // Returns option
    return selected;
}

/**
 * @brief Gets user input in Yes/No form
 * 
 * @param question Question to answer
 * @return true If user chooses Yes
 * @return false If user chooses No
 */
bool Menu::askYesNo(const std::string& question, bool clearTerminal) {
    using namespace ftxui;

    if (clearTerminal) Utils::clear();

    bool result = false;

    auto screen = ScreenInteractive::FitComponent();

    auto btn_yes = Button("Yes", [&] { 
        result = true; 
        screen.Exit(); 
    }, ButtonOption::Animated());

    auto btn_no = Button("No", [&] { 
        result = false; 
        screen.Exit(); 
    }, ButtonOption::Animated());

    auto buttons = Container::Horizontal({
        btn_yes,
        btn_no,
    });

    auto component = Renderer(buttons, [&] {
        return vbox({
            text(question) | bold | center,
            separator(),
            hbox({
                btn_yes->Render() | color(Color::Green),
                text("\t"), // space between buttons
                btn_no->Render()  | color(Color::Red),
            }) | center,
        }) | border | center;
    });

    screen.Loop(component);

    return result;
}

