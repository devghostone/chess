import { emitAction } from '../types/action';
import { ws } from '../types/ws';
import { messageAction } from '../utils/wsAction';

export default function event(ws: ws) {
    ws.on('message', async (message) => {
        try {
            const action = JSON.parse(`${message}`) as emitAction;
            console.log(`${ws.clientId} sent message.\n`);
            messageAction(ws, action);
        } catch (err) {
            console.log('Error reading emitted action: ' + err);
        }
    });
}
