// Control.cpp : implementation file
// Client profile selection and scripted client selection windows

#include "stdafx.h"
#include "CtrlApp.h"
#include "Control.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControl property page - individual client control

IMPLEMENT_DYNCREATE(CControl, CPropertyPage)

CControl::CControl() : CPropertyPage(CControl::IDD)
{
	//{{AFX_DATA_INIT(CControl)
	m_sTestFilePath = "";
	m_bVerbose = TRUE;
	m_nPCwrite = 10;
	m_nPCrandom = 50;
	m_nMaxnum = 0;
	m_dMaxlength = 1024;
	m_nMaxdelay = 0;
	m_bCreateflag = FALSE;
	m_bSharing = FALSE;
	m_nCreate = 0;
	//Added for MS Meter 2
	m_bBlockSize[1] = TRUE;
	m_bBlockSize[2] = TRUE;
	m_bBlockSize[3] = TRUE;
	m_bBlockSize[4] = TRUE;
	m_bBlockSize[5] = TRUE;
	m_bBlockSize[6] = TRUE;
	m_bBlockSize[7] = TRUE;
	m_bBlockSize[8] = FALSE;
	m_bBlockSize[9] = FALSE;
	m_bBlockSize[10] = FALSE;
	m_bBlockSize[11] = FALSE;
	m_bDirectIO = TRUE;
	//}}AFX_DATA_INIT

	//Additions of MSMeter 3
	m_bPerFrame = FALSE;
	m_dFrameSize = 65536;
	m_bNTP = FALSE;
	m_sNTPAddress = "";
}

CControl::~CControl()
{
}

