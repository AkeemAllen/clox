#include "debug.h"
#include "value.h"
#include <stdio.h>

int getLineNumber(int index, Chunk *chunk);

void disassembleChunk(Chunk *chunk, const char *name) {
  printf("== %s ==\n", name);

  for (int offset = 0; offset < chunk->count;) {
    offset = disassembleInstruction(chunk, offset);
  }
}

static int constantInstruction(const char *name, Chunk *chunk, int offset) {
  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant);
  printValue(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}

static int simpleInstruction(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

int disassembleInstruction(Chunk *chunk, int offset) {
  printf("%04d \n", offset);

  if (offset > 0 &&
      getLineNumber(offset, chunk) == getLineNumber(offset - 1, chunk)) {
    printf("   | ");
  } else {
    printf("%4d ", getLineNumber(offset, chunk));
  }

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
  case OP_CONSTANT:
    return constantInstruction("OP_CONSTANT", chunk, offset);
  case OP_ADD:
    return simpleInstruction("OP_ADD", offset);
  case OP_SUBTRACT:
    return simpleInstruction("OP_SUBTRACT", offset);
  case OP_MULTIPLY:
    return simpleInstruction("OP_MULTIPLY", offset);
  case OP_DIVIDE:
    return simpleInstruction("OP_DIVIDE", offset);
  case OP_NEGATE:
    return simpleInstruction("OP_NEGATE", offset);
  case OP_RETURN:
    return simpleInstruction("OP_RETURN", offset);
  default:
    printf("Unknown opcode %d\n", instruction);
    return offset + 1;
  }
}

int sort_lines(void *a, void *b) {
  return ((LineInfo *)a)->lineNumber - ((LineInfo *)b)->lineNumber;
}

int getLineNumber(int index, Chunk *chunk) {
  HASH_SORT(chunk->lines, sort_lines);

  int totalOccurrences = 0;

  LineInfo *lineinfo;
  for (lineinfo = chunk->lines; lineinfo != NULL;
       lineinfo = lineinfo->hh.next) {
    totalOccurrences += lineinfo->occurrences;
    if (totalOccurrences > index) {
      return lineinfo->lineNumber;
    }
  }
  return 0;
}
