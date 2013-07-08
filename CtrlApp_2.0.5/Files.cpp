// Files.cpp : implementation file
//

#include "stdafx.h"
#include "CtrlApp.h"
#include "Files.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFiles property page

IMPLEMENT_DYNCREATE(CFiles, CPropertyPage)

CFiles::CFiles() : CPropertyPage(CFiles::IDD)
{
	//{{AFX_DATA_INIT(CFiles)
	m_save = FALSE;
	m_summary = FALSE;
	m_Slider = 0;
	//}}AFX_DATA_INIT
	//m_OutputFileName = "";		// These are always set by calling function
	//m_save = TRUE;
}

CFiles::~CFiles()
{
}

void CFiles::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFiles)
	DDX_Check(pDX, IDC_SAVE, m_save);
	DDX_Check(pDX, IDC_SUMMARY, m_summary);
	DDX_Slider(pDX, IDC_SLIDER1, m_Slider);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFiles, CPropertyPage)
	//{{AFX_MSG_MAP(CFiles)
	ON_BN_CLICKED(IDC_FILESELECT, OnFileselect)
	ON_BN_CLICKED(IDC_SAVE, OnChange)
	ON_BN_CLICKED(IDC_SUMMARY, OnChange)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER1, OnReleasedcaptureSlider1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFiles message handlers

void CFiles::OnFileselect() 
{
	// TODO: Add your control notification handler code here
	TCHAR szFilters[] = _T ("CSV files (*.csv)|*.csv|All files (*.*)|*.*||");

	CFileDialog dlg (FALSE, _T("csv"), _T ("results.csv"), OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT, szFilters);
	dlg.m_ofn.lpstrInitialDir = (LPCTSTR)m_OutputFilePath;
	dlg.m_ofn.lpstrTitle = "Select results file";

	if ((dlg.DoModal() == IDOK)) {
		//AfxMessageBox(dlg.GetPathName());
		m_OutputFileName = dlg.GetPathName();
	}
	// Store path name for future use
	char dir[_MAX_DIR];
	char drive[_MAX_DRIVE];
	_splitpath((LPCTSTR)m_OutputFileName, drive, dir, NULL, NULL);
	m_OutputFilePath = drive;
	m_OutputFilePath += dir;
	SetModified(TRUE);
}

BOOL CFiles::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetRange(0,6);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetPageSize(1);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetPos(6);
	((CSliderCtrl*)GetDlgItem(IDC_SLIDER1))->SetPos(m_Slider); // This is a bug - I have to set it twice to make it work properly
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFiles::OnChange() 
{
	// TODO: Add your control notification handler code here
	//m_TimerChanged = TRUE;
	//SetModified(TRUE);
}

void CFiles::OnReleasedcaptureSlider1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	m_TimerChanged = TRUE;
	*pResult = 0;
}
