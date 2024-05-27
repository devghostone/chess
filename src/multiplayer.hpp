#ifndef MULTIPLAYER_HPP
#define MULTIPLAYER_HPP

#include <rtc/rtc.hpp>
#include <future>
#include <string>
#include <sstream>
#include <memory>
#include <map>
#include <functional>
#include <vector>

using std::string, std::vector,std::function, std::map, std::promise, std::future, std::shared_ptr, std::make_shared, rtc::WebSocket, rtc::Configuration, rtc::PeerConnection;

class SocketClient{
    string url;
    shared_ptr<WebSocket> ws;

    promise<void> wsPromise;
    future<void> wsFuture;
    function<void()> onClientCreated;
    map<string, vector<function<void(string)>>> callbackFunctions;

    public:
        SocketClient(string url);
        void CreateClient(function<void()> onCreatingClient, function<void()> onClientCreated);
        void SendMessage(string action, map<string, string> data);
        void CloseClient();
        void AddCallback(string eventName, function<void(string)> callbackFunction);
        void RemoveCallback(string eventName);
};

class RtcClient{

    public:
        std::shared_ptr<rtc::PeerConnection> connection;
        std::map<string, vector<function<void()>>> callbackFunctions;

        RtcClient(rtc::Configuration config);
        void CreateDataChannel();
        void SetRemoteDescription(rtc::Description description);
        void AddCallback(string eventName, function<void()> callbackFunction);
        void RemoveCallbacks(string eventName);
        void RemoveCallbackAtIndex(int index);
};

#endif