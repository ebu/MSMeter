#if !defined(AFX_CONTROL_H__FC14175B_8708_4FEE_AD13_ABD0591622D4__INCLUDED_)
#define AFX_CONTROL_H__FC14175B_8708_4FEE_AD13_ABD0591622D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Control.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CControl dialog

class CControl : public CPropertyPage
{
	DECLARE_DYNCREATE(CControl)

// Construction
public:
	//CString makefilepath(void);
	CControl();
	~CControl();

// Dialog Data
	//{{AFX_DATA(CControl)
	enum { IDD = IDD_CONTROL };
	CString	m_sTestFilePath;
	BOOL	m_bVerbose;
	int		m_nPCwrite;
	int		m_nPCrandom;
	int		m_nMaxnum;
	DWORD	m_dMaxlength;
	int		m_nMaxdelay;
	BOOL	m_bCreateflag;
	BOOL	m_bSharing;
	DWORD	m_nCreate;
	BOOL	m_bBlockSize[MAXBLOCKTYPE];
	BOOL	m_bDirectIO;
	//}}AFX_DATA

	//Additions of MSMeter 3
	BOOL	m_bPerFrame;
	int		m_dFrameSize;
	BOOL	m_bNTP;
	CString	m_sNTPAddress;


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CControl)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CControl)
	afx_msg void OnChange();
	afx_msg void OnTestfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBlocksize6();
};



/////////////////////////////////////////////////////////////////////////////
// SControl dialog

class SControl : public CPropertyPage
{
	DECLARE_DYNCREATE(SControl)

// Construction
public:
	//CString makefilepath(void);
	SControl();
	~SControl();

// Dialog Data
	//{{AFX_DATA(SControl)
	enum { IDD = IDD_CONTROL2 };
	CString	m_ScriptFilePath;				//File path to script file inc file name
	CString	m_ScriptFileName;				//Script file name
	CListBox m_SendScriptView;				//Clistbox parameter used to ouptu script file in Script Panel
	CStringArray m_ScriptLineArray;			//Array of the liness from script file
	CStringArray m_ScriptCommandArray;		//Array of the script commands
	int m_CommandType[MAXSCRIPTCOMMANDS];	//Array describing script line types; 0 = Comment, etc
	int m_CommandValue[MAXSCRIPTCOMMANDS];	//Array describing script line types; 0 = Comment, etc
	int m_ClientId[MAXSCRIPTCOMMANDS];		//Array of Cleint Ids from the script
	int m_ScriptLineCount;					//Number of lines in script file
	int m_ScriptCommandLines;				//Number of lines that are commands
	int m_SelectedClientCount;				//Number of clients selected in control dialaogue menu
	int m_ScriptedClientCount;				//Number of clients selected in control dialaogue menu
	int	Read(CString m_ScriptFilePath);		//Routine that laods the scrip[t file line by line
	void SControl::DoSendScriptView(CString str, int lineNo, int lineType, int ClientId, int CommandValue); //Ouptput script lines to listbox
	int SControl::ValidateScriptLine(CString str); //Very basic validation of each script line, returns line type
	int SControl::GetClientId(CString str, int CommandType); //Get the client Id for each script line
	CString SControl::GetClientCommand(CString str, int CommandType); //Get the command and command type for each script line
	int SControl::GetCommandValue(CString str, int CommandType); //Get parameter value for each script line (for Time commands only)
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CControl)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CControl)
	afx_msg void OnChange();
	afx_msg void OnScriptfile(); //When open script button clicked
	afx_msg void OnLoad(); //When load script button clicked
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBlocksize6();
};



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROL_H__FC14175B_8708_4FEE_AD13_ABD0591622D4__INCLUDED_)
