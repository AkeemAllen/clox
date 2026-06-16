#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  initValueArray(&chunk->constants);
}

void freeChunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

LineInfo *findLine(int lineNumber, Chunk *chunk) {
  LineInfo *lineinfo;
  HASH_FIND_INT(chunk->lines, &lineNumber, lineinfo);
  return lineinfo;
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code =
        GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->count++;
  LineInfo *lineinfo = findLine(line, chunk);
  if (lineinfo == NULL) {
    lineinfo = malloc(sizeof(LineInfo));
    lineinfo->lineNumber = line;
    lineinfo->occurrences = 1;
    HASH_ADD_INT(chunk->lines, lineNumber, lineinfo);
  } else {
    lineinfo->occurrences++;
  }
}

int addConstant(Chunk *chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  // Consider if this returns index greater than one byte, i.e., greater than
  // 256
  return chunk->constants.count - 1;
}

void writeConstant(Chunk *chunk, Value value, int line) {
  int index = addConstant(chunk, value);

  if (index > UINT8_MAX) {
    writeChunk(chunk, OP_CONSTANT_LONG, line);
    writeChunk(chunk, (uint8_t)(index >> 16), line);
    writeChunk(chunk, (uint8_t)(index >> 8), line);
    writeChunk(chunk, (uint8_t)index, line);
  } else {
    writeChunk(chunk, OP_CONSTANT, line);
    writeChunk(chunk, (uint8_t)index, line);
  }
}
