#ifndef _OIC_HASHMAP_H
#define _OIC_HASHMAP_H

/** Hashmap structure (forward declaration) */
typedef struct {
  void* data;
  int flags;
  long key;
} hEntry;

typedef struct hashmap{
  hEntry* table;
  long size, count;
} hashmap;

// struct s_hashmap;
// typedef struct s_hashmap hashmap;

/** Creates a new hashmap near the given size. */
extern struct hashmap * hashmapCreate(int startsize);

/** Inserts a new element into the hashmap. */
extern void hashmapInsert(struct hashmap *, const void* data, unsigned long key);

/** Removes the storage for the element of the key and returns the element. */
extern void* hashmapRemove(struct hashmap *, unsigned long key);

/** Returns the element for the key. */
extern void* hashmapGet(struct hashmap *, unsigned long key);

/** Returns the number of saved elements. */
extern long hashmapCount(struct hashmap *);

/** Removes the hashmap structure. */
extern void hashmapDelete(struct hashmap *);

extern unsigned long hashmapGetHashVal(unsigned char* str);

#endif //_OIC_HASHMAP_H