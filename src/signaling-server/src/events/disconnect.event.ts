import { Socket } from 'socket.io';

export default function event(socket: Socket) {
    socket.on('disconnect', async () => {
        console.log('User disconnected');
    });
}
