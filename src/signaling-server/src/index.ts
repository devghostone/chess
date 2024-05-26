import 'dotenv/config';
import { webSocket } from './libs/server';
import { v4 as uuidv4 } from 'uuid';
import { connectedClient } from './utils/activeClient';
import { ws } from './types/ws';
import { loadEvents } from './utils/eventloader.utils';

const port = process.env.PORT ? +process.env.PORT : 3000;
const wss = webSocket.createServer(port);

wss.on('connection', (ws: ws) => {
    ws.clientId = uuidv4();
    ws.send(
        JSON.stringify({
            action: 'socket_connected',
            data: {
                uniqueID: ws.clientId,
            },
        })
    );
    connectedClient.add(ws.clientId, ws);
    loadEvents(ws);
});
