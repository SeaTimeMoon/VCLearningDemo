#include "stdafx.h"
#include "CCheckSK.h"

CCheckSK::CCheckSK()
{
    m_bCheck = FALSE;
    m_bMouseOver = FALSE;

    m_nLedSize = 15;

    //  set default color to green on ON and dark-green on OFF
    m_colLedOn  = RGB(0, 255, 0);
    m_colLedOff = RGB(127, 0, 0);
	m_lineClr = RGB(255,0,0);

    m_hIconOn.hIcon  = NULL;
    m_hIconOff.hIcon = NULL;
    m_tooltip.Create (this);
}

CCheckSK::~CCheckSK()
{
}


BEGIN_MESSAGE_MAP(CCheckSK, CButton)
	//{{AFX_MSG_MAP(CCheckSK)
	ON_WM_MOUSEMOVE()
	/*ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)*/
	//}}AFX_MSG_MAP
    ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(BM_SETCHECK, OnSetCheck)
	ON_MESSAGE(BM_GETCHECK, OnGetCheck)
END_MESSAGE_MAP()


//  ===========================================================================
//  Set check status of control
//  ===========================================================================
void
CCheckSK::SetCheck(BOOL bCheck, BOOL bRepaint)
{
    m_bCheck = bCheck;

    if (bRepaint) Invalidate();
}

//  ===========================================================================
//  Return check status of control
//  ===========================================================================
BOOL
CCheckSK::GetCheck()
{
    return m_bCheck;
}

//  ===========================================================================
//  Set Icon from resource ID
//  ===========================================================================
DWORD
CCheckSK::SetIcon(int nIconOn, int nIconOff)
{
    HICON       hIconOn         = NULL;
    HICON       hIconOff        = NULL;
    HINSTANCE   hInstResource   = NULL;
    
    //  Find correct resource handle
    hInstResource = AfxFindResourceHandle ( MAKEINTRESOURCE(nIconOn), 
                                            RT_GROUP_ICON);
    
    //  Set icon when the mouse is IN the button
    hIconOn = ::LoadIcon(hInstResource, MAKEINTRESOURCE(nIconOn));
    
    hIconOff = ::LoadIcon(hInstResource, MAKEINTRESOURCE(nIconOff));
    
    return SetIcon(hIconOn, hIconOff);
}

//  ===========================================================================
//  Set Icon from file
//  ===========================================================================
DWORD
CCheckSK::SetIcon(LPCTSTR lpszFileNameIn, LPCTSTR lpszFileNameOut)
{
    HICON hIconOn   = NULL;
    HICON hIconOff  = NULL;

    hIconOn  = (HICON)::LoadImage ( 0, lpszFileNameIn,  IMAGE_ICON, 0, 0, 
                                    LR_LOADFROMFILE);
    hIconOff = (HICON)::LoadImage ( 0, lpszFileNameOut, IMAGE_ICON, 0, 0,
                                    LR_LOADFROMFILE);

    return SetIcon(hIconOn, hIconOff);
}

//  ===========================================================================
//  Set bitmap from handle to open bitmap. This is ultimtely called by the 
//  SetIcon for file and resource ID
//  ===========================================================================
DWORD
CCheckSK::SetIcon(HICON hIconOn, HICON hIconOff)
{
    BOOL      bRetValue;
    ICONINFO  ii;
    
    // Free any loaded resource
    FreeResources();
    
    //  =======================================================================
    //  Load icon for check box ON state
    //  =======================================================================
    if (hIconOn)
    {
        //  set the icon when mouse over button?
        m_hIconOn.hIcon = hIconOn;

        //  Get icon dimension
        ::ZeroMemory(&ii, sizeof(ICONINFO));
        bRetValue = ::GetIconInfo(hIconOn, &ii);
        if (bRetValue == FALSE)
        {
            FreeResources();
            return ICNST_INVALIDRESOURCE;
        }
        //  hotspot is always defined to be the center of the icon
        m_hIconOn.dwWidth	= (DWORD)(ii.xHotspot * 2);
        m_hIconOn.dwHeight	= (DWORD)(ii.yHotspot * 2);

        ::DeleteObject(ii.hbmMask);
        ::DeleteObject(ii.hbmColor);
    }
    
    //  =======================================================================
    //  Load icon for check box OFF state
    //  =======================================================================
    if (hIconOff)
    {
        m_hIconOff.hIcon = hIconOff;
            
        //  Get icon dimension
        ::ZeroMemory(&ii, sizeof(ICONINFO));
        bRetValue = ::GetIconInfo(hIconOff, &ii);
        if (bRetValue == FALSE)
        {
            FreeResources();
            return ICNST_INVALIDRESOURCE;
        }

        m_hIconOff.dwWidth	= (DWORD)(ii.xHotspot * 2);
        m_hIconOff.dwHeight	= (DWORD)(ii.yHotspot * 2);
        ::DeleteObject(ii.hbmMask);
        ::DeleteObject(ii.hbmColor);
    }
    Invalidate();
    
    return 1;
}

