export interface wsAction {
    action: 'create_waiting_room' | 'remove_waiting_room' | 'get_waiting_room';
    data: any;
}
