import { connectedClient } from '../utils/activeClient';

export function emitUpdatedActiveCount() {
    const activeClients = connectedClient.getAll();
    activeClients.forEach((client) => {
        client.ws.send(
            JSON.stringify({
                action: 'active_count',
                data: {
                    count: activeClients.length,
                },
            })
        );
    });
}
