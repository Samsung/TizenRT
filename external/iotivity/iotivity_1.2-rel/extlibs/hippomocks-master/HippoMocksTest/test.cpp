#include "hippomocks.h"
#include "Framework.h"

class IA { 
public:
	virtual ~IA() {}
	virtual void f() {}
	virtual void g() = 0;
};

TEST (checkBaseCase)
{
	MockRepository mocks;
	IA *iamock = mocks.Mock<IA>();
	mocks.ExpectCall(iamock, IA::f);
	mocks.ExpectCall(iamock, IA::g);
	iamock->f();
	iamock->g();
}

TEST (checkMultiCall)
{
	MockRepository mocks;
	IA *iamock = mocks.Mock<IA>();
	mocks.ExpectCall(iamock, IA::f);
	mocks.ExpectCall(iamock, IA::g);
	mocks.ExpectCall(iamock, IA::f);
	iamock->f();
	iamock->g();
	iamock->f();
}

TEST (checkMultiCallNotCalled)
{
	bool exceptionCaught = false;
	try {
		MockRepository mocks;
		IA *iamock = mocks.Mock<IA>();
		mocks.ExpectCall(iamock, IA::f);
		mocks.ExpectCall(iamock, IA::g);
		mocks.ExpectCall(iamock, IA::f);
		iamock->f();
		iamock->g();
	}
	catch (HippoMocks::CallMissingException &) 
	{
		exceptionCaught = true;
	}
	CHECK(exceptionCaught);
}

TEST (checkMultiCallWrongOrder)
{
	MockRepository mocks;
	IA *iamock = mocks.Mock<IA>();
	mocks.ExpectCall(iamock, IA::f);
	mocks.ExpectCall(iamock, IA::g);
	mocks.ExpectCall(iamock, IA::f);
	iamock->f();
	bool exceptionCaught = false;
	try {
		iamock->f();
	}
	catch (HippoMocks::ExpectationException &) 
	{
		exceptionCaught = true;
	}
	CHECK(exceptionCaught);
	mocks.reset();
}

TEST (checkExpectationsNotCompleted)
{
	bool exceptionCaught = false;
	try {
		MockRepository mocks;
		IA *iamock = mocks.Mock<IA>();
		mocks.ExpectCall(iamock, IA::f);
		mocks.ExpectCall(iamock, IA::g);
		iamock->f();
	}
	catch (HippoMocks::CallMissingException &) 
	{
		exceptionCaught = true;
	}
	CHECK(exceptionCaught);
}

TEST (checkOvercompleteExpectations)
{
	bool exceptionCaught = false;
	try {
		MockRepository mocks;
		IA *iamock = mocks.Mock<IA>();
		mocks.ExpectCall(iamock, IA::f);
		mocks.ExpectCall(iamock, IA::g);
		iamock->f();
		iamock->g();
		iamock->f();
	}
	catch (HippoMocks::ExpectationException &) 
	{
		exceptionCaught = true;
	}
	CHECK(exceptionCaught);
}

TEST (checkExpectationsAreInOrder)
{
	bool exceptionCaught = false;
	try {
		MockRepository mocks;
		IA *iamock = mocks.Mock<IA>();
		mocks.ExpectCall(iamock, IA::f);
		mocks.ExpectCall(iamock, IA::g);
		iamock->g();
	}
	catch (HippoMocks::ExpectationException &) 
	{
		exceptionCaught = true;
	}
	CHECK(exceptionCaught);
}

