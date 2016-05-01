#ifndef _BPTREE_H_
#define _BPTREE_H_

#include <stdint.h>

#define BPT_OK 0
#define BPT_ERR 1

#ifndef bool
#define bool char
#define false 0
#define true 1
#endif

#define BPT_DEFAULT_ORDER 32

#define BPTREE_NOTUSED(V) ((void) V)

//存储key-value对
typedef struct bptreeEntry {
    void *key;
    union {
        void * val;
        uint64_t u64;
        int64_t s64;
        double d;
    } v;
} bptreeEntry;

typedef struct bptreeNode {
    union {
		//分支结点(存储key，指向孩子的指针)
        struct {
            void **keys;
            struct bptreeNode **children;
        }bptreeInnerNode;

		//叶子结点(存储所有的key-value对，叶子结点前后相连)
        struct {
            bptreeEntry **entries;
            struct bptreeNode *next;
        }bptreeLeafNode;
    } n;
    uint32_t count;  //此结点 key/孩子结点 的个数
    uint32_t level;  //此结点 到 叶子结点之间的 层数
    bool leaf;   //是否是叶子结点
} bptreeNode;

typedef struct bptreeType {
    void *(*keyDup)(void *privdata, const void *key);
    void *(*valDup)(void *privdata, const void *obj);
    int (*keyCompare)(void *privdata, const void *key1, const void *key2);
    void (*keyDestructor)(void *privdata, void *key);
    void (*valDestructor)(void *privdata, void *obj);
} bptreeType;

typedef struct bptree{
    bptreeType *type;
    void *privdata;
    uint32_t innerOrder;  //分支结点中可存储的 最大的关键字个数 innerOrder-1
    uint32_t leafOrder;   //叶结点中可存储的 最大的关键字个数 leafOrder-1
    uint64_t count;    //树中 关键字的总数
	bptreeNode *root;  //根结点
	bptreeNode *min;   //最小key的所在结点
    int iterators;
} bptree;

typedef struct bptreeIterator {
    bptree *t;
    bptreeNode *n;
    long index;
    int safe;
    bptreeEntry *entry;
    long long fingerprint;
} bptreeIterator;

typedef void (bptreeScanFunction)(void *privdata, const bptreeEntry * be);

/* ------------------------------- Macros ------------------------------------*/
#define bptreeFreeVal(t, entry) \
    if ((t)->type->valDestructor) \
        (t)->type->valDestructor((t)->privdata, (entry)->v.val)

#define bptreeSetVal(t, entry, _val_) do {\
    if ((t)->type->valDup) \
        entry->v.val = (t)->type->valDup((t)->privdata, _val_); \
    else \
        entry->v.val = (_val_); \
} while(0)

#define bptreeSetSignedIntegerVal(entry, _val) \
    do { entry->v.s64 = _val_; } while(0)

#define bptreeSetUnsignedIntegerVal(entry, _val_) \
    do { entry->v.u64 = _val_; } while(0)

#define bptreeSetDoubleVal(entry, _val_) \
    do { entry->v.d = _val_; } while(0)

#define bptreeFreeKey(t, entry) \
    if ((t)->type->keyDestructor) \
        (t)->type->keyDestructor((t)->privdata, (entry)->key)

#define bptreeSetKey(t, entry, _key_) do { \
    if ((t)->type->keyDup) \
        entry->key = (t)->type->keyDup((t)->privdata, _key_); \
    else \
        entry->key = (_key_); \
} while(0)

#define bptreeCompareKeys(t, key1, key2) \
    (((t)->type->keyCompare) ? \
        (t)->type->keyCompare((t)->privdata, key1, key2) : \
        (key1) == (key2))

#define bptreeGetKey(he) ((he)->key)
#define bptreeGetVal(he) ((he)->v.val)
#define bptreeGetSignedIntegerVal(he) ((he)->v.s64)
#define bptreeGetUnsignedIntegerVal(he) ((he)->v.u64)
#define bptreeGetDoubleVal(he) ((he)->v.d)
#define bptreeSlots(t) (t->count)
#define bptreeSize(t) (t->count)
#define bptreeIsRehashing(t) 0

//APIs
bptree * bptreeCreate(bptreeType *type, void *privDataPtr);
int bptreeAdd(bptree *t, void *key, void *val);  //只是增加，如果已存在相同的key，则不插入，并返回err
bptreeEntry *bptreeAddRaw(bptree *t, void *key);
int bptreeReplace(bptree * t, void *key, void *val);
bptreeEntry *bptreeReplaceRaw(bptree *t, void *key);
int bptreeDelete(bptree *t, const void *key);
int bptreeDeleteNoFree(bptree *t, const void *key);
void bptreeRelease(bptree *t);
bptreeEntry *bptreeFind(bptree *t, const void *key);
void bptreeBulkLoad(bptree *t, bptreeEntry *e[], uint64_t count);
uint64_t bptreeQuery(bptree *t, bptreeEntry** output, uint64_t len, void *from, void *end);
bptree *bptreeSplit(bptree *t, void *splitKey);
//Iterator
bptreeIterator *bptreeGetIterator(bptree *t);
bptreeIterator *bptreeGetSafeIterator(bptree *t);
bptreeEntry *bptreeNext(bptreeIterator *iter);
void bptreeReleaseIterator(bptreeIterator *iter);
//others
bptreeEntry *bptreeGetRandomKey(bptree *t);
unsigned int bptreeGetSomeKeys(bptree *t, bptreeEntry **des, unsigned int count);
void bptreeEmpty(bptree *t, void(callback)(void*));
unsigned long bptreeScan(bptree *t, unsigned long v, bptreeScanFunction *fn, void *privdata);

#endif
