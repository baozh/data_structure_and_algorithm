//#define TEST
#ifndef TEST
#include "fmacros.h"
#endif
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "bptree.h"

#ifndef TEST
#include "zmalloc.h"
#include "redisassert.h"
#define bptreemalloc zmalloc
#define bptreecalloc zcalloc
#define bptreefree zfree
#else
#include <assert.h>
#define bptreemalloc malloc
#define bptreecalloc(t) calloc(1, t)
#define bptreefree free
#endif

#define pos_t uint32_t

typedef struct bptreePathInfo{
    int         find;
	pos_t*      p;    //保存一路从根结点到叶子结点  查找路径中 经历的每层的结点 在父结点中 child[]数组中的索引
	bptreeNode  **n;  //保存一路从根结点到叶子结点  查找路径中 经历的每层的结点
} bptreePathInfo;

typedef struct scanObject{
    uint32_t pos;
    uint32_t node;
} scanObject;

typedef struct bulkLoadObject{
    bptreeNode *node;
    void *key;
} bulkLoadObject;

bptreeEntry *_bptreeFindEntry(bptree *t, const void *key, bptreePathInfo *path);
int _insertIntoInnerNode(bptree *t, bptreePathInfo *path, bptreeNode *n, void *nk);
int _insertIntoLeafNode(bptree *t, bptreePathInfo *p, bptreeEntry *e);
int _deleteFromLeafNode(bptree *t, bptreePathInfo *p, const void *delkey);

static bptreeNode *_allocateNode(bptree *t, bool leaf) 
{
    bptreeNode * node = NULL;
    node = bptreecalloc(sizeof(*node));

    if(node) 
	{
        node->leaf = leaf;

        if(leaf) 
		{
            node->n.bptreeLeafNode.entries = bptreemalloc((t->leafOrder - 1) * sizeof(bptreeEntry *));
        } 
		else 
		{
			node->n.bptreeInnerNode.keys = bptreemalloc((t->innerOrder - 1) * sizeof(void *));
			node->n.bptreeInnerNode.children = bptreemalloc(t->innerOrder * sizeof(bptreeNode *));
        }
    }

    return node;
}

static void _freeNode(bptreeNode *n) {
    if(n) {
        if(n->leaf) {
            bptreefree(n->n.bptreeLeafNode.entries);
        } else {
            bptreefree(n->n.bptreeInnerNode.keys);
            bptreefree(n->n.bptreeInnerNode.children);
        }
        bptreefree(n);
        n = NULL;
    }
}

static bptreePathInfo *_getPathInfoObj(bptree *t){

    bptreePathInfo *path = NULL;
	path = bptreemalloc(sizeof(bptreePathInfo));
	path->find = 0;
	path->p = bptreemalloc((t->root ? (t->root->level + 1) : 1) * sizeof(pos_t));
	path->n = bptreemalloc((t->root ? (t->root->level + 1) : 1) * sizeof(bptreeNode *));

    return path;
}

static void _freePathInfoObj(bptreePathInfo *path){
    bptreefree(path->p);
    bptreefree(path->n);
    bptreefree(path);
}

bptree *bptreeCreate(bptreeType *type,
            void *privDataPtr) {
    bptree *t = bptreemalloc(sizeof(*t));

    if(t) {
        t->innerOrder = BPT_DEFAULT_ORDER;
        t->leafOrder = BPT_DEFAULT_ORDER * 2;
        t->root = NULL;
        t->min = t->root;
        t->type = type;
        t->privdata = privDataPtr;
        t->iterators = 0;
    }
    return t;
}

int bptreeAdd(bptree *t, void *key, void * val) {
    bptreeEntry *entry = bptreeAddRaw(t, key);

    if(!entry) return BPT_ERR;
    bptreeSetVal(t, entry, val);
    return BPT_OK;
}

/* Low level add. This function adds the entry but instead of setting
 * a value returns the bptreeEntry structure to the user, that will make
 * sure to fill the value field as he wishes.
 *
 * This function is also directly exposed to the user API to be called
 * mainly in order to store non-pointers inside the hash value, example:
 *
 * entry = btreeAddRaw(bptree,mykey);
 * if (entry != NULL) btreeSetSignedIntegerVal(entry,1000);
 *
 * Return values:
 *
 * If key already exists NULL is returned.
 * If key was added, the hash entry is returned to be manipulated by the caller.
 */

bptreeEntry *bptreeAddRaw(bptree *t, void *key) {
    bptreeEntry *e = NULL;
	//为path分配空间，并初始化
    bptreePathInfo *path = _getPathInfoObj(t);

    //key already exists
	//从树中查找key，并记录 一路上查找路径中 经历的结点、child索引
    if(NULL != _bptreeFindEntry(t, key, path)){
        _freePathInfoObj(path);   //释放path空间
        return NULL;
    }

    e = bptreemalloc(sizeof(*e));  //分配一个bptreEntry空间
    if(e) {
        bptreeSetKey(t, e, key);

        //insert into tree
        if(BPT_ERR == _insertIntoLeafNode(t, path, e)) {
			bptreefree(e);
			e = NULL;
        }
    }
    _freePathInfoObj(path);
    return e;
}

/* Add an element, discarding the old if the key already exists.
 * Return 1 if the key was added from scratch, 0 if there was already an
 * element with such key and bptreeReplace() just performed a value update
 * operation. */
