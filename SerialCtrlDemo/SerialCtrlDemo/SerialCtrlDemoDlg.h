// SerialCtrlDemoDlg.h : header file
//

#pragma once
#include "SerialCtrl.h"
#include "afxwin.h"

// CSerialCtrlDemoDlg dialog
class CSerialCtrlDemoDlg : public CDialog
{
// Construction
public:
	CSerialCtrlDemoDlg(CWnd* pParent = NULL);	// standard constructor
	~CSerialCtrlDemoDlg(); // Destructor

// Dialog Data
	enum { IDD = IDD_SERIALCTRLDEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	//virtual void OnEventOpen(BOOL bSuccess);
	//virtual void OnEventClose(BOOL bSuccess);
	//virtual void OnEventRead(char *inPacket,int inLength);
	//virtual void OnEventWrite(int nWritten);
	BOOL TestHandler();
	bool StopTimer(UINT TimerVal);
	UINT StartTimer(UINT TimerDuration);
	void OnTimer(UINT_PTR TimerVal);
	//CComboBox m_comboSN;
	//CComboBox m_comboBR;
	CButton m_btnOpen, m_ButtonStart, m_ButtonStop;
	CEdit m_editWrite, m_EditRead, m_EditStatus;
	// CListBox m_listboxRead;

	CStatic m_staticInfo;
	BOOL bPortOpened;	
protected:
	virtual void OnOK();
public:	
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnCbnSelchangeComboSn();
	afx_msg void OnBnClickedButtonWr();
};
