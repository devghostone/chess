#ifndef MULTIPLAYER_HPP
#define MULTIPLAYER_HPP

#include <rtc/rtc.hpp>
#include <future>
#include <string>
#include <memory>

using std::string, std::promise, std::future, std::shared_ptr, std::make_shared, rtc::WebSocket;

class SocketClient{
    string url;
    shared_ptr<WebSocket> ws;
    promise<void> wsPromise;
    future<void> wsFuture;

    public:
        SocketClient(string url);
        void CreateClient();
        void CloseClient();
};

#endif