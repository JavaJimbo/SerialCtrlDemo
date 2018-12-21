/*  SerialCtrlDemoDlg.cpp : implementation file
 * 
 *	Baudrate bug fix: set to default 19200 in SerialCtrl::OpenPort()
 *	void CSerialCtrlDemoDlg::OnBnClickedButtonWr() add '\r'
 *
 *	12-21-18: Woburn: Removed all Run thread code and CSerialIO class.
 *	12-21-18: Sending and Receiving works great.
 */

#include "stdafx.h"
#include "SerialCtrlDemo.h"
#include "SerialCtrlDemoDlg.h"
#include "SerialCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define	IDT_TIMER_0	WM_USER + 200
#define TIMER_INTERVAL 100  // Main process loop timer interval in milliseconds
 // Actually, it's more like milliseconds x 2
 // So an interval value of 100 corresponds to about 200 milliseconds,
 // or two tenths of a second.

SerialCtrl ComTest;
DCB configSerial;
int seconds = 0;
bool RunFlag = false;
CString strReceive = "";

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
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSerialCtrlDemoDlg dialog




CSerialCtrlDemoDlg::CSerialCtrlDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSerialCtrlDemoDlg::IDD, pParent)
	, bPortOpened(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSerialCtrlDemoDlg::~CSerialCtrlDemoDlg()
{
	ComTest.ClosePort();
}


void CSerialCtrlDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	// DDX_Control(pDX, IDC_COMBO_SN, m_comboSN);
	// DDX_Control(pDX, IDC_COMBO_BR, m_comboBR);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_btnOpen);
	DDX_Control(pDX, IDC_BUTTON_START, m_ButtonStart);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_ButtonStop);
	DDX_Control(pDX, IDC_EDIT_WRITE, m_editWrite);
	DDX_Control(pDX, IDC_EDIT_READ, m_EditRead);
	DDX_Control(pDX, IDC_EDIT_STATUS, m_EditStatus);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staticInfo);
}

BEGIN_MESSAGE_MAP(CSerialCtrlDemoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_START, &CSerialCtrlDemoDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSerialCtrlDemoDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CSerialCtrlDemoDlg::OnBnClickedButtonOpen)
	// ON_CBN_SELCHANGE(IDC_COMBO_SN, &CSerialCtrlDemoDlg::OnCbnSelchangeComboSn)
	ON_BN_CLICKED(IDC_BUTTON_WR, &CSerialCtrlDemoDlg::OnBnClickedButtonWr)
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

	configSerial.ByteSize = 8;
	configSerial.StopBits = ONESTOPBIT;
	configSerial.Parity = NOPARITY;
	configSerial.BaudRate = 19200;
	
	
	// TODO: Add extra initialization here
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


void CSerialCtrlDemoDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	//__super::OnOK();
}



UINT CSerialCtrlDemoDlg::StartTimer(UINT TimerDuration)
{
	UINT TimerVal;
	TimerVal = (UINT)SetTimer(IDT_TIMER_0, TimerDuration, NULL);
	if (TimerVal == 0)
		MessageBox("Unable to obtain timer", "SYSTEM ERROR", MB_OK | MB_SYSTEMMODAL);
	return TimerVal;
}// end StartTimer

bool CSerialCtrlDemoDlg::StopTimer(UINT TimerVal)
{
	if (!KillTimer(TimerVal)) return false;
	else return true;
} // end StopTimer




void CSerialCtrlDemoDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	StartTimer(TIMER_INTERVAL);
	RunFlag = true;
}


void CSerialCtrlDemoDlg::OnBnClickedButtonStop()
{
	// TODO: Add your control notification handler code here
	RunFlag = false;
	StopTimer(TIMER_INTERVAL);
}


void CSerialCtrlDemoDlg::OnBnClickedButtonOpen()
{
	if (!ComTest.GetPortStatus())
	{
		if (ComTest.OpenPort(configSerial, "COM6"))
		{
			m_EditStatus.SetWindowText("COM 6 Opened OK!");
			m_btnOpen.SetWindowTextA("CLOSE");
		}
		else
		{
			m_EditStatus.SetWindowText("Error: could not open COM 6");
			m_btnOpen.SetWindowTextA("OPEN");
		}
	}
	else
	{
		ComTest.ClosePort();
		m_btnOpen.SetWindowTextA("OPEN");
		m_EditStatus.SetWindowText("COM 6 Closed");
	}
}


void CSerialCtrlDemoDlg::OnCbnSelchangeComboSn()
{
	// TODO: Add your control notification handler code here
}


void CSerialCtrlDemoDlg::OnBnClickedButtonWr()
{
	int outLength;
	unsigned long inLength;
	CString strXmitText, strResult;

	// TODO: Add your control notification handler code here
	if (ComTest.GetPortStatus())
	{		
		strReceive = "";
		m_editWrite.GetWindowText(strXmitText);
		strXmitText = strXmitText + '\r';
		outLength = strXmitText.GetLength();
		ComTest.Write(strXmitText, outLength, inLength);
		strResult.Format("%d bytes written", inLength);
		m_EditStatus.SetWindowText(strResult);
	}
	else m_EditStatus.SetWindowText("Port not open");

}

BOOL CSerialCtrlDemoDlg::TestHandler()
{
	char strInPacket[64];
	unsigned long InPacketLength = 0;

	static int timerCounter = 0;
	timerCounter++;

	if (ComTest.GetPortStatus())
	{
		ComTest.Read(strInPacket, 64, InPacketLength);
		if (InPacketLength > 0)
		{
			CString CSpacket(strInPacket);
			strReceive = strReceive + strInPacket;
			m_EditRead.SetWindowText(strReceive);
		}
	}

	if (timerCounter > 5)
	{
		timerCounter = 0;
		seconds++;
		CString strTime;
		strTime.Format("Time: %d seconds", seconds);
		m_EditStatus.SetWindowText(strTime);
	}
	return true;
}

void CSerialCtrlDemoDlg::OnTimer(UINT_PTR TimerVal)
{
	static int tenthSeconds = 0, seconds = 0;
	CString strTimer;
	if (!KillTimer(TimerVal))
	{
		;
	}
	tenthSeconds++;
	if ((tenthSeconds % 10) == 0)
	{
		seconds++;
		strTimer.Format(_T("Time: %d seconds"), seconds);
	}
	TestHandler();
	if (RunFlag) StartTimer(TIMER_INTERVAL);
}
