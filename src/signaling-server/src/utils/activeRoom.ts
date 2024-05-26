(global as any).rooms = new Map<string, string>();

function add(uniqueId: string, offerSDP: string) {
    (global as any).rooms.set(uniqueId, offerSDP);
}

function remove(uniqueId: string) {
    (global as any).rooms.delete(uniqueId);
}

function getAll() {
    const allEntries: IterableIterator<[string, string]> = (
        global as any
    ).rooms.entries();
    const entriesArray: { uniqueId: string; offerSDP: string }[] = Array.from(
        allEntries
    ).map(([key, value]) => ({ uniqueId: key, offerSDP: value }));
    return entriesArray;
}

export const activeRoom = {
    add,
    remove,
    getAll,
};
