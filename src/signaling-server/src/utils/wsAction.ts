import { wsAction } from '../types/action';
import { ws } from '../types/ws';
import { activeRoom } from './activeRoom';

export function messageAction(ws: ws, action: wsAction) {
    switch (action.action) {
        case 'create_waiting_room':
            activeRoom.add(ws.clientId, action.data.offerSDP);
            break;

        case 'remove_waiting_room':
            activeRoom.remove(ws.clientId);
            break;

        case 'get_waiting_room':
            const rooms = activeRoom.getAll();
            ws.send(JSON.stringify(rooms));
    }
}
