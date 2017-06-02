#include <string>
#include "hippomocks.h"
#include "Framework.h"

class IInParam { 
public:
	virtual ~IInParam() {}
	virtual void a(const std::string& in) = 0;
	virtual void b(std::string* in) = 0;
	virtual void c(const char* in) = 0;
};

TEST (checkInParamsAreFilledIn_ConstChar)
{
	MockRepository mocks;
	IInParam *iamock = mocks.Mock<IInParam>();
	std::string teststring;
	mocks.ExpectCall(iamock, IInParam::a).With(In(teststring));
	
	iamock->a("Hello World");

	CHECK(teststring == "Hello World");
}

TEST (checkInParamsAreFilledIn_String)
{
	MockRepository mocks;
	IInParam *iamock = mocks.Mock<IInParam>();
	std::string teststring;
	mocks.ExpectCall(iamock, IInParam::a).With(In(teststring));
	
	std::string in("Hello World");
	iamock->a(in);

	CHECK(teststring == in);
}

TEST (checkInParamsAreFilledIn_PointerToString)
{
	MockRepository mocks;
	IInParam *iamock = mocks.Mock<IInParam>();
	std::string* teststring = NULL;
	mocks.ExpectCall(iamock, IInParam::b).With(In(teststring));
	
	std::string in("Hello World");
	iamock->b(&in);

	CHECK(teststring == &in);
}

TEST (checkInParamsAreFilledIn_Char)
{
	MockRepository mocks;
	IInParam *iamock = mocks.Mock<IInParam>();
	const char* teststring = NULL;
	mocks.ExpectCall(iamock, IInParam::c).With(In(teststring));
	
	const char* in = "Hello World";
	iamock->c(in);

	CHECK(strcmp(teststring, in) == 0);
}
