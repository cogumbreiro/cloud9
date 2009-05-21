//===-- klee.h --------------------------------------------------*- C++ -*-===//
//
//                     The KLEE Symbolic Virtual Machine
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef __KLEE_H__
#define __KLEE_H__

#ifdef __cplusplus
extern "C" {
#endif
  
  /* Add an accesible memory object at a user specified location. It
     is the users responsibility to make sure that these memory
     objects do not overlap. These memory objects will also
     (obviously) not correctly interact with external function
     calls. */
  void klee_define_fixed_object(void *addr, unsigned nbytes);

  /* make the contents of the object pointed to by addr symbolic. addr
   * should always be the start of the object and nbytes must be its
   * entire size. name is an optional name (can be the empty string)
   */
  void klee_make_symbolic_name(void *addr, unsigned nbytes, 
			       const char *name);

  void klee_make_symbolic(void *addr, unsigned nbytes);  
  
  /* Return symbolic value in the (signed) interval [begin,end) */
  int klee_range(int begin, int end, const char *name);

  /* Return a symbolic integer */
  int klee_int(const char *name);

  void *klee_malloc_n(unsigned nelems, unsigned size, unsigned alignment);

  /* terminate the state without generating a test file */
  __attribute__((noreturn))
  void klee_silent_exit(int status);
  
  __attribute__((noreturn))
  void klee_abort(void);  

  /** Report an error and terminate the state. */
  __attribute__((noreturn))
  void klee_report_error(const char *file, 
			 int line, 
			 const char *message, 
			 const char *suffix);
  
  /* called by checking code to get size of memory. */
  unsigned klee_get_obj_size(void *ptr);
  
  /* print the tree associated w/ a given expression. */
  void klee_print_expr(const char *msg, ...);
  
  /* NB: this *does not* fork n times and return [0,n) in children.
   * It makes n be symbolic and returns: caller must compare N times.
   */
  unsigned klee_choose(unsigned n);
  
  /* special klee assert macro. this assert should be used when path consistency
   * across platforms is desired (e.g., in tests).
   * NB: __assert_fail is a klee "special" function
   */
# define klee_assert(expr)                                              \
  ((expr)                                                               \
   ? (void) (0)                                                         \
   : __assert_fail (#expr, __FILE__, __LINE__, __PRETTY_FUNCTION__))    \

  /* Return true if the given value is symbolic (represented by an
   * expression) in the current state. This is primarily for debugging
   * and writing tests but can also be used to enable prints in replay
   * mode.
   */
  unsigned klee_is_symbolic(unsigned n);


  /* The following intrinsics are primarily intended for internal use
     and may have peculiar semantics. */

  void klee_assume(unsigned condition);
  void klee_warning(const char *message);
  void klee_warning_once(const char *message);
  void klee_prefer_cex(void *object, unsigned condition);
  void klee_mark_global(void *object);

  /* Return a possible constant value for the input expression. This
     allows programs to forcibly concretize values on their own. */
  unsigned klee_get_value(unsigned expr);

  /* Ensure that memory in the range [address, address+size) is
     accessible to the program. If some byte in the range is not
     accessible an error will be generated and the state
     terminated. 
  
     The current implementation requires both address and size to be
     constants and that the range lie within a single object. */
  void klee_check_memory_access(const void *address, unsigned size);

  /* Enable/disable forking. */
  void klee_set_forking(unsigned enable);

  /* klee_alias_function("foo", "bar") will replace, at runtime (on
     the current path and all paths spawned on the current path), all
     calls to foo() by calls to bar().  foo() and bar() have to exist
     and have identical types.  Use klee_alias_function("foo", "foo")
     to undo.  Be aware that some special functions, such as exit(),
     may not always work. */
  void klee_alias_function(const char* fn_name, const char* new_fn_name);

#ifdef __cplusplus
}
#endif

#endif /* __KLEE_H__ */
