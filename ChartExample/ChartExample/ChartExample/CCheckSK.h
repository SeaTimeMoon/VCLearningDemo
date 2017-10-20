#ifndef _CCHECKSK_H_
#define _CCHECKSK_H_

#include <afxcmn.h>

#define	ICNST_OK				0
#define	ICNST_INVALIDRESOURCE   1

//  ===========================================================================
//  class   CCheckSK
//  desc    This class can be used to skin check boxes
//          To use this class draw check boxe with the dialog editor and create
//          control variable of type CCheckSK for it and then use 
//          the public methods
//  ===========================================================================
class CCheckSK : public CButton
{
    
public:
    //  =======================================================================
    //  desc    Set the icon for the check box
    //  
    //  arg1    Resource Id for the icon when the check box is ON
    //  arg2    Resource Id for the icon when the check box is OFF
    //  ret     failure => ICNST_INVALIDRESOURCE; success => ICNST_OK
    //  =======================================================================
    DWORD SetIcon(int nIconOn, int nIconOff);

    //  =======================================================================
    //  desc    Set the icon for the check box
    //  
    //  arg1    Handle for the icon when the check box is ON
    //  arg2    Handle for the icon when the check box is OFF
    //  ret     failure => ICNST_INVALIDRESOURCE; success => ICNST_OK
    //  =======================================================================
    DWORD SetIcon(HICON hIconOn, HICON hIconOff);

    //  =======================================================================
    //  desc    Set the icon for the check box
    //  
    //  arg1    File name for the icon when the check box is ON
    //  arg2    File name for the icon when the check box is OFF
    //  ret     failure => ICNST_INVALIDRESOURCE; success => ICNST_OK
    //  =======================================================================
    DWORD SetIcon(LPCTSTR lpszFileNameIn, LPCTSTR lpszFileNameOut);

    //  =======================================================================
    //  desc    changes color of on/off led
    //  arg1    ON  color of LED
    //  arg2    OFF color of LED
    //  =======================================================================
	BOOL SetLedColor(COLORREF colLedOn, COLORREF colLedOff);

    //  =======================================================================
    //  desc    changes Led size
    //  arg1    diameter of the Led
    //  =======================================================================
	BOOL SetLedSize (int nSize);

    //  =======================================================================
    //  desc    Retrieves check status of the Led
    //  =======================================================================
	BOOL GetCheck();

    //  =======================================================================
    //  desc    set/reset check mark or Led on condition
    //  arg1    TRUE sets, FALSE resets
    //  arg2    TRUE repaints the control
    //  =======================================================================
	void SetCheck(BOOL bCheck, BOOL bRepaint=TRUE);
        
    //  =======================================================================
    //  desc    sets the tooltip text
    //  arg1    the tool tip text
    //  =======================================================================
    void SetToolTip (LPCTSTR lpszText);

	//设置线条的颜色
	void SetLineColor(COLORREF clr);

	virtual ~CCheckSK();
	CCheckSK();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckSK)
	private:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	private:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL


	// Generated message map functions
private:
	//{{AFX_MSG(CCheckSK)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	/*afx_msg void OnClicked();*/
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	void FreeResources();
	LONG m_nStyle;
	void CancelHover();
	BOOL m_bMouseOver;
    LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
    LRESULT OnSetCheck(WPARAM wParam, LPARAM lParam);
    LRESULT OnGetCheck(WPARAM wParam, LPARAM lParam);

	BOOL m_bCheck;
	BOOL m_bIsDisabled;
	BOOL m_bIsFocussed;
	BOOL m_bIsPressed;
	int m_nLedSize;
    COLORREF m_colLedOn;
    COLORREF m_colLedOff;

    CToolTipCtrl m_tooltip;
    struct structIcon
    {
	    HICON		hIcon;			// Handle to icon
	    DWORD		dwWidth;		// Width of icon
	    DWORD		dwHeight;		// Height of icon
    };
    structIcon m_hIconOn, m_hIconOff;

	COLORREF m_lineClr;//线条颜色
};

#endif // _CCHECKSK_H_
