#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>

#include <string>
#include <vector>

using namespace ftxui;

char board[8][8] = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
};

std::string move;
std::vector<std::string> moves;

float time_white = 10 * 60;
float time_black = 10 * 60;

Element chessboard() {
    Elements rows;

    for (int y = 0; y < 8; ++y) {
        Elements cells;
        

        for (int x = 0; x < 8; ++x) {
            char piece = board[y][x];

            if (x == 0) cells.push_back(text(std::to_string(8 - y) + " | "));

            if (x % 2 == y % 2) {
                    text((std::string){piece, ' '}) | center | inverted
                );
            } else {
                cells.push_back(
                    text((std::string){piece, ' '}) | center
                );
            }
        }

        rows.push_back(hbox(cells));
    }
    rows.push_back(hbox(text("    - - - - - - - -")));
    rows.push_back(hbox(text("    A B C D E F G H")));
    return vbox(rows);
}
/*

    A   B   C   D   E   F   G   H
    ┌───────┬───────┬───────┬───────┐
8  │   r   │   n   │   b   │   q   │
    ├───────┼───────┼───────┼───────┤
7  │   p   │   p   │   p   │   p   │
    ├───────┼───────┼───────┼───────┤
6  │       │       │       │       │
    ├───────┼───────┼───────┼───────┤
5  │       │       │       │       │
    ├───────┼───────┼───────┼───────┤
4  │       │       │       │       │
    ├───────┼───────┼───────┼───────┤
3  │       │       │       │       │
    ├───────┼───────┼───────┼───────┤
2  │   P   │   P   │   P   │   P   │
    ├───────┼───────┼───────┼───────┤
1  │   R   │   N   │   B   │   Q   │
    └───────┴───────┴───────┴───────┘

*/

int main() {

    auto screen = ScreenInteractive::Fullscreen();


    ButtonOption button_style = ButtonOption::Simple();
    button_style.transform = [](const EntryState& state) {
        return text(state.label) | border;
    };

    Component resign = Button("Resign", [&] {}, button_style);
    Component draw = Button("Draw", [&] {}, button_style);
    Component reset = Button("Reset", [&] {}, button_style);

    Component input = Input(&move, "") | inverted | underlined;

    auto buttons = Container::Horizontal({
        resign,
        draw,
        reset
    });

    auto controls = Container::Vertical({
        buttons,
        input
    });

    controls->SetActiveChild(input);

    auto document = Renderer(controls, [&] {
        std::vector<std::vector<std::string>> protocol = {
            {"Move", "White", "Black"},
        };

        for (std::size_t i = 0; i < moves.size(); ++i) {
            int line_index = static_cast<int>(i / 2 + 1);
            std::vector<std::string> line = {
                std::to_string(line_index), "", ""
            };

            if (i % 2 == 0) {
                line[1] = moves[i];
                protocol.push_back(line);
            } else {
                line[1] = moves[i - 1];
                line[2] = moves[i];
                protocol.back() = line;
            }
        }

        auto table = Table(protocol);
        table.SelectAll().SeparatorVertical(LIGHT);
        table.SelectRow(0).BorderBottom(LIGHT);

        return vbox({
            text("Chess") | bold | center,
            separator(),
            hbox({
                vbox({table.Render()}),
                separator(),
                vbox({
                    filler(),
                    chessboard() | center,
                    filler(),
                    hbox({
                        resign->Render(),
                        draw->Render(),
                        reset->Render()
                    }),
                    hbox({
                        text("Enter move: "),
                        input->Render()
                    })
                })
            })
        }) | border;
    });

    document |= CatchEvent([&](Event event) {
        if (event == Event::Return) {
            if (!move.empty()) {
                moves.push_back(move);
                move.clear();
            }

            return true;
        }

        if (event == Event::Escape) {
            screen.Exit();
            return true;
        }

        return false;
    });

    screen.Loop(document);

    return 0;
}
