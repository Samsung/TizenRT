#include <string>
#include "hippomocks.h"
#include "Framework.h"

using HippoMocks::byRef;

class IDoFunctor { 
public:
	virtual ~IDoFunctor() {}
	virtual void a(std::string arg) = 0;
};

#if __cplusplus > 199711L
TEST (checkDoFunctorCanBeLambdaWithContent)
{
	MockRepository mocks;
	IDoFunctor *iamock = mocks.Mock<IDoFunctor>();
  {
    std::string value = "Hello World";
    mocks.ExpectCall(iamock, IDoFunctor::a).Do([=](std::string arg){
      CHECK(arg == value);
    });
  }

  iamock->a("Hello World");
}
#endif

class II { 
public:
	virtual ~II() {}
	virtual void f() {}
	virtual void g() = 0;
};

bool checked;
void setChecked() { checked = true; }

class functorClass {
public:
	functorClass() : calls(0) {}
	int calls;
	void operator()() { ++calls; }
};

TEST (checkFunctorsCalled)
{
	MockRepository mocks;
	II *iamock = mocks.Mock<II>();
	mocks.ExpectCall(iamock, II::f).Do(setChecked);
	mocks.OnCall(iamock, II::g).Do(setChecked);
	checked = false;
	iamock->g();
	CHECK(checked == true);
	checked = false;
	iamock->f();
	CHECK(checked == true);
	checked = false;
	iamock->g();
	CHECK(checked == true);
}

TEST (checkFunctorObjectCalled)
{
	MockRepository mocks;
	II *iamock = mocks.Mock<II>();
	functorClass obj;
	mocks.ExpectCall(iamock, II::f).Do(byRef(obj));
	mocks.OnCall(iamock, II::g).Do(byRef(obj));
	CHECK(obj.calls == 0);
	iamock->g();
	CHECK(obj.calls == 1);
	iamock->f();
	CHECK(obj.calls == 2);
	iamock->g();
	CHECK(obj.calls == 3);
}

class functorByVal {
public:
  functorByVal(int &count) : count(count) {}
  void operator()() { count++; }
  int &count;
private:
  functorByVal &operator=(const functorByVal&); // Silence VS
};

TEST (checkFunctorObjectByvalCalled)
{
  int count = 0;
	MockRepository mocks;
	II *iamock = mocks.Mock<II>();
	mocks.ExpectCall(iamock, II::f).Do(functorByVal(count));
	mocks.OnCall(iamock, II::g).Do(functorByVal(count));
	CHECK(count == 0);
	iamock->g();
	CHECK(count == 1);
	iamock->f();
	CHECK(count == 2);
	iamock->g();
	CHECK(count == 3);
}


