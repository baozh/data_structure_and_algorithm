#include <stdio.h>
#include <stdlib.h>

#include "bplustree.h"

struct bplus_tree_config {
        int level;
        int order;
        int entries;
}; 

static void
stdin_flush(void)
{
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
                continue;
        }
}

static struct bplus_tree_config *
bplus_tree_setting(struct bplus_tree_config *config)
{
        int i, ret, again;

        fprintf(stderr, "\n-- B+tree setting...\n");

        do {
                fprintf(stderr, "Set b+tree level (<= 10 e.g. 5): ");
                if ((i = getchar()) == '\n') {
                        config->level = 5;
                        again = 0;
                } else {
                        ungetc(i, stdin);
                        ret = fscanf(stdin, "%d", &config->level);
                        if (!ret || getchar() != '\n') {
                                stdin_flush();
                                again = 1;
                        } else if (config->level > MAX_LEVEL) {
                                again = 1;
                        } else {
                                again = 0;
                        }
                }
        } while (again);

        do {
                fprintf(stderr, "Set b+tree non-leaf order (2 < order <= 64 e.g. 7): ");
                if ((i = getchar()) == '\n') {
                        config->order = 7;
                        again = 0;
                } else {
                        ungetc(i, stdin);
                        ret = fscanf(stdin, "%d", &config->order);
                        if (!ret || getchar() != '\n') {
                                stdin_flush();
                                again = 1;
                        } else if (config->order < MIN_ORDER || config->order > MAX_ORDER) {
                                again = 1;
                        } else {
                                again = 0;
                        }
                }
        } while (again);

        do {
                fprintf(stderr, "Set b+tree leaf entries (<= 64 e.g. 10): ");
                if ((i = getchar()) == '\n') {
                        config->entries = 10;
                        again = 0;
                } else {
                        ungetc(i, stdin);
                        ret = fscanf(stdin, "%d", &config->entries);
                        if (!ret || getchar() != '\n') {
                                stdin_flush();
                                again = 1;
                        } else if (config->entries > MAX_ENTRIES) {
                                again = 1;
                        } else {
                                again = 0;
                        }
                }
        } while (again);

        return config;
}

static void
_proc(struct bplus_tree *tree, char op, int n)
{
        switch (op) {
                case 'i':
                        bplus_tree_put(tree, n, n);
                        break;
                case 'r':
                        bplus_tree_put(tree, n, 0);
                        break;
                case 'f':
                        fprintf(stderr, "key:%d data:%d\n", n, bplus_tree_get(tree, n));
                        break;
                default:
                        break;
        }       
}

static int
number_process(struct bplus_tree *tree, char op)
{
        int c, n = 0;
        int start = 0, end = 0;

        while ((c = getchar()) != EOF) {
                if (c == ' ' || c == '\t' || c == '\n') {
                        if (start != 0) {
                                if (n >= start) {
                                        end = n;
                                } else {
                                        n = 0;
                                }
                        }

                        if (start != 0 && end != 0) {
                                for (n = start; n <= end; n++) {
                                        _proc(tree, op, n);
                                }
                        } else {
                                if (n != 0) {
                                        _proc(tree, op, n);
                                }
                        }

                        n = 0;
                        start = 0;
                        end = 0;

                        if (c == '\n') {
                                return 0;
                        } else {
                                continue;
                        }
                }

                if (c >= '0' && c <= '9') {
                        n = n * 10 + c - '0';
                } else if (c == '-' && n != 0) {
                        start = n;
                        n = 0;
                } else {
                        n = 0;
                        start = 0;
                        end = 0;
                        while ((c = getchar()) != ' ' && c != '\t' && c != '\n') {
                                continue;
                        }
                        ungetc(c, stdin);
                }
        }

        fprintf(stderr, "\n");
        return -1;
}

static void
command_tips(void)
{
        fprintf(stderr, "i, insert: Insert key number. E.g. i 1 4-7 9\n");
        fprintf(stderr, "r, remove: Remove key number. E.g. r 1-100\n");
        fprintf(stderr, "f, find: Find the key number. E.g. f 41-60\n");
        fprintf(stderr, "d, dump: Dump the tree content.\n");
        fprintf(stderr, "q, quit.\n");
}

static void
command_process(struct bplus_tree *tree)
{
        int c;

        for (; ;) {
                fprintf(stderr, "\nPlease input operational command ('h' or \"help\" for help): ");

                switch (c = getchar()) {
                case EOF:
                        fprintf(stderr, "\n");
                case 'q':
                        return;
                case 'h':
                        if ((c = getchar()) == '\n' ||
                                (c == 'e' && getchar() == 'l' && getchar() == 'p' &&
                                ((c = getchar()) == ' ' || c == '\t' || c == '\n'))) {
                                command_tips();
                        }
                        break;
                case 'i':
                        if (((c = getchar()) == ' ' || c == '\t') ||
                                ((c = getchar()) == 'n' && getchar() == 's' && getchar() == 'e' &&
                                getchar() == 'r' && getchar() == 't' &&
                                ((c = getchar()) == ' ' || c == '\t'))) {
                                if (number_process(tree, 'i') < 0) {
                                        return;
                                }
                        }
                        break;
                case 'r':
                        if (((c = getchar()) == ' ' || c == '\t') ||
                                (c == 'e' && getchar() == 'm' && getchar() == 'o' &&
                                getchar() == 'v' && getchar() == 'e' &&
                                ((c = getchar()) == ' ' || c == '\t'))) {
                                if (number_process(tree, 'r') < 0) {
                                        return;
                                }
                        }
                        break;
                case 'f':
                        if (((c = getchar()) == ' ' || c == '\t') ||
                                (c == 'i' && getchar() == 'n' && getchar() == 'd' &&
                                ((c = getchar()) == ' ' || c == '\t'))) {
                                if (number_process(tree, 'f') < 0) {
                                        return;
                                }
                        }
                        break;
                case 'd':
                        if ((c = getchar()) == '\n' ||
                                (c == 'u' && getchar() == 'm' && getchar() == 'p' &&
                                ((c = getchar()) == ' ' || c == '\t' || c == '\n'))) {
                                bplus_tree_dump(tree);
                        }
                        break;
                default:
                        break;
                }
        }
}

int
main(void)
{
        struct bplus_tree *tree;
        struct bplus_tree_config config;

        /* B+tree default setting */
        bplus_tree_setting(&config);

        /* Init b+tree */
        tree = bplus_tree_init(config.level, config.order, config.entries);
        if (tree == NULL) {
                fprintf(stderr, "Init failure!\n");
                exit(-1);
        }

        /* Operation process */
        command_process(tree);

        /* Deinit b+tree */
        bplus_tree_deinit(tree);

        return 0;
}
