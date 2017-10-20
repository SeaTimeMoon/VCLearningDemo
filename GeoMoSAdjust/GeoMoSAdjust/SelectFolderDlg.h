#pragma once

//选择文件夹
class CSelectFolderDlg
{
public:
	CSelectFolderDlg(void);
	~CSelectFolderDlg(void);
	CString Show(); //选择文件夹

public:
	HWND  m_hwndOwner;// 父窗口句柄
};
