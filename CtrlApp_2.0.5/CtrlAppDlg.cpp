// © BBC 2011	DPB 1/6/2011
// ControlDlg.cpp : implementation file
// This is where most of the application control stuff is
// together with all of the application system variables

#include "stdafx.h"
#include "CtrlApp.h"
#include "CtrlAppDlg.h"
#include "Data.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CControlApp theApp;
int update_time[] = {1,2,5,10,30,60,120};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlDlg dialog

CControlDlg::CControlDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CControlDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CControlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CControlDlg)
	DDX_Control(pDX, IDC_DATARATE, m_nDisplayRate);
	DDX_Control(pDX, IDC_CLIENTLIST, m_ClientList);
	DDX_Control(pDX, IDC_RUNCOMMAND, m_nRunningCommand);
		//DDX_Control(pDX, IDC_SENDOUTPUT, m_SendOutput);  //Added for MS Meter 2
	//}}AFX_DATA_MAP
}


UINT my_callback(LPVOID param)
{
	//Creates another thread to run the script commnds on script Start
	CControlDlg *my_class = static_cast<CControlDlg*>(param);
	my_class->RunningScript();
	AfxEndThread(0, TRUE);
	return 0;
}


void CControlDlg::DoSendOp(CString str, int CurrentClient)
{
	//Output each test control command to the main output window
	CString opstr, oldstr;

	if (CurrentClient < -1)
		opstr.Format("%s \r\n", str);
	else
		opstr.Format("Cmd> %d   %s \r\n", CurrentClient, str);

	// Using CEdit to allow copy, edit, return, etc
	((CEdit*)GetDlgItem(IDC_SENDOUT2))->GetWindowText(oldstr);
	opstr = oldstr + opstr;
	((CEdit*)GetDlgItem(IDC_SENDOUT2))->SetWindowText(opstr);
	//scroll down with each new o/p
	((CEdit*)GetDlgItem(IDC_SENDOUT2))->SendMessage(WM_VSCROLL, SB_BOTTOM, 0);	
}


BEGIN_MESSAGE_MAP(CControlDlg, CDialog)
	//{{AFX_MSG_MAP(CControlDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONTROLPANEL, OnControlpanel)
	ON_BN_CLICKED(IDC_START, OnStart)
	ON_BN_CLICKED(IDC_PAUSE, OnPause)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SELECTALL, OnSelectall)
	ON_BN_CLICKED(IDC_CLEARSELECTION, OnClearselection)
	ON_LBN_SELCHANGE(IDC_CLIENTLIST, OnSelchangeClientlist)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	//Added for MS Meter 2 Script control pannel
	ON_BN_CLICKED(IDC_CONTROLPANEL2, OnScriptControlpanel)
	ON_BN_CLICKED(IDC_START2, OnScriptStart)
	ON_BN_CLICKED(IDC_PAUSE2, OnScriptPause)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER_APPLY, OnApply)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlDlg message handlers

