#pragma once
#include "MyComInterface.h"
#include "MyComObject.h"

// IClassFactory ����
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
			return CLASS_E_NOAGGREGATION; // �� ������ Aggregation�� �������� ����

		MyComObject* pObj = new (std::nothrow) MyComObject();
		if (!pObj)
			return E_OUTOFMEMORY;

		HRESULT hr = pObj->QueryInterface(riid, ppv);
		pObj->Release(); // ���� �� ���� refcount 2 �� ���⼭ 1 ����
		return hr;
	}

	STDMETHOD(LockServer)(BOOL) { return S_OK; }
};

// COM ��Ÿ�ӿ��� Ŭ���� ��û�� �� ȣ���
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
	factory->Release();  // QueryInterface ���� �� ���� AddRef ����, ���� �� delete��

	return hr;
}