void CControl::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CControl)
	DDX_Text(pDX, IDC_TESTFILEPATH, m_sTestFilePath);
	DDV_MaxChars(pDX, m_sTestFilePath, 255);
	DDX_Check(pDX, IDC_VERBOSE, m_bVerbose);
	DDX_Text(pDX, IDC_WRITE, m_nPCwrite);
	DDV_MinMaxInt(pDX, m_nPCwrite, 0, 100);
	DDX_Text(pDX, IDC_RANDOM, m_nPCrandom);
	DDV_MinMaxInt(pDX, m_nPCrandom, 0, 100);
	DDX_Text(pDX, IDC_NOOFFILES, m_nMaxnum);
	DDV_MinMaxInt(pDX, m_nMaxnum, 0, MAXNOFILES);
	DDX_Text(pDX, IDC_MAXLENGTH, m_dMaxlength);
	DDV_MinMaxDWord(pDX, m_dMaxlength, 2, MAXLENGTH);
	DDX_Text(pDX, IDC_MAXDELAY, m_nMaxdelay);
	DDV_MinMaxInt(pDX, m_nMaxdelay, 0, MAXDELAY);
	DDX_Check(pDX, IDC_CREATEFILES, m_bCreateflag);
	//DDX_Check(pDX, IDC_SHARING, m_bSharing);
	DDX_Text(pDX, IDC_NCREATE, m_nCreate);
	DDV_MinMaxDWord(pDX, m_nCreate, 0, 40960);
	//MS Meter 2 added block size and direct IO check box values
	DDX_Check(pDX, IDC_BLOCKSIZE1, m_bBlockSize[1]);
	DDX_Check(pDX, IDC_BLOCKSIZE2, m_bBlockSize[2]);
	DDX_Check(pDX, IDC_BLOCKSIZE3, m_bBlockSize[3]);
	DDX_Check(pDX, IDC_BLOCKSIZE4, m_bBlockSize[4]);
	DDX_Check(pDX, IDC_BLOCKSIZE5, m_bBlockSize[5]);
	DDX_Check(pDX, IDC_BLOCKSIZE6, m_bBlockSize[6]);
	DDX_Check(pDX, IDC_BLOCKSIZE7, m_bBlockSize[7]);
	DDX_Check(pDX, IDC_BLOCKSIZE8, m_bBlockSize[8]);
	DDX_Check(pDX, IDC_BLOCKSIZE9, m_bBlockSize[9]);
	DDX_Check(pDX, IDC_BLOCKSIZE10, m_bBlockSize[10]);
	DDX_Check(pDX, IDC_BLOCKSIZE11, m_bBlockSize[11]);
	DDX_Check(pDX, IDC_DIRECTIO, m_bDirectIO);
	//MS Meter 3 added ntp time stamp and Buffer measurements
	DDX_Check(pDX, IDC_PERFRAMELA, m_bPerFrame);
	DDX_Text(pDX, IDC_FRAMESIZEB, m_dFrameSize);
	DDX_Text(pDX, IDC_NTPADDRESS, m_sNTPAddress);
	DDX_Check(pDX, IDC_NTP, m_bNTP);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CControl, CPropertyPage)
	//{{AFX_MSG_MAP(CControl)
	ON_EN_CHANGE(IDC_TESTFILEPATH, OnChange)
	ON_EN_CHANGE(IDC_NOOFFILES, OnChange)
	ON_EN_CHANGE(IDC_MAXLENGTH, OnChange)
	ON_EN_CHANGE(IDC_MAXDELAY, OnChange)
	ON_EN_CHANGE(IDC_WRITE, OnChange)
	ON_EN_CHANGE(IDC_RANDOM, OnChange)
	ON_BN_CLICKED(IDC_CREATEFILES, OnChange)
	ON_EN_CHANGE(IDC_NCREATE, OnChange)
	ON_BN_CLICKED(IDC_VERBOSE, OnChange)
	ON_BN_CLICKED(IDC_TESTFILE, OnTestfile)
	//MS Meter 2 added block size and direct IO check box values
	ON_BN_CLICKED(IDC_BLOCKSIZE1, OnChange)
	ON_BN_CLICKED(IDC_BLOCKSIZE2, OnChange)
	ON_BN_CLICKED(IDC_BLOCKSIZE3, OnChange)
	ON_BN_CLICKED(IDC_BLOCKSIZE4, OnChange)
	ON_BN_CLICKED(IDC_BLOCKSIZE5, OnChange)
	ON_BN_CLICKED(IDC_BLOCKSIZE6, OnChange)
	ON_BN_CLICKED(IDC_BLOCKSIZE7, OnChange)
	ON_BN_CLICKED(IDC_BLOCKSIZE8, OnChange)
	ON_BN_CLICKED(IDC_BLOCKSIZE9, OnChange)
	ON_BN_CLICKED(IDC_BLOCKSIZE10, OnChange)
	ON_BN_CLICKED(IDC_BLOCKSIZE11, OnChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControl message handlers


void CControl::OnChange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	//SetModified(TRUE);
}

void CControl::OnTestfile() 
{
	// TODO: Add your control notification handler code here
	// Old selection dialog box method - this requires election of a valid
	// file from the list - impossible if the files don't exist!
	/*
	CFileDialog dlg (TRUE, NULL, NULL, OFN_PATHMUSTEXIST);
	dlg.m_ofn.lpstrInitialDir = (LPCTSTR)m_sTestFilePath;
	dlg.m_ofn.lpstrTitle = "Select test file directory (select one file)";

	if ((dlg.DoModal() == IDOK)) {
		m_sTestFilePath = dlg.GetPathName();
		m_sTestFilePath = m_sTestFilePath.Left(dlg.m_ofn.nFileOffset - 1);
		//AfxMessageBox((LPCTSTR)m_sTestFilePath);
		((CEdit*)GetDlgItem(IDC_TESTFILEPATH))->SetWindowText(m_sTestFilePath);
		SetModified(TRUE);
	}
	*/

	// New selection dialog box method
	BROWSEINFO bi;
	memset((LPVOID)&bi, 0, sizeof(bi));
	TCHAR szDisplayName[_MAX_PATH];
	szDisplayName[0] = '\0';
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDisplayName;
	bi.lpszTitle = _T("Select test file folder");
	bi.ulFlags = BIF_RETURNONLYFSDIRS;

	LPITEMIDLIST pIIL = ::SHBrowseForFolder(&bi);
	
	BOOL bRet = ::SHGetPathFromIDList(pIIL, (char*)((LPCTSTR)m_sTestFilePath));
	if (bRet){
		((CEdit*)GetDlgItem(IDC_TESTFILEPATH))->SetWindowText(m_sTestFilePath);
		SetModified(TRUE);

		LPMALLOC pMalloc;
		HRESULT hr = SHGetMalloc(&pMalloc);
		pMalloc->Free(pIIL);
		pMalloc->Release();
	}
}




