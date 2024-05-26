import { WebSocket } from 'ws';

(global as any).clients = new Map<string, WebSocket>();

function add(uniqueId: string, ws: WebSocket) {
    (global as any).clients.set(uniqueId, ws);
}

function remove(uniqueId: string) {
    (global as any).clients.delete(uniqueId);
}

function get(uniqueId: string) {
    const ws = (global as any).clients.get(uniqueId);
    return ws as WebSocket;
}

export const connectedClient = {
    add,
    remove,
    get,
};
