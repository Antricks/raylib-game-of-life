#include "data-structure.h"

#include <stdbool.h>
#include <stdlib.h>

// New data structure concept: Quad tree that spans over a square of 2n×2n
// If inserted cell is outside that square, new roots with n':=2n are created,
// until current root spans over wanted area. Removal is way simpler now, as a
// leaf can be easily removed and also its containing node if that node holds no
// other sub-trees/cells. QTrees of n = 0 could are considered cells. n values
// are intended to be powers of 2 but it should be okay to use other n's too

// Traverses as many steps up as necessary and one step down towards some goal
struct QTree *traverseTowards(struct QTree *tree, int x, int y) {
  if (tree == NULL) {
    return NULL;
  }

  if (abs(x - tree->x) > tree->n || abs(y - tree->y) > tree->n) {
    if (tree->parent != NULL) {
      return traverseTowards(tree->parent, x, y);
    } else {
      return NULL;
    }
  }

  if (x <= tree->x) {
    if (y <= tree->y) {
      return tree->leftover;
    } else {
      return tree->leftunder;
    }
  } else {
    if (y <= tree->y) {
      return tree->rightover;
    } else {
      return tree->rightunder;
    }
  }

  return NULL;
}

// Traverse until cell is found and return it or null otherwise
struct QTree *findCell(struct QTree *tree, int x, int y) {
  if (tree == NULL) {
    return NULL;
  }

  if (tree->x == x && tree->y == y && tree->n == 0) {
    return tree;
  }

  return findCell(traverseTowards(tree, x, y), x, y);
}

// Allocates memory and constructs a new tree
struct QTree *newTree(int x, int y, int n, struct QTree *parent) {
  struct QTree *tree = malloc(sizeof(struct QTree));

  *tree = (struct QTree){.x = x,
                         .y = y,
                         .n = n,
                         .leftunder = NULL,
                         .leftover = NULL,
                         .rightunder = NULL,
                         .rightover = NULL,
                         .parent = parent};
  return tree;
}

struct QTree **getParentsPtrToSelf(struct QTree *tree) {
  if (tree == NULL || tree->parent == NULL) {
    return NULL;
  }

  struct QTree **self;

  if (tree->x <= tree->parent->x) {
    if (tree->y <= tree->parent->y) {
      self = &(tree->parent->leftover);
    } else {
      self = &(tree->parent->leftunder);
    }
  } else {
    if (tree->y <= tree->parent->y) {
      self = &(tree->parent->rightover);
    } else {
      self = &(tree->parent->rightunder);
    }
  }

  return self;
}

// Recursively frees a tree and its sub-trees, also setting parent's pointer to
// NULL if possible (based on coordinates)
void freeTree(struct QTree *tree) {
  if (tree == NULL) {
    return;
  }

  if (tree->leftover != NULL) {
    freeTree(tree->leftover);
  }

  if (tree->leftunder != NULL) {
    freeTree(tree->leftunder);
  }

  if (tree->rightover != NULL) {
    freeTree(tree->rightover);
  }

  if (tree->rightunder != NULL) {
    freeTree(tree->rightunder);
  }

  struct QTree **self = getParentsPtrToSelf(tree);

  if (self != NULL) {
    *self = NULL;
  }

  free(tree);

  return;
}

// Traverse towards x/y and remove cell at x/y if found, return true if a cell
// has been removed. Also removes as much from the containing branch as possible
// if parent nodes had no other child.
bool removeCellFromTree(struct QTree *tree, int x, int y) {
  if (tree == NULL) {
    return false;
  }

  while (tree != NULL) {
    if (tree->x == x && tree->y == y && tree->n == 0) {
      struct QTree *parent = tree->parent;
      freeTree(tree);
      cleanBranch(parent);
      return true;
    }

    tree = traverseTowards(tree, x, y);
  }

  return false;
};

// Removes as many unneeded nodes upwards as possible
struct QTree *cleanBranch(struct QTree *tree) {
  if (tree == NULL) {
    return NULL;
  }

  if (!tree->leftunder && !tree->leftover && !tree->rightunder &&
      !tree->rightover) {
    struct QTree *parent = tree->parent;
    freeTree(tree);
    return cleanBranch(parent);
  }

  // TODO (not vital but would be quite good to have) implement cleaning of
  // unnecessary node with just one cell as child

  return tree;
}

// Inserts a new cell at coordinates x/y and returns a pointer to that new cell
struct QTree *insertCell(struct QTree *tree, int x, int y) {
  if (tree == NULL) {
    return NULL;
  }

  extendTreeToFit(tree, x, y);

  struct QTree *next = NULL;

