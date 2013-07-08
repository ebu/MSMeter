// ControlDlg.h : header file
//

#if !defined(AFX_PROPERTY2DLG_H__3E8521B3_CEC5_4428_B999_B60B9DD29B85__INCLUDED_)
#define AFX_PROPERTY2DLG_H__3E8521B3_CEC5_4428_B999_B60B9DD29B85__INCLUDED_

#include "PSheet.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AsynLstn.h"
#include "TestClient.h"
#include <afxmt.h>
#include "Data.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// CControlDlg dialog

class CControlDlg : public CDialog
{
// Construction
public:
	void CheckForResponse(void);	//Routine checks for resposne from client worker programs
	double m_fEffRate;
	void PauseClient(int index);
	CData m_Data;
	CString m_OutFilePath;			//Results output file path
	CString m_sOutFileName;			//Results output file name
											//Added for script control in CControlDlg Class
	CString m_sScriptFilePath;				//File path including file name
	CStringArray m_sScriptLineArray;		//Array holding all script lines inc the comments
	int m_sCommandType[MAXSCRIPTCOMMANDS];	//Array of command types, from ValidateScriptLine
	int m_sClientId[MAXSCRIPTCOMMANDS];		//Array of client Ids for each command
	int m_sCommandValue[MAXSCRIPTCOMMANDS]; //Array of command parameter values 9currently only used for Time command values)
	int m_sScriptLineCount;							//Total number of lines in script array inc comments
	int m_sScriptedClientCount;						// Number of scripted clients
	BOOL m_bBlockSize[MAXBLOCKTYPE];				// Array of block sizes to be used for client
	BOOL m_bDirectIO;								// Flag to set direct IO read / write for client
	CTime m_timeStart, m_timeStop;					// General timing used for script
	BOOL m_fScriptRunning;							//Flag to show running state of script 
	BOOL m_fGetNextScriptCommand;					//Flag to show running state of script 
	double m_RunningCommandTime;					// value used to set RunCommand bar
	BOOL m_fProcessData;							// Flag to start and stop prcoessing of data
	//
	int m_nUpdateTime;
	UINT m_nStartAveraging;						//Values used for measurement bars on main window
	double m_fMaxLatency;
	double m_fLatency;
	double m_fRate;
	void UpdateDisplay(double rate, double latency, int blocksize, DWORD length);
	void ProcessData(CString s, int i);
	BOOL m_SaveSummary;							//Flags to o/p results to file and end of Test control tests
	BOOL m_SaveResults;
	DWORD m_dCreate;
	BOOL m_bCreateflag;
	int ListGetEntry(int index);
	int GetNextIndex();
	int ListGetIndex(int i);
	int m_nSelClients;					//Number of selected clients
	int m_pnSelClients[MAXCLIENTS];		//Array of selected client indices
	int m_nClients;						//Registered client number - this always increases
										// and generates the next available index number
	CFile outfile;						//Output file for results
	void StopClient(int index);
	void ProcessPendingRead(int index);	//This is the access point for the main input 
										//data handling routiness
	void DestroyClient(int c);
	int CreateClient();
	CTestClient * m_pTestClient[MAXCLIENTS];	//Array of test clients
	CAsynlstn* m_pLstnSock;						//Initial listener socket
	CString m_strSend;							//General string for sending output
	CString m_strRecv;							//General string for receiving input
	UINT m_port;								//Port number (=50100)

	CControlDlg(CWnd* pParent = NULL);			// standard constructor

	//Script control routines
	void DoSendOp(CString str, int CurrentClient);	//send commands to output window
	void RunningScript();							// Runs script lines until end of script file or <stop>
	

// Dialog Data
	//{{AFX_DATA(CControlDlg)
	enum { IDD = IDD_CONTROL_DIALOG };
	CProgressCtrl	m_nDisplayRate;
	CProgressCtrl	m_nRunningCommand;
	CListBox m_ClientList;
	//CListBox m_SendOutput;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CControlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnControlpanel();
	afx_msg void OnStart();
	afx_msg void OnPause();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnSelectall();
	afx_msg void OnClearselection();
	afx_msg void OnSelchangeClientlist();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRemove();
	//Added for MS Meter 2 Script control
	afx_msg void OnScriptControlpanel();
	afx_msg void OnScriptPause();
	afx_msg void OnScriptStart();
	//afx_msg void RunningScript();

	//}}AFX_MSG
	afx_msg LRESULT OnApply (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedRate3();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTY2DLG_H__3E8521B3_CEC5_4428_B999_B60B9DD29B85__INCLUDED_)
