#include "hippomocks.h"
#include "Framework.h"

class ID {
public:
	virtual ~ID() {}
	virtual int f();
	virtual std::string g() = 0;
};

TEST (checkRetvalAccepted)
{
	MockRepository mocks;
	ID *iamock = mocks.Mock<ID>();
	mocks.ExpectCall(iamock, ID::f).Return(1);
	mocks.ExpectCall(iamock, ID::g).Return("fsck");
	iamock->f();
	iamock->g();
}

TEST (checkRetvalProper)
{
	MockRepository mocks;
	ID *iamock = mocks.Mock<ID>();
	mocks.ExpectCall(iamock, ID::f).Return(1);
	mocks.ExpectCall(iamock, ID::g).Return("fsck");
	CHECK(iamock->f() == 1);
	CHECK(iamock->g() == "fsck");
}

bool replace_g_called = false;
std::string replace_g() 
{
	replace_g_called = true;
	return "";
}

TEST (checkRetvalAfterDo)
{
	MockRepository mocks;
	ID *iamock = mocks.Mock<ID>();
	mocks.ExpectCall(iamock, ID::g).Do(replace_g).Return("fsck");
	replace_g_called = false;
	CHECK(iamock->g() == "fsck");
	CHECK(replace_g_called);
}


