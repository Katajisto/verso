#include "verso.h"
#include <stddef.h>
#include <stdio.h>

int VERSO_ARENA_COUNT = 0;

#define ALIGN_UP(n, align) (((n) + (align) - 1) & ~((align) - 1))

Verso* VersoNew(size_t blockSize) {

  if(blockSize <= 0) {
    return NULL;
  }
  
  Verso* v = malloc(sizeof(Verso));

  if(v == NULL) return NULL;

  v->newBlockSize = blockSize;

  VersoBlock* vb = malloc(sizeof(VersoBlock));

  if(vb == NULL) {
    free(v);
    return NULL;
  }
  
  vb->buffer = aligned_alloc(sizeof(max_align_t), v->newBlockSize);
  
  if(vb->buffer == NULL) {
    free(v);
    free(vb);
    return NULL;
  }
  
  vb->bufferSize = v->newBlockSize;
  vb->currentOffset = 0;
  vb->next = NULL;
  
  v->first = vb;
  v->last = vb;
  v->totalAlloc = 0;
  
  VERSO_ARENA_COUNT++;
  
  return v;
}


void* VersoAlloc(Verso* v, size_t size) {
  if(v == NULL || size <= 0) return NULL;

  size_t alignedSize = ALIGN_UP(size, sizeof(max_align_t));
  
  if(v->last->currentOffset + alignedSize <= v->last->bufferSize) {
    // We can fit the thing in the block.
    void* ptr = (char*)v->last->buffer + v->last->currentOffset;
    v->last->currentOffset += alignedSize;
    v->totalAlloc += alignedSize;
    return ptr;
  } else { // We need a new block!
    if(v->last->next != NULL) { // We have a block left over from a reset.
      VersoBlock* vb = v->last->next;
      if(alignedSize > vb->bufferSize) {
        vb->buffer = realloc(vb->buffer, alignedSize);
        vb->bufferSize = alignedSize;
      }
      vb->currentOffset = alignedSize;
      v->last = vb;
      v->totalAlloc += alignedSize;
      return vb->buffer;
    } else { // We need to make a completely new block.
      VersoBlock* vb = malloc(sizeof(VersoBlock));
      if(vb == NULL) {
        return NULL;
      }
      if(alignedSize <= v->newBlockSize) {
        vb->buffer = aligned_alloc(sizeof(max_align_t), v->newBlockSize);
        vb->bufferSize = v->newBlockSize;
      } else {
        vb->buffer = aligned_alloc(sizeof(max_align_t), alignedSize);
        vb->bufferSize = alignedSize;
      }
  
      if(vb->buffer == NULL) {
        free(vb);
        return NULL;
      }
      vb->currentOffset = alignedSize;
      vb->next = NULL;  
    
      v->last->next = vb;
      v->last = vb;
    
      v->totalAlloc += alignedSize;
    
      return vb->buffer;
    }
  }
}

void VersoFree(Verso* v) {
  if(v == NULL) return;

  VersoBlock* vb = v->first;
  while(vb != NULL) {
    free(vb->buffer);
    VersoBlock* temp = vb;
    vb = vb->next;
    free(temp);
  }
  free(v);

  VERSO_ARENA_COUNT--;
}

void VersoReset(Verso* v) {
  if(v == NULL) return;

  VersoBlock* vb = v->first;
  while(vb != NULL) {
    vb->currentOffset = 0;
    vb = vb->next;
  }

  v->totalAlloc = 0;
}

void VersoCheck() {
  if(VERSO_ARENA_COUNT != 0) {
    printf("VERSO ARENA COUNT IS NON-ZERO! WAS %d!\n", VERSO_ARENA_COUNT);
  }
}
