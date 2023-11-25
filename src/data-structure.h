// PLAN: Game of life cells saved as entries in a quad-tree sorted by x/y and
// bigger/smaller, data structure also keeps track of min/max x & y so it
// doesn't have to be calculated separately

#include <stdbool.h>
#include <stdlib.h>

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
    struct Cell *cell;
    struct QTree *parent;
    struct QTree *leftabove;
    struct QTree *leftunder;
    struct QTree *rightabove;
    struct QTree *rightunder;
};

struct QTree **traverseTowards(struct QTree **tree, int x, int y);

struct Cell *newCell(int x, int y);

void freeField(struct Field *field);

void freeTree(struct QTree *tree);

bool removeFromTree(struct QTree **tree, int x, int y);

bool removeFromField(struct Field *field, int x, int y);

void insertToQTree(struct QTree **tree, struct Cell *cell);

void insertToField(struct Field *field, struct Cell *cell);

bool isAliveQTree(struct QTree *tree, int x, int y);

bool isAlive(struct Field *field, int x, int y);

int naiveNeighbourCount(struct Field *field, int x, int y);

struct QTree *balance(struct QTree *tree);

void traverseWithCallback(struct QTree *tree, void (*callback)(struct Cell *));

void traverseFieldWithCallback(struct Field *field, void (*callback)(struct Cell *));
