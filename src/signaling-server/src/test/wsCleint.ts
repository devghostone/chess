import WebSocket from 'ws';

function connect(socketUrl: string) {
    const socket = new WebSocket(socketUrl);
    return socket;
}

export const wsClient = {
    connect,
};