//  ...........................................................................
//  This method is called when mouse leaves the control. See _TrackMouseEvent
//  ...........................................................................
LRESULT
CCheckSK::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
    CancelHover();
    return 0;
}

//  ...........................................................................
//  BM_SETCHECK message handler
//  ...........................................................................
LRESULT
CCheckSK::OnSetCheck(WPARAM wParam, LPARAM lParam)
{
    switch (wParam)
    {
    case BST_CHECKED:
    case BST_INDETERMINATE:	// Indeterminate state is handled like checked state
        SetCheck(1);
        break;
    default:
        SetCheck(0);
        break;
    }
    return 0;
}

//  ...........................................................................
//  BM_GETCHECK message handler
//  ...........................................................................
LRESULT
CCheckSK::OnGetCheck(WPARAM wParam, LPARAM lParam)
{
	return GetCheck();
}

//  ...........................................................................
//  Change control style in this function
//  ...........................................................................
void
CCheckSK::PreSubclassWindow() 
{
    UINT nBS = GetButtonStyle();
    
    //  the button should not be owner draw
    ASSERT((nBS & SS_TYPEMASK) != BS_OWNERDRAW);

    //  This class supports only check boxes
    ASSERT(nBS & BS_CHECKBOX);
    
    // Switch to owner-draw
    ModifyStyle(SS_TYPEMASK, BS_OWNERDRAW, SWP_FRAMECHANGED);
    
    m_nStyle = GetWindowLong(GetSafeHwnd(), GWL_STYLE);

    CButton::PreSubclassWindow();
}

