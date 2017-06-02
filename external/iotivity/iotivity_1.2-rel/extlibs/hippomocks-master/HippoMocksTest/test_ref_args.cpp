#include "hippomocks.h"
#include "Framework.h"

using HippoMocks::byRef;

class IRefArg {
public:
	virtual void test() = 0;
};

class IK {
public:
	virtual ~IK() {}
	virtual void f(int &);
	virtual void g(int &) = 0;
	virtual int &h() = 0;
	virtual const std::string &k() = 0;
	virtual void l(IRefArg &refArg) { refArg.test();}
};

TEST (checkRefArgumentsAccepted)
{
	MockRepository mocks;
	IK *iamock = mocks.Mock<IK>();
	int x = 42;
	mocks.ExpectCall(iamock, IK::f).With(x);
	mocks.ExpectCall(iamock, IK::g).With(x);
	iamock->f(x);
	iamock->g(x);
}


TEST (checkRefArgumentsChecked)
{
	MockRepository mocks;
	IK *iamock = mocks.Mock<IK>();
	int x = 1, y = 2;
	mocks.ExpectCall(iamock, IK::f).With(x);
	mocks.ExpectCall(iamock, IK::g).With(y);
	bool exceptionCaught = false;
	try 
	{
		iamock->f(y);
	}
	catch (HippoMocks::ExpectationException)
	{
		exceptionCaught = true;
	}
	CHECK(exceptionCaught);
	mocks.reset();
}

void plusplus(int &x) 
{ 
	x++; 
}
void plusequals2(int &x) 
{
	x+=2; 
}

TEST (checkRefArgumentsPassedAsRef)
{
	MockRepository mocks;
	IK *iamock = mocks.Mock<IK>();
	int x = 1, y = 2;
	mocks.ExpectCall(iamock, IK::f).Do(plusplus);
	mocks.ExpectCall(iamock, IK::g).Do(plusequals2);
	iamock->f(x);
	iamock->g(y);
	CHECK(x == 2);
	CHECK(y == 4);
}

TEST (checkRefReturnValues)
{
	MockRepository mocks;
	IK *iamock = mocks.Mock<IK>();
	int x = 0;
	mocks.ExpectCall(iamock, IK::h).Return(x);
	mocks.ExpectCall(iamock, IK::k).Return("Hello World");
	iamock->h() = 1;
	EQUALS(iamock->k(), "Hello World");
	EQUALS(x, 1);
}

bool operator==(const IRefArg &a, const IRefArg &b)
{
	return (&a == &b);
}

TEST (checkRefArgCheckedAsReference)
{
	MockRepository mocks;
	IK *iamock = mocks.Mock<IK>();
	IRefArg *refArg = mocks.Mock<IRefArg>();

	mocks.ExpectCall(iamock, IK::l).With(byRef(*refArg));
	iamock->l(*refArg);
}

class IB {
public:
   virtual ~IB() {}

   virtual void doSomething() const = 0;

};

class IA {
public:
   virtual ~IA() {}

   virtual const IB & getB() const = 0;
   
};

TEST (checkRefReturnAsReference)
{
   MockRepository mocks;

   IB * b = mocks.Mock<IB>();

   IA * a = mocks.Mock<IA>();

   mocks.OnCall(a, IA::getB).ReturnByRef(*b);
   mocks.ExpectCall(b, IB::doSomething);

   CHECK(b == &a->getB());
   a->getB().doSomething();

   mocks.VerifyAll();
}


