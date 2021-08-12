// RecordDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Record.h"
#include "RecordDlg.h"

#include "WaveFile.h"


#include <iostream>
#include <fstream>
#include <string.h>
#include<math.h>
#include<cmath>
#include<stdlib.h>
#include <bitset>
#include <iomanip>
//要在int main()的前面加上函数的声明，因为你的函数写在main函数的后面


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int recording;
extern WAVEFORMATEX wfm;
extern WriteParams params;

extern CRecordDlg* pDlg;

CDC *g_pDC=NULL;
CRect g_Rect;
BOOL g_bVisual=TRUE;

static int m=0;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CString	m_strDesc;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_strDesc = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Text(pDX, IDC_DESC, m_strDesc);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordDlg dialog

CRecordDlg::CRecordDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRecordDlg)
	m_nBits = 0;
	m_nSpeed = 0;
	m_nChannel = 0;
	m_lTime = 0;
	m_strFile = _T("Noname.wav");
	m_strFile1 = _T("Noname.wav");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordDlg)
	DDX_Control(pDX, IDC_SCOPE, m_scope);
	DDX_Radio(pDX, IDC_EIGHT, m_nBits);
	DDX_Radio(pDX, IDC_LOWSPEED, m_nSpeed);
	DDX_Radio(pDX, IDC_MONO, m_nChannel);
	DDX_Text(pDX, IDC_PROCESS, m_lTime);
	DDX_Text(pDX, IDC_FILEEDIT, m_strFile);
	DDX_Text(pDX, IDC_F_File, m_strFile1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRecordDlg, CDialog)
	//{{AFX_MSG_MAP(CRecordDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RECORDBT, OnRecord)
	ON_BN_CLICKED(IDC_FILEBT, OnFile)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_PLAYBT, OnPlay)
	ON_BN_CLICKED(IDC_VISUAL, OnVisual)
	ON_BN_CLICKED(IDC_F_O_BT, OnFOBt)
	ON_BN_CLICKED(IDC_F_PLAY, OnFPlay)
	ON_BN_CLICKED(IDC_F_HPlay_BT, OnFHPlayBT)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordDlg message handlers

BOOL CRecordDlg::OnInitDialog()
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
	pDlg=this;

	recording=0;
	params.pwf=&wfm;
	m_nTimerID=0x1000;
	m_nTimerID=SetTimer(m_nTimerID,1000,NULL);
	
	
	CWnd *pWnd=GetDlgItem(IDC_WAVE);
	g_pDC=pWnd->GetDC();
	pWnd->GetWindowRect(&g_Rect);
	pWnd->ScreenToClient(&g_Rect);

	m_scope.SetDimT(512/2);
	m_scope.SetDimy(5);

	file.Open("data.txt",CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	num=0;
	/*for(int i=0;i<512;i++)
	{
		filewrite(i);
	}*/
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRecordDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRecordDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRecordDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRecordDlg::OnRecord() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);//将控件的值赋值给成员变量

	if(!recording)//recording=0时允许录音，等待录音
	{
		PrepareFormat();
		CreateWaveFile((LPSTR)(LPCSTR)m_strFile,&params);
		RecordStart();//开始录音 //录音正常结束时recording=TRUE，否则recording=FALSE
		m_lTime=0;
		UpdateData(FALSE);//将成员变量的值赋值给控件显示
		if(recording)//录音正常结束时recording=TRUE
			SetDlgItemText(IDC_RECORDBT,"Stop(&T)");//将开始录音按钮显示为停止录音
	}
	else //以前RecordStart()录音正常结束时recording=TRUE，已经录音，此时录音按钮为停止录音
	{
		RecordEnd();//结束录音的收尾工作，断开麦克风，释放内存
		CloseWaveFile(&params);
		if(!recording)
			SetDlgItemText(IDC_RECORDBT,"RecordStart(&R)");
	}
}