/////////////////////////////////////////////////////////////////////////////
// Script Control SControl property page - scripted clients

IMPLEMENT_DYNCREATE(SControl, CPropertyPage)

SControl::SControl() : CPropertyPage(SControl::IDD)
{
	//{{AFX_DATA_INIT(CControl)
	//m_ScriptFilePath = "c:\\Users\\rduser";
	//m_ScriptFileName = "\\script.txt";
}

SControl::~SControl()
{
}

//Data passing for Script control panel
void SControl::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CControl)
	DDX_Text(pDX, IDC_TESTFILEPATH, m_ScriptFilePath); 
	DDX_Control(pDX, IDC_SENDOUT2, m_SendScriptView);
	//Added for MS Meter 2
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SControl, CPropertyPage)
	//{{AFX_MSG_MAP(CControl)
	ON_EN_CHANGE(IDC_TESTFILEPATH, OnChange)
	ON_BN_CLICKED(IDC_TESTFILE, OnScriptfile)
	ON_BN_CLICKED(IDOK, OnLoad)
	//Added for MS Meter 2
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// SControl Message Handlers
void SControl::OnChange() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CPropertyPage::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	//SetModified(TRUE);
	
}


//Browse window list box for selecting script file
void SControl::OnScriptfile() 
{
	// TODO: Add your control notification handler code here
	// Old selection dialog box method - this requires election of a valid
	// file from the list - impossible if the files don't exist!

	TCHAR szFilters[] = _T ("Text Files (*.txt)|*.txt|All files (*.*)|*.*||");

	CFileDialog dlg (TRUE, _T("txt"), _T (m_ScriptFilePath), OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT, szFilters);
	dlg.m_ofn.lpstrInitialDir = (LPCTSTR)m_ScriptFilePath;
	dlg.m_ofn.lpstrTitle = "Select script file";


	if ((dlg.DoModal() == IDOK)) {
	
		m_ScriptFilePath = dlg.GetPathName();

		((CEdit*)GetDlgItem(IDC_TESTFILEPATH))->SetWindowText(m_ScriptFilePath);
		SetModified(TRUE);	
	}
	


}

