#include <curl/curl.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

struct YouTubeSource {
    std::string url;
    std::string quality;
};

std::vector<std::string> split_by_delim(std::string string, const std::string &delimiter) {
    size_t pos = 0;
    std::string token;
    std::vector<std::string> array;
    while ((pos = string.find(delimiter)) != std::string::npos) {
        token = string.substr(0, pos);
        array.push_back(token);
        string.erase(0, pos + delimiter.length());
    }
    array.push_back(string);
    return array;
}

std::string url_decode(const std::string &url) {
    std::string ret;
    char ch;
    int i, ii;
    for (i = 0; i < (int)url.length(); i++) {
        if (int(url[i]) == 37) {
            sscanf(url.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        } else {
            ret += url[i];
        }
    }
    return (ret);
}

static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

std::string request_get(const std::string &url) {
    CURL *curl;

    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return readBuffer;
}

nlohmann::json get_json(const std::string &id_) {
    std::string text = url_decode(request_get("https://www.youtube.com/get_video_info?video_id=" + id_));
    std::size_t sub1 = text.find("player_response=") + 16;
    std::size_t sub2 = text.find("}&");
    text = text.substr(sub1, sub2 - sub1 + 1);

    nlohmann::json json_request = nlohmann::json::parse(text);
    return json_request;
}

std::vector<YouTubeSource> get_sources(const nlohmann::json &requested_json) {
    nlohmann::json formats = requested_json["streamingData"]["adaptiveFormats"];

    std::vector<YouTubeSource> result;

    for (nlohmann::json::iterator it = formats.begin(); it != formats.end(); ++it) {
        if (static_cast<std::string>((*it)["mimeType"]).substr(0, 5) == "audio") {
            YouTubeSource temp_source;
            temp_source.url = (*it)["url"];
            temp_source.quality = (*it)["audioQuality"];
            result.push_back(temp_source);
        }
    }
    return result;
}
