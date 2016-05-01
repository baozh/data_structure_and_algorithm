#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include "bplustree.h"

struct bplus_tree_config {
        int level;
        int order;
        int entries;
};

static unsigned int valid_ins;
static unsigned int valid_query;
static unsigned int valid_del;

static void show_running_info(void)
{
        fprintf(stderr,"$$ VALID INSERT:%u\n",valid_ins);
        fprintf(stderr,"$$ VALID QUERY:%u\n",valid_query);
        fprintf(stderr,"$$ VALID DELETE:%u\n",valid_del);
        fprintf(stderr,"###DONE###\n");
}

static unsigned char huge_array[INT_MAX>>3] = { 0 };

#define TEST_KEY 0
#define has(a, k)       ((a[(k)>>3]) & (1<<((k)&7)))
#define set(a, k)       ((a[(k)>>3]) |= (1<<((k)&7)))
#define unset(a, k)     ((a[(k)>>3]) &= ~(1<<((k)&7)))
//#define DEBUG
#ifdef DEBUG
#define log(fmt, ...)  printf(fmt, ##__VA_ARGS__)
#else
#define log(...)
#endif

void exec_file(char *file, struct bplus_tree *tree)
{
        int got = 0, exist = 0;
        int k1, k2;
        int ret;
        char op;
        FILE *fp = fopen(file,"r");
        if (!fp) {
                fprintf(stderr, "Where is the testing data?\n");
                exit(-1);
        }

        for (; ;) {
                ret = fscanf(fp," %c %d", &op, &k1);
                if (ret == EOF) {
                        fclose(fp);
                        return;
                }

#ifdef DEBUG
                if (got) {
                        if (-1 == bplus_tree_get(tree, TEST_KEY)) {
                                //bplus_tree_dump(tree);
                                return;
                        }
                }
#endif

                switch(op)
                {
                case 'i':
                        log("i %d ", k1);
                        if (has(huge_array, k1)) {
                                exist = 1;
                        } else {
                                exist = 0;
                                set(huge_array, k1);
                        }
                        if (0 == bplus_tree_put(tree, k1, k1)) {
                                valid_ins++;
                                if (k1 == TEST_KEY) {
                                        got = 1;
                                }
                                if (exist) {
                                        fprintf(stderr, "insert key %d error, has been inserted before!\n", k1);
                                        exit(-1);
                                }
                        } else {
                                if (!exist) {
                                        fprintf(stderr, "insert key %d error, could be inserted!\n", k1);
                                        exit(-1);
                                }
                        }
                        break;
                case 's':
                        log("s %d ", k1);
                        if (has(huge_array, k1)) {
                                exist = 1;
                        } else {
                                exist = 0;
                        }
                        ret = bplus_tree_get(tree, k1);
                        if (ret != -1) {
                                valid_query++;
                                if (!exist) {
                                        fprintf(stderr, "search key %d error, found but not exists!\n", k1);
                                        exit(-1);
                                }
                                assert(ret == k1);
                        }
                        else {
                                if (exist) {
                                        fprintf(stderr, "search key %d error, exists but not found!\n", k1);
                                        exit(-1);
                                }
                        }
                        break;
                case 'd':
                        log("d %d ", k1);
                        if (has(huge_array, k1)) {
                                exist = 1;
                                unset(huge_array, k1);
                        } else {
                                exist = 0;
                        }
                        if (0 == bplus_tree_put(tree, k1, 0))
                        {
                                valid_del++;
                                if (k1 == TEST_KEY && got) {
                                        got = 0;
                                }
                                if (!exist) {
                                        fprintf(stderr, "delete key %d error, deleted but not found!\n", k1);
                                        exit(-1);
                                }
                        } else {
                                if (exist) {
                                        fprintf(stderr, "delete key %d error, found but not deleted!\n", k1);
                                        exit(-1);
                                }
                        }
                        break;
                case 'r':
                        log("r %d", k2);
                        ret = fscanf(fp,"%d",&k2);
                        //bplus_tree_get_range(tree,k1,k2);
                        //valid_query++;
                        break;
                case 'q':
                        fclose(fp);
                        return;
                }
                log("\n");
        }
}

int main(void)
{
        struct bplus_tree_config config;
        config.level=10;
        config.order=64;
        config.entries=64;

        struct bplus_tree *tree = bplus_tree_init(config.level, config.order, config.entries);

        exec_file("testcase", tree);

        show_running_info();
        bplus_tree_deinit(tree);

        return 0;
}
