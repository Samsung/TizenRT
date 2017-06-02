#ifndef TEST_H
#define TEST_H

#include <vector>
#include <string.h>
#include <string>
#include <exception>
#include <stdio.h>
#include <stdarg.h>

class BaseException : public std::exception {
public:
  BaseException(std::string fmt, ...) {
    va_list l;
    static char buffer[4096];
    va_start(l, fmt);
    vsnprintf(buffer, 4096, fmt.c_str(), l);
    va_end(l);
    error = buffer;
  }

  ~BaseException() throw() {}
  const char *what() const throw() { return error.c_str(); }
private:
  std::string error;
};

class Test;

class TestRegistry {
    std::vector<Test *> tests;
public:
    static TestRegistry &Instance() { static TestRegistry registry; return registry; }
    void AddTest(Test *test) {
        tests.push_back(test);
    }
    bool RunTests();
};

class Test {
public:
    std::string name;
    Test(std::string name) 
    : name(name)
    {
    }
    virtual void Run() = 0;
};

template <typename T, typename TB>
void check_equal(T a, TB b, const char *sa, const char *sb) {
    if (a != b) {
        throw BaseException("%s(%d) != %s(%d)", sa, a, sb, b);
    }
}

template <>
void check_equal(float a, float b, const char *sa, const char *sb);

template <typename T>
void check_equal(const std::string &a, T b, const char *sa, const char *sb) {
    return check_equal(a.c_str(), b, sa, sb);
}

template <typename T>
void check_equal(T a, const std::string &b, const char *sa, const char *sb) {
    return check_equal(a, b.c_str(), sa, sb);
}

template <>
void check_equal<const char *, const char *>(const char *a, const char *b, const char *sa, const char *sb);

#define CHECK(x) { if (!(x)) { throw BaseException("!(%s)", #x); } }
#define EQUALS(x, y) check_equal(x, y, #x, #y)

#define TEST(X) class Test##X : public Test { public: Test##X() : Test(#X) { TestRegistry::Instance().AddTest(this); } void Run(); };\
static Test##X _Test##X;\
void Test##X::Run()

#endif