int bptreeReplace(bptree *t, void *key, void *val) {
    bptreeEntry *e = NULL;
    bptreeEntry auxentry;
    bptreePathInfo *path = _getPathInfoObj(t);

    //key already exists
    e = _bptreeFindEntry(t, key, path);
    if(NULL != e){   //如果 在树中找到key
        _freePathInfoObj(path);
        auxentry = *e; //copy in bits;
        //insert in this order because of reference counting
        bptreeSetVal(t, e, val);      //设置value
        bptreeFreeVal(t, &auxentry);  //释放原来的value空间
        return 1;
    }
    //add
    e = bptreemalloc(sizeof(*e));
    if(e) {
        bptreeSetKey(t, e, key);
        bptreeSetVal(t, e, val);
        //insert into tree
        if(BPT_ERR == _insertIntoLeafNode(t, path, e)) {   //插入结点
            bptreefree(e);
            e = NULL;
        }
    }
    _freePathInfoObj(path);
    return 0;
}

/* bptreeReplaceRaw() is simply a version of bptreeAddRaw() that always
 * returns the hash entry of the specified key, even if the key already
 * exists and can't be added (in that case the entry of the already
 * existing key is returned.)
 *
 * See bptreeAddRaw() for more information. */
bptreeEntry *bptreeReplaceRaw(bptree *t, void *key) {
    bptreeEntry *e = NULL;
    bptreePathInfo *path = _getPathInfoObj(t);

    //key already exists
    e = _bptreeFindEntry(t, key, path);
    if(NULL != e){
        _freePathInfoObj(path);
        return e;
    }
    //add
    e = bptreemalloc(sizeof(*e));
    if(e) {
        bptreeSetKey(t, e, key);

        //insert into tree
        if(BPT_ERR == _insertIntoLeafNode(t, path, e)) {
            bptreefree(e);
            e = NULL;
        }
    }
    _freePathInfoObj(path);
    return e;
}

/* Search and remove an element */
static int bptreeGenericDelete(bptree *t,const void *key, int nofree) {
    bptreePathInfo *path = NULL;
    bptreeEntry *e = NULL;

    path = _getPathInfoObj(t);
    e = _bptreeFindEntry(t, key, path);

    if(!e){
        _freePathInfoObj(path);
        return BPT_ERR;
    }

    if(BPT_OK == _deleteFromLeafNode(t, path, key)) {
        if(!nofree) {
            bptreeFreeKey(t, e);
            bptreeFreeVal(t, e);
        }
        bptreefree(e);
    }

    _freePathInfoObj(path);
    return BPT_OK;
}

int bptreeDelete(bptree *t, const void *key) {
    return bptreeGenericDelete(t, key, 0);
}

int bptreeDeleteNoFree(bptree *t, const void *key) {
    return bptreeGenericDelete(t, key, 1);
}

int _bptreeClearNode(bptree *t, bptreeNode *node, int freeEntry) {
    uint32_t i = 0;
    bptreeEntry *e = NULL;

    if(!node) return BPT_OK;

    if(node->leaf) {
        for(i = 0; i < node->count; ++i) {
            e = node->n.bptreeLeafNode.entries[i];
            if(freeEntry) {
                bptreeFreeKey(t, e);
                bptreeFreeVal(t, e);
                bptreefree(e);
            }
            node->n.bptreeLeafNode.entries[i] = NULL;
            node->n.bptreeLeafNode.next = NULL;
        }
    } else {
        for(i = 0; i <= node->count; ++i) {
            _bptreeClearNode(t, node->n.bptreeInnerNode.children[i], freeEntry);
            node->n.bptreeInnerNode.children[i] = NULL;
        }
    }

    _freeNode(node);
    return BPT_OK;
}

int _bptreeClear(bptree *t, void(callback)(void *)) {
    BPTREE_NOTUSED(callback);
    return _bptreeClearNode(t, t->root, 1);
}

int _bptreeClearNotFreeEntry(bptree *t){
    return _bptreeClearNode(t, t->root, 0);
}

void bptreeRelease(bptree *t){
    _bptreeClear(t, NULL);
    bptreefree(t);
}

bptreeEntry *_bptreeFindEntry(bptree *t, const void *key, bptreePathInfo *path) {
    pos_t i;
    int cpResult;
    bptreeNode *node;

    if(t->root == NULL) {
        path->n[0] = NULL;
        path->p[0] = 0;
        return NULL;
    }

    node = t->root;
    path->find = 0;
	//从根结点到叶结点，一路找，直到叶子结点
    while(node) {
        if(node->leaf) break;

        for(i = 0; i < node->count; ++i) 
		{
			//找 第一个大于key的 child结点的索引
            if(0 > bptreeCompareKeys(t,
                                    key,
                                    node->n.bptreeInnerNode.keys[i])) 
			{
                break;
            }
        }
        path->n[node->level] = node;  //保存 查找路径中 经历的每层的结点
        path->p[node->level] = i;     // => children[i] 是<= key的 最大结点

		node = node->n.bptreeInnerNode.children[i];
    }
    path->n[0] = node;

    for(i = 0; i < node->count; ++i) {
        if((cpResult = bptreeCompareKeys(t, key, bptreeGetKey(node->n.bptreeLeafNode.entries[i]))) < 0) 
		{
			//找到第一个 大于key的 关键字key[]数组的索引
            break;
        }
		else if (cpResult == 0) 
		{
            path->find = 1;
            path->p[0] = i;
            return node->n.bptreeLeafNode.entries[i];
        }
    }
    path->p[0] = i;

    return NULL;
}

bptreeEntry *bptreeFind(bptree *t, const void *key) {
    bptreePathInfo *path = NULL;
    bptreeEntry *e = NULL;

    if(!t || !key)  return NULL;

    path = _getPathInfoObj(t);
    e = _bptreeFindEntry(t, key, path);
    _freePathInfoObj(path);
    return e;
}

