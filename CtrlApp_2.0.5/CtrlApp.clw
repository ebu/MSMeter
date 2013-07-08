; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CControlDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ctrlapp.h"
LastPage=0

ClassCount=9
Class1=CAsynlstn
Class2=CAsynSvSk
Class3=CControl
Class4=CControlApp
Class5=CAboutDlg
Class6=CControlDlg
Class7=CFiles
Class8=CPSheet
Class9=CTestClient

ResourceCount=4
Resource1=IDD_ABOUTBOX (English (U.S.))
Resource2=IDD_CONTROL_DIALOG (English (U.S.))
Resource3=IDD_CONTROL
Resource4=IDD_FILES

[CLS:CAsynlstn]
Type=0
BaseClass=CAsyncSocket
HeaderFile=AsynLstn.h
ImplementationFile=AsynLstn.cpp

[CLS:CAsynSvSk]
Type=0
BaseClass=CAsyncSocket
HeaderFile=AsynSvSk.h
ImplementationFile=AsynSvSk.cpp

[CLS:CControl]
Type=0
BaseClass=CPropertyPage
HeaderFile=Control.h
ImplementationFile=Control.cpp
LastObject=CControl
Filter=D
VirtualFilter=idWC

[CLS:CControlApp]
Type=0
BaseClass=CWinApp
HeaderFile=CtrlApp.h
ImplementationFile=CtrlApp.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=CtrlAppDlg.cpp
ImplementationFile=CtrlAppDlg.cpp
LastObject=CAboutDlg

[CLS:CControlDlg]
Type=0
BaseClass=CDialog
HeaderFile=CtrlAppDlg.h
ImplementationFile=CtrlAppDlg.cpp
LastObject=IDC_ALH
Filter=D
VirtualFilter=dWC

[CLS:CFiles]
Type=0
BaseClass=CPropertyPage
HeaderFile=Files.h
ImplementationFile=Files.cpp
LastObject=CFiles
Filter=D
VirtualFilter=idWC

[CLS:CPSheet]
Type=0
BaseClass=CPropertySheet
HeaderFile=PSheet.h
ImplementationFile=PSheet.cpp
Filter=W
VirtualFilter=hWC
LastObject=IDC_NCREATE

[CLS:CTestClient]
Type=0
BaseClass=CWnd
HeaderFile=TestClient.h
ImplementationFile=TestClient.cpp
LastObject=CTestClient

[DLG:IDD_CONTROL]
Type=1
Class=CControl
ControlCount=26
Control1=IDC_TESTFILEPATH,edit,1350631552
Control2=IDC_NOOFFILES,edit,1350631552
Control3=IDC_STATIC,static,1342308866
Control4=IDC_STATIC,static,1342308866
Control5=IDC_MAXLENGTH,edit,1350631552
Control6=IDC_MAXDELAY,edit,1350631552
Control7=IDC_WRITE,edit,1350631552
Control8=IDC_RANDOM,edit,1350631552
Control9=IDC_NCREATE,edit,1350631552
Control10=IDC_VERBOSE,button,1342242819
Control11=IDC_STATIC,static,1342308866
Control12=IDC_STATIC,static,1342308866
Control13=IDC_STATIC,static,1342308866
Control14=IDC_STATIC,static,1342308866
Control15=IDC_CREATEFILES,button,1342251011
Control16=IDC_STATIC,static,1342308864
Control17=IDC_STATIC,static,1342308864
Control18=IDC_STATIC,static,1342308864
Control19=IDC_STATIC,static,1342308864
Control20=IDC_STATIC,static,1342308864
Control21=IDC_STATIC,static,1342177792
Control22=IDC_STATIC,button,1342178055
Control23=IDC_STATIC,button,1342178055
Control24=IDC_STATIC,static,1342177792
Control25=IDC_TESTFILE,button,1342242816
Control26=IDC_STATIC,static,1342308352

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg

[DLG:IDD_CONTROL_DIALOG]
Type=1
Class=CControlDlg

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_CONTROL_DIALOG (English (U.S.))]
Type=1
Class=CControlDlg
ControlCount=32
Control1=IDC_START,button,1476460545
Control2=IDC_PAUSE,button,1476460544
Control3=IDC_CONTROLPANEL,button,1342242816
Control4=IDOK,button,1342242816
Control5=IDC_STATIC,button,1342177287
Control6=IDC_CLIENTLIST,listbox,1352730881
Control7=IDC_SELECTALL,button,1342242816
Control8=IDC_CLEARSELECTION,button,1342242816
Control9=IDC_DATARATE,msctls_progress32,1350565889
Control10=IDC_STATIC,static,1342308866
Control11=IDC_TRL,static,1342308352
Control12=IDC_TRH,static,1342308354
Control13=IDC_LATENCY,msctls_progress32,1350565889
Control14=IDC_STATIC,static,1342308866
Control15=IDC_ALL,static,1342308352
Control16=IDC_ALH,static,1342308354
Control17=IDC_AVLAT,static,1350570498
Control18=IDC_MAXLATENCY,msctls_progress32,1350565889
Control19=IDC_STATIC,static,1342308866
Control20=IDC_MLL,static,1342308352
Control21=IDC_MLH,static,1342308354
Control22=IDC_MAXLAT,static,1350570498
Control23=IDC_RATE,static,1350570498
Control24=IDC_DATARATE2,msctls_progress32,1350565889
Control25=IDC_STATIC,static,1342308866
Control26=IDC_TRL2,static,1342308352
Control27=IDC_TRH2,static,1342308354
Control28=IDC_RATE2,static,1350570498
Control29=IDC_STATIC,button,1342177287
Control30=IDC_STATIC,static,1342308352
Control31=IDC_NORESPONSE,static,1342177792
Control32=IDC_REMOVE,button,1476460544

[DLG:IDD_FILES]
Type=1
Class=CFiles
ControlCount=14
Control1=IDC_FILESELECT,button,1342242816
Control2=IDC_SAVE,button,1342242819
Control3=IDC_SUMMARY,button,1342242819
Control4=IDC_SLIDER1,msctls_trackbar32,1342242821
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308353
Control9=IDC_STATIC,static,1342308353
Control10=IDC_STATIC,static,1342308353
Control11=IDC_STATIC,static,1342308353
Control12=IDC_STATIC,static,1342308353
Control13=IDC_STATIC,static,1342308353
Control14=IDC_STATIC,static,1342308353

