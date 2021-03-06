// RUN: cat %s | %cling -DCLING_TMP="\"%/T\"" | FileCheck --check-prefix=CHECKOUT %s
// RUN: cat %T/outfile.txt | FileCheck --check-prefix=CHECK-REDIRECTOUT %s
// RUN: cat %T/errfile.txt | FileCheck --check-prefix=CHECK-REDIRECTERR %s
// RUN: cat %T/bothfile.txt | FileCheck --check-prefix=CHECK-REDIRECTBOTH %s
// RUN: cat %T/anotheroutfile.txt | FileCheck --check-prefix=CHECK-REDIRECTANOTHER %s
// RUN: cat %T/nospace.txt | FileCheck --check-prefix=CHECK-NOSPACE %s
// RUN: cat %s | %cling -DCLING_TMP="\"%/T\"" 2> %T/stderr.txt && cat %T/stderr.txt | FileCheck --check-prefix=CHECKERR %s
// RUN: cat %s | %cling -DCLING_TMP="\"%/T\"" 2>&1 | FileCheck --check-prefix=CHECKERR --check-prefix=CHECKOUT %s

#include <iostream>

extern "C" int setenv(const char *name, const char *value, int overwrite);
extern "C" int _putenv_s(const char *name, const char *value);
static void setup() {
#ifdef _WIN32
 #define setenv(n, v, o) _putenv_s(n,v)
#endif
  ::setenv("CLING_TMP", CLING_TMP, 1);
}
setup();

.2>&1
std::cerr << "Error into stdout.\n";
//CHECKOUT: Error into stdout.
.2>
std::cerr << "Error back from stdout.\n";
//CHECKERR: Error back from stdout.

.1>&2
std::cout << "stdout into stderr.\n";
//CHECKERR: stdout into stderr.
.1>
std::cout << "stdout back from stderr.\n";
//CHECKOUT: stdout back from stderr.

// Test redirect stdout
.> $CLING_TMP/outfile.txt
int a = 101
//CHECK-REDIRECTOUT: (int) 101
int b = 102
//CHECK-REDIRECTOUT: (int) 102
int c = 103
//CHECK-REDIRECTOUT: (int) 103

// Test stderr is not redirected as well.
std::cerr << "Into Error\n";
//CHECKERR: Into Error

// Test toggle back to prompt.
.>
int var = 9
//CHECKOUT: (int) 9

// Test append mode.
.>> $CLING_TMP/outfile.txt
a = 991
//CHECK-REDIRECTOUT: (int) 991
b = 992
//CHECK-REDIRECTOUT: (int) 992
c = 993
//CHECK-REDIRECTOUT: (int) 993

// Test redirect stderr
.2> $CLING_TMP/errfile.txt
std::cerr << "Error redirected.\n";
//CHECK-REDIRECTERR: Error redirected.

// Test stdout is still redirected to the correct file.
var = 20
//CHECK-REDIRECTOUT: (int) 20

// Test toggle only stdout and stderr still redirected.
.>
a = 100
//CHECKOUT: (int) 100
std::cerr << "Error still redirected.\n";
//CHECK-REDIRECTERR: Error still redirected.

// Test toggle stderr back to prompt.
.2>
std::cerr << "Error back to prompt.\n";
//CHECKERR: Error back to prompt.


// Test redirect of both streams.
.&> $CLING_TMP/bothfile.txt
a=310
//CHECK-REDIRECTBOTH: (int) 310
b=311
//CHECK-REDIRECTBOTH: (int) 311
c=312
//CHECK-REDIRECTBOTH: (int) 312
std::cerr << "Redirect both out & err.\n";
//CHECK-REDIRECTBOTH: Redirect both out & err.

// Test toggle both back to the prompt.
.&>
var = 400
//CHECKOUT: (int) 400
std::cerr << "Both back to prompt.\n";
//CHECKERR: Both back to prompt.

// Test append mode for both streams.
.&>> $CLING_TMP/bothfile.txt
a=491
//CHECK-REDIRECTBOTH: (int) 491
b=492
//CHECK-REDIRECTBOTH: (int) 492
c=493
//CHECK-REDIRECTBOTH: (int) 493
std::cerr << "Append mode for both streams.\n";
//CHECK-REDIRECTBOTH: Append mode for both streams.


// Test toggle only stdout to prompt and stderr to file.
.>
var = 699
//CHECKOUT: (int) 699
std::cerr << "Err is still in &> file.\n";
//CHECK-REDIRECTBOTH: Err is still in &> file.


// Test toggle stderr to the prompt when redirected with &.
.2>
std::cerr << "Err back from &> file.\n";
//CHECKERR: Err back from &> file.

// Test redirect to filename without space
.>$CLING_TMP/nospace.txt
a = 1012
//CHECK-NOSPACE: (int) 1012
b = 1023
//CHECK-NOSPACE: (int) 1023
c = 1034
//CHECK-NOSPACE: (int) 1034

// Test append mode to filename without space
.>>$CLING_TMP/nospace.txt
a = 9915
//CHECK-NOSPACE: (int) 9915
b = 9926
//CHECK-NOSPACE: (int) 9926
c = 9937
//CHECK-NOSPACE: (int) 9937

// Test redirect with & and toggle to out file.
.&>> $CLING_TMP/bothfile.txt
var = 999
//CHECK-REDIRECTBOTH: (int) 999

// Test that exiting in a redirected state will flush properly
.1> $CLING_TMP/anotheroutfile.txt
a = 710
//CHECK-REDIRECTANOTHER: (int) 710
b = 711
//CHECK-REDIRECTANOTHER: (int) 711
c = 712
//CHECK-REDIRECTANOTHER: (int) 712
