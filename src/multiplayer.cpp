#include <multiplayer.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

SocketClient::SocketClient(string url)
{
    this->url = url;
    ws = make_shared<WebSocket>();
    wsFuture = wsPromise.get_future();

    ws->onOpen([this]()
               {
        std::cout << "Connected to Web Socket Server" << std::endl;
        this->onClientCreated();
        this->wsPromise.set_value(); });

    ws->onMessage([this](std::variant<rtc::binary, rtc::string> message)
                  { 
                    std::cout << std::get<string>(message) << std::endl;
                    json messageObject = json::parse(std::get<string>(message));
                    for(auto &func: callbackFunctions[messageObject["action"]]){
                        std::cout <<  messageObject["action"] << std::endl;
                        std::stringstream ostream;
                        ostream << messageObject["data"];
                        func(ostream.str());
                    }
                  });

    ws->onClosed([this]()
                 { std::cout << "Web Socket Connection has been closed" << std::endl; });

    ws->onError([](string error)
                { std::cout << "Connection to socket server failed. Error: " << error << std::endl; });
}

void SocketClient::CreateClient(function<void()> onCreatingClient, function<void()> onClientCreated)
{
    ws->open(url);
    onCreatingClient();
    this->onClientCreated = onClientCreated;
}

void SocketClient::AddCallback(string eventName, function<void(string)> callbackFunction){
    this->callbackFunctions[eventName].push_back(callbackFunction);
}

void SocketClient::RemoveCallback(string eventName){
    this->callbackFunctions[eventName].erase(this->callbackFunctions[eventName].begin(), this->callbackFunctions[eventName].end());
}

void SocketClient::SendMessage(string action, map<string, string> data)
{
    std::stringstream ss;
    json dataJson = data;
    ss << "{ \"action\": \"" << action << "\", \"data\": " << dataJson << "}";
    ws->send(ss.str());
    //std::cout << ss.str() << std::endl;
}

void SocketClient::CloseClient()
{
    ws->close();
}

RtcClient::RtcClient(Configuration config)
{
    connection = make_shared<PeerConnection>(config);

    connection->onLocalDescription([&](rtc::Description sdp){ 
        for(auto func: this->callbackFunctions["onLocalDescription"]){
            std::cout << "Create Local Description" << std::endl;
            func();
        }
    });

    connection->onLocalCandidate([&](rtc::Candidate candidate){
        for(auto func: this->callbackFunctions["onLocalCandidate"]){
            func();
        }
        this->candidateGenerated(candidate.candidate(), candidate.mid());
        std::cout << "Candidate: " << candidate.candidate() << ", Mid: " << candidate.mid() << std::endl;
    });


    connection->onStateChange([&](rtc::PeerConnection::State state){
        for(auto func: this->callbackFunctions["onStateChange"]){
            func();
        }
        switch(state){
            case rtc::PeerConnection::State::Closed:
                std::cout << "Connection Closed" << std::endl;
                break;
            case rtc::PeerConnection::State::Connected:
                std::cout << "WebRTC Connection Established!" << std::endl;
                break;
            case rtc::PeerConnection::State::Connecting:
                std::cout << "WebRTC Connecting..." << std::endl;
                break;
            case rtc::PeerConnection::State::Disconnected:
                std::cout << "WebRTC Disconnected" << std::endl;
                break;
            case rtc::PeerConnection::State::Failed:
                std::cout << "WebRTC Failed" << std::endl;
                break;
            case rtc::PeerConnection::State::New:
                std::cout << "New Failed" << std::endl;
                break;
        }
    });

    connection->onGatheringStateChange([&](rtc::PeerConnection::GatheringState state){ 
        for(auto func: this->callbackFunctions["onGatheringStateChange"]){
            func();
        }
    });

    connection->onDataChannel([&](shared_ptr<rtc::DataChannel> dc){
        channel = std::move(dc);
        channel->onOpen([](){
            std::cout << "Data Channel Established!";
        });
        channel->onMessage([](auto data){
            std::cout << "Message Received Here :eyes:" << std::endl;
        });
    });

    callbackFunctions = std::map<string, vector<function<void()>>>();
}

void RtcClient::AddCallback(string eventName, function<void()> callbackFunction){
    callbackFunctions[eventName].push_back(callbackFunction);
}

void RtcClient::RemoveCallbacks(string eventName){
    callbackFunctions[eventName].erase(this->callbackFunctions[eventName].begin(), this->callbackFunctions[eventName].end());
}

void RtcClient::RemoveCallbackAtIndex(string eventName, int index){
    callbackFunctions[eventName].erase(std::next(callbackFunctions[eventName].begin() + index));
}

void RtcClient::CreateDataChannel(){
    channel = connection->createDataChannel("Main");

    channel->onOpen([](){
        std::cout << "Data Channel Established!";
    });

    channel->onMessage([](auto data){
        std::cout << "Message Received Here :eyes:" << std::endl;
    });
}

void RtcClient::SetRemoteDescription(rtc::Description description){
    connection->setRemoteDescription(description);
}