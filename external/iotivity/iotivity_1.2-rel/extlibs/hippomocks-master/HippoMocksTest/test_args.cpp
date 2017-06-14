#include "hippomocks.h"
#include "Framework.h"

class IB {
public:
	virtual ~IB() {}
	virtual void f(int);
	virtual void g(int) = 0;
};

TEST (checkArgumentsAccepted)
{
	MockRepository mocks;
	IB *iamock = mocks.Mock<IB>();
	mocks.ExpectCall(iamock, IB::f).With(1);
	mocks.ExpectCall(iamock, IB::g).With(2);
	iamock->f(1);
	iamock->g(2);
}

TEST (checkArgumentsChecked)
{
	MockRepository mocks;
	IB *iamock = mocks.Mock<IB>();
	mocks.ExpectCall(iamock, IB::f).With(1);
	mocks.ExpectCall(iamock, IB::g).With(1);
	bool exceptionCaught = false;
	try 
	{
		iamock->f(2);
	}
	catch (HippoMocks::ExpectationException)
	{
		exceptionCaught = true;
	}
	CHECK(exceptionCaught);
	mocks.reset();
}

