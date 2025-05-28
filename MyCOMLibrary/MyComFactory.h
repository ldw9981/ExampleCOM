#pragma once
#include "MyComInterface.h"
#include "MyComObject.h"

// IClassFactory 구현
class MyComFactory : public IClassFactory {
	ULONG ref = 1;

public:
	MyComFactory() {
		OutputDebugString(L"MyComFactory created\n");
	}
	virtual ~MyComFactory() {
		OutputDebugString(L"MyComFactory destroyed\n");
	}
public:
	STDMETHOD(QueryInterface)(REFIID riid, void** ppv) {
		if (riid == IID_IUnknown || riid == IID_IClassFactory) {
			*ppv = static_cast<IClassFactory*>(this);
			AddRef();
			return S_OK;
		}
		return E_NOINTERFACE;
	}

	STDMETHOD_(ULONG, AddRef)() { return ++ref; }
	STDMETHOD_(ULONG, Release)() { if (--ref == 0) delete this; return ref; }

	STDMETHOD(CreateInstance)(IUnknown* pUnkOuter, REFIID riid, void** ppv) override {
		if (ppv == nullptr)
			return E_POINTER;
		*ppv = nullptr;

		if (pUnkOuter != nullptr)
			return CLASS_E_NOAGGREGATION; // 이 예제는 Aggregation을 지원하지 않음

		MyComObject* pObj = new (std::nothrow) MyComObject();
		if (!pObj)
			return E_OUTOFMEMORY;

		HRESULT hr = pObj->QueryInterface(riid, ppv);
		pObj->Release(); // 성공 시 내부 refcount 2 → 여기서 1 줄임
		return hr;
	}

	STDMETHOD(LockServer)(BOOL) { return S_OK; }
};

// COM 런타임에서 클래스 요청할 때 호출됨
extern "C" HRESULT __stdcall DllGetClassObject(REFCLSID clsid, REFIID iid, void** ppv)
{
	OutputDebugStringW(L"DllGetClassObject called\n");

	if (ppv == nullptr)
		return E_POINTER;

	*ppv = nullptr;

	if (clsid != CLSID_MyComObject)
		return CLASS_E_CLASSNOTAVAILABLE;

	MyComFactory* factory = new (std::nothrow) MyComFactory();
	if (!factory)
		return E_OUTOFMEMORY;

	HRESULT hr = factory->QueryInterface(iid, ppv);
	factory->Release();  // QueryInterface 성공 시 내부 AddRef 증가, 실패 시 delete됨

	return hr;
}