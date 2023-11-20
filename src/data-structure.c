// PLAN: Game of life cells saved as entries in a quad-tree sorted by x/y and bigger/smaller,
// data structure also keeps track of min/max x & y so it doesn't have to be calculated separately

#include <stdlib.h>
#include <stdbool.h>

struct QTree;

struct Cell {
    int x;
    int y;
    // Could also add stuff like age here
};

struct Field {
    int xMin;
    int xMax;
    int yMin;
    int yMax;
    struct QTree *cells;
};

struct QTree {
    struct QTree *parent;
    struct QTree *leftabove;
    struct QTree *leftunder;
    struct Cell *cell;
    struct QTree *rightabove;
    struct QTree *rightunder;
};

struct QTree* traverseTowards(struct QTree* tree, int x, int y) {
    if(x > tree->cell->x) {
        if(y > tree->cell->y) {
            return tree->rightunder;
        } else {
            return tree->rightabove;
        }
    } else {
        if(y > tree->cell->y) {
            return tree->leftunder;
        } else {
            return tree->leftabove;
        }
    }
}

void insertToQTree(struct QTree *tree, struct Cell *cell) {
    struct QTree *nextSubTree = traverseTowards(tree, cell->x, cell->y);

    if(nextSubTree == NULL) {
        nextSubTree = (struct QTree *) malloc((unsigned long) sizeof(struct QTree));
        *nextSubTree = (struct QTree){.cell=cell, .parent=tree, .leftabove=NULL, .leftunder=NULL, .rightabove=NULL, .rightunder=NULL};
    } else {
        insertToQTree(nextSubTree, cell);
    }
}

// ENHANCEMENT: Could theoretically determine insert depth here to speed up balancing later
void insertToField(struct Field *field, struct Cell *cell) {
    if(cell->x > field->xMax) {
        field->xMax = cell->x;
    } else if(cell->x < field->xMin) {
        field->xMin = cell->x;
    }
    
    if(cell->y > field->yMax) {
        field->yMax = cell->y;
    } else if(cell->y < field->yMin) {
        field->yMin = cell->y;
    }

    insertToQTree(field->cells, cell);
}

bool isAliveQTree(struct QTree* tree, int x, int y) {
    if(tree == NULL) {
        return false;
    }

    if(tree->cell->x == x && tree->cell->y == y) {
        return true;
    } else {
        struct QTree* nextSubTree = traverseTowards(tree, x, y); 
        return isAliveQTree(nextSubTree, x, y);
    }
}

bool isAlive(struct Field* field, int x, int y) {
    return isAliveQTree(field->cells, x, y);
}