BOOL CControlDlg::OnInitDialog()
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
	m_ClientList.AddString("No clients registered.");
	m_port = 50100;
	for(int c = 0 ; c < MAXCLIENTS ; c++) {
		m_pTestClient[c] = NULL;
		m_pnSelClients[c] = 0;
	}
	m_strRecv = _T("");
	m_strSend = _T("");
	m_pLstnSock = NULL;
	m_nClients = 0;
	m_nSelClients = 0;
	m_bCreateflag = FALSE;
	m_SaveResults = TRUE;
	m_SaveSummary = TRUE;
	m_fRate = 0.0;
	m_fLatency = 0.0;
	m_fMaxLatency = 0.0;
	m_nStartAveraging = 0;
	m_nUpdateTime = 0;
	//MS Meter 2 added
	m_bDirectIO = FALSE;
	m_fProcessData = TRUE;		//Flag to start and stop processing of data
	for(int i = 1 ; i < MAXBLOCKTYPE ; i++) {
		m_bBlockSize[i] = FALSE;
	}


	CString str;
	int t;
	str = theApp.m_pszHelpFilePath;		//This is a great kludge - I can't find any other way of finding the directory with the executable 
	t = str.ReverseFind('\\');			// Backstep to remove the filename
	str = str.Left(t);					// and truncate.
	m_OutFilePath = str;
	m_sOutFileName = m_OutFilePath + "\\results.csv";
	//Added for MS Meter 2 script file
	m_sScriptFilePath = str;
	m_sScriptFilePath = m_sScriptFilePath + "\\script.txt";
	//Added for MS Meter 2
	str.Format("Results %s", m_sOutFileName);
	DoSendOp(str, -1);

	// Set ranges for progress bars - fixed
	((CProgressCtrl*)GetDlgItem(IDC_MAXLATENCY))->SetRange(0,100);
	((CProgressCtrl*)GetDlgItem(IDC_LATENCY))->SetRange(0,100);
	((CProgressCtrl*)GetDlgItem(IDC_DATARATE))->SetRange(0,100);
	((CProgressCtrl*)GetDlgItem(IDC_RUNCOMMAND))->SetRange(0,100);

	// Here start up the listener socket
	CString error;
	if (m_pLstnSock == NULL) 
	{
		if ((m_pLstnSock = new CAsynlstn(this)) == NULL) {
			AfxMessageBox ("Failed to allocate listen socket! Close and restart app.");
			exit (0);
		}
		if (!m_pLstnSock->Create(m_port)){
			error.Format("Failed to create listen socket: %d! Close and restart app.", m_pLstnSock->GetLastError());
			AfxMessageBox ((LPCTSTR)error);
			exit (0);
		}
		if (!m_pLstnSock->Listen()) {
			error.Format("Failed to listen: %d! Close and restart app.", m_pLstnSock->GetLastError());
			AfxMessageBox ((LPCTSTR)error);
			exit (0);
		}
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
		return;
	}

	if (nID == SC_CLOSE) {
		if (AfxMessageBox("Are you sure you want to Quit the application?\n\n!! This will shut down all registered clients !!", MB_YESNO | MB_ICONSTOP) == IDYES) {
			int t = 0;
			while (m_nClients > 0){
				if (m_pTestClient[t]) {
					StopClient(t);
					// I may need to wait here until the end data transfer has completed
					DestroyClient(t);
				}
				t++;
			}
		}
		else return;
	}
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CControlDlg::OnPaint() 
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
HCURSOR CControlDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CControlDlg::OnControlpanel() 
{
	// TODO: Add your control notification handler code here

	if ((m_nSelClients = m_ClientList.GetSelItems(m_nClients, m_pnSelClients)) == 0) {
		// If no clients selected output message and do nothing
		AfxMessageBox("No clients selected.");
		return;
	}

	CPSheet ps(_T("Test application controls"));
	ps.m_psh.dwFlags &= ~(PSH_HASHELP);
	int t;

	t = ListGetIndex(m_pnSelClients[0]);		// Get index of first selected client

	// Put all the initial values for the control panel in here
	// Set opening parameters to same as first selected client.
	ps.m_controlpage.m_sTestFilePath = m_pTestClient[t]->m_header.testfilepath;
	ps.m_controlpage.m_nMaxnum = m_pTestClient[t]->m_header.maxnum;
	ps.m_controlpage.m_dMaxlength = m_pTestClient[t]->m_header.maxlength;
	ps.m_controlpage.m_nMaxdelay = m_pTestClient[t]->m_header.maxdelay;
	ps.m_controlpage.m_nPCwrite = m_pTestClient[t]->m_header.pcwrite;
	ps.m_controlpage.m_nPCrandom = m_pTestClient[t]->m_header.pcrand;
	ps.m_controlpage.m_bVerbose = m_pTestClient[t]->m_header.verbose;
	ps.m_controlpage.m_bSharing = m_pTestClient[t]->m_header.sharing;
	ps.m_controlpage.m_bCreateflag = FALSE;
	ps.m_controlpage.m_nCreate = 0;
	// Added for MS Meter 2
	// Put intial values for block size selection and direct IO buttons
	ps.m_controlpage.m_bBlockSize[1] = m_pTestClient[t]->m_header.lstblocksize[1];
	ps.m_controlpage.m_bBlockSize[2] = m_pTestClient[t]->m_header.lstblocksize[2];
	ps.m_controlpage.m_bBlockSize[3] = m_pTestClient[t]->m_header.lstblocksize[3];
	ps.m_controlpage.m_bBlockSize[4] = m_pTestClient[t]->m_header.lstblocksize[4];
	ps.m_controlpage.m_bBlockSize[5] = m_pTestClient[t]->m_header.lstblocksize[5];
	ps.m_controlpage.m_bBlockSize[6] = m_pTestClient[t]->m_header.lstblocksize[6];
	ps.m_controlpage.m_bBlockSize[7] = m_pTestClient[t]->m_header.lstblocksize[7];
	ps.m_controlpage.m_bBlockSize[8] = m_pTestClient[t]->m_header.lstblocksize[8];
	ps.m_controlpage.m_bBlockSize[9] = m_pTestClient[t]->m_header.lstblocksize[9];
	ps.m_controlpage.m_bBlockSize[10] = m_pTestClient[t]->m_header.lstblocksize[10];
	ps.m_controlpage.m_bBlockSize[11] = m_pTestClient[t]->m_header.lstblocksize[11];
	ps.m_controlpage.m_bDirectIO = m_pTestClient[t]->m_header.directio;
	//
	ps.m_filespage.m_OutputFileName = m_sOutFileName;
	ps.m_filespage.m_OutputFilePath = m_OutFilePath;
	ps.m_filespage.m_save = m_SaveResults;
	ps.m_filespage.m_summary = m_SaveSummary;
	ps.m_filespage.m_Slider = m_nUpdateTime;
	ps.m_filespage.m_TimerChanged = FALSE;

	int nResponse = ps.DoModal();
	if (nResponse == IDOK) {
		// TODO: Place code here to handle when the dialog is dismissed with OK
		// Store returned parameters in selected clients
		// I know the selected clients haven't changed because the control panel is modal
		
		m_bCreateflag = ps.m_controlpage.m_bCreateflag;
		m_dCreate = ps.m_controlpage.m_nCreate;
		
		BOOL m_bBlockSizeSet = FALSE;
		for(int i = 1 ; i < MAXBLOCKTYPE ; i++) {
			m_bBlockSize[i] = ps.m_controlpage.m_bBlockSize[i];
			if (m_bBlockSize[i]) {
				m_bBlockSizeSet = TRUE;
			}
			CString str;
			//str.Format("Bs %d = %s  TypeSet = %s", i, (m_bBlockSize[i])?"TRUE":"FALSE", (m_bBlockSizeSet)?"TRUE":"FALSE");
			//DoSendOp(str, 0);
		}

		// No block size types selected error check / messsage
		if (!m_bBlockSizeSet) {
			AfxMessageBox("Error in Block Size, none selected");
			return;
		}


		// file create error check / messages
		if (m_bCreateflag) {
			if (m_nSelClients > 1) {
				AfxMessageBox("Error in file create parameters, more than one client selected");
				return;
			}

			if (m_dCreate == 0) {
				AfxMessageBox("Error in file create parameters, file size = 0");
				return;
			}
			if (ps.m_controlpage.m_nMaxnum == 0) {
				AfxMessageBox("Error in file create parameters, number of files = 0");
				return;
			}
			//return;				// If I put this here then the client wont be updated - do I want that?
		}



		// Load current setting from each selected client
		for (int c = 0 ; c < m_nSelClients ; c++) {
			t = ListGetIndex(m_pnSelClients[c]);
			m_pTestClient[t]->m_header.testfilepath = ps.m_controlpage.m_sTestFilePath;
			m_pTestClient[t]->m_header.maxnum = ps.m_controlpage.m_nMaxnum;
			m_pTestClient[t]->m_header.maxlength = ps.m_controlpage.m_dMaxlength;
			m_pTestClient[t]->m_header.maxdelay = ps.m_controlpage.m_nMaxdelay;
			m_pTestClient[t]->m_header.pcwrite = ps.m_controlpage.m_nPCwrite;
			m_pTestClient[t]->m_header.pcrand = ps.m_controlpage.m_nPCrandom;
			m_pTestClient[t]->m_header.verbose = ps.m_controlpage.m_bVerbose;
			m_pTestClient[t]->m_header.sharing = ps.m_controlpage.m_bSharing;
			m_pTestClient[t]->m_header.changed = TRUE;
			m_pTestClient[t]->m_bCPflag = TRUE;
			//Added for MS Meter 2
			m_pTestClient[t]->m_header.lstblocksize[1] = ps.m_controlpage.m_bBlockSize[1];
			m_pTestClient[t]->m_header.lstblocksize[2] = ps.m_controlpage.m_bBlockSize[2];
			m_pTestClient[t]->m_header.lstblocksize[3] = ps.m_controlpage.m_bBlockSize[3];
			m_pTestClient[t]->m_header.lstblocksize[4] = ps.m_controlpage.m_bBlockSize[4];
			m_pTestClient[t]->m_header.lstblocksize[5] = ps.m_controlpage.m_bBlockSize[5];
			m_pTestClient[t]->m_header.lstblocksize[6] = ps.m_controlpage.m_bBlockSize[6];
			m_pTestClient[t]->m_header.lstblocksize[7] = ps.m_controlpage.m_bBlockSize[7];
			m_pTestClient[t]->m_header.lstblocksize[8] = ps.m_controlpage.m_bBlockSize[8];
			m_pTestClient[t]->m_header.lstblocksize[9] = ps.m_controlpage.m_bBlockSize[9];
			m_pTestClient[t]->m_header.lstblocksize[10] = ps.m_controlpage.m_bBlockSize[10];
			m_pTestClient[t]->m_header.lstblocksize[11] = ps.m_controlpage.m_bBlockSize[11];
			m_pTestClient[t]->m_header.directio = ps.m_controlpage.m_bDirectIO;
		}

		m_SaveResults = ps.m_filespage.m_save;
		m_SaveSummary = ps.m_filespage.m_summary;
		m_nUpdateTime = ps.m_filespage.m_Slider;
		m_sOutFileName = ps.m_filespage.m_OutputFileName;
		m_OutFilePath = ps.m_filespage.m_OutputFilePath;
		if(ps.m_filespage.m_TimerChanged) {
			KillTimer(1);			// It doesn't seem to matter if the timer is not running!
			SetTimer(1, update_time[m_nUpdateTime] * 1000, NULL);
		}
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is dismissed with Cancel
	}
	return;	
}

