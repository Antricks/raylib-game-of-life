// PLAN: Game of life cells saved as entries in a quad-tree sorted by x/y and
// bigger/smaller, data structure also keeps track of min/max x & y so it
// doesn't have to be calculated separately

#include <data-structure.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Cell *newCell(int x, int y) {
    struct Cell *cell = malloc(sizeof(struct Cell));
    *cell = (struct Cell){.x = x, .y = y};

    return cell;
}

struct QTree **traverseTowards(struct QTree **treeP, int x, int y) {
    struct QTree *tree = *treeP;

    if (tree == NULL) {
        return treeP;
    }

    if (x > tree->cell->x) {
        if (y > tree->cell->y) {
            return &(tree->rightunder);
        } else {
            return &(tree->rightabove);
        }
    } else {
        if (y > tree->cell->y) {
            return &(tree->leftunder);
        } else {
            return &(tree->leftabove);
        }
    }
}

void insertToQTree(struct QTree **tree, struct Cell *cell) {
    struct QTree **nextSubTree = traverseTowards(tree, cell->x, cell->y);

    if (*nextSubTree == NULL) {
        *nextSubTree = malloc(sizeof(struct QTree));
        **nextSubTree = (struct QTree){.cell = cell,
                                       .parent = *tree,
                                       .leftabove = NULL,
                                       .leftunder = NULL,
                                       .rightabove = NULL,
                                       .rightunder = NULL};
    } else {
        insertToQTree(nextSubTree, cell);
    }
}

// ENHANCEMENT: Could theoretically determine insert depth here to speed up balancing later
void insertToField(struct Field *field, struct Cell *cell) {
    if (cell->x > field->xMax) {
        field->xMax = cell->x;
    } else if (cell->x < field->xMin) {
        field->xMin = cell->x;
    }

    if (cell->y > field->yMax) {
        field->yMax = cell->y;
    } else if (cell->y < field->yMin) {
        field->yMin = cell->y;
    }

    insertToQTree(&field->cells, cell);
}

// TODO this is much more inefficient than it would have to be.
// I'll have to further enhance the data structure for that though.
void naiveInsertSubTree(struct QTree **destination, struct QTree *subtree) {
    if (subtree == NULL) {
        return;
    }

    insertToQTree(destination, subtree->cell);

    naiveInsertSubTree(destination, subtree->leftunder);
    naiveInsertSubTree(destination, subtree->leftabove);
    naiveInsertSubTree(destination, subtree->rightunder);
    naiveInsertSubTree(destination, subtree->rightabove);

    free(subtree);
}

// TODO implement enhancement referred to at naiveInsertSubTree.
void insertSubTree(struct QTree **destination, struct QTree *subtree) {
    naiveInsertSubTree(destination, subtree);
}

bool removeFromTree(struct QTree **tree, int x, int y) {
    if (*tree == NULL)
        return false;

    struct QTree **nextSubTree = traverseTowards(tree, x, y);

    if (nextSubTree == NULL || *nextSubTree == NULL || (*nextSubTree)->cell == NULL) {
        return false;
    }

    if ((*nextSubTree)->cell->x == x || (*nextSubTree)->cell->y == y) {
        struct QTree* leftunder = (*nextSubTree)->leftunder;
        struct QTree* leftabove = (*nextSubTree)->leftabove;
        struct QTree* rightunder = (*nextSubTree)->rightunder;
        struct QTree* rightabove = (*nextSubTree)->rightabove;
        
        free((*nextSubTree)->cell);
        free(*nextSubTree);

        *nextSubTree = NULL;

        insertSubTree(tree, leftunder);
        insertSubTree(tree, leftabove);
        insertSubTree(tree, rightunder);
        insertSubTree(tree, rightabove);
        
        return true;
    } else {
        return removeFromTree(nextSubTree, x, y);
    }
}

bool removeFromField(struct Field *field, int x, int y) {
    return removeFromTree(&field->cells, x, y);
}

void freeTree(struct QTree *tree) {
    if (tree == NULL) {
        return;
    }

    if (tree->cell != NULL) {
        free(tree->cell);
    }

    freeTree(tree->leftunder);
    freeTree(tree->leftabove);
    freeTree(tree->rightunder);
    freeTree(tree->rightabove);

    free(tree);
}

void freeField(struct Field *field) {
    freeTree(field->cells);
}

bool isAliveQTree(struct QTree *tree, int x, int y) {
    if (tree == NULL) {
        return false;
    }

    if (tree->cell->x == x && tree->cell->y == y) {
        return true;
    } else {
        struct QTree **nextSubTree = traverseTowards(&tree, x, y);
        return isAliveQTree(*nextSubTree, x, y);
    }
}

bool isAlive(struct Field *field, int x, int y) {
    return isAliveQTree(field->cells, x, y);
}

// ENHANCEMENT: This is way more inefficient than it would have to be. Works for now though.
int naiveNeighbourCount(struct Field *field, int x, int y) {
    int neighbours = 0;

    for (int a = -1; a <= 1; a++) {
        for (int b = -1; b <= 1; b++) {
            if (isAlive(field, x + a, y + b))
                neighbours++;
        }
    }

    return neighbours;
}

void iterate(struct Field *field) {}

void traverseWithCallback(struct QTree *tree, void (*callback)(struct Cell *)) {
    if (tree == NULL)
        return;
    callback(tree->cell);
    traverseWithCallback(tree->leftunder, callback);
    traverseWithCallback(tree->leftabove, callback);
    traverseWithCallback(tree->rightunder, callback);
    traverseWithCallback(tree->rightabove, callback);
}

void traverseFieldWithCallback(struct Field *field, void (*callback)(struct Cell *)) {
    traverseWithCallback(field->cells, callback);
}
