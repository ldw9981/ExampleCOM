#pragma once
#include <Unknwn.h> // IUnknown

// v1 인터페이스
struct __declspec(uuid("4100C14D-4959-479C-97AD-1EEE5E006E3C"))
	IMyComInterface : public IUnknown {
	virtual void Hello() = 0;
};

// v2 인터페이스 (v1 확장)
struct __declspec(uuid("209E5F30-944D-47B7-97A5-C8100F8896FA"))
	IMyComInterface2 : public IMyComInterface {
	virtual void Goodbye() = 0;
};

// 클래스 ID 정의
static const CLSID CLSID_MyComObject =
{ 0xd0830308, 0x935b, 0x4e16, { 0x84, 0x4d, 0x38, 0x74, 0x33, 0x7e, 0xe3, 0xb4 } };