void CControlDlg::OnStart() 
{
	// TODO: Add your control notification handler code here
	// AfxMessageBox("Start control clicked");
	// Now limited to selected clients only.

	m_fProcessData = TRUE;		//Flag to start and stop processing of data

	if ((m_nSelClients = m_ClientList.GetSelItems(m_nClients, m_pnSelClients)) == 0) {
		AfxMessageBox("No clients selected.");
		return;
	}

	int t, c;
	// Check to see if clients are configured
	for (c = 0 ; c < m_nSelClients ; c++) {	
		t = ListGetIndex(m_pnSelClients[c]);
		if (!m_pTestClient[t]->m_bCPflag) {
			CString str;
			str.Format("Warning - control panel initialisation not done for client #%d.\nYou probably want to at least set the path to the test files.\n\nDo you want to continue?", t);
			if(AfxMessageBox((LPCTSTR) str, MB_YESNO | MB_ICONQUESTION) != IDYES){
				return;
			}
		}
	}

	m_strSend = "start";

	//Test can crate resutls file
	if(m_SaveResults || m_SaveSummary){
		if (m_sOutFileName.GetLength() == 0) {
			AfxMessageBox("File saving set but no filename selected.");
		}
		else {
			if (outfile.m_hFile == CFile::hFileNull) {
				CFileException e;
				if(!outfile.Open(m_sOutFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyWrite, &e)) {
					CString str;
					str.Format("Warning - failed to open output file.\n\n");
					switch (e.m_cause){
						case CFileException::fileNotFound : str += "The file could not be located." ; break ;
						case CFileException::badPath : str += "All or part of the path is invalid." ; break ;
						case CFileException::tooManyOpenFiles : str += "The permitted number of open files was exceeded." ; break ;
						case CFileException::accessDenied : str += "The file could not be accessed." ; break ;
						case CFileException::invalidFile : str += "There was an attempt to use an invalid file handle." ; break ;
						case CFileException::removeCurrentDir : str += "The current working directory cannot be removed." ; break ;
						case CFileException::directoryFull : str += "There are no more directory entries." ; break ;
						case CFileException::badSeek : str += "There was an error trying to set the file pointer." ; break ;
						case CFileException::hardIO : str += "There was a hardware error." ; break ;
						case CFileException::sharingViolation : str += "SHARE.EXE was not loaded, or a shared region was\n locked. (Is the file open in another application?)." ; break ;
						case CFileException::lockViolation : str += "There was an attempt to lock a region that was already locked." ; break ;
						case CFileException::diskFull : str += "The disk is full." ; break ;
						case CFileException::endOfFile : str += "The end of file was reached." ; break ;
						default : str += "An unspecified error occurred." ; break ;
					}
					str += "\n\nResults will not be saved.";
					AfxMessageBox(str);
				}
			}
		}
	}

	//Check clients are running and listening
	int flag = 0, tstop = m_nSelClients;
	for (c = 0 ; c < tstop ; c++) {
		t = ListGetIndex(m_pnSelClients[c]);
		if (!m_pTestClient[t]->m_bRunning) {
			if (m_pTestClient[t]->m_pSrvrSock) {  
				// Here if it finds the socket open
				// put the start stuff in here
				m_pTestClient[t]->SendHeader(m_bCreateflag, m_dCreate);

				//Send client commands to the CEdit o/p window
				CString hdrstr = m_pTestClient[t]->CreateHeaderStr(m_bCreateflag, m_dCreate);
				DoSendOp(hdrstr, t);

				if (!m_bCreateflag) {
					m_pTestClient[t]->Send(m_strSend);
					//Send to CEdit Window
					DoSendOp(m_strSend, t);
					m_timeStart = CTime::GetCurrentTime();	//Current time used to measure duration between start & stop
					//CString timestr;
					//timestr.Format("%d", m_timeStart);
					//DoSendOp(timestr, t);
					m_pTestClient[t]->m_bRunning = TRUE;
					m_pTestClient[t]->m_bResponding = FALSE;
					flag = 1;		//Test is running
				}
				m_bCreateflag = FALSE;
			}
			else {  
				// Here if it finds the socket not open.
				/* This loop can work up to a certain point then fail. The ones already dealt with
				don't need doing again. The loop resets the selected list and continues up to the next
				failure or the end. */
				CString str;
				str.Format("Error - client socket, Index %d not open.\nClient #%d will be removed from list.", t, t);
				AfxMessageBox((LPCTSTR)str);
				DestroyClient(t);
				m_nSelClients = m_ClientList.GetSelItems(m_nClients, m_pnSelClients);
				tstop = m_nSelClients;		// New upper limit on loop
				c--;						// set counter back one to catch the next valid client
			}
		}
	}

	if (flag){
		// Disable normal start button
		GetDlgItem(IDC_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_PAUSE)->EnableWindow(TRUE);
		// Disbale script control, start and stop buttons
		GetDlgItem(IDC_START2)->EnableWindow(FALSE);
		GetDlgItem(IDC_PAUSE2)->EnableWindow(FALSE);
		GetDlgItem(IDC_CONTROLPANEL2)->EnableWindow(FALSE);
	}
	if (!SetTimer(1, update_time[m_nUpdateTime] * 1000, NULL)){
		AfxMessageBox("System failure - cannot set a valid timer");
	}
	// Initialise display parameters
	m_fRate = 0.0;
	m_fEffRate = 0.0;
	m_fLatency = 0.0;
	m_fMaxLatency = 0.0;
	m_nStartAveraging = 0;
	m_Data.Clear();
	// Zero display windows
	((CProgressCtrl*)GetDlgItem(IDC_DATARATE))->SetPos(0);
	((CProgressCtrl*)GetDlgItem(IDC_DATARATE2))->SetPos(0);
	((CProgressCtrl*)GetDlgItem(IDC_LATENCY))->SetPos(0);
	((CProgressCtrl*)GetDlgItem(IDC_MAXLATENCY))->SetPos(0);
	((CStatic*)GetDlgItem(IDC_RATE))->SetWindowText("");
	((CStatic*)GetDlgItem(IDC_RATE2))->SetWindowText("");
	((CStatic *)GetDlgItem(IDC_AVLAT))->SetWindowText("");
	((CStatic *)GetDlgItem(IDC_MAXLAT))->SetWindowText("");
	((CStatic *)GetDlgItem(IDC_PULSE))->SetWindowText("");
	((CProgressCtrl*)GetDlgItem(IDC_RUNCOMMAND))->SetPos(0);
	GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
}

void CControlDlg::OnPause() 
{
	// TODO: Add your control notification handler code here
	// Now limited to selected clients only.

	m_fProcessData = FALSE;		//Flag to start and stop processing of data, paused

	if ((m_nSelClients = m_ClientList.GetSelItems(m_nClients, m_pnSelClients)) == 0) {
		AfxMessageBox("No clients selected.");
		return;
	}

	m_strSend = "pause";
	int flag = 0;
	int t, tstop = m_nSelClients;

	for (int c = 0 ; c < tstop ; c++) {
		t = ListGetIndex(m_pnSelClients[c]);
		if (m_pTestClient[t]->m_bRunning) {
			if (m_pTestClient[t]->m_pSrvrSock) {
				// put the stop stuff in here
				m_pTestClient[t]->Send(m_strSend);
				//Send command to CEdit Window
				// Measure and dispaly time in seconds, since last start
				m_timeStop = CTime::GetCurrentTime();
				CString timestr;
				timestr.Format("time %d", (m_timeStop - m_timeStart));
				DoSendOp(timestr, -1);
				DoSendOp(m_strSend, t);
				m_timeStart = CTime::GetCurrentTime();
				m_pTestClient[t]->m_bRunning = FALSE;
				flag = 1;
			}
			else {
				// Here if it finds the socket not open.
				/* This loop can work up to a certain point then fail. Same
				as 'start' command */
				CString str;
				str.Format("Error - client socket, Index %d not open.\nClient #%d will be removed from list.", t, t);
				AfxMessageBox((LPCTSTR)str);
				DestroyClient(t);
				m_nSelClients = m_ClientList.GetSelItems(m_nClients, m_pnSelClients);
				tstop = m_nSelClients;		// New upper limit on loop
				c--;						// set counter back one to catch the next valid client
			}
		}
	}

	if (flag){
		// Enable start button
		GetDlgItem(IDC_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_PAUSE)->EnableWindow(FALSE);
		// Enable script control button
		GetDlgItem(IDC_CONTROLPANEL2)->EnableWindow(TRUE);
	}
	if (outfile.m_hFile != CFile::hFileNull) {
		if (m_SaveSummary) m_Data.Printout(&outfile);
		outfile.Close();
	}
	KillTimer(1);
	m_nStartAveraging = 0;
	
	GetDlgItem(IDC_REMOVE)->EnableWindow(TRUE);
}

