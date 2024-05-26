import { io } from '../libs/server';
import { loadEvents } from '../utils/eventloader.utils';

function handleSocketEvents() {
    console.log('Waiting for socket events');
    io.on('connection', async (socket) => {
        console.log('A user connected. Id : ' + socket.id);
        loadEvents(socket);
    });
}

export { handleSocketEvents };
