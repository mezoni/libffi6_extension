#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <assert.h>
#include "dart_api.h"

#ifdef _WIN32
#define snprintf _snprintf
#endif

Dart_Handle HandleError(Dart_Handle handle);

Dart_NativeFunction ResolveName(Dart_Handle name, int argc, bool* auto_setup_scope);

DART_EXPORT Dart_Handle ffi6_extension_Init(Dart_Handle parent_library) {
  Dart_Handle result_code;

  if(Dart_IsError(parent_library)) {
    return parent_library;
  }

  result_code = Dart_SetNativeResolver(parent_library, ResolveName, NULL);
  if(Dart_IsError(result_code)) {
    return result_code;
  }  

  return Dart_Null();
}

Dart_Handle HandleError(Dart_Handle handle) {
  if (Dart_IsError(handle)) {
    Dart_PropagateError(handle);
  }

  return handle;
}

void DartError(const char* message, const char* type) {
  Dart_Handle dh_class;
  Dart_Handle dh_instance;
  Dart_Handle dh_library;
  Dart_Handle list[1];

  dh_library = Dart_LookupLibrary(Dart_NewStringFromCString("dart:core"));
  dh_class = Dart_GetClass(dh_library, Dart_NewStringFromCString(type));
  list[0] = Dart_NewStringFromCString(message);
  dh_instance = Dart_New(dh_class, Dart_Null(), 1, list);
  Dart_ThrowException(dh_instance);
}

/*---------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*ffi_call)(void* cif, void* fn, void* rvalue, void* avalue);
typedef int (*ffi_prep_cif)(void* cif, int abi, unsigned int nargs, void* rtype, void* atypes);
typedef int (*ffi_prep_cif_var)(void* cif, int abi, unsigned int nfixedargs, unsigned int ntotalargs, void* rtype, void* atypes);

typedef void* (*ffi_closure_alloc)(size_t size, void** code);
typedef void (*ffi_closure_free)(void* closure);
typedef int (*ffi_prep_closure)(void* closure, void* cif, void *fun, void *user_data);
typedef int (*ffi_prep_closure_loc)(void* closure, void *cif, void *fun, void* user_data, void* codeloc);

typedef void (*ffi_closure)(void* cif, void* ret, void* args, void* user_data);

void closure_handler(void *cif, void *ret, void* args[], Dart_WeakPersistentHandle *handler) {  
  Dart_Handle arguments[2]; 
  
  arguments[0] = Dart_NewInteger((int64_t)*args);
  arguments[1] = Dart_NewInteger((int64_t)ret);          
  HandleError(Dart_InvokeClosure((Dart_Handle)*handler, 2, arguments));  
}

#ifdef __cplusplus
}
#endif

/*---------------------------------------------------------------------------*/

static ffi_call pffi_call;
static ffi_prep_cif pffi_prep_cif;
static ffi_prep_cif_var pffi_prep_cif_var;

static ffi_closure_alloc pffi_closure_alloc;
static ffi_closure_free pffi_closure_free;
static ffi_prep_closure pffi_prep_closure;
static ffi_prep_closure_loc pffi_prep_closure_loc;

/*---------------------------------------------------------------------------*/

void Libffi_FfiCall(Dart_NativeArguments arguments) {  
  int64_t avalue;
  int64_t cif;
  int64_t fn;  
  int64_t rvalue;
  
  Dart_GetNativeIntegerArgument(arguments, 0, &cif);
  Dart_GetNativeIntegerArgument(arguments, 1, &fn);
  Dart_GetNativeIntegerArgument(arguments, 2, &rvalue);
  Dart_GetNativeIntegerArgument(arguments, 3, &avalue);

  assert(pffi_call != NULL);
  pffi_call((void*)cif, (void*)fn, (void*)rvalue, (void*)avalue);
  Dart_SetReturnValue(arguments, Dart_Null());
}

