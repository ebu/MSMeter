#if !defined(AFX_FILES_H__DBC14EE8_37AC_4414_BFF1_2E55135A49C0__INCLUDED_)
#define AFX_FILES_H__DBC14EE8_37AC_4414_BFF1_2E55135A49C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Files.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFiles dialog

class CFiles : public CPropertyPage
{
	DECLARE_DYNCREATE(CFiles)

// Construction
public:
	BOOL m_TimerChanged;
	CString m_OutputFilePath;
	CString m_OutputFileName;
	CFiles();
	~CFiles();

// Dialog Data
	//{{AFX_DATA(CFiles)
	enum { IDD = IDD_FILES };
	BOOL	m_save;
	BOOL	m_summary;
	int		m_Slider;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CFiles)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CFiles)
	afx_msg void OnFileselect();
	virtual BOOL OnInitDialog();
	afx_msg void OnChange();
	afx_msg void OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILES_H__DBC14EE8_37AC_4414_BFF1_2E55135A49C0__INCLUDED_)
