#include "vlcstream.hpp"

void _stream_from_url(const std::string &url, int lenght_ms) {
    auto instance = VLC::Instance(0, nullptr);
    auto media = VLC::Media(instance, url, VLC::Media::FromType::FromLocation);
    auto mp = VLC::MediaPlayer(media);
    mp.play();
    std::this_thread::sleep_for(std::chrono::milliseconds(lenght_ms));
    mp.stop();
}