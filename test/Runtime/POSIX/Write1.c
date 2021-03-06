// RUN: %llvmgcc %s -emit-llvm -O0 -c -o %t.bc
// RUN: %klee --exit-on-error --libc=uclibc --posix-runtime --init-env %t.bc --sym-files 1 10 --sym-stdout 2>%t.log

#include <stdio.h>
#include <assert.h>

int main(int argc, char** argv) {
  char buf[32];
  
  FILE* f = fopen("A", "w");
  if (!f)
    klee_silent_exit(0);
  fwrite("Hello", sizeof("Hello"), 1, f);
  fclose(f);
  
  f = fopen("A", "r");
  if (!f)
    klee_silent_exit(0); // Write-only files?
  fread(buf, sizeof("Hello"), 1, f);
  fclose(f);

  assert(memcmp(buf, "Hello", sizeof("Hello")) == 0);

  return 0;
}
