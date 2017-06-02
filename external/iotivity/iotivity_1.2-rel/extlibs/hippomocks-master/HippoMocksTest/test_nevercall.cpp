#include "Framework.h"
#include "hippomocks.h"

class IR { 
public:
	virtual ~IR() {}
	virtual void f() {}
	virtual void g() {}
	virtual int h() { return 0; }
};

TEST (checkNeverCallWorks)
{
	bool exceptionCaught = false;
	MockRepository mocks;
	IR *iamock = mocks.Mock<IR>();
	Call &callF = mocks.ExpectCall(iamock, IR::f);
	mocks.OnCall(iamock, IR::g);
	mocks.NeverCall(iamock, IR::g).After(callF);
	iamock->g();
	iamock->g();
	iamock->f();
	try 
	{
		iamock->g();
	}
	catch (HippoMocks::ExpectationException &)
	{
		exceptionCaught = true;
	}
	CHECK(exceptionCaught);
}

TEST (checkNeverCallExceptionDetail)
{
	bool exceptionCaught = false;
	MockRepository mocks;
	IR *iamock = mocks.Mock<IR>();
	mocks.NeverCall(iamock, IR::g);
	try
	{
		iamock->g();
	}
	catch (HippoMocks::ExpectationException &ex)
	{
		exceptionCaught = true;
		CHECK(strstr(ex.what(), "IR::g") != NULL);
	}
	CHECK(exceptionCaught);
}

TEST (checkInteractionBetweenCallTypesWorks)
{
	bool exceptionCaught = false;
	MockRepository mocks;
	mocks.autoExpect = false;
	IR *iamock = mocks.Mock<IR>();
	Call &callF = mocks.ExpectCall(iamock, IR::f);
	Call &onCallG = mocks.OnCall(iamock, IR::g);
	mocks.OnCall(iamock, IR::h).Return(2);
	Call &returnThree = mocks.ExpectCall(iamock, IR::h).After(onCallG).Return(3);
	Call &returnFour = mocks.ExpectCall(iamock, IR::h).After(callF).Return(4);
	mocks.NeverCall(iamock, IR::h).After(returnThree).After(returnFour);
	CHECK(iamock->h() == 2);
	CHECK(iamock->h() == 2);
	iamock->f();
	CHECK(iamock->h() == 4);
	CHECK(iamock->h() == 2);
	iamock->g();
	CHECK(iamock->h() == 3);
	try 
	{
		iamock->h();
	}
	catch (HippoMocks::ExpectationException &)
	{
		exceptionCaught = true;
	}
	CHECK(exceptionCaught);
}