void CControlDlg::OnOK() 
{
	// TODO: Add extra validation here
	//AfxMessageBox("Quit control clicked");

	if(AfxMessageBox("Are you sure you want to Quit the application?\n\n!! This will shut down all registered clients !!", MB_YESNO | MB_ICONSTOP) == IDNO)
		return;
	int t = 0;
	while (m_nClients > 0){
		if (m_pTestClient[t]) {
			StopClient(t);
			// I may need to wait here until the end data transfer has completed
			DestroyClient(t);
		}
		t++;
	}
	CDialog::OnOK();
}

void CControlDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if(m_pLstnSock) {
		m_pLstnSock->ShutDown();
		m_pLstnSock->Close();
		delete m_pLstnSock;
	}

	int t = 0;
	while (m_nClients > 0){
		if (m_pTestClient[t]) {
			StopClient(t);
			// I may need to wait here until the end data transfer has completed
			DestroyClient(t);
		}
		t++;
	}

	if(outfile.m_hFile != CFile::hFileNull){
		//close output file
		outfile.Close();
	}
}

int CControlDlg::CreateClient()
{
	int t;
	if ((t = GetNextIndex()) == -1)
	{
		CString str;
		str.Format("Only %d test Clients allowed at a time", MAXCLIENTS);
		AfxMessageBox ((LPCTSTR)str);
		return -1;
	}

	if ((m_pTestClient[t] = new CTestClient(this, t)) == NULL){
		AfxMessageBox("Failed to create client");
		return -1;
	}
	else {
		m_nClients++;
		if(m_nClients == 1) m_ClientList.ResetContent();
	}
	
	//CString str;
	//str.Format("Number of registered clients = %d", m_nClients);
	//AfxMessageBox((LPCTSTR) str);

	return t;				// return the index of the CTestClient object
}

