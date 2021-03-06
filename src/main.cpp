#include "stdafx.h"
#include "CVernierUI.h"

class CFrameWnd:public WindowImplBase
{
public:
	CFrameWnd() {};
	LPCTSTR GetWindowClassName() const {return _T("UIFrame");};
	UINT GetClassStyle() const {return UI_CLASSSTYLE_DIALOG;};
	virtual CDuiString GetSkinFile() {return _T("hello_world.xml");}
	virtual CDuiString GetSkinFolder() {return _T("");}
	void OnFinalMessage(HWND /*hWnd*/) {delete this;};
	/*自定义控件回调函数*/
	CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if(_tcsicmp(pstrClass,_T("Vernier"))==0)
			return new CVernierUI();
		return NULL;
	}
	void Init()
	{
		m_pMinBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("minbtn")));
		m_pMaxBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("maxbtn")));
		m_pRestoreBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("restorebtn")));
		m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closebtn")));
	}
	/*
	窗口创建消息响应函数
	*/
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		m_PaintManager.Init(m_hWnd);
		CDialogBuilder builder;
		CControlUI* pRoot = builder.Create(_T("hello_world.xml"), (UINT)0, this, &m_PaintManager);
		ASSERT(pRoot && "Failed to parse XML");
		m_PaintManager.AttachDialog(pRoot);
		m_PaintManager.AddNotifier(this);//添加消息响应，在Notify函数处理消息

		Init();
		return 0;
	}
	/*
	退出窗口消息响应函数
	*/
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		::PostQuitMessage(0L);
		bHandled = FALSE;
		return 0;
	}
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        if( ::IsIconic(*this) ) bHandled = FALSE;
        return (wParam == 0) ? TRUE : FALSE;
	}
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 0;
	}
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        /*有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND*/
        if( wParam == SC_CLOSE ) {
            ::PostQuitMessage(0L);
            bHandled = TRUE;
            return 0;
        }
		BOOL bZoomed = ::IsZoomed(*this);
		LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
		if( ::IsZoomed(*this) != bZoomed )
		{
			if( !bZoomed )
			{
				CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(false);
				pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(true);
			}
			else
			{
				CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
				if( pControl ) pControl->SetVisible(true);
				pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
				if( pControl ) pControl->SetVisible(false);
			}
		}
        return lRes;
	}
    LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
        POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
        ::ScreenToClient(*this, &pt);

        RECT rcClient;
        ::GetClientRect(*this, &rcClient);

        if( !::IsZoomed(*this) ) {
            RECT rcSizeBox = m_PaintManager.GetSizeBox();
            if( pt.y < rcClient.top + rcSizeBox.top ) {
                if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
                if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
                return HTTOP;
            }
            else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
                if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
                if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
                return HTBOTTOM;
            }
            if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
        }

        RECT rcCaption = m_PaintManager.GetCaptionRect();
        if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
            && pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
                CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
                if( pControl && _tcscmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
                    _tcscmp(pControl->GetClass(), _T("OptionUI")) != 0 &&
                    _tcscmp(pControl->GetClass(), _T("TextUI")) != 0 )
                    return HTCAPTION;
        }

        return HTCLIENT;
    }
	/*
	控件的事件处理函数
	*/
	void Notify(TNotifyUI&msg)
	{
		if(msg.sType==_T("click"))
		{
			if(msg.pSender->GetName()==_T("btnHello"))
			{
				::MessageBox(NULL,_T("button click!"),_T("tips"),NULL);
			}
			else if(msg.pSender->GetName()==_T("minbtn"))
			{
				SendMessage(WM_SYSCOMMAND,SC_MINIMIZE,0);
				return;
			}
			else if(msg.pSender->GetName()==_T("maxbtn"))
			{
				SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE, 0);
				return;
			}
			else if(msg.pSender->GetName()==_T("restorebtn"))
			{
				SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
				return;
			}
			else if(msg.pSender->GetName()==_T("closebtn"))
			{
				PostQuitMessage(0L);
				return;
			}
		}
		if(msg.sType==_T("dropdown"))
		{
			if(msg.pSender->GetName()==_T("btnvernier"))
			{
				//::MessageBox(NULL,_T("set threshold"),_T("tips"),NULL);
			}
		}
	}
	/*
	消息处理
	*/
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
	{
		LRESULT lRes = 0;
		BOOL bHandled = TRUE;
		switch (uMsg)
		{
		case WM_CREATE: lRes = OnCreate(uMsg,wParam,lParam,bHandled); break;
		case WM_DESTROY: lRes = OnDestroy(uMsg,wParam,lParam,bHandled); break;
		case WM_SYSCOMMAND: lRes = OnSysCommand(uMsg,wParam,lParam,bHandled); break;
		/*以下三个消息用于屏蔽系统标题栏*/
		case WM_NCACTIVATE: lRes = OnNcActivate(uMsg,wParam,lParam,bHandled); break;
		case WM_NCCALCSIZE: lRes = OnNcCalcSize(uMsg,wParam,lParam,bHandled); break;
		case WM_NCPAINT: lRes = OnNcPaint(uMsg,wParam,lParam,bHandled); break;
		case WM_NCHITTEST: lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
		default: bHandled=FALSE;
		}
		if(bHandled) return lRes;

		if(m_PaintManager.MessageHandler(uMsg,wParam,lParam,lRes))
		{
			return lRes;
		}
		return CWindowWnd::HandleMessage(uMsg,wParam,lParam);
	}
public:
	CPaintManagerUI m_PaintManager;
private:
		CButtonUI* m_pMinBtn;
		CButtonUI* m_pMaxBtn;
		CButtonUI* m_pRestoreBtn;
		CButtonUI* m_pCloseBtn;
};
/*
程序入口
*/
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE/*hPrevInstance*/,LPSTR/*lpCmdLine*/,int nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));
	
	HRESULT Hr=::CoInitialize(NULL);
	if(FAILED(Hr)) return 0;
	
	CFrameWnd* pFrame = new CFrameWnd();
	if(pFrame ==NULL) return 0;
	pFrame->Create(NULL,_T("Hello World"),UI_WNDSTYLE_FRAME,0);
	pFrame->CenterWindow();
	pFrame->ShowWindow(TRUE);
	CPaintManagerUI::MessageLoop();
	
	::CoUninitialize();
	return 0;
}