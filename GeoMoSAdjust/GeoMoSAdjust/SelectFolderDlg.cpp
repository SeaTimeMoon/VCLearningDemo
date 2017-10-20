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
	sInfo.lpszTitle = _T("请选择报告存放的目录：");  
	sInfo.ulFlags   = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;  
	sInfo.lpfn     = NULL;  

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
	if (lpidlBrowse != NULL)  
	{  
		 // 取得文件夹名  
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
