#include "hippomocks.h"
#include "Framework.h"
#include <string>

class IT {
public:
	std::string text;
	virtual std::string getText() { return text; }
};

TEST (checkMemberWorks)
{
	MockRepository mocks;
	IT *iamock = mocks.Mock<IT>();
	mocks.Member(iamock, &IT::text);
	iamock->text = "helloworld";
	EQUALS("helloworld", iamock->text);
}

