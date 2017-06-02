#include "hippomocks.h"
#include "Framework.h"

class BaseA { 
public:
	virtual void a() = 0;
};

class BaseB { 
public:
	virtual void b() = 0;
};

class ClassC : public BaseA, public BaseB { 
public:
	virtual void c() = 0;
};

TEST (checkMICallsWork)
{
	MockRepository mocks;
	ClassC *iamock = mocks.Mock<ClassC>();
	mocks.ExpectCall(iamock, ClassC::a);
	mocks.ExpectCall(iamock, ClassC::c);
	mocks.ExpectCall(iamock, ClassC::b);
	iamock->a();
	iamock->c();
	iamock->b();
}

