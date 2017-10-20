#include "StdAfx.h"
#include "SelectFolderDlg.h"

CSelectFolderDlg::CSelectFolderDlg(void)
{
}

CSelectFolderDlg::~CSelectFolderDlg(void)
{
}

CString CSelectFolderDlg::Show()
{
	TCHAR   szFolderPath[MAX_PATH] = {0};  
	CString   strFolderPath = TEXT("");  
	       
	BROWSEINFO      sInfo;  
    ::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.hwndOwner=m_hwndOwner;
	sInfo.pidlRoot = 0;  
	sInfo.lpszTitle = _T("��ѡ�񱨸��ŵ�Ŀ¼��");  
	sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;  
	sInfo.lpfn     = NULL;  

	// ��ʾ�ļ���ѡ��Ի���  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
	if (lpidlBrowse != NULL)  
	{  
		 // ȡ���ļ�����  
		if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath))    
		{  
			strFolderPath = szFolderPath;  
		}  
	}  
	if(lpidlBrowse != NULL)  
	{  
		::CoTaskMemFree(lpidlBrowse);  
	 }  
	return strFolderPath;  


}