/* finggerprint */
long long bptreeFingerprint(bptree *t) {
    long long integers[5], hash = 0;
    int j;

    integers[0] = t->innerOrder;
    integers[1] = t->leafOrder;
    integers[2] = t->count;
    integers[3] = (long)t->root;
    integers[4] = t->root->level;
    //calc fingerprint
    for(j = 0; j < 5; ++j) {
        hash += integers[j];
        /*hash*/
        hash = (~hash) + (hash << 21);
        hash = hash ^ (hash >> 24);
        hash = (hash + (hash << 3)) + (hash << 8);
        hash = hash ^ (hash >> 14);
        hash = (hash + (hash << 2)) + (hash << 4);
        hash = hash ^ (hash >> 28);
        hash = hash + (hash << 31);
    }

    return hash;
}

bptreeIterator *bptreeGetIterator(bptree *t) {
    bptreeNode *node;
    bptreeIterator *iter = bptreecalloc(sizeof(*iter));

    node = t->min;

    if(iter){
        iter->t = t;
        iter->n = node;
        iter->index = -1;
    }

    return iter;
}

bptreeIterator *bptreeGetSafeIterator(bptree *t) {
    bptreeIterator *i = bptreeGetIterator(t);

    i->safe = 1;
    return i;
}

bptreeEntry *bptreeNext(bptreeIterator *iter) {
    bptreeEntry *e = NULL;
    //todo
    if(iter) {
        if(iter->n == NULL)
            return NULL;

        if(iter->index == -1) {
            if(iter->safe)
                iter->t->iterators++;
            else
                iter->fingerprint = bptreeFingerprint(iter->t);

            iter->index = 0;
        }
        if(iter->index >= iter->n->count) {
            iter->n = iter->n->n.bptreeLeafNode.next;
            if(iter->n) {
                iter->index = 0;
            }
        }
        if(iter->n) {
            e = iter->n->n.bptreeLeafNode.entries[iter->index];
            ++iter->index;
        }
    }
    return e;
}

void bptreeReleaseIterator(bptreeIterator *iter) {
    if(iter->safe)
        iter->t->iterators--;
    else
        assert(iter->fingerprint == bptreeFingerprint(iter->t));

    bptreefree(iter);
}

bptreeEntry *bptreeGetRandomKey(bptree *t) {
    bptreeNode *node = NULL;
    uint64_t i = 0;

    if(t->count == 0) return NULL;

    i = random();
    node = t->root;

    if(node == NULL || node->count == 0) return NULL;

    while(!node->leaf) {
        node = node->n.bptreeInnerNode.children[i%(node->count + 1)];
    }

    return node->n.bptreeLeafNode.entries[i%(node->count)];
}

unsigned int bptreeGetSomeKeys(bptree *t, bptreeEntry **des, unsigned int count) {
    bptreeNode *node = NULL;
    unsigned int stored = 0;
    unsigned int base = 0;
    unsigned int step = 0;
    unsigned int offset = 0;
    unsigned int index = 0;

    if(count == 0) return 0;
    if(t->count < count) count = t->count;

    step = ceil((double)t->count/count);
    offset = random()%step;
    node = t->min;
    index = offset;

    while((base + offset) < t->count) {
        while(node && index >= node->count) {
            index -= node->count;
            node = node->n.bptreeLeafNode.next;
        }
        if(node == NULL || node->count == 0) return stored;

        des[stored] = node->n.bptreeLeafNode.entries[index];
        ++stored;

        index += step;
        base += step;
    }

    return stored;
}

unsigned long bptreeScan(bptree *t,
                        unsigned long v,
                        bptreeScanFunction *fn,
                        void *privdata) {
    scanObject *s = NULL;
    uint32_t i = 0;
    bptreeNode *node = NULL;

    s = (scanObject *)&v;
    node = t->min;

    while(node && i < s->node) {
        node = node->n.bptreeLeafNode.next;
        ++i;
    }

    if(node == NULL) return 0;

    if(s->pos >= node->count){
        if(node->n.bptreeLeafNode.next == NULL)
            return 0;

        ++s->node;
        node = node->n.bptreeLeafNode.next;
        s->pos = 0;
    }

    fn(privdata, node->n.bptreeLeafNode.entries[s->pos]);
    ++s->pos;

    return v;
}

void bptreeEmpty(bptree * t, void(callback)(void*)){
    _bptreeClear(t, callback);
    t->root = NULL;
    t->min = NULL;
    t->count = 0;
    t->iterators = 0;
}

