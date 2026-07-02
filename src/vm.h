#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"
#include "table.h"
#include "object.h"

#define FRAMES_MAX 64
#define STACK_MAX (FRAMES_MAX * UINT8_COUNT)

typedef struct {
  ObjClosure *closure;
  uint8_t *ip;
  Value *slots;
} CallFrame;

typedef struct {
  // function calls
  CallFrame frames[FRAMES_MAX];
  int frameCount;

  // stack
  Value stack[STACK_MAX];
  Value *stackTop;

  // objects
  Table globals;
  Table strings;
  ObjString *initString;
  ObjUpvalue *openUpvalues;
  Obj *objects;

  // GC parameters to control throughput and latency
  size_t bytesAllocated;
  size_t nextGC;
  
  // gray stack (for GC)
  int grayCount;
  int grayCapacity;
  Obj **grayStack;
} VM;

typedef enum {
  INTERPRET_OK,
  INTERPRET_COMPILE_ERROR,
  INTERPRET_RUNTIME_ERROR
} InterpretResult;

extern VM vm;

void initVM();
void freeVM();
InterpretResult interpret(const char *source);

void push(Value value);
Value pop();

#endif
