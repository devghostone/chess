import { WebSocket } from 'ws';

export interface ws extends WebSocket {
    clientId: string;
}
