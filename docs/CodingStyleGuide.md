# Overview

Unified coding style makes it easy to maintain the TizenRT. The platform developers should follow the coding styles described in this document. The preferred style is BSD/Allman. **[Mxx]** : Mandatory **[Rxx]** : Recommend

# C Coding Style

## Comment

**[R01]** Both comment styles are allowed : C89 "/* … */" style & C99 “//”style.

[Example-Do]
```c
/*
    test example
    write();
    read();
*/

//test example
//write();
//read();
```
## Tips

Insert functions separator (80 chars) to make code more readable:
```c
//------------------------------------------------------------------------------

int func1()
{
;
}

//------------------------------------------------------------------------------

int func2()
{
;
}

//------------------------------------------------------------------------------
```
## Indentation

**[M01]** Use tabs. All tabs are 4 characters. Indentation use only tab (No space key).
```
 •|---means tab (with 4 spaces option)
 •|-------means tab (with 8 spaces option)
 •^means one char of space
```
[Example-Do]
```c
static int config_init(void)
{
|---int index;
|---/* create configfile */
|---entry = create(“abcdefg", NULL, NULL, NULL, NULL,
|---|---|---^^NULL, NULL);
|---if (!entry) {
|---|---return 0;
|---}
|---entry->size = 0;
|---return 0;
}
```
[Example-Don't]
```c
static int__init ikconfig_init(void)
{
|-------int index;
/* create configfile */
^^^^^^^entry = create(“abcdefg", NULL, NULL, NULL, NULL,
^^^^^^^^^^^^^^NULL, NULL);
^^if (!entry)
|-------return 0;
^^entry->size = 0;
^^return 0;
}
```

<br>

**[M02]** In a switch statement, 'case' has a same indent with 'switch,' and it is placed on a new line.

[Example-Do]
```c
switch (state) {
case START:
    break;
case RUNNING:
    break;
case STOP:
    break;
default:
    break;
}
```
[Example-Don't]
```c
switch (state) {
    case START:
        break;
    case RUNNING:
        break;
    case STOP:
        break;
    default:
        break;
}
```

<br>

**[R03]** goto labels aren't indented, allow a single space however.

[Example-Do]
```c
    if (value != value2) {
        goto out;
    }

…
out:
    return 0;
```
[Example-Don't]
```c
    if (value != value2) {
        goto out;
    }

…
    out:
    return 0;
```
## Space

**[M03]** Keywords have following space rules.

- Put a space after (most) keywords

    - example: if, switch, case, for, do, while

- Do not put a space after function like keywords

    - example: sizeof, typeof, alignof, __attribute__

- ^means one char of space

[Example-Do]
```c
if^(!value) {
    result = 0;
…
for^(;;) {
    i = j;
…
while^(!value) {
    i++
…
printf(“test%d”, sizeof(value));
```
[Example-Don't]
```c
if(!value) {
    result = 0;
…
for(;;) {
    i = j;
…
while(!value) {
    i++
…
printf(“test%d”, sizeof^(value));
```

<br>

**[M04]** Operators have following space rules.

- Put a space around(on each side of) most binary and ternary operators

    - example: = + -< > * / % | & ^ <= >= == != ? :

- Do not put a space after unary operators

    - example: & * + -~ !

- Unary "++" and unary "--" are allowed no space on one side

- Do not put a space around the "." and "->" structure member operators

- Do not put a space after cast operator

- ^means one char of space

[Example-Do]
```c
bool value^=^true;
function(&people->hand);
people->age++;
struct exlink^*link;
struct exlink^*saved_link;
read(&link->left);
tmp += (unsigned long)test[i];
```
[Example-Don't]
```c
bool value=true;
function(&^people->hand);
people->age^++;
struct exlink^*^link;
struct exlink^*^saved_link;
read(&link^->^left);
tmp += (unsigned long)^test[i];
```

<br>

**[M05]** Separators have following space rules.

- Put a space after closing brace when it has anything on the line.

    - exception : comma after closing brace '},'

- Put a space after comma

- Do not put space inside parenthesis '(', ')'

- Do not put space before open square bracket '[' and inside square bracket '[', ']'

- Do not put a space after the function name in function calls.

- ^means one char of space

[Example-Do]
```c
struct exlink[10];
read(&link->left, &link->right);
if (value > 0) {
    ;
}^else if (value == 0) {
    ;
}^else {
    ;
}

if (function()) {
    read(1);
    write(&people->hand);
}	
```
[Example-Don't]
```c
struct exlink^[^10^];
read(^&link->left^, &link->right^);
if (value > 0) {
    ;
}else if (value == 0) {
    ;
}else {
    ;
}

if (function^()) {
    read^(1);
    write^(&people->hand);
}
```
## Brace

**[M06]** Functions have the opening brace at the beginning of the next line.

[Example-Do]
```c
int function(void)
{
    ;
} 
```
[Example-Don't]
```c
int function(void) {
    ;
}
```

<br>

**[M07]** The open brace last on the line. The closing brace is empty on a line of its own.

- exception : else, while should follow close brace '}'

[Example-Do]
```c
if (value > 0) {
    ;
} else if (value == 0) {
    ;
}
…..
do {
    i = j;
} while (result);
```
[Example-Don't]
```c
if (value > 0)
{	
    ;
} 
else if (value == 0)
{
    ;
}
…..
do
{
    i = j;
} while (result);
```
**[M08]** Open braces for enum, union and struct go on the same line.

[Example-Do]
```c
struct people {
    string name;
    int age;
};
```
[Example-Don't]
```c
struct people 
{
    string name;
    int age;
};
```

<br>

~~**[R04]** Do not unnecessarily use braces where a single statement will do.~~

## Line

**[R05]** Do not put trailing white space at the ends of lines.

**[R06]** Check for adding lines without a newline at end of file.

- [EOF] means end of file

- ^means one char of space

[Example-Do]
```c
void function(void)
{
…
    printf(“test\n”);
…
}
EXPORT_SYMBOL(log);
[EOF]
```
[Example-Don't]
```c
void function(void)
{
…
    printf(“test\n”);^^^^^^^^^^
…
}
EXPORT_SYMBOL(log);[EOF]
```
## File organization

**[R07]** The Sequence of the code organization.

- Copyright

- File comment

- Including header file

- Define of constants and macro

- Implementation of exported(public) functions

- Implementation of internal(protected) functions

[Example-Do]
```c
/**
 * @copyright
 * In XYZ R&D Center (XYZ under a contract between)
 * @par
 * LLC "XYZ", Ltd (X3, ... of ...)
 * @par
 * Copyright (c) XYZ Ltd 2018. All rights reserved.
 */
/**
 * @file [package/]src/functionname.c
 * @author Xyan Pedro <xy.po@tizen.org>
 * @version 42
 * @date Created Mon, 4 Jul 2016 17:14:00 +0300
 * @date Modified Fri, 17 Feb 2017 12:43:12 +0200
 */

#include "headerfile.h"
..
#define MAGIC_START 	“START"
..
int public_function(void)
{
..
}
..
static int protectedfuction(void)
{
...
}
```
the dates and version might be obtained via: git log -1 --format=format:%aD $file

**[M09]** Separate external public header(API) and internal header.

|Type|Call By|Defined at|
|---|---|---|
|Exported(Public) functions|External Modules|External public header|
|Internal(Protected) functions|Inside of module|Internal header|
|Static(private) functions|Functions in same file|Source file|

~~**[M10]** External public headers have the doxygen style comment.~~

## Declaration/Definition

**[R02]** inline keyword should sit between storage class and type.

[Example-Do]
```c
static inline int example_function(void)
{
    return (0);
}
```
[Example-Don't]
```c
inline static int example_function(void)
{
    return (0);
}
```
**[M11]** In function prototypes, include parameter names with their data types and return type.

[Example-Do]
```c
extern int function(struct type *ex);
```
[Example-Don't]
```c
extern int function(struct type *);
```
**[R08]** Macros with multiple statements should be enclosed in a ‘do–while’ block.

**[R09]** '#' symbol for macro shall be located at the first column.

**[R10]** In macro definition, every expression and the arguments in the expressions shall be enclosed by '(' and ')' for each.

[Example-Do]
```c
#define example_kiocb(x, filp)              \
    do                                      \
    {                                       \
        struct task_struct*tsk= current;    \
        (x)->ki_flags= 0;                   \
        (x)->ki_users= 1;                   \
        (x)->ki_key= 3;                     \
        (x)->ki_filp= (filp);               \
        (x)->ki_ctx= NULL;                  \
     } while (0)
```
[Example-Don't]
```c
#define example_kiocb(x, filp)              \
    {                                       \
        struct task_struct*tsk= current;    \
        x->ki_flags= 0;                     \
        x->ki_users= 1;                     \
        x->ki_key= 3;                       \
        x->ki_filp= filp;                   \
        x->ki_ctx= NULL;                    \
    }
```
**[R11]** Don’t declare both a structure tag and variables or typedefs in the same declaration.

[Example-Do]
```c
struct example {
    int lock;
    const char *device_id;
};
static struct example example_busy[CHANNELS] = {
    [4] = { 1, "cascade" },
};
```
[Example-Don't]
```c
struct example{
    int lock;
    const char *device_id;
}example_busy[CHANNELS] = {
    [4] = { 1, "cascade" },
};

typedef struct example_t{
    int lock;
    const char *device_id;
}example;
```
**[R12]** Each variable shall be declared in the new line.

[Example-Do]
```c
void example(void)
{
    int i;
    int index;
```
[Example-Don't]
```c
void example(void)
{
    int i, index;
```
## Naming

**[M12]** No mixed-case, Use underscores('_') to separate words in an name.

[Example-Do]
```c
#define TEST_VALUE	32
extern struct list_head_hash[TEST_VALUE];

int example_function(void)
{
    return(TEST_VALUE - 1);
}
```
[Example-Don't]
```c
#define Test_Value	32
extern struct list_head_hash[Test_Value];

int ExampleFunction(void)
{
    return(Test_Value - 1);
}
```
**[R13]** Names of macros defining constants and labels in enums are composed of capital letters, numbers and '_' character.

[Example-Do]
```c
#define MAGIC_START     “START"
#define MAGIC_END       “END“

#define MAGIC_SIZE(sizeof(MAGIC_START) -1)
```
[Example-Don't]
```c
#define MagicStart      “START"
#define magic_end       “END“

#define magic_size(sizeof(MagicStart) -1)
```
**[R14]** Name of functions are Verb + Noun to have a good representation of features.

**[R15]** Local functions whose scope is limited to the current source file have ‘static’ keyword and names started with ‘__’.

**[R16]** Names of protected(global) functions in internal header are started with '_' character.

[Example-Do]
```c
EXPORT_API void datacall_prepare_call(void);
static boolean __call_dc_outgoingcall_endhandle(void);
```
[Example-Don't]
```c
EXPORT_API void datacall_call_prepare(void);
static boolean call_dc_outgoingcall_endhandle(void);
```

# C++ Coding Style

## Define Guard

**[M01]**The guard symbol format should be __<PROJECT>_<FILE>_H__.

- __<PROJECT> can be omitted, if the uniqueness can be guaranteed without it.

[Example-Do]
```cpp
// MySample.h
#ifndef __TEST_MY_SAMPLE_H__
#define __TEST_MY_SAMPLE_H__
class MySample
{
    // Do something...
};
#endif /* __TEST_MY_SAMPLE_H__ */
Or
 
// MySample.h
#ifndef __MY_SAMPLE_H__
#define __MY_SAMPLE_H__
  
class MySample
{ 
    // Do something...
};
#endif /* __MY_SAMPLE_H__ */
```
## Naming

### File Name

**[R01]** Filenames should be same as class names. For the uniqueness, the abbreviation of namespace can be added as prefix. ( no unserscore, camel notation )

[Example-Do]
```cpp
// Sample.h
class Sample
{
    ...
};

Or

// ViTest.h
namespace visual
{
class Test
{
...
};
}
```
### Namespace name

**[M02]** It should be all lowercase and can include underscores in the middle.

- example: app_assist

### Type name

**[M03]** class name, structure name, enumeration name. It should start with an uppercase letter, Camel notation, no underscore

### Variable name

**[M04]** It should start with a lowercase letter.

### Function name

**[M05]** The name should start with lowercase letters, Camel notation. Keep the style 'verb' + 'noun‘.

  
- Regular functions

    - global - starts with a lowercase letter and all the letters should be lowercase. Underscores can be inserted. ( same as C coding rule)

[Example-Do]
```cpp
const char* test_get_name(void);
```
- static - It should have two underscores as prefix

[Example-Do]
```cpp
static int __aaa_bbb(); 
```
- Member functions

    - It should starts with a lowercase letter and have a uppercase letter for each new word. No underscores.

[Example-Do]
```cpp
int createSomething();
```
### Member variable name

**[M06]** It should have two underscores and lowercase letters should follow it. camel notation, no underscore in the middle of the name.

- exception : structure member variable should start with lowercase letters without underscores

[Example-Do]
```cpp
private:
    int __aaaaBbbbb;
```
### Constant name

**[M07]** All the letter should be uppercase. Underscores are allowed.

[Example-Do]
```cpp
// header file in class (public)
public:
    static const int TEST_MAX = 10; 
// header file in class (private)
private:
    static const int __TEST_MAX = 10;
// cpp file
static const int __TEST_MAX = 10;
```
## Indentation

**[M08]** Use tabs. All tabs are 4 characters. Indentation use only tab (No space key).

**[M09]** Switch and case should be at the same indent.

[Example-Do]
```cpp
switch (...) {
case 1:
    break;
case 2:
    break;
default:
    break;
};
```
**[R02]** Constructor initializer can be all on one line or with subsequent lines with one indent.

[Example-Do]
```cpp
// Point.cpp
 
#include "Point.h"
 
Point::Point() :
    __x(0),
    __y(0)
{
    // TO DO...
}
```
## Space

**[M10]** Keywords have following space rules.

- Put a space after (most) keywords

    - example: if, switch, case, for, do, while

- Do not put a space after function like keywords

    - example: sizeof, typeof, alignof, __attribute__

- ^means one char of space

[Example-Do]
```cpp
if^(!value) {
    result = 0;
…
for^(;;) {
    i = j;
…
while^(!value) {
    i++
…
printf(“test%d”, sizeof(value));
```
[Example-Don't]
```cpp
if(!value) {
    result = 0;
…
for(;;) {
    i = j;
…
while(!value) {
    i++
…
printf(“test%d”, sizeof^(value));
```
**[M11]** Operators have following space rules.

- Put a space around(on each side of) most binary and ternary operators

    - example: = + -< > * / % | & ^ <= >= == != ? :

- Do not put a space after unary operators

    - example: & * + -~ !

- Unary "++" and unary "--" are allowed no space on one side

- Do not put a space around the "." and "->" structure member operators

- Do not put a space after cast operator

- ^means one char of space

[Example-Do]
```cpp
bool value^=^true;
function(&people->hand);
people->age++;
struct exlink^*link;
strcut exlink^*saved_link;
read(&link->left);
tmp += (unsigned long)test[i];
```
[Example-Don't]
```cpp
bool value=true;
function(&^people->hand);
people->age^++;
struct exlink^*^link;
strcut exlink^*^saved_link;
read(&link^->^left);
tmp += (unsigned long)^test[i];
```
**[M12]** Separators have following space rules.

- Put a space after closing brace when it has anything on the line.

    - exception : comma after closing brace '},'

- Put a space after comma

- Do not put space inside parenthesis '(', ')'

- Do not put space before open square bracket '[' and inside square bracket '[', ']'

- Do not put a space after the function name in function calls.

- ^means one char of space

[Example-Do]
```cpp
struct exlink[10];
read(&link->left, &link->right);
if (value > 0) {
    ;
}^else if (value == 0) {
    ;
}^else {
    ;
}

if (function()) {
    read(1);
    write(&people->hand);
}	
```
[Example-Don't]
```cpp
struct exlink^[^10^];
read(^&link->left^, &link->right^);
if (value > 0) {
    ;
}else if (value == 0) {
    ;
}else {
    ;
}

if (function^()) {
    read^(1);
    write^(&people->hand);
}
```
**[M13]** Pointer Expressions : The asterisk can be placed adjacent to either the type or variable name.

[Example-Do]
```cpp
int *a;
int* b; // preferred by BSD style
```
## Line

**[R03]** Do not put trailing whitespace at the ends of lines.

- ^means one char of space

[Example-Do]
```cpp
void function(void)
{
…
    printf(“test\n”);
…
}
EXPORT_SYMBOL(log);
```
[Example-Don't]
```cpp
void function(void)
{
…
    printf(“test\n”);^^^^^^^^^^
…
}
EXPORT_SYMBOL(log);
```
## Brace

**[M14]** Functions have the opening brace at the beginning of the next line.

[Example-Do]
```cpp
int function(void)
{
    ;
} 
```
[Example-Don't]
```cpp
int function(void) {
    ;
}
```
**[M15]** The open brace last on the line. The closing brace is empty on a line of its own.

- exception : else, while should follow close brace '}'

[Example-Do]
```cpp
if (value > 0) {
    ;
} else if (value == 0) {
    ;
}
…..
do {
    i = j;
} while (result);
```
[Example-Don't]
```cpp
if (value > 0)
{	
    ;
} 
else if (value == 0)
{
    ;
}
…..
do 
{
    i = j;
} while (result);
```
**[M16]** Open braces for enum, union and struct go on the same line.

[Example-Do]
```cpp
struct people {
    string name;
    int age;
};
```
[Example-Don't]
```cpp
struct people
{
    string name;
    int age;
};
```
~~**[R04]** Do not unnecessarily use braces where a single statement will do.~~

## Ordering

### Function parameter

**[R05]** When defining a function, parameter order is: inputs, in/out then outputs.

[Example-Do]
```cpp
void Foo::goo(int in, int* b, int* out)
{
    // to do...
}
```
### Include

**[R06]** Use standard order for readability and to avoid hidden dependencies :

C library, C++ library, Related header, other libraries' .h, your project's .h.

[Example-Do]
```cpp
#include <sys/types.h>
#include <unistd.h>
#include <hash_map>
#include <vector>
#include "foo/server/fooserver.h"
#include "base/basictypes.h"
#include "base/commandlineflags.h"
#include "foo/server/bar.h
```
### Declaration

**[R07]** Use the specified order of declarations within a class:

public: before private:, methods before data members (variables), etc.

- Typedefs and Enums

- Constants

- public methods

- public member variables

- protected methods

- protected member variables

- private methods

- private member variables

- friend

[Example-Do]
```cpp
#ifndef _SAMPLE_H_
#define _SAMPLE_H_
// header
#include <iostream>
#include <algorithm>
// Typedef
typedef unsigned int uint;
typedef unsigned long long ull;
// Constants
const int NUMBER = 100;
const char* const MY_NAME = "KilDong";
class Sample
{
public: // public member function
    Sample();
    ~Sample();
    void pubFunc();
public: // public member variable
    int a;
    int b;
protected: // protected member function
    void proFunc();
protected: // protected member variable
    int __c;
    int __d;
private: // private member function
    void priFunc();
private: // private member variable
    int __e;
    int __f;
// friend
friend class SampleImpl;
}; // Sample
#endif // _SAMPLE_H_
```
## Class

### Constructor

**[R08]** Do not put complex initialization in constructors.

### Reference arguments

**[R09]** All the reference type should be constant type. If it is output parameter, declare it as pointer type. If you want to pass in a null pointer or the function saves a pointer of the input, you can use const pointer type instead of const reference type.

[Example-Do]
```cpp
void printVec(const std::string& vec, int* out);
```
### Explicit

**[R10]** If the constructor has one parameter, use 'explicit' keyword to avoid undesirable type conversion.

### Interface class

**[R11]** Composed by pure virtual functions, Add prefix 'I‘

[Example-Do]
```cpp
class IShape
{
public:
    virtual void draw() = 0;
    virtual void setColor(int r, int g, int b, int a) = 0;
    virtual void setPosition(int x, int y) = 0;
    virtual ~IShape() { }
};
```
### Inheritance

**[R12]** Multiple inheritance is allowed only for interface classes

### Template class

**[R13]** Do not use complicated template class

### Exception

**[R14]** Do not use Exception

### Stream class

**[R15]** Do not use stream classes

## Reliability

**[R16]** Highly recommend to use const keyword whenever it is reasonable.

**[R17]** Do not make long and complicated functions.

**[R18]** Recommend to use C++ style cast

- static_cast - It is the equivalent of a C-style cast.

- const_cast - It removes the const qualifier.

- reinterpret_cast - Use this only if you know what you are doing with this unsafe conversion. Normally it will be not used.

- dynamic_cast : In applications, avoid to use this casting.

  
**[R19]** Do not use 'using namespace' in header files.

[Example-Do]
```cpp
// Widget.h
#include <map>
#include <string>

class Widget 
{
public:
    // ctor/dtor/member functions...
private:
   typedef std::map<std::string, Widget*> WidgetTable;
   WidgetTable table_;
};
```
[Example-Don't]
```cpp
// Widget.h
#include <map>
#include <string>
using namespace std;

class Widget 
{
public:
    // ctor/dtor/member functions...
private:
    typedef map<string, Widget*> WidgetTable;
    WidgetTable table_;
};
```
**[R20]** Avoid to use default arguments.

**[R21]** Do not use sizeof(type name).

**[R22]** Use doxygen veil for multi-including header protector:

```
/** @cond DOXYGEN_VEIL */
#ifndef __A_DAEMON_H__
#define __A_DAEMON_H__
/** @endcond DOXYGEN_VEIL */
```