/* private function */
//将 结点rc的父结点 分裂成两个结点
int _insertSplitInnerNode(bptree *t, bptreePathInfo *path, bptreeNode *rc, void *newk) 
{
    bptreeNode *nn = NULL;
    bptreeNode *n = NULL;
    bptreeNode *lc = NULL;
    void *nk = NULL;
    uint32_t half = 0;
    pos_t p = 0;

    half = (t->innerOrder- 1) >> 1;
    nn = _allocateNode(t, false);   //右结点nn 分配空间
    if(!nn) return BPT_ERR;

    lc = path->n[rc->level];
    n = path->n[rc->level + 1];
    p = path->p[rc->level + 1];
    nn->level = n->level;

    if(p == half) 
	{
        //newkey is the newkey, add lc into n, add rc into nn
        memmove(&nn->n.bptreeInnerNode.keys[0], &n->n.bptreeInnerNode.keys[half], (n->count - half) * sizeof(void *));
        memmove(&nn->n.bptreeInnerNode.children[1], &n->n.bptreeInnerNode.children[half + 1], (n->count - half) * sizeof(bptreeNode *));
        nn->n.bptreeInnerNode.children[0] = rc;
        nn->count = n->count - half;
        n->count = half;
        nk = newk;
    } 
	else if(p < half) 
	{
		//将newk 插入到 左父结点
        //n->bptreeInnerNode.keys[half - 1] is the newkey, add lc, rc into n
        memmove(&nn->n.bptreeInnerNode.keys[0], &n->n.bptreeInnerNode.keys[half], (n->count - half) * sizeof(void *));
        memmove(&nn->n.bptreeInnerNode.children[0], &n->n.bptreeInnerNode.children[half], (n->count - half + 1) * sizeof(bptreeNode *));
        nk = n->n.bptreeInnerNode.keys[half - 1];
        memmove(&n->n.bptreeInnerNode.keys[p + 1], &n->n.bptreeInnerNode.keys[p], ((half - 1) - p) * sizeof(void *));
        memmove(&n->n.bptreeInnerNode.children[p + 2], &n->n.bptreeInnerNode.children[p + 1], ((half - 1) - p) * sizeof(bptreeNode *));
        n->n.bptreeInnerNode.keys[p] = newk;
        n->n.bptreeInnerNode.children[p + 1] = rc;
        nn->count = n->count - half;
        n->count = half;
    } 
	else if(p > half) 
	{
		//将newk 插入到 右父结点
        //n->bptreeInnerNode.keys[half] is the newkey, add lc, rc into nn
        memmove(&nn->n.bptreeInnerNode.keys[0], &n->n.bptreeInnerNode.keys[half + 1], (p - half - 1) * sizeof(void *));
        memmove(&nn->n.bptreeInnerNode.children[0], &n->n.bptreeInnerNode.children[half + 1], (p - half - 1) * sizeof(bptreeNode *));
        nn->n.bptreeInnerNode.keys[p - half - 1] = newk;
        nn->n.bptreeInnerNode.children[p - half - 1] = lc;
        nn->n.bptreeInnerNode.children[p - half] = rc;
        memmove(&nn->n.bptreeInnerNode.keys[p - half], &n->n.bptreeInnerNode.keys[p], (n->count - p) * sizeof(void *));
        memmove(&nn->n.bptreeInnerNode.children[p - half + 1], &n->n.bptreeInnerNode.children[p + 1], (n->count - p) * sizeof(bptreeNode *));
        nk = n->n.bptreeInnerNode.keys[half];
        nn->count = n->count - half;
        n->count = half;
    }

    return _insertIntoInnerNode(t, path, nn, nk);
}

int _insertIntoInnerNodeNotFull(bptreePathInfo *path, bptreeNode *n, void *nk) {
    bptreeNode *pn = NULL;  //parent node
    pos_t p = 0;

    pn = path->n[n->level + 1];
    p = path->p[n->level + 1];

	//调整父结点的keys[], children[]，然后将 nk 插入到 父结点的keys[p]
    memmove(&pn->n.bptreeInnerNode.keys[p + 1], &pn->n.bptreeInnerNode.keys[p], (pn->count - p) * sizeof(void *));
    memmove(&pn->n.bptreeInnerNode.children[p + 2], &pn->n.bptreeInnerNode.children[p + 1], (pn->count - p) * sizeof(bptreeNode *));
    pn->n.bptreeInnerNode.keys[p] = nk;
    pn->n.bptreeInnerNode.children[p + 1] = n;
    ++pn->count;

    return BPT_OK;
}

//将nk 插入到 n的父结点中
int _insertIntoInnerNode(bptree *t, bptreePathInfo *path, bptreeNode *n, void *nk) {
    bptreeNode *node = NULL;
    uint32_t l = 0;

    l = n->level + 1;

    if(l > t->root->level) {   //说明 n 是根结点, 需要将 它分裂成两个结点。此时 树高增加一层.
        //root node
        node = _allocateNode(t, false);
        if(!node) return BPT_ERR;

        node->level = l;
        node->count = 1;
        node->n.bptreeInnerNode.keys[0] = nk;
        node->n.bptreeInnerNode.children[0] = path->n[n->level];
        node->n.bptreeInnerNode.children[1] = n;
        t->root = node;
    } else {
        node = path->n[l];  //n的父结点: node
        if(node->count == (t->innerOrder- 1)) {   //父结点插入key后，若导致 key个数 满了 => 父结点分裂
            return _insertSplitInnerNode(t, path, n, nk);
        } else {
            return _insertIntoInnerNodeNotFull(path, n, nk);
        }
    }

    return BPT_OK;
}

int _insertSpiltLeafNode(bptree *t, bptreePathInfo *path, bptreeEntry *e) 
{
    bptreeNode* nn = NULL;
    bptreeNode* n = NULL;
    void * nk = NULL;
    uint32_t half = 0;
    pos_t p = 0;

    half = t->leafOrder>> 1;
    n = path->n[0];
    p = path->p[0];            //e 应插入到叶子结点的entry数组的索引
    nn = _allocateNode(t, n->leaf);   //分裂成左、右两个结点，则new右兄弟结点的空间
    if(!nn) return BPT_ERR;

	nn->level = n->level;    //层级相同

    if(p < half)    //e应该 插入到左结点
	{
        //insert into node
		//将 左结点中entry[half-1...leafOrder-1] 拷贝到 右结点中
        memmove(&nn->n.bptreeLeafNode.entries[0], &n->n.bptreeLeafNode.entries[half - 1], (t->leafOrder- half) * sizeof (bptreeEntry *));
		//将左结点中的 entry[p...half - 1] 后移一个位置，腾出位置entry[p]
        memmove(&n->n.bptreeLeafNode.entries[p + 1], &n->n.bptreeLeafNode.entries[p], ((half - 1) - p) * sizeof (bptreeEntry *));
        n->n.bptreeLeafNode.entries[p] = e; //插入
        nn->count = t->leafOrder- half;
        n->count = half;
    } 
	else  //e应该 插入到右结点
	{
        //insert into newnode
		//将 左结点中entry[p...count-p] 拷贝到 右结点[p - (half - 1)]中
        memmove(&nn->n.bptreeLeafNode.entries[p - (half - 1)], &n->n.bptreeLeafNode.entries[p], (n->count - p) * sizeof (bptreeEntry *));
		nn->n.bptreeLeafNode.entries[p - half] = e;   //插入
		//将 左结点中entry[] 拷贝到 右结点[0 ...p-half-1]中
		memmove(&nn->n.bptreeLeafNode.entries[0], &n->n.bptreeLeafNode.entries[half], (p - half) * sizeof (bptreeEntry *));
        nn->count = t->leafOrder - half;
        n->count = half;
    }

	nk = bptreeGetKey(nn->n.bptreeLeafNode.entries[0]);
	//将nn结点 链接到n 结点之后
	nn->n.bptreeLeafNode.next = n->n.bptreeLeafNode.next;
	n->n.bptreeLeafNode.next = nn;

	//将nn插入作为到 n结点的父结点
    return _insertIntoInnerNode(t, path, nn, nk);
}

