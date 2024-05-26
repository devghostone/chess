import { Socket } from 'socket.io';
import fs from 'fs';
import path from 'path';

export function loadEvents(socket: Socket) {
    const eventsPath = path.join(__dirname, '..', 'events');
    fs.readdirSync(eventsPath).forEach((file) => {
        if (file.endsWith('.ts') || file.endsWith('.js')) {
            const event = require(path.join(eventsPath, file)).default;
            if (event && typeof event === 'function') {
                try {
                    event(socket);
                } catch (error) {
                    console.error(
                        `Error executing event '${event.name}':`,
                        error
                    );
                }
            }
        }
    });
}
