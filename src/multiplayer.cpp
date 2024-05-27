#include <multiplayer.hpp>

SocketClient::SocketClient(string url){
    this->url = url;
    ws = make_shared<WebSocket>();
    wsFuture = wsPromise.get_future();

    ws->onOpen([this](){
        std::cout << "Connected to Web Socket Server" << std::endl;
        this->wsPromise.set_value();
    });

    ws->onMessage([this](std::variant<rtc::binary, rtc::string> message){
        std::cout << std::get<rtc::string>(message) << std::endl;
    });

    ws->onClosed([this](){
        std::cout << "Web Socket Connection has been closed" << std::endl;
    });

    ws->onError([](string error){
        std::cout <<"Connection to socket server failed. Error: " << error << std::endl;
    });
}

void SocketClient::CreateClient(){
    ws->open(url);
}

void SocketClient::CloseClient(){

}