int _insertIntoLeafNodeNotFull(bptreePathInfo *p, bptreeEntry *e) 
{
	//将叶子结点中的 entry[p[0]...count] 后移一个位置，腾出位置entry[p[0]]
    memmove(&p->n[0]->n.bptreeLeafNode.entries[p->p[0] + 1],
			&p->n[0]->n.bptreeLeafNode.entries[p->p[0]], 
			(p->n[0]->count - p->p[0]) * sizeof(bptreeEntry *));
    p->n[0]->n.bptreeLeafNode.entries[p->p[0]] = e;  //插入e
    ++p->n[0]->count;

    return BPT_OK;
}

int _insertIntoLeafNode(bptree *t, bptreePathInfo *p,  bptreeEntry *e) {
    int result = BPT_OK;

    if(t->root == NULL) {
        t->root = _allocateNode(t, true);
        t->min = t->root;
        p->n[0] = t->root;
    }

    if(p->n[0]->count == (t->leafOrder- 1))     //entry插入后，此节点的关键字个数 满了，所以需要 分裂
        result = _insertSpiltLeafNode(t, p, e);
    else    //插入后，不需要分裂
        result = _insertIntoLeafNodeNotFull(p, e);

    ++t->count;

    return result;
}

//找 子树n 的 最小的entry(往最左路径的那个entry)
bptreeEntry *_findMinEntry(bptreeNode *n) {
    bptreeNode* node = NULL;

    node = n;
    while(!node->leaf) {
        node = node->n.bptreeInnerNode.children[0];
    }

    return node->n.bptreeLeafNode.entries[0];
}

//delete
//将 path中 level-1层的 最小key 赋到 其父结点中，作为B+树中 分支结点的 关键字索引 
int _updateInnerNode(bptreePathInfo *path, uint32_t level) {
    bptreeEntry *nk = NULL;
    pos_t p = 0;

    p = path->p[level];
	//path->n[level]->n.bptreeInnerNode.children[p + 1]: 是path中 第level-1层 path->n[level-1]结点的 右兄弟结点
    nk = _findMinEntry(path->n[level]->n.bptreeInnerNode.children[p + 1]);
    path->n[level]->n.bptreeInnerNode.keys[p] = bptreeGetKey(nk);  //将 nk的key 赋到 它的父结点的位置[p]

    return BPT_OK;
}

//将path中 涉及的结点 删除delkey
int _removeKeyInPath(bptree *t, bptreePathInfo *p, const void *delkey){
    bptreeNode *node = NULL;
    void *nextkey = NULL;
    pos_t pos = 0;
    uint32_t i = 0;

    pos = p->p[0];   //待删除的delkey 在 叶子结点中的索引
    node = p->n[0];  //叶子结点
    if(pos == 0) {
        if(pos < (node->count - 1)) {
            nextkey = bptreeGetKey(node->n.bptreeLeafNode.entries[pos + 1]);
        } else if(node->n.bptreeLeafNode.next) {
            nextkey = bptreeGetKey(node->n.bptreeLeafNode.next->n.bptreeLeafNode.entries[0]);
        }

        if(nextkey) 
		{
            //if delete key in path update the key
            for(i = 1; i <= t->root->level; ++i) 
			{
                node = p->n[i];
                pos = p->p[i];
                if(pos == 0) 
					continue;
                else 
				{
					//若删除的delkey 在分支结点中也有，则用 nextkey 替换 delkey
                    if(bptreeCompareKeys(t, delkey, node->n.bptreeInnerNode.keys[pos - 1]) == 0) 
					{
                        node->n.bptreeInnerNode.keys[pos - 1] = nextkey;
                        break;
                    }
                }
            }
        }
    }

    return BPT_OK;
}