//Load script file when Load button clicked
void SControl::OnLoad() 
{
	// TODO: Add your control notification handler code here
	// Intialsie script values
	int CommandType = 0;
	int CommandValue = 0;
	int ErrorCount = 0;
	int CommandCount = 0;
	int ClientId = -1;
	int MaxClientId = 0;
	CString CommandStr = "Unknown";
	CString scriptstr, teststr;

	//Clear the CListBox
	m_SendScriptView.ResetContent();
	
	//Clear script parameter arrays
	for (int i = 0 ; i < MAXSCRIPTCOMMANDS ; i++) {
		m_CommandType[i] = 0;	
		m_ClientId[i] = 0;
		m_CommandValue[i] = 0;	
	}
	//Clear script arrays
	m_ScriptLineArray.RemoveAll();
	m_ScriptCommandArray.RemoveAll();

	//Update field in case edited text directly in CEdit field
	((CEdit*)GetDlgItem(IDC_TESTFILEPATH))->GetWindowText(m_ScriptFilePath);
		
	m_ScriptLineCount = Read(m_ScriptFilePath); //Get number of lines in script file, lines are global class variable 

	if (m_ScriptLineCount > 0)
	{
		DoSendScriptView(m_ScriptFilePath, 0, 7, -1, 0);

		for (int i = 0; i < m_ScriptLineCount; i++)
		{
			scriptstr = m_ScriptLineArray.GetAt(i);				//Get line type, client id, client command and command value
			CommandType = ValidateScriptLine(scriptstr);
			ClientId = GetClientId(scriptstr, CommandType);
			CommandStr = GetClientCommand(scriptstr, CommandType);
			CommandValue = GetCommandValue(CommandStr, CommandType);

			if (CommandType == 4 && CommandValue == 0)	// if no time value, then error
				CommandType = 6; //ERROR
			
			//Store the command line, command types, commad value (for time commnad) and client Ids starting at postion 0
			m_CommandType[i] = CommandType;
			m_CommandValue[i] = CommandValue;
			m_ClientId[i] = ClientId;
			m_ScriptCommandArray.Add((LPCTSTR)CommandStr);
			
			
			if (CommandType == 6) //errors in current line, count errors
				ErrorCount++;
			
			if (CommandType > 0 && CommandType < 6) //commands in current line, count commands
				CommandCount++;

			if (ClientId > MaxClientId) //max number of clients used in script
				MaxClientId = ClientId; //Script control Max client important, results only saved after last client is paused

				//DoSendScriptView(scriptstr, (i+1), CommandType, ClientId);
				
				//teststr.Format("%s    %s", CommandStr, scriptstr);
				DoSendScriptView(CommandStr, (i+1), CommandType, ClientId, CommandValue);

		}

		// Output script summary to script output window
		DoSendScriptView("", 0, 7, -1, 0); // \r\n
		scriptstr.Format("Total Lines %d", m_ScriptLineCount);
		DoSendScriptView(scriptstr, 0, 7, -1, 0); //Total Line count
		scriptstr.Format("Total Commands %d", CommandCount);
		DoSendScriptView(scriptstr, CommandCount, 7, -1, 0); //Command count
		scriptstr.Format("Total Errors %d", ErrorCount);
		DoSendScriptView(scriptstr, ErrorCount, 7, -1, 0); //Error count
		
		m_ScriptedClientCount = MaxClientId;
		scriptstr.Format("Max SCRIPTED Clients %d", (m_ScriptedClientCount+1));
		DoSendScriptView(scriptstr, MaxClientId, 7, -1, 0); //Script Client count
		scriptstr.Format("Number SELECTED Clients %d", m_SelectedClientCount);
		DoSendScriptView(scriptstr, m_SelectedClientCount, 7, -1, 0); //selected Client count

		//Must be no errors
		if (ErrorCount > 0) 
		{
			CString str;
			str.Format("%d Script Error\n See script view and error lines.\n SCRIPT NOT LOADED", ErrorCount);
			AfxMessageBox((LPCTSTR)str);
			m_ScriptLineCount = 0; //Set script line count = 0, so no lines loaded
		}
		//Number of scripted clients and number of selected clients should be equal
		else if (m_SelectedClientCount != (m_ScriptedClientCount+1)) 
		{
			CString str;
			str.Format("Warning / Error\n %d client(s) are scripted \n %d client(s) are selected.", (m_ScriptedClientCount+1), m_SelectedClientCount);
			AfxMessageBox((LPCTSTR)str);
		}
		
	}
	else
	{
		AfxMessageBox(_T("Script file was empty"));
	}

}



