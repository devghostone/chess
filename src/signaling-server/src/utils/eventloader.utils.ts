import fs from 'fs';
import path from 'path';
import { ws } from '../types/ws';

export function loadEvents(ws: ws) {
    const eventsPath = path.join(__dirname, '..', 'events');
    fs.readdirSync(eventsPath).forEach((file) => {
        if (file.endsWith('.ts') || file.endsWith('.js')) {
            const event = require(path.join(eventsPath, file)).default;
            if (event && typeof event === 'function') {
                try {
                    event(ws);
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
