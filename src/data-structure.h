#ifndef DATA_STRUCTURE_NEW_H_
#define DATA_STRUCTURE_NEW_H_

#include <stdbool.h>

struct QTree {
  int x;
  int y;
  int n;

  // left := x_child <= x
  // above := y_child <= y
  // right := not left
  // under := not above

  struct QTree *leftover;
  struct QTree *leftunder;
  struct QTree *rightover;
  struct QTree *rightunder;

  struct QTree *parent;
};

struct QTree *traverseTowards(struct QTree *tree, int x, int y);

struct QTree *findCell(struct QTree *tree, int x, int y);

struct QTree *newTree(int x, int y, int n, struct QTree *parent);

struct QTree **getParentsPtrToSelf(struct QTree *tree);

void freeTree(struct QTree *tree);

bool removeCellFromTree(struct QTree *tree, int x, int y);

struct QTree *cleanBranch(struct QTree *tree);

struct QTree *insertCell(struct QTree *tree, int x, int y);

bool treeFits(struct QTree *tree, int x, int y);

void extendTreeToFit(struct QTree *tree, int x, int y);

bool isAlive(struct QTree *tree, int x, int y);

int neighbourCount(struct QTree *tree, int x, int y);

void nextGeneration(struct QTree *tree);

void traverseWithCallbackOnCell(struct QTree *tree, void (*callback)(int x, int y));

#endif // DATA_STRUCTURE_NEW_H_
