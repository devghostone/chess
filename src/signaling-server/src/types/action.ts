export interface emitAction {
    action:
        | 'create_username'
        | 'create_waiting_room'
        | 'remove_waiting_room'
        | 'get_waiting_room'
        | 'send_answer_sdp';
    data: any;
}
