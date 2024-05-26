import 'dotenv/config';
import { httpServer } from './libs/server';
import { handleSocketEvents } from './services/socket.service';

httpServer.listen(process.env.PORT || 3000, () => {
    console.log(`Server is running on port ${process.env.PORT || 3000}`);
    handleSocketEvents();
});
