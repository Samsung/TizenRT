#include "Framework.h"
#include "hippomocks.h"

class IR { 
public:
	virtual ~IR() {}
	virtual void f0() {}
	virtual void f1(int) {}
	virtual void f2(int, int) {}
	virtual void f3(int, int, int) {}
	virtual void f4(int, int, int, int) {}
	virtual void f5(int, int, int, int, int) {}
	virtual void f6(int, int, int, int, int, int) {}
	virtual void f7(int, int, int, int, int, int, int) {}
	virtual void f8(int, int, int, int, int, int, int, int) {}
	virtual void f9(int, int, int, int, int, int, int, int, int) {}
	virtual void f10(int, int, int, int, int, int, int, int, int, int) {}
	virtual void f11(int, int, int, int, int, int, int, int, int, int, int) {}
	virtual void f12(int, int, int, int, int, int, int, int, int, int, int, int) {}
	virtual void f13(int, int, int, int, int, int, int, int, int, int, int, int, int) {}
	virtual void f14(int, int, int, int, int, int, int, int, int, int, int, int, int, int) {}
	virtual void f15(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) {}
	virtual void f16(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) {}
};

TEST (checkArgCount16SupportedAndWorking)
{
	MockRepository mocks;
	IR *iamock = mocks.Mock<IR>();
	mocks.ExpectCall(iamock, IR::f0);
	mocks.ExpectCall(iamock, IR::f1).With(0);
	mocks.ExpectCall(iamock, IR::f2).With(1,2);
	mocks.ExpectCall(iamock, IR::f3).With(3,4,5);
	mocks.ExpectCall(iamock, IR::f4).With(6,7,8,9);
	mocks.ExpectCall(iamock, IR::f5).With(10,11,12,13,14);
	mocks.ExpectCall(iamock, IR::f6).With(15,16,17,18,19,20);
	mocks.ExpectCall(iamock, IR::f7).With(21,22,23,24,25,26,27);
	mocks.ExpectCall(iamock, IR::f8).With(28,29,30,31,32,33,34,35);
	mocks.ExpectCall(iamock, IR::f9).With(36,37,38,39,40,41,42,43,44);
	mocks.ExpectCall(iamock, IR::f10).With(45,46,47,48,49,50,51,52,53,54);
	mocks.ExpectCall(iamock, IR::f11).With(55,56,57,58,59,60,61,62,63,64,65);
	mocks.ExpectCall(iamock, IR::f12).With(66,67,68,69,70,71,72,73,74,75,76,77);
	mocks.ExpectCall(iamock, IR::f13).With(78,79,80,81,82,83,84,85,86,87,88,89,90);
	mocks.ExpectCall(iamock, IR::f14).With(91,92,93,94,95,96,97,98,99,100,101,102,103,104);
	mocks.ExpectCall(iamock, IR::f15).With(105,106,107,108,109,110,111,112,113,114,115,116,117,118,119);
	mocks.ExpectCall(iamock, IR::f16).With(120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135);
	iamock->f0();
	iamock->f1(0);
	iamock->f2(1,2);
	iamock->f3(3,4,5);
	iamock->f4(6,7,8,9);
	iamock->f5(10,11,12,13,14);
	iamock->f6(15,16,17,18,19,20);
	iamock->f7(21,22,23,24,25,26,27);
	iamock->f8(28,29,30,31,32,33,34,35);
	iamock->f9(36,37,38,39,40,41,42,43,44);
	iamock->f10(45,46,47,48,49,50,51,52,53,54);
	iamock->f11(55,56,57,58,59,60,61,62,63,64,65);
	iamock->f12(66,67,68,69,70,71,72,73,74,75,76,77);
	iamock->f13(78,79,80,81,82,83,84,85,86,87,88,89,90);
	iamock->f14(91,92,93,94,95,96,97,98,99,100,101,102,103,104);
	iamock->f15(105,106,107,108,109,110,111,112,113,114,115,116,117,118,119);
	iamock->f16(120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135);
}