// Load the script file, return number of lines and globl class array containing individual lines
int SControl::Read(CString m_ScriptFilePath)
{
	CString sLine;
	int iLineCount = 0;
	CFileException ex;
	CStdioFile StdFile;

	// clear the line array
	m_ScriptLineArray.RemoveAll();
	m_ScriptCommandArray.RemoveAll(); 

	if(!StdFile.Open(m_ScriptFilePath, CFile::modeNoTruncate | CFile::modeRead, &ex))
	{
		ex.ReportError();
		//AfxMessageBox(_T("Script file not be found"));
	}
	else
	{
		TRY
		{
			//Read 1 line at a time
			while (StdFile.ReadString(sLine))
			{
				m_ScriptLineArray.Add((LPCTSTR)sLine);
				iLineCount ++;
			}
			StdFile.Close();
		}
	
		CATCH (CFileException, e)
		{
			AfxMessageBox(_T("Script file could not be opened %d"), e->m_cause);
		}
		END_CATCH
	}

	return iLineCount;
}


//Send individual script line and line classification to script output window
void SControl::DoSendScriptView(CString str, int lineNo, int lineType, int ClientId, int CommandValue)
{
	CString opstr, msgtype;
	// = 0 (Comment) = 1 (Results file) ; = 2 (start) ; = 3 (pause) ; = 4 (time) ; 5 (Client command) ; 6 (NOT VALID)
	switch (lineType) {

	case 0 : msgtype = "Comment   ";
		break;
	case 1 : msgtype = "Results      ";
		break;
	case 2 : msgtype = "Start          ";
		break;
	case 3 : msgtype = "Pause        ";
		break;
	case 4 : msgtype = "Time          ";
		break;
	case 5 : msgtype = "Command";
		break;
	case 6 : msgtype = "ERROR     ";
		break;
	}

	if (lineType < 7) 
		opstr.Format("%d  %-9s Cmd> %d %s       %d \r\n", lineNo, msgtype, ClientId, str, CommandValue);
	else
		opstr.Format("%s \r\n", str);

	// Using CListBox to display text only
	m_SendScriptView.AddString(opstr);
	//scroll down with each new o/p
	m_SendScriptView.SetTopIndex(m_SendScriptView.GetCount() - 1);

}


