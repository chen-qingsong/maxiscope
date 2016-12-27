#ifndef CVERNIERUI_H
#define CVERHIERUI_H
#include "stdafx.h"

using namespace DuiLib;

class CVernierUI:public CButtonUI
{
public:
	CVernierUI();
	~CVernierUI();
public:
	LPCTSTR GetClass() const;
	LPVOID GetInterface(LPCTSTR pstrName);
	void SetAttibute(LPCTSTR pstrName, LPCTSTR pstrValue);
	void DoEvent(TEventUI &event);
	BOOL IsEnable();
	void DoPostPaint(HDC hdc,const RECT &rcPaint);
	void NotifyDrag(CControlUI* pControl, RECT rcNewPos);
private:
	POINT m_ptLastMouse;
	RECT m_rcNewPos;
	CPaintManagerUI m_PaintManager;
};
#endif