//  ...........................................................................
//  This function is called when the control needs to be drawn
//  ...........................................................................
void
CCheckSK::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
    //  this class works only for push buttons
    ASSERT (lpDIS->CtlType == ODT_BUTTON);
    
    //  get the device context pointer to draw into
    CDC*	pDC = CDC::FromHandle(lpDIS->hDC);

    //  create brush and pen
    CBrush brushLed;
    CPen penLed(PS_SOLID,1,RGB(0,0,0));

    //  .......................................................................
    //  GET THE BOUNDARY RECTANGLE OF THE BUTTON CONTROL
    //  .......................................................................
    CRect   rectItem = lpDIS->rcItem;
    CPen*	pOldPen;
    CBrush* pOldBrush;

    pDC->SetBkMode(TRANSPARENT);

    //  .......................................................................
    //  GET BUTTON CONDITION
    //  .......................................................................
    m_bIsPressed  = (lpDIS->itemState & ODS_SELECTED);
    m_bIsFocussed = (lpDIS->itemState & ODS_FOCUS);
    m_bIsDisabled = (lpDIS->itemState & ODS_DISABLED);

    //  .......................................................................
    //  GIVE COLOR TO CHECK BOX BASED ON WHETHER THE MOUSE IS OVER IT
    //  .......................................................................
    CBrush brushForeGnd;
    CPen pen;
    if ( m_bMouseOver )
    {
        brushForeGnd.CreateSolidBrush (::GetSysColor(COLOR_BTNHILIGHT));
        pen.CreatePen (PS_SOLID, 1, RGB(0,0,0));
        pOldBrush = pDC->SelectObject (&brushForeGnd);
        pOldPen = pDC->SelectObject(&pen);

        CPoint pt (4,4);
        pDC->RoundRect (&rectItem, pt);
        rectItem.DeflateRect (2,2);
    }
    else
    {
        brushForeGnd.CreateSolidBrush (::GetSysColor(COLOR_3DFACE));
        pen.CreatePen (PS_SOLID, 1, ::GetSysColor(COLOR_3DFACE));
        pOldBrush = pDC->SelectObject (&brushForeGnd);
        pOldPen = pDC->SelectObject(&pen);

        pDC->Rectangle(&rectItem);
        rectItem.DeflateRect (2,2);
    }

    //  .......................................................................
    //  IF CHECK BOX HAS FOCUS DRAW THE FOCUS BOX
    //  .......................................................................
    if (m_bIsFocussed)
        pDC->DrawFocusRect(&rectItem);
    rectItem.DeflateRect(1,1);
    
    //  .......................................................................
    //  DRAW THE ON/OFF LED
    //  .......................................................................
    //  calculate LED's and text's rectangle
    CRect rectLed;

    if (m_hIconOn.hIcon && m_hIconOff.hIcon)
        m_nLedSize = m_hIconOn.dwWidth;

    if (m_nStyle & 0x20)
    {
        //  for text left of led
        rectLed.left   = rectItem.right - m_nLedSize;
        rectLed.right  = rectLed.left + m_nLedSize;
        rectItem.right = rectLed.left - 2; // keep a gap of 3 px between text and LED
    }
    else
    {
        //  for text right of led
        rectLed.left   = rectItem.left;
        rectLed.right  = rectLed.left + m_nLedSize;
        rectItem.left  = rectLed.right + 2; // keep a gap of 3 px between text and LED
    }

    rectLed.top    = (rectItem.Height() - m_nLedSize)/2 + 2;
    rectLed.bottom = rectLed.top + m_nLedSize;

    //  If icon is given then use the icon
    if (m_hIconOn.hIcon && m_hIconOff.hIcon)
    {
        rectLed.top    = 0;
        rectLed.bottom = rectLed.top + m_hIconOn.dwWidth;

        //  if preseed then move the rectangle to give a button is pressed down look
        if (m_bIsPressed)
            rectLed.OffsetRect(1, 1);

        structIcon *pSIcon;
        //  Select the icon to use based on whether mouse is over the button or not
        if (m_bCheck && m_hIconOff.hIcon != NULL)
            pSIcon = &m_hIconOn;
        else
            pSIcon = &m_hIconOff;
   
        //  draw the icon on the button
        pDC->DrawState(	rectLed.TopLeft(), rectLed.Size(),  pSIcon->hIcon, 
            (m_bIsDisabled ? DSS_DISABLED : DSS_NORMAL), (CBrush*)NULL);
    }
    else
    {
        //  if preseed then move the rectangle to give a button is pressed down look
        if (m_bIsPressed)
            rectLed.OffsetRect(1, 1);

        //  create brush and pen
        CBrush brushLed;
        CPen penLed(PS_SOLID,1,RGB(0,0,0));
        if (m_bIsDisabled)
            brushLed.CreateSolidBrush (::GetSysColor(COLOR_3DSHADOW));
        else
        {
            if (m_bCheck)
                brushLed.CreateSolidBrush (m_colLedOn);
            else
                brushLed.CreateSolidBrush (m_colLedOff);
        }

        pDC->SelectObject(&brushLed);
        pDC->SelectObject(&penLed);
    
        //  draw the led
        //pDC->Ellipse(&rectLed);
		pDC->Rectangle(&rectLed);
        CPen penLeadAntiAliase (PS_SOLID, 1, m_colLedOff);
        pDC->SelectObject(&penLeadAntiAliase);
        rectLed.DeflateRect(1,1);
        //pDC->Ellipse(&rectLed);
		pDC->Rectangle(&rectLed);

        pDC->SelectObject(pOldPen);
        pDC->SelectObject(pOldBrush);
        brushLed.DeleteObject();
        penLed.DeleteObject();
    }


	//.......................................................................
	//绘制颜色线条
	//.......................................................................
	CPoint stPoint,endPoint;
	CRect rectOrg=lpDIS->rcItem;
	stPoint.x=rectItem.left;
	stPoint.y=rectOrg.Height()/2;
	endPoint.x=stPoint.x+rectOrg.Width()/3;
	endPoint.y=stPoint.y;
	CPen linecolor(PS_SOLID,2,m_lineClr);
	pOldPen=pDC->SelectObject(&linecolor);
	pDC->MoveTo(stPoint);
	pDC->LineTo(endPoint);
	pDC->SelectObject(pOldPen);
	rectItem.left=endPoint.x+2;

    //  .......................................................................
    //  GET & DISPLAY THE BUTTON TEXT
    //  .......................................................................
    //  get the text on the check box
    CString sTitle;
    GetWindowText(sTitle);
    
    if ( !sTitle.IsEmpty())
    {
        //  ...................................................................
        //  ALIGN TEXT HORIZONTALLY BASED ON THE STYLE 
        //  ...................................................................
        CRect centerRect = rectItem;
                
        UINT uFormat;

        //  center align
        if ((m_nStyle & 0x00000F00) == 0x300)
        {
            uFormat = DT_WORDBREAK | DT_CENTER | DT_VCENTER;
            pDC->DrawText(sTitle, -1, &rectItem, uFormat | DT_CALCRECT);
            rectItem.OffsetRect((centerRect.Width() - rectItem.Width())/2, 
                (centerRect.Height() - rectItem.Height())/2);
        }
        //  right align
        else if ((m_nStyle & 0x00000F00) == 0x200)
        {
            uFormat = DT_WORDBREAK | DT_RIGHT | DT_VCENTER;
            pDC->DrawText(sTitle, -1, &rectItem, uFormat | DT_CALCRECT);
            rectItem.OffsetRect(centerRect.Width() - rectItem.Width(), 
                (centerRect.Height() - rectItem.Height())/2);
        }
        //  by default left-align
        else // if ((m_nStyle & 0x00000F00) == 0x100)
        {
            uFormat = DT_WORDBREAK | DT_LEFT | DT_VCENTER;
            pDC->DrawText(sTitle, -1, &rectItem, uFormat | DT_CALCRECT);
            rectItem.OffsetRect(0, (centerRect.Height() - rectItem.Height())/2);
        }

        //  if preseed then move the rectangle to give a button is pressed down look
        if (m_bIsPressed)
            rectItem.OffsetRect(1, 1);

        pDC->SetBkMode(TRANSPARENT);

        //  ...................................................................
        //  Draw the text
        //  ...................................................................
        if (m_bIsDisabled)
        {
            //  for disabled button draw etched text
            rectItem.OffsetRect(1, 1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
            pDC->DrawText(sTitle, -1, &rectItem, uFormat);
            rectItem.OffsetRect(-1, -1);
            pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
            pDC->DrawText(sTitle, -1, &rectItem, uFormat);
        }
        else
        {
            //  for non-disabled button get the color for background and foreground
            if (m_bMouseOver || m_bIsPressed)
            {
                pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
                pDC->SetBkColor(::GetSysColor(COLOR_BTNFACE));
            }
            else 
            {
                pDC->SetTextColor(::GetSysColor(COLOR_BTNTEXT));
                pDC->SetBkColor(::GetSysColor(COLOR_BTNFACE));
            }
            //  draw the text
            pDC->DrawText(sTitle, -1, &rectItem, uFormat);
        }
    }

    //  .......................................................................
    //  Release resources
    //  .......................................................................
    pDC->SelectObject(pOldPen);
    pDC->SelectObject(pOldBrush);

    penLed.DeleteObject();
    brushForeGnd.DeleteObject();
    pen.DeleteObject();
    brushLed.DeleteObject();
}