void CControlDlg::DestroyClient(int index)
{
	// Delete item from ClientList, decrement number of clients and set pointer to NULL 
	// Delete from ListBox.
	// This is very complicated - I need a list of running client index numbers.
	// I need to find out which ListBox item corresponds to the index
	// because the ListBox may have been changed by other things.

	if (m_pTestClient[index]) {
		delete m_pTestClient[index];
		m_pTestClient[index] = NULL;
		m_ClientList.DeleteString(ListGetEntry(index));
		m_nClients--;
	}

	// If there are no clients left then set 'running' flag, ListBox  and control 
	// button states ...

	if (m_nClients == 0){
		m_ClientList.ResetContent();
		m_ClientList.AddString("No clients registered.");
		GetDlgItem(IDC_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_PAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
	}
}

void CControlDlg::ProcessPendingRead(int index) {

	CString in_str, str;
	in_str = m_pTestClient[index]->m_pSrvrSock->m_strReturn;

	if (m_pTestClient[index]->first_time == 1) {
		// If we don't get the logon message then close down
		// the server and reset the pointer and list window.
		if (in_str.CompareNoCase("hello") != 0 /* && in_str.CompareNoCase("bye") != 0 */) {
			DestroyClient(index);
		}
		else {
			m_strSend = "hellotoyou";
			m_pTestClient[index]->Send(m_strSend);
			m_pTestClient[index]->GetName();
			str.Format("%3d - %s", index, m_pTestClient[index]->m_strSockName);
			m_ClientList.AddString((LPCTSTR)str);
			GetDlgItem(IDC_START)->EnableWindow(TRUE);
			m_pTestClient[index]->first_time = 0;
		}
		return;
	}

	if (in_str.CompareNoCase("bye") == 0) 	{
		DestroyClient(index);
		return;
	}

	if (in_str.CompareNoCase("nofiles") == 0) 	{
		PauseClient(index);	
		str.Format("Client, reference index number %d, cannot find test the files.\n\n Re-select clients to reset controls.", index);
		AfxMessageBox((LPCTSTR)str);
		//I probably need to update the button states to accomodate this event
		return;
	}

	ProcessData(in_str, index);
}

void CControlDlg::PauseClient(int index)
{
	m_strSend = "pause";
	m_pTestClient[index]->Send(m_strSend);
	//Send command to CEdit Window
	DoSendOp(m_strSend, index);
	m_pTestClient[index]->m_bRunning = FALSE;
}

void CControlDlg::StopClient(int index)
{
	m_strSend = "stop";
	//Send command to CEdit Window
	DoSendOp(m_strSend, index);
	//
	m_pTestClient[index]->Send(m_strSend);
}

void CControlDlg::OnSelectall() 
{
	// TODO: Add your control notification handler code here
	if (m_nClients > 0) {
		switch(m_ClientList.GetCount()){
			case 0	: break ;
			case 1	: m_ClientList.SetSel(0, TRUE); break;
			default : m_ClientList.SelItemRange(TRUE, 0, m_ClientList.GetCount() - 1);
		}
	}
	OnSelchangeClientlist();
}

void CControlDlg::OnClearselection() 
{
	// TODO: Add your control notification handler code here
	if (m_nClients > 0) {
		switch(m_ClientList.GetCount()){
			case 0	: break ;
			case 1	: m_ClientList.SetSel(0, FALSE); break;
			default : m_ClientList.SelItemRange(FALSE, 0, m_ClientList.GetCount() - 1);
		}
	}
	OnSelchangeClientlist();
}

void CControlDlg::OnSelchangeClientlist() 
{
	// TODO: Add your control notification handler code here
	m_nSelClients = m_ClientList.GetSelItems(m_nClients, m_pnSelClients);

	// When selection changes, update 'start' and 'stop' buttons states
	int c, t;
	BOOL running_flag = FALSE, notrunning_flag = FALSE; 
	for (c = 0 ; c < m_nSelClients ; c ++){
		t = ListGetIndex(m_pnSelClients[c]);
		if (m_pTestClient[t]->m_bRunning) {
			running_flag = TRUE;
		}
		else {
			notrunning_flag = TRUE;
		}
	}
	GetDlgItem(IDC_PAUSE)->EnableWindow(running_flag);
	GetDlgItem(IDC_START)->EnableWindow(notrunning_flag);
}

int CControlDlg::ListGetIndex(int index)
{
	//Returns the client index from the CListBox corresponding to the list box entry.
	CString str;

	m_ClientList.GetText(index, str);
	if (str.GetLength() == 0){
		str.Format("Invalid Index number, %d", index);
		AfxMessageBox((LPCTSTR)str);
		return -1;
	}
	else
		return atoi((LPCTSTR)str.Left(3));
}

int CControlDlg::GetNextIndex()
{
	int c = 0;
	//Look for NULL (= available) pointer, limited to MAXCLIENTS
	while (c < MAXCLIENTS && m_pTestClient[c] != NULL) {c++;};
	if (c == MAXCLIENTS) return -1;
	else return c;
}

int CControlDlg::ListGetEntry(int index)
{
	//Finds the entry in the CListBox corresponding to the client index number.
	CString str;
	int t;

	str.Format("%3d", index);
	t = m_ClientList.FindString(0, str);
	if (t == LB_ERR){
		str.Format("Failed to find string. Index = %d", index);
		AfxMessageBox(str);
		return -1;
	}
	else
		return t;
}

LRESULT CControlDlg::OnApply (WPARAM wParam, LPARAM lParam) {
	Header * h = (Header*)lParam;
	//Configure clients if apply is clicked
	for (int c = 0 ; c < m_nSelClients ; c++) {
		int t = ListGetIndex(m_pnSelClients[c]);
		m_pTestClient[t]->m_header.testfilepath = h->testfilepath;
		m_pTestClient[t]->m_header.maxnum = h->maxnum;
		m_pTestClient[t]->m_header.maxlength = h->maxlength;
		m_pTestClient[t]->m_header.maxdelay = h->maxdelay;
		m_pTestClient[t]->m_header.pcwrite = h->pcwrite;
		m_pTestClient[t]->m_header.pcrand = h->pcrand;
		m_pTestClient[t]->m_header.verbose = h->verbose;
		m_pTestClient[t]->m_header.sharing = h->sharing;
		m_pTestClient[t]->m_header.changed = TRUE;
		m_pTestClient[t]->m_bCPflag = TRUE;
		//Added block size and direct IO for each client
		m_pTestClient[t]->m_header.directio = h->directio;
		for (int i = 1 ; i < MAXBLOCKTYPE ; i++) {
			m_pTestClient[t]->m_header.lstblocksize[i] = h->lstblocksize[i];
		}
	}
	return 0;
}


void CControlDlg::ProcessData(CString s, int i)
{
	//TODO: modify function so that behaves as normal when msmeters send "reports" like before, or while it's the first report.

	if (s.GetLength() == 0) return;

	//executed if its only the "classic" way of updating or 1st line
	if (s.GetAt(0) != ':') {

		CString str;
		if (m_SaveResults && outfile.m_hFile != CFile::hFileNull) {
			str.Format("%3d,%s\n", i, (LPCTSTR)s);
			if(m_fProcessData) {		//Test Flag to start and stop processing 
				outfile.Write((LPCTSTR)str, str.GetLength());
			}
		}

		// Parse the string
		// eg. - "  0,test002,0,357,64,391.836525,1.849120"
		// index (3 digit fixed), filename (variable length string), mode (0,1,2,3), total length (Kbyte, DWORD),
		// block length (Kbyte, int), transfer rate (Mbytes/s, double), latency (ms, double)
		//there is some other data behind, the array of block timestamps
		// The index is already passed as a parameter
		//CString file;
		int mode, blocksize, t1, t2;
		DWORD length;
		double rate, latency;

		t1 = s.Find(',');
		t2 = s.Find(',', t1 + 1);
		mode = atoi((LPCTSTR)s.Mid(t1 + 1, t2 - t1 - 1));

		t1 = t2;
		t2 = s.Find(',', t1 + 1);
		length = atol((LPCTSTR)s.Mid(t1 + 1, t2 - t1 - 1));

		t1 = t2;
		t2 = s.Find(',', t1 + 1);
		blocksize = atoi((LPCTSTR)s.Mid(t1 + 1, t2 - t1 - 1));

		t1 = t2;
		t2 = s.Find(',', t1 + 1);							//This should be the last ','
		rate = atof((LPCTSTR)s.Mid(t1 + 1, t2 - t1 - 1));

		latency = atof((LPCTSTR)s.Mid(t2 + 1));

		/*
		t1 = t2;
		t2 = s.Find(',', t1 + 1);							//This should be the last ',' new comment: not anymore
		rate = atof((LPCTSTR)s.Mid(t1 + 1, t2 - t1 - 1));

		t1 = t2;											//new comment: this should be the last ','
		t2 = s.Find(',', t1 + 1);

		latency = atof((LPCTSTR)s.Mid(t1 + 1, t2 - t1 - 1));
		*/

		//there still is data after that, in the form of ",timestamp:timestamp:timestamp"

		//str.Format("%s, %d, %ld, %d, %f, %f",(LPCTSTR)s, mode, length, blocksize, rate, latency);
		//AfxMessageBox(str);
		UpdateDisplay(rate, latency, blocksize, length);
	
		if(m_fProcessData) {		//Test Flag to start and stop processing 
			m_Data.AddResult(mode, length, blocksize, rate, latency);
		}

		
	} else {
		//here we have received a string starting with ':', means we'll just write it in the csv file, without
		//updating any values on the CtrlApp screen
		//executed when receiving the timestamp or the delta times 
		CString str;
		if (m_SaveResults && outfile.m_hFile != CFile::hFileNull) {
			str.Format("%3d,%s\n", i, (LPCTSTR)s);
			if(m_fProcessData) {		//Test Flag to start and stop processing 
				outfile.Write((LPCTSTR)str, str.GetLength());
			}
		}
	}
	//do we need to left this code to be executed even when only transferring timestamps?
	//1st check, leave it as it is
	m_pTestClient[i]->m_bResponding = TRUE;
	if((m_nStartAveraging % (m_nSelClients * 10)) == 0) {
		CheckForResponse();
	}

	
}

void CControlDlg::UpdateDisplay(double rate, double latency, int blocksize, DWORD length )
{
	DWORD nbytes = blocksize * (length / blocksize);	// = number of bytes in whole blocks
	double time = (double)nbytes / rate;				// = time for transfer
	time += latency;									// = total time, including latency
	double effrate = (double)nbytes / time;				// effective rate = number of bytes / total time

	if(m_nStartAveraging < 20) {						// This gives a faster startup
		m_fRate = m_fRate * 0.9 + rate * 0.1;
		m_fEffRate = m_fEffRate * 0.9 + effrate * 0.1;
		m_fLatency = m_fLatency * 0.9 + latency * 0.1;
	}
	else {
		m_fRate = m_fRate * 0.99 + rate * 0.01;
		m_fEffRate = m_fEffRate * 0.99 + effrate * 0.01;
		m_fLatency = m_fLatency * 0.99 + latency * 0.01;
	}
	if (++m_nStartAveraging > 65000) m_nStartAveraging = 20;
	m_fMaxLatency = __max(m_fMaxLatency, latency / 1000);		//Max latency stored in seconds
}

void CControlDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	// Clock ticks and timers - I need something that will update display
	// at regular intervals. This will need some more member variables
	// - for average and maximum values and the update rate.

	if(m_nStartAveraging == 0) return;						//This is a handy indication of whether the system

	double aggregate_rate = m_fRate * m_nSelClients;
	double aggregate_eff_rate = m_fEffRate * m_nSelClients;

	// Set scale for transfer rate and write data
	if (aggregate_rate < 1.0) {
		((CStatic*)GetDlgItem(IDC_TRH))->SetWindowText("1.0");
		((CProgressCtrl*)GetDlgItem(IDC_DATARATE))->SetPos((int)(aggregate_rate * 100.0));
	}
	else {
		if (aggregate_rate < 10.0) {
			((CStatic*)GetDlgItem(IDC_TRH))->SetWindowText("10");
			((CProgressCtrl*)GetDlgItem(IDC_DATARATE))->SetPos((int)(aggregate_rate * 10.0));
		}
		else {
			if (aggregate_rate < 100.0) {
				((CStatic*)GetDlgItem(IDC_TRH))->SetWindowText("100");
				((CProgressCtrl*)GetDlgItem(IDC_DATARATE))->SetPos((int)aggregate_rate);
			}
			else {
				((CStatic*)GetDlgItem(IDC_TRH))->SetWindowText("1000");
				((CProgressCtrl*)GetDlgItem(IDC_DATARATE))->SetPos((int)(aggregate_rate / 10.0));
			}
		}
	}

	// Set scale for effective transfer rate and write data
	if (aggregate_eff_rate < 1.0) {
		((CStatic*)GetDlgItem(IDC_TRH2))->SetWindowText("1.0");
		((CProgressCtrl*)GetDlgItem(IDC_DATARATE2))->SetPos((int)(aggregate_eff_rate * 100.0));
	}
	else {
		if (aggregate_eff_rate < 10.0) {
			((CStatic*)GetDlgItem(IDC_TRH2))->SetWindowText("10");
			((CProgressCtrl*)GetDlgItem(IDC_DATARATE2))->SetPos((int)(aggregate_eff_rate * 10.0));
		}
		else {
			if (aggregate_eff_rate < 100.0) {
				((CStatic*)GetDlgItem(IDC_TRH2))->SetWindowText("100");
				((CProgressCtrl*)GetDlgItem(IDC_DATARATE2))->SetPos((int)aggregate_eff_rate);
			}
			else {
				((CStatic*)GetDlgItem(IDC_TRH2))->SetWindowText("1000");
				((CProgressCtrl*)GetDlgItem(IDC_DATARATE2))->SetPos((int)(aggregate_eff_rate / 10.0));
			}
		}
	}


	// Set scale for average latency and write data
	if (m_fLatency < 1.0) {
		((CStatic*)GetDlgItem(IDC_ALH))->SetWindowText("1.0");
		((CProgressCtrl*)GetDlgItem(IDC_LATENCY))->SetPos((int)(m_fLatency * 100));
	}
	else {
		if (m_fLatency < 10.0) {
			((CStatic*)GetDlgItem(IDC_ALH))->SetWindowText("10");
			((CProgressCtrl*)GetDlgItem(IDC_LATENCY))->SetPos((int)(m_fLatency * 10));
		}
		else {
			if (m_fLatency < 100.0) {
				((CStatic*)GetDlgItem(IDC_ALH))->SetWindowText("100");
				((CProgressCtrl*)GetDlgItem(IDC_LATENCY))->SetPos((int)m_fLatency);
			}
			else {
				((CStatic*)GetDlgItem(IDC_ALH))->SetWindowText("1000");
				((CProgressCtrl*)GetDlgItem(IDC_LATENCY))->SetPos((int)(m_fLatency / 10.0));
			}
		}
	}

	// Set scale for maximum latency and write data
	if (m_fMaxLatency < 1) {
		((CStatic*)GetDlgItem(IDC_MLH))->SetWindowText("1");
		((CProgressCtrl*)GetDlgItem(IDC_MAXLATENCY))->SetPos((int)(m_fMaxLatency * 100));
	}
	else {
		if (m_fMaxLatency < 10) {
			((CStatic*)GetDlgItem(IDC_MLH))->SetWindowText("10");
			((CProgressCtrl*)GetDlgItem(IDC_MAXLATENCY))->SetPos((int)(m_fMaxLatency * 10));
		}
		else {
			((CStatic*)GetDlgItem(IDC_MLH))->SetWindowText("100");
			((CProgressCtrl*)GetDlgItem(IDC_MAXLATENCY))->SetPos((int)(m_fMaxLatency));
		}
	}

	CString str;

			
	str.Format("%5.1f", aggregate_rate);
	((CStatic*)GetDlgItem(IDC_RATE))->SetWindowText(str);

		str.Format("%5.1f", aggregate_eff_rate);
	((CStatic*)GetDlgItem(IDC_RATE2))->SetWindowText(str);

	str.Format("%5.1f", m_fLatency);
	((CStatic *)GetDlgItem(IDC_AVLAT))->SetWindowText(str);

	str.Format("%4.2f", m_fMaxLatency);
	((CStatic *)GetDlgItem(IDC_MAXLAT))->SetWindowText(str);

	CDialog::OnTimer(nIDEvent);
}