int _deleteFromInnerNode(bptree *t, bptreePathInfo *path, uint32_t level) 
{
	bptreeNode *n = NULL;
	bptreeNode *ln = NULL;
	bptreeNode *rn = NULL;
	unsigned int half = 0;
	pos_t p = 0;
	pos_t posInParent = 0;

	half = (t->innerOrder- 1) >> 1;
	p = path->p[level];
	n = path->n[level];

    if (n->count > half)   //若结点n 比较丰满
	{
        //delete
		//删除 结点p的元素 key[p],  children[p+1]
        memmove(&n->n.bptreeInnerNode.keys[p], &n->n.bptreeInnerNode.keys[p + 1], (n->count - 1 - p) * sizeof(void *));
        memmove(&n->n.bptreeInnerNode.children[p + 1], &n->n.bptreeInnerNode.children[p + 2], (n->count - 1 - p) * sizeof(bptreeNode *));
        --n->count;
        return BPT_OK;
    }

    if(level == t->root->level)   //到根结点层
	{
        if(t->root->count == 1)   //若此时 根结点只有一个关键字了，则 直接删除这个根结点（树的高度会减1）
		{
            //decrease height
            n = t->root;
            t->root = n->n.bptreeInnerNode.children[0];
            _freeNode(n);
        }
        else 
		{
            //delete
			//删除 结点p的元素 key[p],  children[p+1]
            memmove(&n->n.bptreeInnerNode.keys[p], &n->n.bptreeInnerNode.keys[p + 1], (n->count - 1 - p) * sizeof(void *));
            memmove(&n->n.bptreeInnerNode.children[p + 1], &n->n.bptreeInnerNode.children[p + 2], (n->count - 1 - p) * sizeof(bptreeNode *));
            --n->count;
        }
    } 
	else   //若结点n刚刚 脱贫，则 删除 结点p的元素 key[p], children[p+1]; 向它的兄弟结点借一个关键字， 或者 合并它的兄弟结点
	{
        posInParent = path->p[level + 1];
        //get left side node and right side node
        if(posInParent > 0)
            ln = path->n[level + 1]->n.bptreeInnerNode.children[posInParent - 1];  //n的左兄弟结点
        if(posInParent < (path->n[level + 1]->count))
            rn = path->n[level + 1]->n.bptreeInnerNode.children[posInParent + 1];  //n的右兄弟结点

        //need edit
        if(ln && ln->count > half)   //若左结点比较丰满
		{
            //redistribute
			//删除 结点p的元素 key[p], children[p+1]
            memmove(&n->n.bptreeInnerNode.keys[1], &n->n.bptreeInnerNode.keys[0], p * sizeof(void *));
            memmove(&n->n.bptreeInnerNode.children[1], &n->n.bptreeInnerNode.children[0], (p + 1) * sizeof(bptreeEntry *));
			//向左结点ln 借 最大的关键字
            n->n.bptreeInnerNode.children[0] = ln->n.bptreeInnerNode.children[ln->count];
            n->n.bptreeInnerNode.keys[0] = bptreeGetKey(_findMinEntry(n->n.bptreeInnerNode.children[1]));
            --ln->count;
            --path->p[level + 1];
            _updateInnerNode(path, level + 1);
        } 
		else if (rn && rn->count > half) //若右结点比较丰满
		{
            //redistibute
            memmove(&n->n.bptreeInnerNode.keys[p], &n->n.bptreeInnerNode.keys[p + 1], ((n->count - 1) - p) * sizeof(void *));
            memmove(&n->n.bptreeInnerNode.children[p + 1], &n->n.bptreeInnerNode.children[p + 2], ((n->count - 1) - p) * sizeof(bptreeNode *));
			//向右结点rn 借 最小的关键字
            n->n.bptreeInnerNode.children[n->count] = rn->n.bptreeInnerNode.children[0];
            n->n.bptreeInnerNode.keys[n->count - 1] = bptreeGetKey(_findMinEntry(n->n.bptreeInnerNode.children[n->count]));
            memmove(&rn->n.bptreeInnerNode.keys[0], &rn->n.bptreeInnerNode.keys[1], (rn->count - 1) * sizeof(void *));
            memmove(&rn->n.bptreeInnerNode.children[0], &rn->n.bptreeInnerNode.children[1], rn->count * sizeof(bptreeNode *));
            --rn->count;
            _updateInnerNode(path, level + 1);
        } 
		else if (ln)  //如果左右兄弟结点刚刚脱贫（都只有m/2-1个），则该结点与某一个兄弟结点合并（删除父结点的一个索引关键字，再合并）成一个结点。
		{
            //merge inner node
            ln->n.bptreeInnerNode.keys[ln->count] = path->n[level + 1]->n.bptreeInnerNode.keys[posInParent - 1];
            memmove(&ln->n.bptreeInnerNode.keys[ln->count + 1], &n->n.bptreeInnerNode.keys[0], p * sizeof(void *));
            memmove(&ln->n.bptreeInnerNode.keys[ln->count + 1 + p], &n->n.bptreeInnerNode.keys[p + 1], (n->count - 1 - p) * sizeof(void *));
            memmove(&ln->n.bptreeInnerNode.children[ln->count + 1], &n->n.bptreeInnerNode.children[0], (p + 1) * sizeof(bptreeNode *));
            memmove(&ln->n.bptreeInnerNode.children[ln->count + 2 + p], &n->n.bptreeInnerNode.children[p + 2], (n->count - 1 - p) * sizeof(bptreeNode *));
            ln->count += n->count;
            --path->p[level + 1];
            _freeNode(n);  //释放结点n
            _deleteFromInnerNode(t, path, level + 1);  //向上 调整
        } 
		else if (rn) 
		{
            //merge inner node
            memmove(&n->n.bptreeInnerNode.keys[p], &n->n.bptreeInnerNode.keys[p + 1], (n->count - 1 - p) * sizeof(void *));
            memmove(&n->n.bptreeInnerNode.children[p + 1], &n->n.bptreeInnerNode.children[p + 2], (n->count - 1 - p) * sizeof(bptreeNode *));
            n->n.bptreeInnerNode.keys[n->count - 1] = path->n[level + 1]->n.bptreeInnerNode.keys[posInParent];
            memmove(&n->n.bptreeInnerNode.keys[n->count], &rn->n.bptreeInnerNode.keys[0], rn->count * sizeof(void *));
            memmove(&n->n.bptreeInnerNode.children[n->count], &rn->n.bptreeInnerNode.children[0], (rn->count + 1) * sizeof(bptreeNode *));
            n->count += rn->count;
            _freeNode(rn);
            _deleteFromInnerNode(t, path, level + 1);
        }
    }

    return BPT_OK;
}

