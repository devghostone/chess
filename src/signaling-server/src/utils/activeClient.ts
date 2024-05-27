import { WebSocket } from 'ws';
import { ws } from '../types/ws';

(global as any).clients = new Map<string, WebSocket>();

function add(uniqueId: string, ws: WebSocket) {
    (global as any).clients.set(uniqueId, ws);
}

function remove(uniqueId: string) {
    (global as any).clients.delete(uniqueId);
}

function get(uniqueId: string) {
    const wsClient = (global as any).clients.get(uniqueId);
    return wsClient as ws;
}

function getAll() {
    const allEntries: IterableIterator<[string, WebSocket]> = (
        global as any
    ).clients.entries();
    const entriesArray: {
        uniqueId: string;
        ws: WebSocket;
    }[] = Array.from(allEntries).map(([key, value]) => ({
        uniqueId: key,
        ws: value,
    }));
    return entriesArray;
}

export const connectedClient = {
    add,
    remove,
    get,
    getAll,
};