void CControlDlg::CheckForResponse()
{
	/* 
	Check all running clients for recent response, using the CTestClient m_bResponding flag.
	This flag must be cleared on starting and set by any returned result.
	If any client flag is not set when the test is carried out then the message text is 
	displayed, that client is set as non-running and removed from the registered client list.
	The selected client list is updated.

	It can't be managed for clients not running in 'verbose' mode. */
	CString str;
	int t, c, flag = 0;
	for (c = 0 ; c < m_nSelClients ; c++) {	
		t = ListGetIndex(m_pnSelClients[c]);
		if (m_pTestClient[t]->m_header.verbose && m_pTestClient[t]->m_bRunning && !m_pTestClient[t]->m_bResponding ) {
			str.Format("Warning - client index %d may not be responding.", t);
			SetDlgItemText(IDC_NORESPONSE, (LPCTSTR)str);
			// Remove client from list
			//m_ClientList.DeleteString(ListGetEntry(t));
			// remove the test client
			//delete m_pTestClient[t];
			flag = 1;
			}
		m_pTestClient[t]->m_bResponding = FALSE;
	}
	if (!flag) {
		str.Format("");
		SetDlgItemText(IDC_NORESPONSE, (LPCTSTR)str);
	}

	//Update selected client list;
	//m_nSelClients = m_ClientList.GetSelItems(m_nClients, m_pnSelClients);
}

void CControlDlg::OnRemove() 
{
	// TODO: Add your control notification handler code here
	if ((m_nSelClients = m_ClientList.GetSelItems(m_nClients, m_pnSelClients)) == 0) {
		AfxMessageBox("No clients selected.");
		return;
	}

	int c, t, tstop = m_nSelClients;

	for (c = 0 ; c < tstop ; c++) {
		t = ListGetIndex(m_pnSelClients[c]);
		if (m_pTestClient[t]->m_pSrvrSock) {
			StopClient(t);
		}
		DestroyClient(t);
		m_nSelClients = m_ClientList.GetSelItems(m_nClients, m_pnSelClients);
		tstop = m_nSelClients;		// New upper limit on loop
		c--;						// set counter back one to catch the next valid client
	}

	if (m_nClients == 0){
		GetDlgItem(IDC_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_PAUSE)->EnableWindow(FALSE);
	}
}