void CRecordDlg::OnFile() 
{
	// TODO: Add your control notification handler code here
 	CFileDialog dlg(FALSE,
					"wav",
					"Noname",
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Wave File(*.wav)|*.wav||");
	if(dlg.DoModal() == IDOK)
	{
		m_strFile=dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CRecordDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	RecordEnd();
	CloseWaveFile(&params);
	KillTimer(m_nTimerID);
}


////////////////////////////////////////////
void CRecordDlg::PrepareFormat()
{
	long bps=1;
	wfm.wFormatTag=WAVE_FORMAT_PCM;
	wfm.cbSize=sizeof(WAVEFORMATEX);

	if(m_nChannel == 1)
	{
		wfm.nChannels=2;
		bps*=2;
	}
	else
		wfm.nChannels=1;
	
	if(m_nBits == 1)
	{
		wfm.wBitsPerSample=16;
		bps*=2;
	}
	else
		wfm.wBitsPerSample=8;

	wfm.nBlockAlign=(unsigned short)bps;

	if(m_nSpeed == 2)
	{
		wfm.nSamplesPerSec=(DWORD)44100;
		bps*=44100;
	}
	else if(m_nSpeed == 1)
	{
		wfm.nSamplesPerSec=(DWORD)22050;
		bps*=22050;
	}
	else
	{
		wfm.nSamplesPerSec=(DWORD)11025;
		bps*=11025;
	}

	wfm.nAvgBytesPerSec=bps;
}

void CRecordDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(recording)
	{
		m_lTime++;
		UpdateData(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}

void CRecordDlg::OnPlay() 
{
	// TODO: Add your control notification handler code here
	LPSTR pData;
	LONG len;

	UpdateData(TRUE);
	PlayStart(m_strFile,&wfm,&pData,&len);
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_strDesc.LoadString(IDS_DESC);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRecordDlg::OnVisual() 
{
	// TODO: Add your control notification handler code here
	g_bVisual=!g_bVisual;
}



void CRecordDlg::scope(int data)
{
	m_scope.AddValue(m++,(-(double)data+128)/20);
	num++;
	if(num==512)
	{
		m_scope.UpdateCurve();
		num=0;
	}

}

void CRecordDlg::filewrite(int data)
{
	/*/将数据存入文本文件
	char szBuffer[5];
	//int j=data;
	ZeroMemory(szBuffer, sizeof(szBuffer)); //缓存清零
	 _itoa(data,szBuffer,10);
	file.Write(szBuffer,sizeof(szBuffer));*/
	//将数据存入文本文件
		char szBuffer[60];
		ZeroMemory(szBuffer, sizeof(szBuffer)); //缓存清零
		sprintf(szBuffer, "%6d",data);
		file.WriteString(szBuffer);

}

void CRecordDlg::OnFOBt() 
{
	// TODO: Add your control notification handler code here
  	CFileDialog dlg(TRUE,
					"wav",
					"Noname",
					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
					"Wave File(*.wav)|*.wav||");
	if(dlg.DoModal() == IDOK)
	{
		m_strFile1=dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CRecordDlg::OnFHPlayBT() 
{
	// TODO: Add your control notification handler code here
	LPSTR pData;
	LONG len;
	UpdateData(TRUE);
    PlayStart2(m_strFile1,&wfm,&pData,&len);
}

void CRecordDlg::OnFPlay() 
{
	// TODO: Add your control notification handler code here
	LPSTR pData;
	LONG len;
	UpdateData(TRUE);
	PlayStart1(m_strFile1,&wfm,&pData,&len);

}


/********************************进制转换**********************************/
int hex_char_value(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	else if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	else if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	//assert(0);
	return 0;
}
double hex_to_decimal(char *s)
{
	int len = 2;
	double result = 0;
	for (int i = 0; i < len; i++)
	{
		result += (int)pow((float)16, (int)len - i - 1) * hex_char_value(s[i]);
	}
	return result;
}

char decimal_to_hex(int a)
{
   using namespace std;
   int i=0,n=0,m,num[100];
   char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
   while(a>0)
   {
   num[i++]=a%16;
   a=a/16;
   }
   for(i=i-1;i>=0;i--)
   {
    m=num[i];
   }
   n++;
   return hex[m];
}
/********************************进制转换**********************************/

