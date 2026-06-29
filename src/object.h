#ifndef clox_object_h
#define clox_object_h

#include "common.h"
#include "value.h"
#include "chunk.h"

#define OBJ_TYPE(value) (AS_OBJ(value)->type)

#define IS_STRING(value)   (isObjType(value, OBJ_STRING))
#define IS_CLOSURE(value)  (isObjType(value, OBJ_CLOSURE))
#define IS_FUNCTION(value) (isObjType(value, OBJ_FUNCTION))
#define IS_NATIVE(value)   (isObjType(value, OBJ_NATIVE))

#define AS_STRING(value)   ((ObjString*)AS_OBJ(value))
#define AS_CSTRING(value)  (((ObjString*)AS_OBJ(value))->chars)
#define AS_FUNCTION(value) ((ObjFunction*)AS_OBJ(value))
#define AS_CLOSURE(value) ((ObjClosure*)AS_OBJ(value))
#define AS_NATIVE(value)   (((ObjNative*)AS_OBJ(value))->function)

typedef enum {
  OBJ_CLOSURE,
  OBJ_FUNCTION,
  OBJ_NATIVE,
  OBJ_STRING,
  OBJ_UPVALUE
} ObjType;

struct Obj {
  ObjType type;
  bool isMarked;
  struct Obj *next;
};

struct ObjString {
  Obj obj;
  int length;
  char *chars;
  uint32_t hash;
};

ObjString *takeString(char *chars, int length);
ObjString *copyString(const char* chars, int length);
void printObject(Value value);

typedef struct {
  Obj obj;
  int arity;
  int upvalueCount;
  Chunk chunk;
  ObjString *name;
} ObjFunction;

ObjFunction *newFunction();

typedef Value (*NativeFn)(int argCount, Value *args);

typedef struct {
  Obj obj;
  NativeFn function;
} ObjNative;

ObjNative *newNative(NativeFn function);

typedef struct ObjUpvalue {
  Obj obj;
  Value *location;
  Value closed;
  struct ObjUpvalue *next;
} ObjUpvalue;

ObjUpvalue *newUpvalue(Value *slot);

typedef struct {
  Obj obj;
  ObjFunction *function;
  ObjUpvalue **upvalues;
  int upvalueCount;
} ObjClosure;

ObjClosure *newClosure(ObjFunction *function);

static inline bool isObjType(Value value, ObjType type) {
  return IS_OBJ(value) && AS_OBJ(value)->type == type;
}

#endif