// Script comtrol panel button clicked
void CControlDlg::OnScriptControlpanel() 
{

	// Must have selected client(s) before configuring script
	if ((m_nSelClients = m_ClientList.GetSelItems(m_nClients, m_pnSelClients)) == 0) {
		// If no clients selected output message and do nothing
		AfxMessageBox("No clients selected.");
		return;
	}


	// Open script window pointing at script file path
	SPSheet ps(_T("Script application controls"));
	ps.m_controlpage.m_ScriptFilePath = m_sScriptFilePath;												//Set default or current script file path
	ps.m_controlpage.m_SelectedClientCount = m_ClientList.GetSelItems(m_nClients, m_pnSelClients);		//Pass the number of selected clients
	ps.m_psh.dwFlags &= ~(PSH_HASHELP);


		int nResponse = ps.DoModal();
	
	if (nResponse == IDOK) {
		// TODO: Place code here to handle when the dialog is dismissed with OK
		// Store returned script parameters
		m_sScriptFilePath = ps.m_controlpage.m_ScriptFilePath;
		m_sScriptLineCount = ps.m_controlpage.m_ScriptLineCount;
		m_sScriptedClientCount = ps.m_controlpage.m_ScriptedClientCount;

		if (m_sScriptLineCount > 0 )	//Enable the Scipt Start button if script loaded (if script lines in file)
		{
			GetDlgItem(IDC_START2)->EnableWindow(TRUE);
			GetDlgItem(IDC_PAUSE2)->EnableWindow(FALSE);
			
			// Load the client Id, cammand type and command scripts
			m_sScriptLineArray.Copy(ps.m_controlpage.m_ScriptCommandArray); //Copy the script array from the script control pannel
			
			for (int i = 0 ; i < m_sScriptLineCount ; i++)	{
				m_sCommandType[i] = ps.m_controlpage.m_CommandType[i];
				m_sClientId[i] = ps.m_controlpage.m_ClientId[i];
				m_sCommandValue[i] = ps.m_controlpage.m_CommandValue[i];
			}
			m_fScriptRunning = TRUE;
			m_fGetNextScriptCommand = TRUE;
		}
	}
}



//Script Control Start button clicked
void CControlDlg::OnScriptStart() 
{
	// TODO: Add your control notification handler code here
	// AfxMessageBox("Script Start control clicked");
	// Now limited to selected clients only.
	
	m_fScriptRunning = TRUE;
	// Disable script start
	GetDlgItem(IDC_START2)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAUSE2)->EnableWindow(TRUE);
	// Disable normal test control start, stop
	GetDlgItem(IDC_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_PAUSE)->EnableWindow(FALSE);

	// Script must contain script lines and scripted clients < Number of selected clients, scripted clients are counted from 0
	if (m_sScriptLineCount > 0 && m_sScriptedClientCount < m_ClientList.GetSelItems(m_nClients, m_pnSelClients))
	{
		DoSendOp(m_sScriptFilePath, -2);	//Send script file path to the CEdit output window

		
		AfxBeginThread(my_callback, this);	//Call the process for running script in new thread
	
	}
	else
	{

		CString str;
		str.Format("Script file was not valid\n %d client(s) are scripted \n %d client(s) are selected\n %d script lines", (m_sScriptedClientCount+1), m_ClientList.GetSelItems(m_nClients, m_pnSelClients), m_sScriptLineCount);
		AfxMessageBox((LPCTSTR)str);
	}
}



void CControlDlg::OnScriptPause()
{
	CString ststr;
	// Enable script start
	GetDlgItem(IDC_START2)->EnableWindow(TRUE);
	GetDlgItem(IDC_PAUSE2)->EnableWindow(FALSE);
	// Enable test control normal start
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_PAUSE)->EnableWindow(FALSE);
	
	m_fScriptRunning = FALSE;
	ststr = "Scripted stopped before completion";
	DoSendOp(ststr, -2); 
	
	// Pause any running clients
	m_strSend = "pause";
	int t, tstop = m_nSelClients;

	for (int c = 0 ; c < tstop ; c++) {
		t = ListGetIndex(m_pnSelClients[c]);
		if (m_pTestClient[t]->m_bRunning) {
			if (m_pTestClient[t]->m_pSrvrSock) {
				// put the stop stuff in here
				m_pTestClient[t]->Send(m_strSend);
				DoSendOp(m_strSend, t);
				m_pTestClient[t]->m_bRunning = FALSE;
				//flag = 1;
			}
			else {
				// Here if it finds the socket not open.
				/* This loop can work up to a certain point then fail. Same
				as 'start' command */
				CString str;
				str.Format("Error - client socket, Index %d not open.\nClient #%d will be removed from list.", t, t);
				AfxMessageBox((LPCTSTR)str);
				DestroyClient(t);
				m_nSelClients = m_ClientList.GetSelItems(m_nClients, m_pnSelClients);
				tstop = m_nSelClients;		// New upper limit on loop
				c--;						// set counter back one to catch the next valid client
			}
		}
	}

	//Stop listening, save and close the results file

	m_fProcessData = FALSE;		//Flag to start and stop processing of data

	if (outfile.m_hFile != CFile::hFileNull) {
		if (m_SaveSummary) m_Data.Printout(&outfile);
				outfile.Close();
	}
	KillTimer(1);
	m_nStartAveraging = 0;	
	GetDlgItem(IDC_REMOVE)->EnableWindow(TRUE);

}



