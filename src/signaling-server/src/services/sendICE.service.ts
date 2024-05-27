import { ws } from '../types/ws';
import { connectedClient } from '../utils/activeClient';

export function sendICE(
    clientId: string,
    candidate: string,
    mid: string,
    senderWS: ws
) {
    try {
        const wsClient = connectedClient.get(clientId);
        wsClient.send(
            JSON.stringify({
                action: 'ice_received',
                data: {
                    clientId: senderWS.clientId,
                    username: senderWS.username,
                    candidate,
                    mid,
                },
            })
        );
    } catch (err) {
        console.log('error sending ice: ' + err);
    }
}