//validate each script line, 1 at a time
int SControl::ValidateScriptLine(CString str)
{
// e.g.
//Cmd> -1   Results C:\Users\myuser\results.csv 
//Cmd> 0   -p /mnt/eon -n 1 -l 1024 -d 0 -w 0 -r 0 -v -f- -dio 1 -bn 7 -bs 1 2 3 4 5 6 7 0 0 0 0 
//Cmd> 0   start 
//Cmd> -1   time 10 
//Cmd> 0   pause 

	int Valid = 0;	// = 0 (Comment) = 1 (Results file) ; = 2 (start) ; = 3 (pause) ; = 4 (time) ; 5 (Client command) ; 6 Error (NOT VALID)
	int StringLength = 0;				//Length of the script line in characters
	
	//Limits defined in StdAfx.h, value here initialised  above the limits
	int ClientId = MAXCLIENTS + 1;		//Max number of clients (200 see StdAfx.h)
	int PauseTime = MAXPAUSETIME + 1;   // Max time value before next script for Cmd> -1 time XXXX
	int NoFiles = MAXNOFILES +1;		//Max number of iles tested (999 see StdAfx.h)
	int MaxLength = MAXLENGTH + 1;		//Max length in Kbytes of data chunk for read or write (2097151 see StdAfx.h)	
	int MaxDelay= MAXDELAY + 1;			//Max delay (10,00ms, see StdAfx.h) 
	int PercentWrite = 101;				// Max write percentage 100% of read + write
	int PercentRand = 101;				//Max random perecentage 100% of random + sequential
	int DirectIo = 2;					//DirectIO off = 0, DirectIO on = 1
	int BlockNo = 12;					//Number of blcok sizes alloed (11, see StdAfx.h)
	int BS[11] = {0,0,0,0,0,0,0,0,0,0,0};//Array of block sizes (none set)
	CString FileStr = "";
	CString opstr = "";
	CString CommandStr = "";

	if (str.Find("Cmd>") == 0)	//Command lines start with Cmd>, use of spaces is important
	{
		
			if (str.Find("Cmd> -1 ") == 0)
			{
				if (str.Find("Results ") > 6 && str.Find("Results ") < 15) Valid = 1;	//Cmd> -1 Results

				else if (str.Find("time ") > 6 && str.Find("time ") < 15)				//Cmd> -1 time X
				{
					Valid = 4;
					CommandStr = str.Right(str.GetLength() - str.Find("time ")); //Copy from time to end
					sscanf_s(CommandStr,"time %d", &PauseTime);
					if (PauseTime > MAXPAUSETIME || PauseTime < 1) Valid = 6;// Error
				}
				else Valid = 6; //Error
			}

			else
			{
				if (str.Find("start") > 6 && str.Find("start") < 15)					//Cmd> X start
				{
					Valid = 2;		//Cmd> X  start
					sscanf_s(str,"Cmd> %d ", &ClientId);
					if (ClientId > MAXCLIENTS || ClientId < 0) Valid = 6;// Error to amny clients

				}

				else if (str.Find("pause") > 6 && str.Find("pause") < 15)			//Cmd> X pause
				{
					Valid = 3;	//Cmd> X  pause
					sscanf_s(str,"Cmd> %d ", &ClientId);
					if (ClientId > MAXCLIENTS || ClientId < 0) Valid = 6;// Error
				}

				else if (str.Find("-p ") > 6 && str.Find("-p ") < 15) //Cmd> X -p
				{
					Valid = 5;	//e.g.Cmd> -X  -p /mnt/eon -n 1 -l 1024 -d 0 -w 0 -r 0 -v -f- -dio 1 -bn 7 -bs 1 2 3 4 5 6 7 0 0 0 0 
					sscanf_s(str,"Cmd> %d", &ClientId);
					if (ClientId > MAXCLIENTS || ClientId < 0) Valid = 6;// Error
					
					//Check for basic parameter type specifier flags, -n etc
					if ((str.Find("-n ") > 0) && (str.Find("-l ") > 0) & (str.Find("-d ") > 0) && (str.Find("-w ") > 0) 
						&& (str.Find("-r ") > 0) && (str.Find("-v") > 0) && (str.Find("-v") > 0) && (str.Find("-f") > 0)
						&& (str.Find("-dio ") > 0) && (str.Find("-bn ") > 0) && (str.Find("-bs ") > 0))
					{
						//-n 1 -l 1024 -d 0 -w 0 -r 0 
						CommandStr = str.Right(str.GetLength() - str.Find("-n ")); // Copy everything from -n onwards
						sscanf_s(CommandStr,"-n %d -l %d -d %d -w %d -r %d ", &NoFiles, &MaxLength, &MaxDelay, &PercentWrite, &PercentRand);
						if (NoFiles > MAXNOFILES || NoFiles < 0) Valid = 6;// Error number of files out of range
						if (MaxLength > MAXLENGTH || MaxLength < MINLENGTH) Valid = 6;// Error r/w length out of range
						if (MaxDelay > MAXDELAY || MaxDelay < 0) Valid = 6;// Error Delay between r/w operatiosn out of range
						if (PercentWrite > 100 || PercentWrite < 0) Valid = 6;// Error percentage of writes out of range
						if (PercentRand > 100 || PercentRand < 0) Valid = 6;// Error percentage of random r/w out of range
						
						// -dio 1 -bn 7 -bs 1 2 3 4 5 6 7 0 0 0 0
						CommandStr = str.Right(str.GetLength() - str.Find("-dio ")); // Copy everything from -dio onwards
						sscanf_s(CommandStr,"-dio %d -bn %d -bs %d %d %d %d %d %d %d %d %d %d %d", 
						&DirectIo, &BlockNo, &BS[0], &BS[1], &BS[2], &BS[3], &BS[4], &BS[5], &BS[6], &BS[7], &BS[8], &BS[9], &BS[10], &BS[11]);
						if (DirectIo > 1 && DirectIo < 0) Valid = 6; //Error in Direct IO paramete
						if (BlockNo > 11 && DirectIo < 1) Valid = 6; //Error, Number of block types out of range

						for (int i = 0 ; i < BlockNo ; i++) 
						{
							if (BS[i] == 0) Valid = 6;
						}
					}
					else Valid = 6;
				}

				else Valid = 6;															//Cmd> followed by no valid arguments
			}
	}

	return Valid;

}

