#include "vlcstream.hpp"

int stream_from_url(const std::string &url) {
    auto instance = VLC::Instance(0, nullptr);
    auto media = VLC::Media(instance, url, VLC::Media::FromPath);
    auto mp = VLC::MediaPlayer(media);
    mp.play();
}