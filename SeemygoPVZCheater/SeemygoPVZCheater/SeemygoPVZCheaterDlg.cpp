
// SeemygoPVZCheaterDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SeemygoPVZCheater.h"
#include "SeemygoPVZCheaterDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 监控线程的刷新时间
#define MONITOR_REFRESH_TIME_INTERVAL 1000

// 植物大战僵尸的进程句柄
static HANDLE g_processHandle;
// 用来监控植物大战僵尸的线程句柄
static HANDLE g_monitoringThreadHandle;
// 辅助窗口
static CSeemygoPVZCheaterDlg *g_dlg;

// 将某个值写入植物大战僵尸内存（后面的可变参数是地址链，要以-1结尾）
void WriteMemory(void *value, DWORD valueSize, ...)
{
	if (value == NULL || valueSize == NULL || g_processHandle == NULL) return;

	DWORD tempValue = NULL;

	// 遍历所有的地址
	va_list addresses;
	va_start(addresses, valueSize);
	DWORD offset = NULL;
	DWORD lastAddress = NULL;
	while ((offset = va_arg(addresses, DWORD)) != -1)
	{
		lastAddress = tempValue + offset;
		::ReadProcessMemory(g_processHandle, (LPCVOID)lastAddress, &tempValue, sizeof(DWORD), NULL);
	}
	va_end(addresses);

	// 写
	::WriteProcessMemory(g_processHandle, (LPVOID)lastAddress, value, valueSize, NULL);
}

// 用来监控植物大战僵尸的线程
DWORD WINAPI MonitoringThreadProc(LPVOID lpParam)
{
	while (1)
	{
		// 找窗口
		HWND hwnd = ::FindWindow(TEXT("MainWindow"), TEXT("植物大战僵尸中文版"));
		if (hwnd == NULL)
		{
			::CloseHandle(g_processHandle);
			g_processHandle = NULL;

			// 禁止点击
			g_dlg->m_sunshine.SetCheck(FALSE);
			g_dlg->m_sunshine.EnableWindow(FALSE);

			g_dlg->m_unbreakable.SetCheck(FALSE);
			g_dlg->m_unbreakable.EnableWindow(FALSE);
			g_dlg->OnBnClickedUnbreakable();

			g_dlg->m_cd.SetCheck(FALSE);
			g_dlg->m_cd.EnableWindow(FALSE);
			g_dlg->OnBnClickedCd();

			g_dlg->m_background.SetCheck(FALSE);
			g_dlg->m_background.EnableWindow(FALSE);
			g_dlg->OnBnClickedBackground();

			g_dlg->m_kill.SetCheck(FALSE);
			g_dlg->m_kill.EnableWindow(FALSE);
			g_dlg->OnBnClickedKill();

			g_dlg->m_zoombieUnbreakable.SetCheck(FALSE);
			g_dlg->m_zoombieUnbreakable.EnableWindow(FALSE);
			g_dlg->OnBnClickedZoombieUnbreakable();

			g_dlg->m_cancel.EnableWindow(FALSE);
			g_dlg->m_coin.EnableWindow(FALSE);
		}
		else if (g_processHandle == NULL)
		{
			// 找进程ID
			DWORD pid = NULL;
			::GetWindowThreadProcessId(hwnd, &pid);

			// 找进程句柄
			g_processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

			// 开启点击
			g_dlg->m_sunshine.EnableWindow(TRUE);
			g_dlg->m_unbreakable.EnableWindow(TRUE);
			g_dlg->m_cd.EnableWindow(TRUE);
			g_dlg->m_coin.EnableWindow(TRUE);
			g_dlg->m_cancel.EnableWindow(TRUE);
			g_dlg->m_background.EnableWindow(TRUE);
			g_dlg->m_kill.EnableWindow(TRUE);
			g_dlg->m_zoombieUnbreakable.EnableWindow(TRUE);
		}

		if (g_dlg->m_sunshine.GetCheck())
		{ // 如果需要无限阳光
			DWORD value = 9990;
			WriteMemory(&value, sizeof(value), 0x6A9EC0, 0x320, 0x8, 0x0, 0x8, 0x144, 0x2c, 0x5560, -1);
		}

		::Sleep(MONITOR_REFRESH_TIME_INTERVAL);
	}
	return 0;
}