//  ...........................................................................
//  WM_MOUSEMOVE message handler
//  ...........................................................................
void CCheckSK::OnMouseMove(UINT nFlags, CPoint point) 
{
    CWnd*               wndUnderMouse = NULL;
    CWnd*               wndActive = this;
    TRACKMOUSEEVENT     csTME;
    
    ClientToScreen(&point);
    wndUnderMouse = WindowFromPoint(point);
    
    // If the mouse enter the button with the left button pressed then do nothing
    if (nFlags & MK_LBUTTON && m_bMouseOver == FALSE) return;
    
    if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd)
    {
        if (!m_bMouseOver)
        {
            m_bMouseOver = true;
            
            Invalidate();
            
            csTME.cbSize = sizeof(csTME);
            csTME.dwFlags = TME_LEAVE;
            csTME.hwndTrack = m_hWnd;
            ::_TrackMouseEvent(&csTME);
        }
    } 
    else 
        CancelHover();

	CButton::OnMouseMove(nFlags, point);
}

void
CCheckSK::CancelHover()
{
    if (m_bMouseOver)
        m_bMouseOver = FALSE;

    Invalidate();
}


//void
//CCheckSK::OnClicked() 
//{
//	m_bCheck = !m_bCheck;
//
//    Invalidate();
//}

BOOL
CCheckSK::SetLedSize(int nSize)
{
    if (nSize < 0)
        return FALSE;
    else
    {
        m_nLedSize = nSize;
        Invalidate();
        return TRUE;
    }
}

BOOL
CCheckSK::SetLedColor(COLORREF colLedOn, COLORREF colLedOff)
{
    m_colLedOn  = colLedOn;
    m_colLedOff = colLedOff;
    Invalidate();
    return TRUE;
}

void
CCheckSK::SetToolTip(LPCTSTR lpszText)
{
    if (!lpszText)
        return;
    
    TOOLINFO ti;
    
    ti.cbSize   = sizeof(TOOLINFO);
    ti.lpszText = (LPTSTR)lpszText;
    ti.hinst    = AfxGetInstanceHandle();
    ti.hwnd     = this->GetParent()->GetSafeHwnd();
    ti.uFlags   = TTF_SUBCLASS | TTF_IDISHWND;
    ti.uId      = (UINT) this->GetSafeHwnd();
    
    m_tooltip.SendMessage(TTM_ADDTOOL, 0, (LPARAM) &ti);
}

void CCheckSK::FreeResources()
{
    if (m_hIconOn.hIcon)
        ::DeleteObject(m_hIconOn.hIcon);
    if (m_hIconOn.hIcon)
        ::DeleteObject(m_hIconOn.hIcon);

    m_hIconOn.hIcon  = NULL;
    m_hIconOn.hIcon = NULL;

}

void CCheckSK::SetLineColor( COLORREF clr )
{
	m_lineClr=clr;
}
