import WebSocket from 'ws';

function createServer(port: number) {
    const wss = new WebSocket.Server({ port: port });
    return wss;
}

export const webSocket = {
    createServer,
};
