#include "Framework.h"
#include <hippomocks.h>

#ifdef _MSC_VER

#include <ComSupport.h>
#include <atlbase.h>
#include <atlcom.h>


namespace ComSupportTests
{

class ICom 
{
public:
	virtual ~ICom() {}
	virtual long __stdcall A(void) = 0;
	virtual long __stdcall B(int) = 0;
	virtual long __stdcall C(int, int) = 0;
	virtual long __stdcall D(int, int, int) = 0;
	virtual long __stdcall E(int, int, int, int) = 0;
	virtual long __stdcall F(int, int, int, int, int) = 0;
	virtual long __stdcall G(int, int, int, int, int, int) = 0;
	virtual long __stdcall H(int, int, int, int, int, int, int) = 0;
	virtual long __stdcall I(int, int, int, int, int, int, int, int) = 0;
	virtual long __stdcall J(int, int, int, int, int, int, int, int, int ) = 0;
	virtual long __stdcall K(int, int, int, int, int, int, int, int, int, int) = 0;
	virtual long __stdcall L(int, int, int, int, int, int, int, int, int, int, int) = 0;
	virtual long __stdcall M(int, int, int, int, int, int, int, int, int, int, int, int) = 0;
	virtual long __stdcall N(int, int, int, int, int, int, int, int, int, int, int, int, int) = 0;
	virtual long __stdcall O(int, int, int, int, int, int, int, int, int, int, int, int, int, int) = 0;
	virtual long __stdcall P(int, int, int, int, int, int, int, int, int, int, int, int, int, int, int) = 0;
};


TEST(checkStdCallBase)
{
	MockRepository mocks;

	ICom* ic = mocks.Mock<ICom>();
	mocks.ExpectCall(ic, ICom::A)
		.Return(1);

	long actual = ic->A();
	EQUALS(1, actual);
}

TEST(checkStdCallTemplateWithArgumentsImplementations)
{
	MockRepository mocks;

	ICom* ic = mocks.Mock<ICom>();

	mocks.ExpectCall(ic, ICom::B)
		.Return(2);
	mocks.ExpectCall(ic, ICom::C)
		.Return(3);
	mocks.ExpectCall(ic, ICom::D)
		.Return(4);
	mocks.ExpectCall(ic, ICom::E)
		.Return(5);
	mocks.ExpectCall(ic, ICom::F)
		.Return(6);
	mocks.ExpectCall(ic, ICom::G)
		.Return(7);
	mocks.ExpectCall(ic, ICom::H)
		.Return(8);
	mocks.ExpectCall(ic, ICom::I)
		.Return(9);
	mocks.ExpectCall(ic, ICom::J)
		.Return(10);
	mocks.ExpectCall(ic, ICom::K)
		.Return(11);
	mocks.ExpectCall(ic, ICom::L)
		.Return(12);
	mocks.ExpectCall(ic, ICom::M)
		.Return(13);
	mocks.ExpectCall(ic, ICom::N)
		.Return(14);
	mocks.ExpectCall(ic, ICom::O)
		.Return(15);
	mocks.ExpectCall(ic, ICom::P)
		.Return(16);

	ic->B(1);
	ic->C(1,2);
	ic->D(1,2,3);
	ic->E(1,2,3,4);
	ic->F(1,2,3,4,5);
	ic->G(1,2,3,4,5,6);
	ic->H(1,2,3,4,5,6,7);
	ic->I(1,2,3,4,5,6,7,8);
	ic->J(1,2,3,4,5,6,7,8,9);
	ic->K(1,2,3,4,5,6,7,8,9,10);
	ic->L(1,2,3,4,5,6,7,8,9,10,11);
	ic->M(1,2,3,4,5,6,7,8,9,10,11,12);
	ic->N(1,2,3,4,5,6,7,8,9,10,11,12,13);
	ic->O(1,2,3,4,5,6,7,8,9,10,11,12,13,14);
	ic->P(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15);
}

TEST(checkUnexpectedMethodCallThrowsException)
{
	//MockRepository mocks;

	//ICom* ic = mocks.Mock<ICom>();

	//ic->A();
}


template <int s>
int virtual_function_index(unsigned char* pb)
{
	if (*pb == 0xE9)
	{
		return virtual_function_index<0>(pb + 5 + *(unsigned int *)(pb+1));
	}
	if (!pb) return -1;
	int pboff = -1;
	if (pb[0] == 0x8b && pb[1] == 0x01) 
	{	//mov eax, [ecx]
		pboff = 2;
	}
	else if (pb[0] == 0x8b && pb[1] == 0x44 && pb[2] == 0x24 && pb[3] == 0x04 &&		//mov eax, [esp+arg0]
		pb[4] == 0x8b && pb[5] == 0x00) //mov eax, [eax]
	{										
		pboff = 6;
	}

	if (pboff > 0) 
	{
		if (pb[pboff] == 0xff) 
		{
			switch (pb[pboff + 1]) 
			{
			case 0x20:	//jmp dword ptr [eax]
				return 0;
			case 0x60:	//jmp dword ptr [eax+0xNN]
				return (((int)pb[pboff + 2]) & 0xff) / 4;
			case 0xa0:	//jmp dword ptr [eax+0xNNN]
				return (*(unsigned int*)(pb + (pboff + 2))) / 4;
			default:
				break;
			}
		}
	}
	return -1;
}

template <class T>
int VirtualFIndex(T func)
{
	union {
		T pfn;
		unsigned char* pb;
	};
	pfn = func;
	return virtual_function_index<0>(pb);
}

struct AA
{
	virtual int Get1() = 0;
	virtual int Get2() {}
};

//class with only 1 virtual method
class IAA {
public:
	virtual int f(int a);
};


MIDL_INTERFACE("4745C05E-23E6-4c6d-B9F2-E483359A8B89")
COMInterface1 : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE getTObjectCount( 
        /* [out] */ unsigned long *pCount) = 0;
};
 