//向左右兄弟结点 借一个key 或者 与左/右 结点合并成一个结点
int _distributeOrMergeLeafNode(bptree *t, bptreePathInfo *path) 
{
	bptreeNode *n = NULL;
	bptreeNode *ln = NULL;
	bptreeNode *rn = NULL;
	unsigned int half = 0;
    pos_t p = 0;
    pos_t posInParent = 0;

    half = t->leafOrder >> 1;
    p = path->p[0];
    n = path->n[0];   //叶子结点

    posInParent = path->p[1];  //n的父结点 的索引位置
    //get left side node and right side node
    if(posInParent > 0)
        ln = path->n[1]->n.bptreeInnerNode.children[posInParent - 1];   //n的左兄弟结点
    if(posInParent < (path->n[1]->count))
        rn = path->n[1]->n.bptreeInnerNode.children[posInParent + 1];   //n的右兄弟结点

    if(ln && ln->count > half)   //若左兄弟结点比较丰满(关键字个数超过m/2)
	{
        //redistribute
		//向 ln 结点 借最大的一个关键字entries[ln->count - 1]
        memmove(&n->n.bptreeLeafNode.entries[1], &n->n.bptreeLeafNode.entries[0], p * sizeof(bptreeEntry *));  //元素后移，空出位置entry[0]
        n->n.bptreeLeafNode.entries[0] = ln->n.bptreeLeafNode.entries[ln->count - 1];
        --ln->count;
        --path->p[1];
		_updateInnerNode(path, 1);     //将 叶子结点n的 最小key 赋到 其父结点中，作为B+树中 分支结点的 关键字索引 
	} 
	else if (rn && rn->count > half)   //若右兄弟结点比较丰满(关键字个数超过m/2)
	{
        //redistibute
		//向 rn 结点 借最小的一个关键字entries[0]
        memmove(&n->n.bptreeLeafNode.entries[p], &n->n.bptreeLeafNode.entries[p + 1], ((n->count - 1) - p) * sizeof(bptreeEntry *));
        n->n.bptreeLeafNode.entries[n->count - 1] = rn->n.bptreeLeafNode.entries[0];
        memmove(&rn->n.bptreeLeafNode.entries[0], &rn->n.bptreeLeafNode.entries[1], (rn->count - 1) * sizeof(bptreeEntry *));
        --rn->count;
        _updateInnerNode(path, 1);    //将 叶子结点n的 最小key 赋到 其父结点中，作为B+树中 分支结点的 关键字索引
    } 
	else if (ln)   //如果左右兄弟结点刚刚脱贫（都只有m/2-1个），则该结点与某一个兄弟结点合并（删除父结点的一个索引关键字，再合并）成一个结点。
	{
		//merge
		//叶子结点n 与 左兄弟结点ln 合并
		//将 n结点的 所有value（除了entry[p]）拷到 ln结点
        memmove(&ln->n.bptreeLeafNode.entries[ln->count], &n->n.bptreeLeafNode.entries[0], p * sizeof(bptreeEntry *));
        memmove(&ln->n.bptreeLeafNode.entries[ln->count + p], &n->n.bptreeLeafNode.entries[p + 1], (n->count - 1 - p) * sizeof(bptreeEntry *));
        ln->count += n->count - 1;
        --path->p[1];
        ln->n.bptreeLeafNode.next = n->n.bptreeLeafNode.next;
        _freeNode(n);     //释放 结点n的空间
        _deleteFromInnerNode(t, path, 1);
    } 
	else if (rn) 
	{
        //merge
        memmove(&n->n.bptreeLeafNode.entries[p], &n->n.bptreeLeafNode.entries[p + 1], ((n->count - 1) - p) * sizeof(bptreeEntry *));
        memmove(&n->n.bptreeLeafNode.entries[n->count - 1], &rn->n.bptreeLeafNode.entries[0], rn->count * sizeof(bptreeEntry *));
        n->count += rn->count - 1;
        n->n.bptreeLeafNode.next = rn->n.bptreeLeafNode.next;
        _freeNode(rn);
        _deleteFromInnerNode(t, path, 1);
    }

    return BPT_OK;
}

int _deleteFromLeafNodeMoreThanHalf(bptreePathInfo *p) 
{
	//覆盖 叶子结点中的 entry[p->p[0]]
    memmove(&p->n[0]->n.bptreeLeafNode.entries[p->p[0]], &p->n[0]->n.bptreeLeafNode.entries[p->p[0] + 1], ((p->n[0]->count - 1) - p->p[0]) * sizeof(bptreeEntry *));
    --p->n[0]->count;

    return BPT_OK;
}

int _deleteFromLeafNode(bptree *t, bptreePathInfo *p,const void *delkey) 
{
    int result = BPT_OK;

    _removeKeyInPath(t, p, delkey);   //将path中 涉及的结点（除了叶子结点） 删除delkey
    if(p->n[0]->count > (t->leafOrder>> 1) || t->root->level == 0)  //若叶结点p->n[0]删除关键字后，关键字的个数大于等于m/2-1 或者 当前只有一个根结点
        result = _deleteFromLeafNodeMoreThanHalf(p);
    else   //若叶结点p->n[0]删除关键字后，关键字的个数小于m/2-1 => 向左右兄弟结点 借一个key 或者 与左/右 结点合并成一个结点
        result = _distributeOrMergeLeafNode(t, p);

    --t->count;

    return result;
}