  // loop is exclusively broken from inside
  while (true) {
    next = traverseTowards(tree, x, y);

    if (next == NULL) {
      // We reached a node that has room to store the new cell
      struct QTree *newCell = newTree(x, y, 0, tree);
      if (x <= tree->x) {
        if (y <= tree->y) {
          tree->leftover = newCell;
        } else {
          tree->leftunder = newCell;
        }
      } else {
        if (y <= tree->y) {
          tree->rightover = newCell;
        } else {
          tree->rightunder = newCell;
        }
      }
      return newCell;
    } else if (next->n == 0) {
      if (next->x == x && next->y == y) {
        // Cell at x/y already exists.
        return next;
      }

      struct QTree *containingNode;

      if (x <= tree->x) {
        if (y <= tree->y) {
          containingNode = newTree(tree->x - (tree->n / 2),
                                   tree->y - (tree->n / 2), tree->n / 2, tree);
          tree->leftover = containingNode;
        } else {
          containingNode = newTree(tree->x - (tree->n / 2),
                                   tree->y + (tree->n / 2), tree->n / 2, tree);
          tree->leftunder = containingNode;
        }
      } else {
        if (y <= tree->y) {
          containingNode = newTree(tree->x + (tree->n / 2),
                                   tree->y - (tree->n / 2), tree->n / 2, tree);
          tree->rightover = containingNode;
        } else {
          containingNode = newTree(tree->x + (tree->n / 2),
                                   tree->y + (tree->n / 2), tree->n / 2, tree);
          tree->rightunder = containingNode;
        }
      }

      if (next->x <= containingNode->x) {
        if (next->y <= containingNode->y) {
          containingNode->leftover = next;
        } else {
          containingNode->leftunder = next;
        }
      } else {
        if (next->y <= containingNode->y) {
          containingNode->rightover = next;
        } else {
          containingNode->rightunder = next;
        }
      }

      next->parent = containingNode;

      return insertCell(containingNode, x, y);
    }

    tree = next;
  }
}

bool treeFits(struct QTree *tree, int x, int y) {
  return tree != NULL &&
         (abs(x - tree->x) <= tree->n && abs(y - tree->y) <= tree->n);
}

void extendTreeToFit(struct QTree *tree, int x, int y) {
  // TODO maybe I could use some assert infrastructure to e.g. make sure n is
  // not 0 here. Don't really want to just return from that but also don't want
  // an unexplained endless loop
  while (!treeFits(tree, x, y)) {
    struct QTree *lift = malloc(sizeof(struct QTree));
    *lift = *tree; // hope this does what I want (copying the contents tree
                   // points to to the memory lift points to) - otherwise I'll
                   // have to use memcpy or something like that

    if (lift->leftover != NULL) {
      lift->leftover->parent = lift;
    }

    if (lift->leftunder != NULL) {
      lift->leftunder->parent = lift;
    }

    if (lift->rightover != NULL) {
      lift->rightover->parent = lift;
    }

    if (lift->rightunder != NULL) {
      lift->rightunder->parent = lift;
    }

    tree->leftover = NULL;
    tree->leftunder = NULL;
    tree->rightover = NULL;
    tree->rightunder = NULL;

    if (x > tree->x) {
      if (y > tree->y) {
        tree->leftover = lift;
        tree->x += tree->n;
        tree->y += tree->n;
      } else {
        tree->leftunder = lift;
        tree->x += tree->n;
        tree->y -= tree->n;
      }
    } else {
      if (y > tree->y) {
        tree->rightover = lift;
        tree->x -= tree->n;
        tree->y += tree->n;
      } else {
        tree->rightunder = lift;
        tree->x -= tree->n;
        tree->y -= tree->n;
      }
    }

    tree->n = 2 * tree->n;
  }
}

bool isAlive(struct QTree *tree, int x, int y) {
  return findCell(tree, x, y) != NULL;
}

int neighbourCount(struct QTree *tree, int x, int y) {
  // cur is used for (hopefully) more efficient traversal
  // instead of having to go from root for every cell
  struct QTree *cur = tree;
  struct QTree *search;
  int count = 0;

  for (int i = -1; i <= 1; i++) {
    for (int j = -1; j <= 1; j++) {
      if (i == 0 && j == 0)
        continue;

      search = findCell(cur, x + i, y + j);

      if (search != NULL) {
        cur = search;
        count++;
      }
    }
  }

  return count;
}

void nextGeneration(struct QTree *tree) {
  return; // TODO implement
}

void traverseWithCallbackOnCell(struct QTree *tree,
                                void (*callback)(int x, int y)) {
  if (tree == NULL) {
    return;
  }

  if (tree->n == 0) {
    callback(tree->x, tree->y);
  }

  traverseWithCallbackOnCell(tree->leftover, callback);
  traverseWithCallbackOnCell(tree->leftunder, callback);
  traverseWithCallbackOnCell(tree->rightover, callback);
  traverseWithCallbackOnCell(tree->rightunder, callback);

  return;
}
