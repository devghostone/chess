import { wsAction } from '../types/action';
import { ws } from '../types/ws';
import { messageAction } from '../utils/wsAction';

export default function event(ws: ws) {
    ws.on('message', async (message) => {
        const action = JSON.parse(`${message}`) as wsAction;
        console.log(`${ws.clientId} sent message.\n`);
        messageAction(ws, action);
    });
}
