#include "hippomocks.h"
#include "Framework.h"
#include <string>

// non-reference is bad, but should work
class IC {
public:
	virtual ~IC() {}
	virtual void f(std::string s);
	virtual void g(std::string s) = 0;
};

TEST (checkClassArgumentsAccepted)
{
	MockRepository mocks;
	IC *iamock = mocks.Mock<IC>();
	mocks.OnCall(iamock, IC::f).With("hi");
	mocks.OnCall(iamock, IC::g).With("bye");
	iamock->f("hi");
	iamock->g("bye");
}

TEST (checkClassArgumentsChecked)
{
	MockRepository mocks;
	IC *iamock = mocks.Mock<IC>();
	mocks.OnCall(iamock, IC::f).With("hi");
	mocks.OnCall(iamock, IC::g).With("bye");
	bool exceptionCaught = false;
	try 
	{
		iamock->f("bye");
	}
	catch (HippoMocks::ExpectationException)
	{
		exceptionCaught = true;
	}
	CHECK(exceptionCaught);
	mocks.reset();
}

TEST (checkClassArgumentsIgnored)
{
	MockRepository mocks;
	IC *iamock = mocks.Mock<IC>();
	mocks.OnCall(iamock, IC::f);
	iamock->f("bye");
}

