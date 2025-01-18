/*
  Super simple and stupid C arena implementation.
*/

#ifndef VERSO
#define VERSO

#include <stdlib.h>
#include <stddef.h>

#define VERSO_DEFAULT_BLOCK 4096

typedef struct VersoBlock {
  size_t bufferSize;
  size_t currentOffset;
  struct VersoBlock* next;
  void* buffer;
} VersoBlock;

typedef struct {
  VersoBlock* first;
  VersoBlock* last;
  size_t newBlockSize;
  size_t totalAlloc;
} Verso;

// New arena! Block size will be the block size new blocks will be allocated.
Verso* VersoNew(size_t blockSize);

// Allocate size bytes of memory and return the pointer.
void* VersoAlloc(Verso* v, size_t size);

void VersoFree(Verso* v);

void VersoCheck();

void VersoReset(Verso* v);

#endif
