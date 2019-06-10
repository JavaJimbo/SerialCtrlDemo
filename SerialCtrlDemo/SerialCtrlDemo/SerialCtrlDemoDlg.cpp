// SerialCtrlDemoDlg.cpp : implementation file
/* Bug: sets baud rate to 115200 only
 * 6-8-19 JBS: Added Edit box to receive data. 
 * 6-9-19 JBS: Original version, works with singleton function.
 */

#include "stdafx.h"
#include "SerialCtrlDemo.h"
#include "SerialCtrlDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSerialCtrlDemoDlg dialog




CSerialCtrlDemoDlg::CSerialCtrlDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialCtrlDemoDlg::IDD, pParent)
	, bPortOpened(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSerialCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SN, m_comboSN);
	DDX_Control(pDX, IDC_COMBO_BR, m_comboBR);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_EDIT_WRITE, m_editWrite);	
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
	DDX_Control(pDX, IDC_EDIT_RECEIVE, m_EditReceive);
}

BEGIN_MESSAGE_MAP(CSerialCtrlDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CSerialCtrlDemoDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_WR, &CSerialCtrlDemoDlg::OnBnClickedButtonWr)
	ON_CBN_SELCHANGE(IDC_COMBO_BR, &CSerialCtrlDemoDlg::OnCbnSelchangeComboBr)
	ON_CBN_SELCHANGE(IDC_COMBO_SN, &CSerialCtrlDemoDlg::OnCbnSelchangeComboSn)
END_MESSAGE_MAP()


// CSerialCtrlDemoDlg message handlers

BOOL CSerialCtrlDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_comboSN.InsertString(0,"COM1");
	m_comboSN.InsertString(1,"COM2");
	m_comboSN.InsertString(2,"COM3");
	m_comboSN.InsertString(3,"COM4");
	m_comboSN.InsertString(4,"COM5");
	m_comboSN.InsertString(5,"COM6");
	m_comboSN.InsertString(6,"COM7");
	m_comboSN.InsertString(7,"COM8");
	m_comboSN.InsertString(8,"COM9");
	m_comboSN.InsertString(9,"COM10");
	m_comboSN.SetCurSel(0);

	m_comboBR.InsertString(0,"4800");
	m_comboBR.InsertString(1,"9600");
	m_comboBR.InsertString(2,"19200");
	m_comboBR.InsertString(3,"115200");
	m_comboBR.SetCurSel(1);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSerialCtrlDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSerialCtrlDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSerialCtrlDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSerialCtrlDemoDlg::OnEventOpen(BOOL bSuccess)
{
	CString str;
	if(bSuccess)
	{
		str=m_strPortName+" open successfully";

		bPortOpened=TRUE;
		m_btnOpen.SetWindowText("Close");
		
	}
	else
	{
		str=m_strPortName+" open failed";
	}
	m_staticInfo.SetWindowText(str);
}

void CSerialCtrlDemoDlg::OnEventClose(BOOL bSuccess)
{
	CString str;
	if(bSuccess)
	{
		str=m_strPortName+" close successfully";
		bPortOpened=FALSE;
		m_btnOpen.SetWindowText("Open");

	}
	else
	{
		str=m_strPortName+" close failed";
	}
	m_staticInfo.SetWindowText(str);
}

void CSerialCtrlDemoDlg::OnEventWrite(int nWritten)
{
	if(nWritten>0)
	{
		CString str;
		str.Format("%d bytes written",nWritten);
		m_staticInfo.SetWindowText(str);
	}
	else
	{
		m_staticInfo.SetWindowText("Write failed");
	}

}

void CSerialCtrlDemoDlg::OnBnClickedButtonOpen()
{
	// TODO: Add your control notification handler code here
    if(bPortOpened==FALSE)
	{
		CString strPortName;
		CString strBaudRate;
		m_comboSN.GetLBText(m_comboSN.GetCurSel(),strPortName);
		m_comboBR.GetLBText(m_comboBR.GetCurSel(),strBaudRate);
		OpenPort(strPortName,strBaudRate);

	}
	else
	{
		ClosePort();

	}
	
}

void CSerialCtrlDemoDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//__super::OnOK();
}

void CSerialCtrlDemoDlg::OnBnClickedButtonWr()
{
	// TODO: Add your control notification handler code here

	CString strW;
	m_editWrite.GetWindowText(strW);
	Write((LPTSTR)(LPCTSTR)strW,strW.GetLength());
}


void CAboutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}

void CSerialCtrlDemoDlg::OnEventRead(char *inPacket, int inLength)
{
	static CString strReceiveText = "";

	if (strReceiveText.GetLength() > 128) strReceiveText = "";  // Purge Edit box when it overflows
	
	strReceiveText = strReceiveText + inPacket;
	m_EditReceive.SetWindowTextA(strReceiveText);  // Copy incoming text to Edit box

	// m_listboxRead.AddString(inPacket);

	CString str;
	str.Format("%d bytes read", inLength);

	m_staticInfo.SetWindowText(str);

}


void CSerialCtrlDemoDlg::OnCbnSelchangeComboBr()
{
	// TODO: Add your control notification handler code here
}


void CSerialCtrlDemoDlg::OnCbnSelchangeComboSn()
{
	// TODO: Add your control notification handler code here
}