typedef GUID ESTypeID;

MIDL_INTERFACE("356D44D9-980A-4149-A586-C5CB8B191437")
COMInterface2 : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE getMappablePackages( 
        /* [out] */ long *pSize,
        /* [size_is][size_is][out] */ ESTypeID **pIdList) = 0;
};
#if 0
TEST(EnsureThat_VirtualFunctionIndexGenericWorksforAllCases)
{
	EQUALS(1, VirtualFIndex(&AA::Get2));
	EQUALS(0, VirtualFIndex(&AA::Get1));
	EQUALS(0, VirtualFIndex(&IAA::f));
		
	EQUALS(0, VirtualFIndex(&IUnknown::QueryInterface));
	EQUALS(1, VirtualFIndex(&IUnknown::AddRef));
	EQUALS(2, VirtualFIndex(&IUnknown::Release));
		
	EQUALS(0, VirtualFIndex(&COMInterface1::QueryInterface));
	EQUALS(1, VirtualFIndex(&COMInterface1::AddRef));
	EQUALS(2, VirtualFIndex(&COMInterface1::Release));
}
#endif

TEST(CheckThat_AddCommExpectations_Stubs_QueryInterface_AddRef_Release)
{
	MockRepository mocks; 
	COMInterface1* deviceMock = mocks.Mock<COMInterface1>();

	AddComExpectations(mocks, deviceMock);
			
	{
		CComPtr<IUnknown> pUnk = deviceMock; 
		CComQIPtr<COMInterface1> pDevice = pUnk;

		CHECK(pDevice == pUnk);

		IUnknown* p = NULL;
		pDevice->QueryInterface(__uuidof(IUnknown), (void**)&p);

		CHECK(p == deviceMock);

	}
}

TEST(CheckThat_ConnectComInterfaces_Stubs_QueryInterface_ToEachOther)
{
	MockRepository mocks; 
	COMInterface1* deviceMock = mocks.Mock<COMInterface1>();
	COMInterface2* devMappingMock = mocks.Mock<COMInterface2>();

	ConnectComInterfaces(mocks, deviceMock, devMappingMock);

	{
		//Com objects can reach each other
		CComQIPtr<COMInterface2> pDevMapping = deviceMock;
			
		CHECK(pDevMapping != NULL);
		CHECK(pDevMapping == devMappingMock);

		CComQIPtr<COMInterface1> pDevNavigate = devMappingMock;

		CHECK(pDevNavigate != NULL);
		CHECK(pDevNavigate == deviceMock);
	}

}
} // end of namespace

#endif
