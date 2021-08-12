// RecordDlg.h : header file
//

#if !defined(AFX_RECORDDLG_H__49502547_C214_11D3_B232_00A0C9397E48__INCLUDED_)
#define AFX_RECORDDLG_H__49502547_C214_11D3_B232_00A0C9397E48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "scope.h"
/////////////////////////////////////////////////////////////////////////////
// CRecordDlg dialog

class CRecordDlg : public CDialog
{
// Construction
public:
	void filewrite(int data);
	void scope(int data);
	CRecordDlg(CWnd* pParent = NULL);	// standard constructor
	void PrepareFormat();

	//CFile file;
	CStdioFile file;

	int num;
// Dialog Data
	//{{AFX_DATA(CRecordDlg)
	enum { IDD = IDD_RECORD_DIALOG };
	CScope	m_scope;
	int		m_nBits;
	int		m_nSpeed;
	int		m_nChannel;
	long	m_lTime;
	CString	m_strFile;
	CString	m_strFile1;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecordDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT m_nTimerID;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRecordDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRecord();
	afx_msg void OnFile();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPlay();
	afx_msg void OnVisual();
	afx_msg void OnFilter();
	afx_msg void OnFOBt();
	afx_msg void OnFPlay();
	afx_msg void OnDoubleclickedCancel2();
	afx_msg void OnFHPlayBT();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECORDDLG_H__49502547_C214_11D3_B232_00A0C9397E48__INCLUDED_)
