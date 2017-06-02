#include "hippomocks.h"
#include "Framework.h"

class IA { 
public:
	virtual ~IA() {}
	virtual void f() = 0;
	virtual void f(int) = 0;
};

typedef void (IA::*mf)();

TEST (checkOverloadWithCastWorks)
{
	MockRepository mocks;
	IA *iamock = mocks.Mock<IA>();
	mocks.ExpectCallOverload(iamock, (mf)&IA::f);
	iamock->f();
}