void Libffi_FfiPrepCif(Dart_NativeArguments arguments) {
  int64_t abi;  
  int64_t atypes;
  int64_t cif;  
  int64_t nargs;
  int64_t rtype;
  int status;
  
  Dart_GetNativeIntegerArgument(arguments, 0, &cif);
  Dart_GetNativeIntegerArgument(arguments, 1, &abi);
  Dart_GetNativeIntegerArgument(arguments, 2, &nargs);
  Dart_GetNativeIntegerArgument(arguments, 3, &rtype);
  Dart_GetNativeIntegerArgument(arguments, 4, &atypes);

  assert(pffi_prep_cif != NULL);
  status = pffi_prep_cif((void*)cif, (int)abi, (unsigned int)nargs, (void*)rtype, (void*)atypes);
  Dart_SetReturnValue(arguments, Dart_NewInteger(status));
}

void Libffi_FfiPrepCifVar(Dart_NativeArguments arguments) {
  int64_t abi;  
  int64_t atypes;
  int64_t cif;  
  int64_t nfixedargs;
  int64_t ntotalargs;
  int64_t rtype;
  int status;
  
  Dart_GetNativeIntegerArgument(arguments, 0, &cif);
  Dart_GetNativeIntegerArgument(arguments, 1, &abi);
  Dart_GetNativeIntegerArgument(arguments, 2, &nfixedargs);
  Dart_GetNativeIntegerArgument(arguments, 3, &ntotalargs);
  Dart_GetNativeIntegerArgument(arguments, 4, &rtype);
  Dart_GetNativeIntegerArgument(arguments, 5, &atypes);
  
  assert(pffi_prep_cif_var != NULL);
  status = pffi_prep_cif_var((void*)cif, (int)abi, (unsigned int)nfixedargs, (unsigned int)ntotalargs, (void*)rtype, (void*)atypes);
  Dart_SetReturnValue(arguments, Dart_NewInteger(status));
}

void Libffi_FfiClosureAlloc(Dart_NativeArguments arguments) {  
  void* code;  
  Dart_Handle list;
  int64_t result;  
  int64_t size;
  
  Dart_GetNativeIntegerArgument(arguments, 0, &size);
  list = Dart_GetNativeArgument(arguments, 1);
  
  assert(pffi_closure_alloc != NULL);
  result = (int64_t)pffi_closure_alloc((size_t)size, &code);
  Dart_ListSetAt(list, 0, Dart_NewInteger((int64_t)code));
  Dart_SetReturnValue(arguments, Dart_NewInteger(result));
}

void Libffi_FfiClosureFree(Dart_NativeArguments arguments) {  
  int64_t closure;  
  
  Dart_GetNativeIntegerArgument(arguments, 0, &closure);  
  assert(pffi_closure_free != NULL);
  pffi_closure_free((void *)closure);
}

void Libffi_FfiPrepClosure(Dart_NativeArguments arguments) {  
  int64_t cif;
  int64_t closure;  
  int64_t fun;
  int64_t user_data;    
  int status;
  
  Dart_GetNativeIntegerArgument(arguments, 0, &closure);
  Dart_GetNativeIntegerArgument(arguments, 1, &cif);
  Dart_GetNativeIntegerArgument(arguments, 2, &fun);
  Dart_GetNativeIntegerArgument(arguments, 3, &user_data);   
  
  assert(pffi_prep_closure != NULL);
  status = pffi_prep_closure((void*)closure, (void*)cif, (void*)fun, (void*)user_data);
  Dart_SetReturnValue(arguments, Dart_NewInteger(status));
}

void Libffi_FfiPrepClosureLoc(Dart_NativeArguments arguments) {  
  int64_t cif;
  int64_t closure;
  int64_t codeloc;
  int64_t fun;
  int64_t user_data;  
  int status;
  
  Dart_GetNativeIntegerArgument(arguments, 0, &closure);
  Dart_GetNativeIntegerArgument(arguments, 1, &cif);
  Dart_GetNativeIntegerArgument(arguments, 2, &fun);
  Dart_GetNativeIntegerArgument(arguments, 3, &user_data);
  Dart_GetNativeIntegerArgument(arguments, 4, &codeloc);
  
  assert(pffi_prep_closure_loc != NULL);
  status = pffi_prep_closure_loc((void*)closure, (void*)cif, (void*)fun, (void*)user_data, (void*)codeloc);
  Dart_SetReturnValue(arguments, Dart_NewInteger(status));
}

/*---------------------------------------------------------------------------*/