void bptreeBulkLoad(bptree *t, bptreeEntry *e[], uint64_t count) {
    uint32_t leaves = 0;
    uint32_t parents = 0;
    uint32_t children = 0;
    uint32_t i = 0;
    uint32_t j = 0;
    uint32_t index = 0;
    bptreeNode *node = NULL;
    bptreeNode *leaf = NULL;
    bulkLoadObject *nextlevel = NULL;

    t->count = count;
    leaves = ceil(count/(double)(t->leafOrder- 1));

    for(i = 0; i < leaves; ++i) {
        if(node) {
            node->n.bptreeLeafNode.next = _allocateNode(t, true);
            node = node->n.bptreeLeafNode.next;
        } else {
            node = _allocateNode(t, true);
            t->min = node;
        }

        if(!node)
            assert(false);

        node->count = floor(count/(double)(leaves - i));

        for(j = 0; j < node->count; ++j)
            node->n.bptreeLeafNode.entries[j] = e[index + j];

        count -= node->count;
        index += node->count;
    }

    assert(count == 0);

    if(leaves == 1){
        t->root = t->min;
        return ;
    }

    //build inner node
    parents = ceil(leaves/(double)t->innerOrder);
    nextlevel = bptreemalloc(sizeof(bulkLoadObject) * parents);
    leaf = t->min;
    //level - 1
    for (i = 0;i < parents; ++i) {
        node = _allocateNode(t, false);
        node->level = 1;
        node->count = floor(leaves/(double)(parents - i)) - 1;
        node->n.bptreeInnerNode.children[0] = leaf;
        leaf = leaf->n.bptreeLeafNode.next;
        for(j = 0; j < node->count; ++j) {
            node->n.bptreeInnerNode.keys[j] = bptreeGetKey(leaf->n.bptreeLeafNode.entries[0]);
            node->n.bptreeInnerNode.children[j + 1] = leaf;
            leaf = leaf->n.bptreeLeafNode.next;
        }

        leaves -= node->count + 1;
        //save current key and node for next level build
        if(leaf)
            nextlevel[i].key = bptreeGetKey(leaf->n.bptreeLeafNode.entries[0]);
        nextlevel[i].node = node;
    }

    //high level
    while(parents != 1) {
        children = parents;
        parents = ceil(parents/(double)t->innerOrder);
        index = 0;

        for(i = 0; i < parents; ++i) {
            node = _allocateNode(t, false);
            node->level = nextlevel[index].node->level + 1;
            node->count = floor(children/(double)(parents - i)) - 1;

            for(j = 0; j < node->count; ++j) {
                node->n.bptreeInnerNode.keys[j] = nextlevel[index + j].key;
                node->n.bptreeInnerNode.children[j] = nextlevel[index + j].node;
            }
            index += node->count;
            node->n.bptreeInnerNode.children[node->count] = nextlevel[index].node;

            nextlevel[i].node = node;
            nextlevel[i].key = nextlevel[index].key;
            ++index;

            children -= node->count + 1;
        }

        assert(children == 0);
    }

    t->root = node;
    bptreefree(nextlevel);
}

uint64_t bptreeQuery(bptree *t, bptreeEntry** output, uint64_t len, void *from, void *end) {
    bptreeNode *node = NULL;
    pos_t pos = 0;
    uint64_t count = 0;
    bptreePathInfo path;

    if(from) {
        _bptreeFindEntry(t, from, &path);
        node = path.n[0];
        pos = path.p[0];
    } else {
        node = t->min;
        pos = 0;
    }

    while(node) {
        for( ; pos < node->count; ++pos ){
            if(end) {
                if(0 >= bptreeCompareKeys(t, end, bptreeGetKey(node->n.bptreeLeafNode.entries[pos])))
                    return count;
            }

            if(output){
                if(count < len) {
                    output[count] = node->n.bptreeLeafNode.entries[pos];
                } else
                    return count;
            }
            ++count;
        }
        node = node->n.bptreeLeafNode.next;
        pos = 0;
    }

    return count;
}

bptree *bptreeSplit(bptree *t, void *splitKey) {
    bptree *splitTree = NULL;
    bptreeEntry **entries = NULL;
    uint64_t len = 0;
    uint64_t index = 0;

    splitTree = bptreeCreate(t->type, t->privdata);
    len = t->count;
    entries = bptreemalloc(len * sizeof(bptreeEntry *));

    assert(t->count == bptreeQuery(t, entries, len, NULL, NULL));
    if(splitKey)
        index = bptreeQuery(t, NULL, 0, NULL, splitKey);
    else
        index = t->count >> 1;
    //spilt tree
    bptreeBulkLoad(splitTree, entries + index,t->count - index);
    //orgin tree
    _bptreeClearNotFreeEntry(t);
    bptreeBulkLoad(t, entries, index);

    bptreefree(entries);
    return splitTree;
}

void bptreeVerifyNode(bptree *t, bptreeNode *node) {
    uint32_t i = 0;
    bptreeNode *child = NULL;

    if(node) {
        if(node->leaf) {
            for(i = 1; i < node->count; ++i) {
                assert(bptreeCompareKeys(t, bptreeGetKey(node->n.bptreeLeafNode.entries[i]),
                                            bptreeGetKey(node->n.bptreeLeafNode.entries[i - 1])));
            }
        } else {
            for(i = 0; i < node->count; ++i) {
                child = node->n.bptreeInnerNode.children[i];
                if(child->leaf) {
                    assert(bptreeCompareKeys(t, node->n.bptreeInnerNode.keys[i],
                                                bptreeGetKey(child->n.bptreeLeafNode.entries[child->count - 1])));
                } else {
                    assert(bptreeCompareKeys(t, node->n.bptreeInnerNode.keys[i],
                                                child->n.bptreeInnerNode.keys[child->count - 1]));
                }
                bptreeVerifyNode(t, child);
            }
            child = node->n.bptreeInnerNode.children[i];
            if(child->leaf) {
                assert(0 <= bptreeCompareKeys(t, bptreeGetKey(child->n.bptreeLeafNode.entries[0]),
                                                node->n.bptreeInnerNode.keys[i - 1]));
            } else {
                assert(0 <= bptreeCompareKeys(t, child->n.bptreeInnerNode.keys[0],
                                                node->n.bptreeInnerNode.keys[i - 1]));
            }
            bptreeVerifyNode(t, child);
        }
    }
}

bool bptreeVerify(bptree *t) {
    uint64_t count = 0;
    bptreeNode* node = NULL;

    node = t->min;

    while(node) {
        count += node->count;
        node = node->n.bptreeLeafNode.next;
    }

    bptreeVerifyNode(t, t->root);

    return count == t->count;
}
