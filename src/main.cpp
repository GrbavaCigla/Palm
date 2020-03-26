#include <csv.h>
#include <ncurses.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

#include "vlcstream.hpp"
#include "yt.hpp"

using namespace nlohmann;

std::string format_secs(int& secs) {
    int hours = secs / 3600;
    int minutes = secs / 60 - hours * 60;
    std::string secs_new = std::to_string(secs % 60);
    if (secs_new.length() == 1) {
        secs_new = "0" + secs_new;
    }

    if (hours == 0) {
        return std::to_string(minutes) + ":" + secs_new;
    }
    return std::to_string(hours) + ":" + std::to_string(minutes) + ":" + secs_new;
}

int main() {
    initscr();

    int width, height;
    getmaxyx(stdscr, height, width);

    endwin();

    json request = get_json("qQRQT0n3GDo");
    std::vector<YouTubeSource> sources = get_sources(request);

    for (auto const& source : sources) {
        stream_from_url(source.url);
    }

    return 0;
}