void Libffi_GetClosureHandler(Dart_NativeArguments arguments) {  
  Dart_SetReturnValue(arguments, Dart_NewInteger((int64_t)&closure_handler));
}

/*---------------------------------------------------------------------------*/

void Libffi__ClosureFinalizer(void* isolate_callback_data, Dart_WeakPersistentHandle handle, void *peer) {
  pffi_closure_free(peer);  
}

void Libffi_RegisterClosure(Dart_NativeArguments arguments) {  
  Dart_Handle dh_object;
  int64_t handle;

  dh_object = Dart_GetNativeArgument(arguments, 0);
  Dart_GetNativeIntegerArgument(arguments, 1, &handle);
  assert(pffi_closure_free != NULL);
  Dart_NewWeakPersistentHandle(dh_object, (void*)handle, 1, Libffi__ClosureFinalizer);  
}

/*---------------------------------------------------------------------------*/

struct FfiFunctionLookup {
  const char* name;
  void** function;
};

struct FfiFunctionLookup ffi_function_list[] = {
  {"ffi_call", (void**)&pffi_call},
  {"ffi_prep_cif", (void**)&pffi_prep_cif},
  {"ffi_prep_cif_var", (void**)&pffi_prep_cif_var},
  {"ffi_closure_alloc", (void**)&pffi_closure_alloc},
  {"ffi_closure_free", (void**)&pffi_closure_free},
  {"ffi_prep_closure", (void**)&pffi_prep_closure},
  {"ffi_prep_closure_loc", (void**)&pffi_prep_closure_loc},
  
  {NULL, NULL}
};

void Libffi_ResolveFunction(Dart_NativeArguments arguments) {
  int64_t addr;
  char buffer[100];
  Dart_Handle dh_name; 
  void **fn;
  int i;
  const char* name;
  
  dh_name = Dart_GetNativeArgument(arguments, 0);
  Dart_GetNativeIntegerArgument(arguments, 1, &addr);
  Dart_StringToCString(dh_name, &name);
  fn = NULL;   
  for (i = 0; ffi_function_list[i].name != NULL; ++i) {  
    if (strcmp(ffi_function_list[i].name, name) == 0) {      
      fn = ffi_function_list[i].function;
      break;
    }
  }  
  
  if (fn == NULL) {
    snprintf(buffer, sizeof(buffer) / sizeof(char), "Unknown function '%s'", name);   
    DartError(buffer, "ArgumentError");
  } else {
    if (*fn != NULL) {
      snprintf(buffer, sizeof(buffer) / sizeof(char), "Function '%s' already resolved", name);   
      DartError(buffer, "ArgumentError");
    } else {    
      *fn = (void*)addr;
    }
  }  
}

/*---------------------------------------------------------------------------*/

struct FunctionLookup {
  const char* name;
  Dart_NativeFunction function;
};

struct FunctionLookup function_list[] = {
  {"Libffi_FfiCall", Libffi_FfiCall},  
  {"Libffi_FfiPrepCif", Libffi_FfiPrepCif},
  {"Libffi_FfiPrepCifVar", Libffi_FfiPrepCifVar},
  
  {"Libffi_FfiClosureAlloc", Libffi_FfiClosureAlloc},
  {"Libffi_FfiClosureFree", Libffi_FfiClosureFree},
  {"Libffi_FfiPrepClosure", Libffi_FfiPrepClosure},
  {"Libffi_FfiPrepClosureLoc", Libffi_FfiPrepClosureLoc},    
  
  {"Libffi_GetClosureHandler", Libffi_GetClosureHandler},
  
  {"Libffi_RegisterClosure", Libffi_RegisterClosure},
  
  {"Libffi_ResolveFunction", Libffi_ResolveFunction},

  {NULL, NULL}
};

Dart_NativeFunction ResolveName(Dart_Handle name, int argc, bool* auto_setup_scope) {
  const char* cname;
  Dart_NativeFunction dh_result;
  int i;

  if (!Dart_IsString(name)) {
    return NULL;
  }

  dh_result = NULL;
  HandleError(Dart_StringToCString(name, &cname));
  for (i = 0; function_list[i].name != NULL; ++i) {
    if (strcmp(function_list[i].name, cname) == 0) {
      dh_result = function_list[i].function;
      break;
    }
  }

  return dh_result;
}
