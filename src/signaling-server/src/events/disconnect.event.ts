import { ws } from '../types/ws';
import { connectedClient } from '../utils/activeClient';
import { activeRoom } from '../utils/activeRoom';

export default function event(ws: ws) {
    ws.on('close', async () => {
        connectedClient.remove(ws.clientId);
        activeRoom.remove(ws.clientId);
        console.log('User disconnected');
    });
}
