import { ws } from '../types/ws';
import { connectedClient } from '../utils/activeClient';

export function sendAnswerSDP(
    clientId: string,
    answerSDP: string,
    senderWS: ws
) {
    const wsClient = connectedClient.get(clientId);
    wsClient.send(
        JSON.stringify({
            action: 'answer_sdp_received',
            data: {
                clientId: senderWS.clientId,
                username: senderWS.username,
                answerSDP,
            },
        })
    );
}
