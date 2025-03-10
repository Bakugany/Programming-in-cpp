Implement strqueue module. The strqueue module should provide the following functions.

unsigned long strqueue_new();

Creates a new, empty string queue and returns its identifier.

void strqueue_delete(unsigned long id);

If a string queue with the identifier id exists, it deletes it; otherwise, it does nothing.

size_t strqueue_size(unsigned long id);

If a string queue with the identifier id exists, it returns the number of its elements; otherwise, it returns 0.

void strqueue_insert_at(unsigned long id, size_t position, const char* str);

If a string queue with the identifier id exists and str != NULL, it inserts the string str before the element at the given position, or at the end of the queue if the position is greater than or equal to the number of strings in the queue. If the queue does not exist or str == NULL, it does nothing. Positions in the queue are numbered starting from zero.

void strqueue_remove_at(unsigned long id, size_t position);

If a string queue with the identifier id exists and the value of position is less than the number of strings in the queue, it removes the string at position; otherwise, it does nothing.

const char* strqueue_get_at(unsigned long id, size_t position);

If a string queue with the identifier id exists and the value of position is less than the number of strings in the queue, it returns a pointer to the string at that position; otherwise, it returns NULL.

void strqueue_clear(unsigned long id);

If a string queue with the identifier id exists, it removes all strings from the queue; otherwise, it does nothing.

int strqueue_comp(unsigned long id1, unsigned long id2);

Compares the string queues identified by id1 and id2 lexicographically, returning
-1, when strqueue(id1) < strqueue(id2),
 0, when strqueue(id1) == strqueue(id2),
 1, when strqueue(id1) > strqueue(id2).
If a string queue for any of the identifiers does not exist, it is considered as an empty queue.