// 针对XP提高程序的访问权限
BOOL ImproveAccessPrivilege()
{
	HANDLE tokenHandle;
	LUID privilegeValue;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &tokenHandle)) return FALSE;

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &privilegeValue))
	{
		CloseHandle(tokenHandle);
		return FALSE;
	}

	TOKEN_PRIVILEGES privileges;
	privileges.PrivilegeCount = 1;
	privileges.Privileges[0].Luid = privilegeValue;
	privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	if (!AdjustTokenPrivileges(tokenHandle, FALSE, &privileges, sizeof(privileges), NULL, NULL))
	{
		CloseHandle(tokenHandle);
		return FALSE;
	}

	return TRUE;
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CSeemygoPVZCheaterDlg 对话框
CSeemygoPVZCheaterDlg::CSeemygoPVZCheaterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SEEMYGOPVZCHEATER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSeemygoPVZCheaterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SUNSHINE, m_sunshine);
	DDX_Control(pDX, IDC_UNBREAKABLE, m_unbreakable);
	DDX_Control(pDX, IDC_COIN, m_coin);
	DDX_Control(pDX, IDC_CANCEL, m_cancel);
	DDX_Control(pDX, IDC_KILL, m_kill);
	DDX_Control(pDX, IDC_BACKGROUND, m_background);
	DDX_Control(pDX, IDC_ZOOMBIE_UNBREAKABLE, m_zoombieUnbreakable);
	DDX_Control(pDX, IDC_CD, m_cd);
	DDX_Control(pDX, IDC_COPYRIGHT, m_copyright);
}

BEGIN_MESSAGE_MAP(CSeemygoPVZCheaterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_COIN, &CSeemygoPVZCheaterDlg::OnBnClickedCoin)
	ON_BN_CLICKED(IDC_CANCEL, &CSeemygoPVZCheaterDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_UNBREAKABLE, &CSeemygoPVZCheaterDlg::OnBnClickedUnbreakable)
	ON_BN_CLICKED(IDC_KILL, &CSeemygoPVZCheaterDlg::OnBnClickedKill)
	ON_BN_CLICKED(IDC_BACKGROUND, &CSeemygoPVZCheaterDlg::OnBnClickedBackground)
	ON_BN_CLICKED(IDC_CD, &CSeemygoPVZCheaterDlg::OnBnClickedCd)
	ON_BN_CLICKED(IDC_ZOOMBIE_UNBREAKABLE, &CSeemygoPVZCheaterDlg::OnBnClickedZoombieUnbreakable)
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_COURSE, &CSeemygoPVZCheaterDlg::OnBnClickedCourse)
END_MESSAGE_MAP()


// CSeemygoPVZCheaterDlg 消息处理程序
BOOL CSeemygoPVZCheaterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// 针对XP系统提高程序的权限
	ImproveAccessPrivilege();

	// 辅助窗口
	g_dlg = this;

	// 启动监控线程
	g_monitoringThreadHandle = ::CreateThread(NULL, 0, MonitoringThreadProc, NULL, 0, NULL);

	// 加载版权所有的鼠标
	m_copyrightCursor = ::LoadCursor(NULL, IDC_HAND);

	// 版权所有的矩形框
	this->m_copyright.GetWindowRect(&m_copyrightRect);
	ScreenToClient(&m_copyrightRect);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSeemygoPVZCheaterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSeemygoPVZCheaterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSeemygoPVZCheaterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSeemygoPVZCheaterDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialogEx::OnMouseMove(nFlags, point);

	if (point.x > m_copyrightRect.left && point.x < m_copyrightRect.right &&
		point.y > m_copyrightRect.top  && point.y < m_copyrightRect.bottom)
	{
		::SetCursor(m_copyrightCursor);
	}
}

void CSeemygoPVZCheaterDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialogEx::OnLButtonUp(nFlags, point);

	if (point.x > m_copyrightRect.left && point.x < m_copyrightRect.right &&
		point.y > m_copyrightRect.top  && point.y < m_copyrightRect.bottom)
	{
		::SetCursor(m_copyrightCursor);
		::ShellExecute(NULL, TEXT("open"), TEXT("http://www.520it.com/?CPP_Class_PVZ_UE"), NULL, NULL, SW_SHOWNORMAL);
	}
}

HBRUSH CSeemygoPVZCheaterDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_COPYRIGHT)
	{
		// 文字为红色
		pDC->SetTextColor(RGB(0, 0, 255));
	}

	return hbr;
}

// 关闭窗口
void CSeemygoPVZCheaterDlg::OnClose()
{
	CDialogEx::OnClose();

	// 去掉外挂效果
	this->OnBnClickedCancel();

	// 杀死线程
	::TerminateThread(g_monitoringThreadHandle, 0);
	::CloseHandle(g_monitoringThreadHandle);

	// 关闭句柄
	::CloseHandle(g_processHandle);
}

// 植物不死-checkbox
void CSeemygoPVZCheaterDlg::OnBnClickedUnbreakable()
{
	DWORD address1 = 0x52FCF0;
	DWORD address2 = 0x46D7A6;
	DWORD address3 = 0x45EC63;
	DWORD address4 = 0x46CFEB;
	if (this->m_unbreakable.GetCheck())
	{ // 如果需要植物不死
		BYTE data1[] = { 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(data1, sizeof(data1), address1, -1);

		BYTE data2[] = { 0x90, 0x90, 0x90 };
		WriteMemory(data2, sizeof(data2), address2, -1);

		BYTE data3[] = { 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(data3, sizeof(data3), address3, -1);

		BYTE data4[] = { 0x90, 0x90, 0x90 };
		WriteMemory(data4, sizeof(data4), address4, -1);
	}
	else
	{ // 如果不需要植物不死
		BYTE data1[] = { 0x83, 0x46, 0x40, 0xFC };
		WriteMemory(data1, sizeof(data1), address1, -1);

		BYTE data2[] = { 0x29, 0x4E, 0X40 };
		WriteMemory(data2, sizeof(data2), address2, -1);

		BYTE data3[] = { 0x83, 0x46, 0x40, 0xCE };
		WriteMemory(data3, sizeof(data3), address3, -1);

		BYTE data4[] = { 0x29, 0x50, 0x40 };
		WriteMemory(data4, sizeof(data4), address4, -1);
	}
}

// 无CD-checkbox
void CSeemygoPVZCheaterDlg::OnBnClickedCd()
{
	DWORD address = 0x487296;
	if (this->m_cd.GetCheck())
	{ // 如果需要无CD
		BYTE data[] = { 0x90, 0x90 };
		WriteMemory(data, sizeof(data), address, -1);
	}
	else
	{ // 如果不需要无CD
		BYTE data[] = { 0x7E, 0x14 };
		WriteMemory(data, sizeof(data), address, -1);
	}
}

// 后台运行
void CSeemygoPVZCheaterDlg::OnBnClickedBackground()
{
	DWORD address = 0x54E1C2;
	if (this->m_background.GetCheck())
	{ // 如果需要后台运行
		BYTE data[] = { 0x90, 0x90, 0x90 };
		WriteMemory(data, sizeof(data), address, -1);
	}
	else
	{ // 如果不需要后台运行
		BYTE data[] = { 0x0F, 0x95, 0xC0 };
		WriteMemory(data, sizeof(data), address, -1);
	}
}

// 秒杀僵尸
void CSeemygoPVZCheaterDlg::OnBnClickedKill()
{
	DWORD address1 = 0x53130F;
	DWORD address2 = 0x531066;
	DWORD address3 = 0x530CB1;
	if (this->m_kill.GetCheck())
	{ // 如果需要秒杀僵尸
	  // 去除僵尸不死
		g_dlg->m_zoombieUnbreakable.SetCheck(FALSE);
		g_dlg->OnBnClickedZoombieUnbreakable();

		BYTE data1[] = { 0x2B, 0xFF, 0x90, 0x90 };
		WriteMemory(data1, sizeof(data1), address1, -1);

		BYTE data2[] = { 0x90, 0x90 };
		WriteMemory(data2, sizeof(data2), address2, -1);

		BYTE data3[] = { 0x90, 0x90 };
		WriteMemory(data3, sizeof(data3), address3, -1);
	}
	else
	{ // 如果不需要秒杀僵尸
		BYTE data1[] = { 0x2B, 0x7C, 0x24, 0x20 };
		WriteMemory(data1, sizeof(data1), address1, -1);

		BYTE data2[] = { 0x75, 0x11 };
		WriteMemory(data2, sizeof(data2), address2, -1);

		BYTE data3[] = { 0x75, 0x17 };
		WriteMemory(data3, sizeof(data3), address3, -1);
	}
}

// 僵尸不死
void CSeemygoPVZCheaterDlg::OnBnClickedZoombieUnbreakable()
{
	DWORD address1 = 0x53130F;
	DWORD address2 = 0x531042;
	DWORD address3 = 0x530C9F;
	if (this->m_zoombieUnbreakable.GetCheck())
	{ // 如果需要僵尸不死
	  // 去除秒杀僵尸
		g_dlg->m_kill.SetCheck(FALSE);
		g_dlg->OnBnClickedKill();

		BYTE data1[] = { 0x83, 0xEF, 0x00, 0x90 };
		WriteMemory(data1, sizeof(data1), address1, -1);

		BYTE data2[] = { 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(data2, sizeof(data2), address2, -1);

		BYTE data3[] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
		WriteMemory(data3, sizeof(data3), address3, -1);
	}
	else
	{ // 如果不需要僵尸不死
		BYTE data1[] = { 0x2B, 0x7C, 0x24, 0x20 };
		WriteMemory(data1, sizeof(data1), address1, -1);

		BYTE data2[] = { 0x2B, 0xF0, 0x2B, 0xC8 };
		WriteMemory(data2, sizeof(data2), address2, -1);

		BYTE data3[] = { 0x2B, 0xD0, 0x29, 0x86, 0xDC, 0x00, 0x00, 0x00 };
		WriteMemory(data3, sizeof(data3), address3, -1);
	}
}

// 无限金币
void CSeemygoPVZCheaterDlg::OnBnClickedCoin()
{
	DWORD value = 99990;
	WriteMemory(&value, sizeof(value), 0x6A9EC0, 0x82C, 0x28, -1);
}

// 一键取消
void CSeemygoPVZCheaterDlg::OnBnClickedCancel()
{
	this->m_sunshine.SetCheck(FALSE);

	this->m_unbreakable.SetCheck(FALSE);
	this->OnBnClickedUnbreakable();

	this->m_cd.SetCheck(FALSE);
	this->OnBnClickedCd();

	this->m_background.SetCheck(FALSE);
	this->OnBnClickedBackground();

	this->m_kill.SetCheck(FALSE);
	this->OnBnClickedKill();

	this->m_zoombieUnbreakable.SetCheck(FALSE);
	this->OnBnClickedZoombieUnbreakable();
}

// 获取外挂开发教程
void CSeemygoPVZCheaterDlg::OnBnClickedCourse()
{
	::ShellExecute(NULL, TEXT("open"), 
		TEXT("https://ke.qq.com/course/336509"), 
		NULL, NULL, SW_SHOWNORMAL);
}
