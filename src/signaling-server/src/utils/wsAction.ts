import { sendAnswerSDP } from '../services/sendAnsSDP.service';
import { emitAction } from '../types/action';
import { ws } from '../types/ws';
import { activeRoom } from './activeRoom';

export function messageAction(ws: ws, action: emitAction) {
    switch (action.action) {
        case 'create_username':
            ws.username = action.data.username;
            break;

        case 'create_waiting_room':
            activeRoom.add(ws.clientId, ws.username, action.data.offerSDP);
            console.log(activeRoom.getAll());
            break;

        case 'remove_waiting_room':
            activeRoom.remove(ws.clientId);
            break;

        case 'get_waiting_room':
            const rooms = activeRoom.getAll();
            ws.send(
                JSON.stringify({
                    action: 'waiting_room',
                    data: rooms,
                })
            );
            break;

        case 'send_answer_sdp':
            sendAnswerSDP(action.data.clientID, action.data.answerSDP, ws);
            break;
    }
}
