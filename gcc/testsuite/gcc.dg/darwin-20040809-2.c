/* Test dead code strip support.  */
/* Contributed by Devang Patel  <dpatel@apple.com>  */

/* { dg-do compile { target *-*-darwin* } } */
/* { dg-options "-gstabs+ -fno-eliminate-unused-debug-symbols" } */
/* LLVM LOCAL llvm doesn't currently support stabs. */
/* { dg-require-stabs "" } */

int
main ()
{
  return 0;
}

/* { dg-final { scan-assembler ".stabd.46,0,0" } } */
/* { dg-final { scan-assembler ".stabd.78,0,0" } } */