void CControlDlg::RunningScript()
{

	CString scriptstr;
	int ScriptIndex = 0;
	int CommandType = 0;
	int ClientId = -1;
	int CommandValue = 0;
	int Duration = 0;
	int LastDuration = 0;
	CTime timeStart = 0;
	UINT_PTR ScriptTimerID = 0;

	//Run script one line at a time	
	while (ScriptIndex < m_sScriptLineCount && m_fScriptRunning)
	{
		scriptstr = m_sScriptLineArray.GetAt(ScriptIndex);
		CommandType = m_sCommandType[ScriptIndex];
		CommandValue = m_sCommandValue[ScriptIndex];
		ClientId = m_sClientId[ScriptIndex];

		
		switch (CommandType) {
				
			case 0 : //Comment
				DoSendOp(scriptstr, -2); 
			break;
		
			case 1 : //Results - Open a results file to save data

				m_sOutFileName = scriptstr;
			
				if (outfile.m_hFile == CFile::hFileNull) {
					CFileException e;
					if(!outfile.Open(m_sOutFileName, CFile::modeCreate | CFile::modeWrite | CFile::typeBinary | CFile::shareDenyWrite, &e)) {
						CString str;
						str.Format("Warning - failed to open output file.\n\n");
						switch (e.m_cause){
							case CFileException::fileNotFound : str += "The file could not be located." ; break ;
							case CFileException::badPath : str += "All or part of the path is invalid." ; break ;
							case CFileException::tooManyOpenFiles : str += "The permitted number of open files was exceeded." ; break ;
							case CFileException::accessDenied : str += "The file could not be accessed." ; break ;
							case CFileException::invalidFile : str += "There was an attempt to use an invalid file handle." ; break ;
							case CFileException::removeCurrentDir : str += "The current working directory cannot be removed." ; break ;
							case CFileException::directoryFull : str += "There are no more directory entries." ; break ;
							case CFileException::badSeek : str += "There was an error trying to set the file pointer." ; break ;
							case CFileException::hardIO : str += "There was a hardware error." ; break ;
							case CFileException::sharingViolation : str += "SHARE.EXE was not loaded, or a shared region was\n locked. (Is the file open in another application?)." ; break ;
							case CFileException::lockViolation : str += "There was an attempt to lock a region that was already locked." ; break ;
							case CFileException::diskFull : str += "The disk is full." ; break ;
							case CFileException::endOfFile : str += "The end of file was reached." ; break ;
							default : str += "An unspecified error occurred." ; break ;
						}
						str += "\n\nResults will not be saved.";
						AfxMessageBox(str);
					}
					DoSendOp(scriptstr, ClientId); 
				}				
			break;
				
			case 2 : //Start
				
				m_fProcessData = TRUE;		//Flag to start and stop processing of data
				m_strSend = "start";
					if (!m_pTestClient[ClientId]->m_bRunning) {
						if (m_pTestClient[ClientId]->m_pSrvrSock) {  
							// Here if it finds the socket open
							// put the start stuff in here
							//m_pTestClient[ClientId]->SendHeader(m_bCreateflag, m_dCreate);

							if (!m_bCreateflag) {
								m_pTestClient[ClientId]->Send(m_strSend);
								m_pTestClient[ClientId]->m_bRunning = TRUE;
								m_pTestClient[ClientId]->m_bResponding = FALSE;
								//flag = 1;
							}
							m_bCreateflag = FALSE;
						}
						else {  
							// Here if it finds the socket not open.
							/* This loop can work up to a certain point then fail. The ones already dealt with
							don't need doing again. The loop resets the selected list and continues up to the next
							failure or the end. */
							CString str;
							str.Format("Error - client socket, Index %d not open.\nClient #%d will be removed from list.", ClientId, ClientId);
							AfxMessageBox((LPCTSTR)str);
							DestroyClient(ClientId);
							m_nSelClients = m_ClientList.GetSelItems(m_nClients, m_pnSelClients);
						}
	
					}


					if (!SetTimer(1, update_time[m_nUpdateTime] * 1000, NULL)){
						AfxMessageBox("System failure - cannot set a valid timer");
					}
	
					// Initialise display parameters
					m_fRate = 0.0;
					m_fEffRate = 0.0;
					m_fLatency = 0.0;
					m_fMaxLatency = 0.0;
					m_nStartAveraging = 0;
					m_Data.Clear();
					// Zero display windows
					((CProgressCtrl*)GetDlgItem(IDC_DATARATE))->SetPos(0);
					((CProgressCtrl*)GetDlgItem(IDC_DATARATE2))->SetPos(0);
					((CProgressCtrl*)GetDlgItem(IDC_LATENCY))->SetPos(0);
					((CProgressCtrl*)GetDlgItem(IDC_MAXLATENCY))->SetPos(0);
					((CStatic*)GetDlgItem(IDC_RATE))->SetWindowText("");
					((CStatic*)GetDlgItem(IDC_RATE2))->SetWindowText("");
					((CStatic *)GetDlgItem(IDC_AVLAT))->SetWindowText("");
					((CStatic *)GetDlgItem(IDC_MAXLAT))->SetWindowText("");
					GetDlgItem(IDC_REMOVE)->EnableWindow(FALSE);
				
					DoSendOp(scriptstr, ClientId); 
			break;

			case 3 : //Pause
					
					m_strSend = "pause";
					//int flag = 0;
					//int t, tstop = m_nSelClients;
					//t = ListGetIndex(m_pnSelClients[c]);
					if (m_pTestClient[ClientId]->m_bRunning) {
						if (m_pTestClient[ClientId]->m_pSrvrSock) {
							// put the stop stuff in here
							m_pTestClient[ClientId]->Send(m_strSend);
							m_pTestClient[ClientId]->m_bRunning = FALSE;
							//flag = 1;
						}
						else {
							// Here if it finds the socket not open.
							/* This loop can work up to a certain point then fail. Same
							as 'start' command */
							CString str;
							str.Format("Error - client socket, Index %d not open.\nClient #%d will be removed from list.", ClientId, ClientId);
							AfxMessageBox((LPCTSTR)str);
							DestroyClient(ClientId);
							m_nSelClients = m_ClientList.GetSelItems(m_nClients, m_pnSelClients);
							//tstop = m_nSelClients;		// New upper limit on loop
							//c--;						// set counter back one to catch the next valid client
						}
					}


					// Stop listening, save data and close file  - only after last scripted client is paused

					if (ClientId == m_sScriptedClientCount) {

						m_fProcessData = FALSE;		//Flag to start and stop processing of data

						if (outfile.m_hFile != CFile::hFileNull) {
							if (m_SaveSummary) m_Data.Printout(&outfile);
								outfile.Close();
						}
	
						KillTimer(1);
						m_nStartAveraging = 0;
						GetDlgItem(IDC_REMOVE)->EnableWindow(TRUE);
					}

					DoSendOp(scriptstr, ClientId);
			break;

			case 4 : //Time
				DoSendOp(scriptstr, ClientId);
				timeStart = CTime::GetCurrentTime();
				Duration = 0;
				LastDuration = 0;
				
				while ((CTime::GetCurrentTime() - timeStart) < CommandValue && m_fScriptRunning )
				{ 
					// wait and update the run Command Progress bar
					time_t CTimeStart = timeStart.GetTime(); // converted timeStart into time_t type, to be used as integer
					time_t CTimePos = CTime::GetCurrentTime().GetTime();
					Duration = CTimePos - CTimeStart; 
					((CProgressCtrl*)GetDlgItem(IDC_RUNCOMMAND))->SetPos((int)(Duration * 100 / CommandValue));
					
					//Uppdate the counter when duration changes (every second)
					if (Duration != LastDuration) {
						CString timestr;
						timestr.Format("%d", Duration);
						((CStatic *)GetDlgItem(IDC_PULSE))->SetWindowText(timestr);
						LastDuration = Duration;
					}
				}
				((CProgressCtrl*)GetDlgItem(IDC_RUNCOMMAND))->SetPos(100);
				((CStatic *)GetDlgItem(IDC_PULSE))->SetWindowText("");
			break;
			case 5 : //Command -p ...
					m_strSend = scriptstr;
					if (!m_pTestClient[ClientId]->m_bRunning) {
						if (m_pTestClient[ClientId]->m_pSrvrSock) {  
							// Here if it finds the socket open
							// put the start stuff in here
							//m_pTestClient[ClientId]->SendHeader(m_bCreateflag, m_dCreate);

							if (!m_bCreateflag) {
								m_pTestClient[ClientId]->Send(m_strSend);
								m_pTestClient[ClientId]->m_bRunning = FALSE;
								m_pTestClient[ClientId]->m_bResponding = FALSE;
								//flag = 1;
							}
							m_bCreateflag = FALSE;
						}
						else {  
							// Here if it finds the socket not open.
							/* This loop can work up to a certain point then fail. The ones already dealt with
							don't need doing again. The loop resets the selected list and continues up to the next
							failure or the end. */
							CString str;
							str.Format("Error - client socket, Index %d not open.\nClient #%d will be removed from list.", ClientId, ClientId);
							AfxMessageBox((LPCTSTR)str);
							DestroyClient(ClientId);
							m_nSelClients = m_ClientList.GetSelItems(m_nClients, m_pnSelClients);
						}
	
					}

					DoSendOp(scriptstr, ClientId);
			break;
			case 6 : //Error
				DoSendOp(scriptstr, -2); 
			break;
		}

		ScriptIndex++;
	}

	//Finsihed script or stopped script, enable start buttons
	GetDlgItem(IDC_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_START2)->EnableWindow(TRUE);
	GetDlgItem(IDC_PAUSE2)->EnableWindow(FALSE);

	//Clear the running script bar
	((CProgressCtrl*)GetDlgItem(IDC_RUNCOMMAND))->SetPos(0);
	((CStatic *)GetDlgItem(IDC_PULSE))->SetWindowText("");

}
