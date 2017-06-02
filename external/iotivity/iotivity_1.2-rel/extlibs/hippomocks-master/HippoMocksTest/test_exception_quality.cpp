#include "Framework.h"
#include "hippomocks.h"
#include <stdio.h>

struct X {};

bool operator==(const X &, const X &)
{
	return true;
}

class IS {
public:
	virtual ~IS() {}
	virtual int f() { return 1; }
	virtual int g(int, int) { return 2; }
	virtual void h() = 0;
	virtual void i(int, const X &) = 0;
};

TEST (checkNoResultContainsFuncName)
{
	bool exceptionCaught = false;
	MockRepository mocks;
	IS *iamock = mocks.Mock<IS>();
	mocks.ExpectCall(iamock, IS::f);
	try {
		iamock->f();
	} catch(HippoMocks::NoResultSetUpException &ex) {
		exceptionCaught = true;
		CHECK(strstr(ex.what(), "IS::f") != NULL);
	}
	CHECK(exceptionCaught);
}

TEST (checkNoResultContainsBlankArgSpec)
{
	bool exceptionCaught = false;
	MockRepository mocks;
	IS *iamock = mocks.Mock<IS>();
	mocks.ExpectCall(iamock, IS::g);
	try {
		iamock->g(1,2);
	} catch(HippoMocks::NoResultSetUpException &ex) {
		exceptionCaught = true;
		CHECK(strstr(ex.what(), "IS::g") != NULL);
		CHECK(strstr(ex.what(), "(...)") != NULL);
	}
	CHECK(exceptionCaught);
}

TEST (checkNoResultContainsActualArgSpec)
{
	bool exceptionCaught = false;
	MockRepository mocks;
	IS *iamock = mocks.Mock<IS>();
	mocks.ExpectCall(iamock, IS::g).With(1,2);
	try {
		iamock->g(1,2);
	} catch(HippoMocks::NoResultSetUpException &ex) {
		exceptionCaught = true;
		CHECK(strstr(ex.what(), "IS::g") != NULL);
		CHECK(strstr(ex.what(), "(1,2)") != NULL);
	}
	CHECK(exceptionCaught);
}

TEST (checkNoResultContainsActualUnprintableArgSpec)
{
	bool exceptionCaught = false;
	MockRepository mocks;
	IS *iamock = mocks.Mock<IS>();
	mocks.NeverCall(iamock, IS::i).With(42, X());
	try {
		iamock->h();
	} catch(HippoMocks::NotImplementedException &ex) {
		exceptionCaught = true;
		CHECK(strstr(ex.what(), "IS::i") != NULL);
		CHECK(strstr(ex.what(), "(42,??\?)") != NULL);
	}
	CHECK(exceptionCaught);
}

TEST (checkNoResultDoesNotComplainIfNotCalled)
{
	MockRepository mocks;
	IS *iamock = mocks.Mock<IS>();
	mocks.OnCall(iamock, IS::g).With(1,2);
	mocks.OnCall(iamock, IS::g).With(3,4).Return(42);
	iamock->g(3,4);
}

TEST(checkNotImplementedExceptionToContainInfo)
{
	bool exceptionCaught = false;
	MockRepository mocks;
	mocks.autoExpect = false;
	IS *ismock = mocks.Mock<IS>();
	mocks.ExpectCall(ismock, IS::f).Return(1);
	mocks.OnCall(ismock, IS::f).Return(2);
	mocks.ExpectCall(ismock, IS::g).With(1,2).Return(2);
	mocks.OnCall(ismock, IS::g).Return(3);
	mocks.NeverCall(ismock, IS::g).With(3,4);

	try
	{
		ismock->h();
	}
	catch (HippoMocks::NotImplementedException &ex)
	{
		CHECK(strstr(ex.what(), "Expectation for IS::f()") != NULL);
		CHECK(strstr(ex.what(), "Result set for IS::f()") != NULL);
		CHECK(strstr(ex.what(), "Expectation for IS::g(1,2)") != NULL);
		CHECK(strstr(ex.what(), "Result set for IS::g(...)") != NULL);
		CHECK(strstr(ex.what(), "Result set for IS::g(3,4)") != NULL);
		CHECK(strstr(ex.what(), __FILE__) != NULL);
		exceptionCaught = true;
	}
	mocks.reset();
	CHECK(exceptionCaught);
}

TEST(checkExpectationExceptionToContainInfo)
{
	bool exceptionCaught = false;
	MockRepository mocks;
	mocks.autoExpect = false;
	IS *ismock = mocks.Mock<IS>();
	mocks.ExpectCall(ismock, IS::f).Return(1);
	mocks.OnCall(ismock, IS::f).Return(2);
	mocks.ExpectCall(ismock, IS::g).With(1,2).Return(2);
	mocks.OnCall(ismock, IS::g).With(2,3).Return(3);
	mocks.NeverCall(ismock, IS::g).With(3,4);
	mocks.NeverCall(ismock, IS::h);

	try
	{
		ismock->g(0,1);
	}
	catch (HippoMocks::ExpectationException &ex)
	{
		CHECK(strstr(ex.what(), "Function IS::g(0,1) called") != NULL);
		CHECK(strstr(ex.what(), "Expectation for IS::f()") != NULL);
		CHECK(strstr(ex.what(), "Result set for IS::f()") != NULL);
		CHECK(strstr(ex.what(), "Expectation for IS::g(1,2)") != NULL);
		CHECK(strstr(ex.what(), "Result set for IS::g(2,3)") != NULL);
		CHECK(strstr(ex.what(), "Result set for IS::g(3,4)") != NULL);
		CHECK(strstr(ex.what(), "Result set for IS::h()") != NULL);
		CHECK(strstr(ex.what(), __FILE__) != NULL);
		exceptionCaught = true;
	}
	mocks.reset();
	CHECK(exceptionCaught);
}

TEST(checkCallMissingExceptionToContainInfo)
{
	bool exceptionCaught = false;
	try
	{
		MockRepository mocks;
		mocks.autoExpect = false;
		IS *ismock = mocks.Mock<IS>();
		mocks.ExpectCall(ismock, IS::f).Return(1);
		mocks.OnCall(ismock, IS::f).Return(2);
		mocks.ExpectCall(ismock, IS::g).With(1,2).Return(2);
		mocks.OnCall(ismock, IS::g).Return(3);
		mocks.NeverCall(ismock, IS::g).With(3,4);
	}
	catch (HippoMocks::CallMissingException &ex)
	{
		CHECK(strstr(ex.what(), "Expectation for IS::f()") != NULL);
		CHECK(strstr(ex.what(), "Result set for IS::f()") != NULL);
		CHECK(strstr(ex.what(), "Expectation for IS::g(1,2)") != NULL);
		CHECK(strstr(ex.what(), "Result set for IS::g(...)") != NULL);
		CHECK(strstr(ex.what(), "Result set for IS::g(3,4)") != NULL);
		CHECK(strstr(ex.what(), __FILE__) != NULL);
		exceptionCaught = true;
	}
	CHECK(exceptionCaught);
}

