
#include "MyComInterface.h"


class MyComObject : public IMyComInterface2 {
	ULONG refCount = 1;

public:
	// IUnknown
	STDMETHOD(QueryInterface)(REFIID riid, void** ppv) override {

		OutputDebugString(L"MyComObject::QueryInterface called\n");

		if (riid == __uuidof(IUnknown))         *ppv = static_cast<IMyComInterface2*>(this);
		else if (riid == __uuidof(IMyComInterface)) *ppv = static_cast<IMyComInterface*>(this);
		else if (riid == __uuidof(IMyComInterface2)) *ppv = static_cast<IMyComInterface2*>(this);
		else return E_NOINTERFACE;
		AddRef();
		return S_OK;
	}

	STDMETHOD_(ULONG, AddRef)() override { return ++refCount; }
	STDMETHOD_(ULONG, Release)() override {
		ULONG r = --refCount;
		if (r == 0) delete this;
		return r;
	}

	// IMyInterface
	void Hello() override {
		MessageBoxA(nullptr, "Hello from v1", "COM", MB_OK);
	}

	// IMyInterface2
	void Goodbye() override {
		MessageBoxA(nullptr, "Goodbye from v2", "COM", MB_OK);
	}
};
