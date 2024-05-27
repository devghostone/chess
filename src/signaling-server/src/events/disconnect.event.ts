import { emitUpdatedActiveCount } from '../services/activeCount.service';
import { ws } from '../types/ws';
import { connectedClient } from '../utils/activeClient';
import { activeRoom } from '../utils/activeRoom';

export default function event(ws: ws) {
    ws.on('close', async () => {
        try {
            console.log('User disconnected');
            connectedClient.remove(ws.clientId);
            activeRoom.remove(ws.clientId);
            emitUpdatedActiveCount();
        } catch (err) {
            console.log('Error disconnecting user: ' + err);
        }
    });
}
