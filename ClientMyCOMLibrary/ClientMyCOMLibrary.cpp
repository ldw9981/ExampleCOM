// ClientMyCOMLibrary.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

#include "../MyCOMLibrary/MyComInterface.h"
#include <objbase.h>

int main() {
	CoInitialize(nullptr);

	IMyComInterface2* p = nullptr;
	// COM 라이브러리를 사용하기위해 필요한것 
	// 1. 레지스트리에 등록된 ClassID (DLL위치가 저장됨) 
	// 2. 라이브러리 클래스의 인스턴스를 사용하기위한 인터페이스
	HRESULT hr = CoCreateInstance(CLSID_MyComObject, nullptr, CLSCTX_INPROC_SERVER, __uuidof(IMyComInterface2), (void**)&p);
	if (SUCCEEDED(hr)) {
		p->Hello();
		p->Goodbye();
		p->Release();
	}

	CoUninitialize();
	return 0;
}