#include "hippomocks.h"
#include "Framework.h"
#include <string>

using HippoMocks::byRef;

class IR {
public:
	virtual void func() = 0;
};

class R : public IR {
public:
	virtual void func() {}
};

class IM { 
public:
	virtual ~IM() {}
	virtual void d(std::string var) = 0;
	virtual void e(const std::string &var) = 0;
	virtual void f(const IR &arg) = 0;
};

TEST (checkTemporaryClassParam)
{
  MockRepository mocks;
	IM *iamock = mocks.Mock<IM>();
	mocks.ExpectCall(iamock, IM::d).With("Hello");
	iamock->d("Hello");
}

TEST (checkConstRefClassParam)
{
	MockRepository mocks;
	IM *iamock = mocks.Mock<IM>();
	mocks.ExpectCall(iamock, IM::e).With("Hello");
	iamock->e("Hello");
}

bool operator==(const IR &objA, const IR &objB)
{
	return &objA == &objB;
}

TEST (checkConstRefAbstractClassParam)
{
	R r;
	MockRepository mocks;
	IM *iamock = mocks.Mock<IM>();
	mocks.ExpectCall(iamock, IM::f).With(byRef(r));
	iamock->f(r);
}

