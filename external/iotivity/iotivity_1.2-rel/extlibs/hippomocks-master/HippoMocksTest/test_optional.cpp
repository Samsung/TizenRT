#include "hippomocks.h"
#include "Framework.h"

class II { 
public:
	virtual ~II() {}
	virtual void f() {}
	virtual void g() = 0;
};

TEST (checkBaseCaseOptionalOk)
{
	MockRepository mocks;
	II *iamock = mocks.Mock<II>();
	mocks.ExpectCall(iamock, II::f);
	mocks.OnCall(iamock, II::g);
	iamock->g();
	iamock->f();
}

TEST (checkBaseCaseOptionalNotRequired)
{
	MockRepository mocks;
	II *iamock = mocks.Mock<II>();
	mocks.ExpectCall(iamock, II::f);
	mocks.OnCall(iamock, II::g);
	iamock->f();
}

TEST (checkBaseCaseOptionalTwiceIsOK)
{
	MockRepository mocks;
	II *iamock = mocks.Mock<II>();
	mocks.ExpectCall(iamock, II::f);
	mocks.OnCall(iamock, II::g);
	iamock->g();
	iamock->f();
	iamock->g();
}

