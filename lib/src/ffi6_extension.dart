library libffi6_extension.ffi6_extension;

import 'dart-ext:ffi6_extension';

class LibffiExtension {
  static final int closureHandler = _getClosureHandler();

  static void ffiCall(int cif, int fn, int rvalue, int avalue) {
    if (cif == null) {
      throw new ArgumentError.notNull("cif");
    }

    if (fn == null) {
      throw new ArgumentError.notNull("fn");
    }

    if (rvalue == null) {
      throw new ArgumentError.notNull("rvalue");
    }

    if (avalue == null) {
      throw new ArgumentError.notNull("avalue");
    }

    _ffiCall(cif, fn, rvalue, avalue);
  }

  static int ffiClosureAlloc(int size, List<int> code) {
    if (size == null) {
      throw new ArgumentError.notNull("size");
    }

    if (code == null) {
      throw new ArgumentError.notNull("code");
    }

    if (code.length < 1) {
      throw new ArgumentError("Length of the 'code' should be not less than 1");
    }

    return _ffiClosureAlloc(size, code);
  }

  static void ffiClosureFree(int closure) {
    if (closure == null) {
      throw new ArgumentError.notNull("closure");
    }

    _ffiClosureFree(closure);
  }

  static int ffiPrepCif(int cif, int abi, int nargs, int rtype, int atypes) {
    if (cif == null) {
      throw new ArgumentError.notNull("cif");
    }

    if (abi == null) {
      throw new ArgumentError.notNull("abi");
    }

    if (nargs == null) {
      throw new ArgumentError.notNull("nargs");
    }

    if (rtype == null) {
      throw new ArgumentError.notNull("rtype");
    }

    if (atypes == null) {
      throw new ArgumentError.notNull("atypes");
    }

    return _ffiPrepCif(cif, abi, nargs, rtype, atypes);
  }

  static int ffiPrepCifVar(int cif, int abi, int nfixedargs, int ntotalargs, int rtype, int atypes) {
    if (cif == null) {
      throw new ArgumentError.notNull("cif");
    }

    if (abi == null) {
      throw new ArgumentError.notNull("abi");
    }

    if (nfixedargs == null) {
      throw new ArgumentError.notNull("nfixedargs");
    }

    if (ntotalargs == null) {
      throw new ArgumentError.notNull("ntotalargs");
    }

    if (rtype == null) {
      throw new ArgumentError.notNull("rtype");
    }

    if (atypes == null) {
      throw new ArgumentError.notNull("atypes");
    }

    return _ffiPrepCifVar(cif, abi, nfixedargs, ntotalargs, rtype, atypes);
  }

  static int ffiPrepClosure(int closure, int cif, int fn, int userData) {
    if (closure == null) {
      throw new ArgumentError.notNull("closure");
    }

    if (cif == null) {
      throw new ArgumentError.notNull("cif");
    }

    if (fn == null) {
      throw new ArgumentError.notNull("fn");
    }

    if (userData == null) {
      throw new ArgumentError.notNull("user_data");
    }

    return _ffiPrepClosure(closure, cif, fn, userData);
  }

  static int ffiPrepClosureLoc(int closure, int cif, int fn, int userData, int codeloc) {
    if (closure == null) {
      throw new ArgumentError.notNull("closure");
    }

    if (cif == null) {
      throw new ArgumentError.notNull("cif");
    }

    if (fn == null) {
      throw new ArgumentError.notNull("fn");
    }

    if (codeloc == null) {
      throw new ArgumentError.notNull("codeloc");
    }

    return _ffiPrepClosureLoc(closure, cif, fn, userData, codeloc);
  }

  static int registerClosure(Object closure, int handle) {
    if (closure == null) {
      throw new ArgumentError.notNull("closure");
    }

    if (handle == null) {
      throw new ArgumentError.notNull("handle");
    }

    return _registerClosure(closure, handle);
  }

  static void resolveFunction(String name, int addr) {
    if (name == null) {
      throw new ArgumentError.notNull("name");
    }

    if (addr == null) {
      throw new ArgumentError.notNull("addr");
    }

    _resolveFunction(name, addr);
  }

  static void _ffiCall(int cif, int fn, int rvalue, int avalue) native "Libffi_FfiCall";

  static int _ffiClosureAlloc(int size, List<int> code) native "Libffi_FfiClosureAlloc";

  static void _ffiClosureFree(int closure) native "Libffi_FfiClosureFree";

  static int _ffiPrepCif(int cif, int abi, int nargs, int rtype, int atypes) native "Libffi_FfiPrepCif";

  static int _ffiPrepCifVar(
      int cif, int abi, int nfixedargs, int ntotalargs, int rtype, int atypes) native "Libffi_FfiPrepCifVar";

  static int _ffiPrepClosure(int closure, int cif, int fn, int user_data) native "Libffi_FfiPrepClosure";

  static int _ffiPrepClosureLoc(
      int closure, int cif, int fn, int user_data, int codeloc) native "Libffi_FfiPrepClosureLoc";

  static int _getClosureHandler() native "Libffi_GetClosureHandler";

  static int _registerClosure(Object closure, int handle) native "Libffi_RegisterClosure";

  static void _resolveFunction(String name, int addr) native "Libffi_ResolveFunction";
}