int SControl::GetClientId(CString str, int CommandType)
{
	// CommandType = 0 (Comment) = 1 (Results file) ; = 2 (start) ; = 3 (pause) ; = 4 (time) ; 5 (Client command) ; 6 (NOT VALID)	
	int ClientId = -1;
	int i = 0;
	//CString ClientIdStr;
	
	switch (CommandType) {

	case 0 : 
		//msgtype = Comment
		ClientId = -1;
		break;
	case 1 : 
		//msgtype = Results
		ClientId = -1;
		break;
	case 2 : 
		//msgtype = Start X
		// old method: While not space or end of string, add characters to string, then convert to integer
		//i = 0;
		//while (CString(str.GetAt(5 + i)).Compare(" ") != 0 && i < str.GetLength()) {
		//	ClientIdStr += str.GetAt(5 + i);
		//	i++;
		//}
		//ClientId = atoi(ClientIdStr);
		sscanf_s(str,"Cmd> %d ", &ClientId);
		break;
	case 3 : 
		//msgtype = Pause X
		sscanf_s(str,"Cmd> %d ", &ClientId);
		break;
	case 4 : 
		//msgtype = time X
		ClientId = -1;
		break;
	case 5 : 
		//msgtype = Cmd> X -p
		sscanf_s(str,"Cmd> %d ", &ClientId);
		break;
	case 6 : 
		//msgtype ERROR
		ClientId = -1;
		break;
	}

	return ClientId;
}

CString SControl::GetClientCommand(CString str, int CommandType)
{
	// CommandType = 0 (Comment) = 1 (Results file) ; = 2 (start) ; = 3 (pause) ; = 4 (time) ; 5 (Client command) ; 6 (NOT VALID)	
	CString CommandStr = "";
	int i = 0;

	switch (CommandType) {

	case 0 : 
		//msgtype = Comment
		CommandStr = str;
		break;
	case 1 : 
		//msgtype = Results
		CommandStr = str.Right(str.GetLength() - (str.Find("Results")+8));
		break;
	case 2 : 
		//msgtype = X Start
		CommandStr = "start";
		break;
	case 3 : 
		//msgtype = X Pause
		CommandStr = "pause";
		break;
	case 4 : 
		//msgtype = time X
		CommandStr = str.Right(str.GetLength() - str.Find("time"));
		break;
	case 5 : 
		//msgtype = Cmd> X -p
		// While not space, add characters to string
		CommandStr = str.Right(str.GetLength() - str.Find("-p"));
		break;
	case 6 : 
		//msgtype ERROR
		CommandStr.Format("ERROR > %s",str);
		break;
	}

	return CommandStr;

}

int SControl::GetCommandValue(CString str, int CommandType)
{
	// CommandType = 0 (Comment) = 1 (Results file) ; = 2 (start) ; = 3 (pause) ; = 4 (time) ; 5 (Client command) ; 6 (NOT VALID)	
	
	int CommandValue = 0;
	//int ClientId = -1;
	//int i = 5;
	
	//Currently only gets the time value
	CString CommandStr;
	
	if (CommandType == 4) {
		//old method
		//while (CString(str.GetAt(i)).Compare(" ") != 0 && i < str.GetLength()) {
			//ValStr += str.GetAt(i);
			//DoSendScriptView(ValStr, i, CommandType, 0, CommandValue);
		//i++;
		//}
		//CommandValue = atoi(ValStr);
		CommandStr = str.Right(str.GetLength() - str.Find("time ")); // Copy everything from time onwards
		sscanf_s(CommandStr,"time %d", &CommandValue);
	}

	return CommandValue;
}