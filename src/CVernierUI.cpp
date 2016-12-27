#include "stdafx.h"
#include "CVernierUI.h"

CVernierUI::CVernierUI()
{
}
CVernierUI::~CVernierUI()
{
}
LPCTSTR CVernierUI::GetClass() const
{
	return _T("VernierUI");
}
LPVOID CVernierUI::GetInterface(LPCTSTR pstrName)
{
	/*xml控件的名称*/
	if(_tcscmp(pstrName,_T("Vernier"))==0)
		return static_cast<CVernierUI*>(this);
	return CButtonUI::GetInterface(pstrName);
}

void CVernierUI::SetAttibute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if(_tcscmp(pstrName,_T("textEx"))==0)
	{
		/**/
	}
	CButtonUI::SetAttribute(pstrName,pstrValue);
}
BOOL CVernierUI::IsEnable()
{
	return TRUE;
}
void CVernierUI::DoEvent(TEventUI& event)
{
	if(event.Type==UIEVENT_BUTTONDOWN && IsEnable())
	{
		if(::PtInRect(&m_rcItem,event.ptMouse))
		{
			m_uButtonState |= UISTATE_CAPTURED;
			m_ptLastMouse=event.ptMouse;
			m_rcNewPos=m_rcItem;
			if(m_pManager)
				m_pManager->AddPostPaint(this);
			return;
		}
	}
	if(event.Type==UIEVENT_BUTTONUP)
	{
		if((m_uButtonState&UISTATE_CAPTURED)!=0)
		{
			m_uButtonState&=~UISTATE_CAPTURED;
			CVernierUI* pParent = static_cast<CVernierUI*>(m_pParent);
			pParent->NotifyDrag(this,m_rcNewPos);
			if(m_pManager)
			{
				m_pManager->RemovePostPaint(this);
				m_pManager->Invalidate(m_rcNewPos);
			}
			NeedParentUpdate();
			m_pManager->SendNotify(this,DUI_MSGTYPE_DROPDOWN);
			return;
		}
	}
	if(event.Type==UIEVENT_MOUSEMOVE)
	{
		if((m_uButtonState&UISTATE_CAPTURED)!=0)
		{
			LONG cx=event.ptMouse.x-m_ptLastMouse.x;
			LONG cy=event.ptMouse.y-m_ptLastMouse.y;
			m_ptLastMouse=event.ptMouse;
			RECT rcCurPos=m_rcNewPos;
			rcCurPos.left+=cx;
			rcCurPos.right+=cx;
			rcCurPos.top+=cy;
			rcCurPos.bottom+=cy;
			CDuiRect rcInvalidate=m_rcNewPos;
			/*边界*/
			/*
			CDuiRect rcParent=m_pParent->GetPos();
			rcCurPos.left = rcCurPos.left<rcParent.left?rcParent.left:rcCurPos.left;
			rcCurPos.top=rcCurPos.top<rcParent.top?rcParent.top:rcCurPos.top;
			rcCurPos.right=rcCurPos.right>rcParent.right?rcParent.right:rcCurPos.right;
			rcCurPos.bottom=rcCurPos.bottom>rcParent.bottom?rcParent.bottom:rcCurPos.bottom;
			*/
			m_rcNewPos=rcCurPos;
			rcInvalidate.Join(m_rcNewPos);
			if(m_pManager)
				m_pManager->Invalidate(rcInvalidate);
			return;
		}
	}
	if(event.Type==UIEVENT_SETCURSOR)
	{
		if(IsEnabled())
		{
			::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(IDC_HAND)));
			return;
		}
	}
	CLabelUI::DoEvent(event);
}
void CVernierUI::DoPostPaint(HDC hDC, const RECT &rcPaint)
{
	if((m_uButtonState&UISTATE_CAPTURED)!=0)
	{
		CDuiRect rcParent=m_pParent->GetPos();
		RECT rcUpdate={0};
		rcUpdate.left=m_rcNewPos.left<rcParent.left?rcParent.left:m_rcNewPos.left;
		rcUpdate.top=m_rcNewPos.top<rcParent.top?rcParent.top:m_rcNewPos.top;
		rcUpdate.right=m_rcNewPos.right>rcParent.right?rcParent.right:m_rcNewPos.right;
		rcUpdate.bottom=m_rcNewPos.bottom>rcParent.bottom?rcParent.bottom:m_rcNewPos.bottom;
		//rcUpdate.left=100;
		//rcUpdate.top=100;
		//rcUpdate.right=200;
		//rcUpdate.bottom=200;
		//CRenderEngine::DrawColor(hDC,rcUpdate,0xAA000000);
		CRenderEngine::DrawRect(hDC,rcUpdate,1,RGB(255,0,0));
		//CRenderEngine::DrawLine(hDC,rcUpdate,1,RGB(0,0,255),PS_SOLID);
	}
}
void CVernierUI::NotifyDrag(CControlUI* pControl, RECT rcNewPos)
{
	//CDuiRect rcParent=m_pParent->GetPos();
	//RECT rcUpdate = {0};
	//rcUpdate.left = rcNewPos.left<rcParent.left?rcParent.left:rcNewPos.left;
	//rcUpdate.top = rcNewPos.top<rcParent.top?rcParent.top:rcNewPos.top;
	//rcUpdate.right=rcNewPos.right>rcParent.right?rcParent.right:rcNewPos.right;
	//rcUpdate.bottom=rcNewPos.bottom>rcParent.bottom?rcParent.bottom:rcNewPos.bottom;
	//rcUpdate.left = (rcNewPos.left<rcParent.left) ? rcParent.left : ((rcNewPos.left>rcParent.right)?rcParent.right:rcNewPos.left);
	//rcUpdate.top = (rcNewPos.top<rcParent.top) ? rcParent.top : ((rcNewPos.top>rcParent.bottom)?rcParent.bottom:rcNewPos.top);

	//rcUpdate.right = rcNewPos.right>rcParent.right?rcParent.right:rcNewPos.right;
	//rcUpdate.bottom=rcNewPos.bottom>rcParent.bottom?rcParent.bottom:rcNewPos.bottom;
	pControl->SetPos(rcNewPos);
}