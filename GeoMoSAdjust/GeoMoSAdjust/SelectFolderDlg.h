#pragma once

//ѡ���ļ���
class CSelectFolderDlg
{
public:
	CSelectFolderDlg(void);
	~CSelectFolderDlg(void);
	CString Show(); //ѡ���ļ���

public:
	HWND  m_hwndOwner;// �����ھ��
};
