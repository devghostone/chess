(global as any).rooms = new Map<
    string,
    { username: string; offerSDP: string }
>();

function add(uniqueId: string, username: string, offerSDP: string) {
    if ((global as any).rooms.get(uniqueId)) return;
    (global as any).rooms.set(uniqueId, {
        username,
        offerSDP: `${offerSDP}`,
    });
}

function remove(uniqueId: string) {
    (global as any).rooms.delete(uniqueId);
}

function getAll() {
    const allEntries: IterableIterator<
        [string, { username: string; offerSDP: string }]
    > = (global as any).rooms.entries();
    const entriesArray: {
        uniqueId: string;
        username: string;
        offerSDP: string;
    }[] = Array.from(allEntries).map(([key, value]) => ({
        uniqueId: key,
        username: value.username,
        offerSDP: value.offerSDP,
    }));
    return entriesArray;
}

export const activeRoom = {
    add,
    remove,
    getAll,
};
