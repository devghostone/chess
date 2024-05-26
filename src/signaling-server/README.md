### 📡 Signaling Server

This is a simple signaling server implementation using WebSocket (ws). To start the project, you must install the necessary packages using `npm, yarn, pnpm, or bun`. After installation, use the `run dev` command to start the development server. If you want to change the port, you can create a `.env` file by referencing `.env.example`.

#### 🤔 Basic Concept of How It Works

We are using WebSocket message channels to send data between the server and clients. Within this message data, we specify the type of actions we want to perform, along with the necessary data. Both the client and server will receive the action from the message data because both need to specify actions when sending data.

#### 🧠 Types of Actions to Consider

There are two types of actions to consider when implementing the signaling from the client app: EmitActions and ReceiveActions.

| ReceiveActions | actionName       | returnedData                                                                           | Comment                                                                                                                              |
| -------------- | ---------------- | -------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------ |
|                | socket_connected | `{ action: 'socket_connected', data: { uniqueID: ws.clientId } }`                      | This action will fire immediately after the client successfully connects to the socket.                                              |
|                | waiting_room     | `{ action: 'waiting_room', data: [{ uniqueId: 'unique_id', offerSDP: 'offer_sdp' }] }` | This action will provide information about waiting rooms. It will be triggered after the client emits the 'get_waiting_room' action. |

| EmitActions | actionName          | payloadData                                                   |
| ----------- | ------------------- | ------------------------------------------------------------- |
|             | create_waiting_room | `{ action: 'create_waiting_room', data: { offerSDP: 'gg' } }` |
|             | remove_waiting_room | `{ action: 'remove_waiting_room', data: null }`               |
|             | get_waiting_room    | `{ action: 'get_waiting_room', data: null }`                  |
