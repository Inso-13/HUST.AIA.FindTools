
// demoDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "demo.h"
#include "demoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_ABOUTBOX};
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// CdemoDlg 对话框

CdemoDlg::CdemoDlg(CWnd *pParent /*=nullptr*/)
	: CDialogEx(IDD_DEMO_DIALOG, pParent), m_bRun(FALSE), m_hCam(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdemoDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdemoDlg, CDialogEx)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_OpenCam, &CdemoDlg::OnBnClickedOpencam)
ON_BN_CLICKED(IDC_StartGrab, &CdemoDlg::OnBnClickedStartgrab)
ON_BN_CLICKED(IDC_CloseCam, &CdemoDlg::OnBnClickedClosecam)
ON_BN_CLICKED(IDC_Test, &CdemoDlg::OnBnClickedTest)
END_MESSAGE_MAP()

// CdemoDlg 消息处理程序

BOOL CdemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu *pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	SetIcon(m_hIcon, TRUE);	 // 设置大图标
	SetIcon(m_hIcon, FALSE); // 设置小图标

	// TODO: 在此添加额外的初始化代码
	MVSTATUS_CODES r;
	r = MVInitLib();
	if (r != MVST_SUCCESS)
	{
		MessageBox("函数库初始化失败！", "提示", MB_ICONWARNING);
		return TRUE;
	}
	r = MVUpdateCameraList();
	if (r != MVST_SUCCESS)
	{
		MessageBox("查找连接计算机的相机失败！", "提示", MB_ICONWARNING);
		return TRUE;
	}
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
	GetDlgItem(IDC_Test)->EnableWindow(true);

	return TRUE; // 除非将焦点设置到控件，否则返回 TRUE
}

void CdemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CdemoDlg::OnPaint()
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
HCURSOR CdemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void getFiles(string path, vector<string> &files)
{
	//文件句柄
	intptr_t hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
void CdemoDlg::OnBnClickedOpencam()
{
	// TODO: 在此添加控件通知处理程序代码
	int nCams = 0;
	MVGetNumOfCameras(&nCams);
	if (nCams == 0)
	{
		MessageBox("没有找到相机 请确认连接和相机 IP 设置", "提示", MB_ICONWARNING);
		return;
	}
	MVSTATUS_CODES r = MVOpenCamByIndex(0, &m_hCam);
	if (m_hCam == NULL)
	{
		if (r == MVST_ACCESS_DENIED)
			MessageBox("无法打开相机,可能正被别的软件控制", "提示", MB_ICONWARNING);
		else
			MessageBox("无法打开相机", "提示", MB_ICONWARNING);
		return;
	}
	int w, h;
	MVGetWidth(m_hCam, &w);
	MVGetHeight(m_hCam, &h);
	MVGetPixelFormat(m_hCam, &m_PixelFormat);
	m_image_3C_raw.CreateByPixelFormat(w, h, m_PixelFormat);

	cv::namedWindow("src", cv::WINDOW_AUTOSIZE);

	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(true);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
	GetDlgItem(IDC_Test)->EnableWindow(false);
}

void CdemoDlg::OnBnClickedStartgrab()
{
	// TODO: 在此添加控件通知处理程序代码
	TriggerModeEnums enumMode;
	MVGetTriggerMode(m_hCam, &enumMode);
	if (enumMode != TriggerMode_Off)
	{
		MVSetTriggerMode(m_hCam, TriggerMode_Off);
		Sleep(100);
	}
	MVStartGrab(m_hCam, (MVStreamCB)StreamCB, (ULONG_PTR)this);
	m_bRun = true;
	GetDlgItem(IDC_OpenCam)->EnableWindow(false);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(true);
	GetDlgItem(IDC_Test)->EnableWindow(false);
}

void CdemoDlg::DrawImage()
{
	CRect rct;
	GetDlgItem(pic)->GetClientRect(&rct);
	int dstW = rct.Width();
	int dstH = rct.Height();
	CDC *pDC = GetDlgItem(pic)->GetDC();
	{
		pDC->SetStretchBltMode(COLORONCOLOR);
#ifdef SHOW_1C
		m_image_1C.Draw(pDC->GetSafeHdc(), 0, 0, dstW, dstH);
#else
		m_image_3C_raw.Draw(pDC->GetSafeHdc(), 0, 0, dstW, dstH);
#endif
	}
	ReleaseDC(pDC);
}
int CdemoDlg::OnStreamCB(MV_IMAGE_INFO *pInfo)
{
	MVInfo2Image(m_hCam, pInfo, &m_image_3C_raw);
	PreProcessImage(0, 0, 0, 255, 255, 190);
	ProcessImage();
	DrawImage();
	return 0;
}
int __stdcall StreamCB(MV_IMAGE_INFO *pInfo, ULONG_PTR nUserVal)
{
	CdemoDlg *pDlg = (CdemoDlg *)nUserVal;
	return (pDlg->OnStreamCB(pInfo));
}

void CdemoDlg::OnBnClickedClosecam()
{
	// TODO: 在此添加控件通知处理程序代码
	MVStopGrab(m_hCam);
	MVCloseCam(m_hCam);
	m_bRun = false;
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
	GetDlgItem(IDC_Test)->EnableWindow(true);
}
void CdemoDlg::OnClose()
{
	if (m_bRun != false)
		MVStopGrab(m_hCam);
	MVTerminateLib();
	CDialog::OnClose();
}

void CdemoDlg::OnBnClickedTest()
{
	// TODO: 在此添加控件通知处理程序代码
	static int first = 0;
	static vector<string> files;
	if (first == 0)
	{
		m_image_3C_raw.Load(TEST_IMG_PATH);
		getFiles(TEST_IMG_DIR_PATH, files);
	}
	else
	{
		m_image_3C_raw.Load(files[min(first, (int)files.size()) - 1].c_str());
	}
	// PreProcessImage(0, 0, 25, 255, 255, 200);
	PreProcessImage(0, 0, 0, 255, 255, 190);
	ProcessImage();
	DrawImage();

	first++;
	GetDlgItem(IDC_OpenCam)->EnableWindow(true);
	GetDlgItem(IDC_StartGrab)->EnableWindow(false);
	GetDlgItem(IDC_CloseCam)->EnableWindow(false);
	GetDlgItem(IDC_Test)->EnableWindow(true);
}
inline uchar get_pixel_3C(const uchar *imagePtr, const int &x, const int &y, const int channel, const int width)
{
	return *(imagePtr + (y * width + x) * 3 + channel);
}
inline uchar get_pixel_1C(const uchar *imagePtr, const int &x, const int &y, const int width)
{
	return *(imagePtr + y * width + x);
}
inline void set_pixel_1C(uchar *imagePtr, const int &x, const int &y, const uchar val, const int width)
{
	*(imagePtr + y * width + x) = val;
}
inline int round4(int x)
{
	int temp = x % 4;
	if (temp == 0)
		return x;
	else
		return x + 4 - temp;
}
// void CdemoDlg::dilate(const int kx, const int ky)
// {
// 	uchar *new_img_ptr = (uchar *)malloc(m_height * m_width);
// 	uchar temp = 0;

// 	for (int i = 0; i < m_width; i++)
// 	{
// 		for (int j = 0; j < m_height; j++)
// 		{
// 			temp = 0;
// 			for (int u = i - kx / 2; u < i + kx / 2; u++)
// 			{
// 				for (int v = j - ky / 2; v < j + ky / 2; v++)
// 				{
// 					if (u >= 0 && u < m_width && v >= 0 && v < m_height)
// 						temp = max(temp, get_pixel_1C(m_imagePtr_1C, u, v, 0, 1));
// 				}
// 			}
// 			set_pixel_1C(new_img_ptr, i, j, 0, temp, 1);
// 		}
// 	}
// 	memmove_s(m_imagePtr_1C, m_width * m_height, new_img_ptr, m_width * m_height);
// 	free(new_img_ptr);
// }
void erode_to(uchar *imagePtr_1C, int width, CRect rect, uchar *temp_image, int kernel)
{
	uchar temp = 255;
	int y1 = rect.top;
	int y2 = rect.bottom;
	int x2 = rect.right;
	int x1 = rect.left;
	int delta = (kernel + 1) / 2 - 1;
	int o_width = round4(x2 - x1 + 2 - kernel);

	for (int i = x1 + delta; i <= x2 - delta; i++)
	{
		for (int j = y1 + delta; j <= y2 - delta; j++)
		{
			temp = 255;
			for (int u = i - delta; u <= i + delta; u++)
			{
				for (int v = j - delta; v <= j + delta; v++)
				{
					if (!temp)
						break;
					if (!get_pixel_1C(imagePtr_1C, u, v, width))
					{
						temp = 0;
						break;
					}
				}
			}
			set_pixel_1C(temp_image, i - x1 - delta, j - y1 - delta, temp, o_width);
		}
	}
}

// void CdemoDlg::get_bound()
// {
// 	char mask;
// 	for (int j = 0; j < m_height; j++)
// 	{
// 		mask = get_pixel_1C(m_imagePtr_1C, m_width / 2, j, 0, 1);
// 		if (!mask)
// 		{
// 			m_min_y = j + 15;
// 			break;
// 		}
// 	}
// 	for (int j = m_height - 1; j >= 0; j--)
// 	{
// 		mask = get_pixel_1C(m_imagePtr_1C, m_width / 2, j, 0, 1);
// 		if (!mask)
// 		{
// 			m_max_y = j - 15;
// 			break;
// 		}
// 	}
// 	for (int i = 0; i < m_width; i++)
// 	{
// 		mask = get_pixel_1C(m_imagePtr_1C, i, m_height / 2, 0, 1);
// 		if (!mask)
// 		{
// 			m_min_x = i + 15;
// 			break;
// 		}
// 	}
// 	for (int i = m_width - 1; i >= 0; i--)
// 	{
// 		mask = get_pixel_1C(m_imagePtr_1C, i, m_height / 2, 0, 1);
// 		if (!mask)
// 		{
// 			m_max_x = i - 15;
// 			break;
// 		}
// 	}
// }
int pow_int(const int x, const int a)
{
	int ans = 1;
	for (int i = 0; i < a; i++)
	{
		ans *= x;
	}
	return ans;
}

int CdemoDlg::get_size(const CRect &rect) const
{
	int y1 = rect.top;
	int y2 = rect.bottom;
	int x2 = rect.right;
	int x1 = rect.left;
	int sum = 0;
	for (int i = x1; i <= x2; i++)
	{
		for (int j = y1; j <= y2; j++)
		{
			if (get_pixel_1C(m_imagePtr_1C, i, j, m_width))
			{
				sum += 1;
			}
		}
	}
	return sum;
}

// void CdemoDlg::convert_to_1C(const uchar lower_r, const uchar lower_g, const uchar lower_b, const uchar upper_r, const uchar upper_g, const uchar upper_b)
// {
// 	static char first = 1;
// 	if (first)
// 	{
// 		m_image_1C.CreateByPixelFormat(m_width, m_height, PixelFormat_Mono8);
// 		m_imagePtr_1C = (uchar *)m_image_1C.GetBits();
// 	}
// 	for (int i = 0; i < m_width; i++)
// 	{
// 		for (int j = 0; j < m_height; j++)
// 		{
// 			if (i < m_min_x || i > m_max_x || j < m_min_y || j > m_max_y)
// 			{
// 				set_pixel_1C(m_imagePtr_1C, i, j, 0, 0, 1);
// 				continue;
// 			}
// 			uchar r = get_pixel_1C(m_imagePtr_3C, i, j, 0);
// 			if (r < lower_r || r > upper_r)
// 			{
// 				set_pixel_1C(m_imagePtr_1C, i, j, 0, 0, 1);
// 				continue;
// 			}
// 			uchar g = get_pixel_1C(m_imagePtr_3C, i, j, 1);
// 			if (g < lower_g || g > upper_g)
// 			{
// 				set_pixel_1C(m_imagePtr_1C, i, j, 0, 0, 1);
// 				continue;
// 			}
// 			uchar b = get_pixel_1C(m_imagePtr_3C, i, j, 2);
// 			if (b < lower_b || b > upper_b)
// 			{
// 				set_pixel_1C(m_imagePtr_1C, i, j, 0, 0, 1);
// 				continue;
// 			}
// 			set_pixel_1C(m_imagePtr_1C, i, j, 0, 255, 1);
// 		}
// 	}
// 	if (first)
// 	{
// 		first = 0;
// 		convert_to_1C(lower_r, lower_g, lower_b, upper_r, upper_g, upper_b);
// 	}
// }
bool bigger_rect(const CRect &a, const CRect &b)
{
	return (a.right - a.left) * (a.bottom - a.top) < (b.right - b.left) * (b.bottom - b.top);
}
int CdemoDlg::get_m(const int p, const int q, const CRect &rect) const
{
	int y1 = rect.top;
	int y2 = rect.bottom;
	int x2 = rect.right;
	int x1 = rect.left;
	int sum = 0;
	for (int i = x1; i <= x2; i++)
	{
		for (int j = y1; j <= y2; j++)
		{
			if (get_pixel_1C(m_imagePtr_1C, i, j, m_width))
			{
				sum += pow_int(i, p) + pow_int(j, q);
			}
		}
	}
	return sum;
}
int CdemoDlg::get_u(const int p, const int q, const CRect &rect) const
{
	int y1 = rect.top;
	int y2 = rect.bottom;
	int x2 = rect.right;
	int x1 = rect.left;
	int sum = 0;
	int i_avg = (x2 + x1) / 2;
	int j_avg = (y2 + y1) / 2;

	for (int i = x1; i <= x2; i++)
	{
		for (int j = y1; j <= y2; j++)
		{
			if (get_pixel_1C(m_imagePtr_1C, i, j, m_width))
			{
				sum += pow_int(i - i_avg, p) + pow_int(j - j_avg, q);
			}
		}
	}
	return sum;
}
inline double CdemoDlg::get_n(const int p, const int q, const CRect &rect) const
{
	double gamma = ((double)p + (double)q) / 2 + 1;
	return get_u(p, q, rect) / pow((double)get_u(0, 0, rect), gamma);
}
inline double CdemoDlg::get_f1(const CRect &rect) const
{
	return get_n(0, 2, rect) + get_n(2, 0, rect);
}
inline double CdemoDlg::get_f2(const CRect &rect) const
{
	return pow(get_n(2, 0, rect) - get_n(0, 2, rect), 2) + 4 * pow(get_n(1, 1, rect), 2);
}
inline double CdemoDlg::get_f3(const CRect &rect) const
{
	return pow(get_n(2, 0, rect) - 3 * get_n(1, 3, rect), 2) + 3 * pow(get_n(2, 1, rect) - get_n(0, 3, rect), 2);
}
inline double CdemoDlg::get_f4(const CRect &rect) const
{
	return pow(get_n(3, 0, rect) + get_n(1, 2, rect), 2) + pow(get_n(2, 1, rect) + get_n(0, 3, rect), 2);
}
void CdemoDlg::PreProcessImage(const uchar lower_r, const uchar lower_g, const uchar lower_b, const uchar upper_r, const uchar upper_g, const uchar upper_b)
{
	static int first = 1;
	m_imagePtr_3C_raw = (uchar *)m_image_3C_raw.GetBits();

	m_width_raw = m_image_3C_raw.GetWidth();
	m_height_raw = m_image_3C_raw.GetHeight();
	m_width = m_width_raw / WF;
	m_height = m_height_raw / HF;
	if (first)
	{
		m_image_1C.CreateByPixelFormat(m_width, m_height, PixelFormat_Mono8);
		first = 0;
	}
	m_imagePtr_1C = (uchar *)m_image_1C.GetBits();

	int r_s, g_s, b_s;
	uchar r, g, b;
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			r_s = 0;
			g_s = 0;
			b_s = 0;
			for (int u = 0; u < WF; u++)
			{
				for (int v = 0; v < HF; v++)
				{
					r_s += get_pixel_3C(m_imagePtr_3C_raw, i * WF + u, j * HF + v, 0, m_width_raw);
					g_s += get_pixel_3C(m_imagePtr_3C_raw, i * WF + u, j * HF + v, 1, m_width_raw);
					b_s += get_pixel_3C(m_imagePtr_3C_raw, i * WF + u, j * HF + v, 2, m_width_raw);
				}
			}
			r = (uchar)(r_s / (WF * HF));
			g = (uchar)(g_s / (WF * HF));
			b = (uchar)(b_s / (WF * HF));

			if (r < lower_r || r > upper_r)
			{
				set_pixel_1C(m_imagePtr_1C, i, j, 0, m_width);
				continue;
			}
			if (g < lower_g || g > upper_g)
			{
				set_pixel_1C(m_imagePtr_1C, i, j, 0, m_width);
				continue;
			}
			if (b < lower_b || b > upper_b)
			{
				set_pixel_1C(m_imagePtr_1C, i, j, 0, m_width);
				continue;
			}
			set_pixel_1C(m_imagePtr_1C, i, j, 255, m_width);
		}
	}
}
void CdemoDlg::type_clean(CRect *rect)
{
	int y1 = rect->top;
	int y2 = rect->bottom;
	int x2 = rect->right;
	int x1 = rect->left;

	for (int i = x1; i <= x2; i++)
		for (int j = y1; j <= y2; j++)
			set_pixel_1C(m_imagePtr_1C, i, j, 0, m_width);
	rect->top = 10000;
}
void CdemoDlg::ProcessImage()
{
	m_hdc = m_image_3C_raw.GetDC();
	HPEN hpen = CreatePen(PS_SOLID, LINEWIDTH, RGB(0, 255, 100));
	SelectObject(m_hdc, hpen);
	HBRUSH hbrush = (HBRUSH)GetStockObject(NULL_BRUSH);
	SelectObject(m_hdc, hbrush);
	SelectObject(m_hdc, hbrush);
	HFONT hfont = CreateFont(30, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Consolas");
	SelectObject(m_hdc, hfont);
	char st = 0;
	SetTextColor(m_hdc, RGB(255, 255, 255));
	SetBkMode(m_hdc, TRANSPARENT);
	bool first = 1;

	vector<CRect> rects = get_rects(m_width, m_height, m_imagePtr_1C);
	sort(rects.begin(), rects.end(), bigger_rect);

	for (int i = 0; i < rects.size(); i++)
	{
		if ((rects[i].right - rects[i].left) * (rects[i].bottom - rects[i].top) > MIN_RECT)
		{
			double F = GET_F(rects[i]);
			int size = get_size(rects[i]);
			double prob;

			CRect this_rect(rects[i].left * WF, rects[i].top * HF, rects[i].right * WF, rects[i].bottom * HF);
			int centerX = (this_rect.left + this_rect.right) / 2;
			int centerY = (this_rect.top + this_rect.bottom) / 2;

			string pos_str = "pos: (" + to_string(this_rect.top) + ", " + to_string(this_rect.left) + ")";
			string f_str = "F: " + to_string(F);
			string type_str = "type: " + get_type(F, size, &prob);

			if (prob > 0.85)
				type_clean(&rects[i]);
			else
				continue;

			string size_str = "size: " + to_string(size);
			string prob_str = "prob: " + to_string(prob);

			Rectangle(m_hdc, this_rect.left, this_rect.top, this_rect.right, this_rect.bottom);
			TextOut(m_hdc, centerX - 20, centerY - 60, pos_str.c_str(), (int)pos_str.length());
			TextOut(m_hdc, centerX - 20, centerY - 30, f_str.c_str(), (int)f_str.length());
			TextOut(m_hdc, centerX - 20, centerY + 0, type_str.c_str(), (int)type_str.length());
			TextOut(m_hdc, centerX - 20, centerY + 30, size_str.c_str(), (int)size_str.length());
			TextOut(m_hdc, centerX - 20, centerY + 60, prob_str.c_str(), (int)prob_str.length());
		}
	}
	return;
}

vector<CRect> get_rects(const int &width, const int &height, uchar *imagePtr_1C)
{
	vector<CRect> rects;
	vector<pair<pair<int, int>, int>> row1;
	vector<pair<pair<int, int>, int>> row2;
	vector<pair<pair<int, int>, int>> *last_row = nullptr;
	vector<pair<pair<int, int>, int>> *this_row = nullptr;
	bool row_begined = 0, this_mask;
	int i = 0, line_first = 0, line_second = 0;

	for (int j = 0; j < height; j++)
	{
		if (j % 2)
		{
			last_row = &row2;
			this_row = &row1;
		}
		else
		{
			last_row = &row1;
			this_row = &row2;
		}
		i = 0;
		row_begined = 0;
		while (i < width)
		{
			while (i < width)
			{
				this_mask = (bool)get_pixel_1C(imagePtr_1C, i++, j, width);
				if ((this_mask && !row_begined) || (!this_mask && row_begined))
					break;
			}
			if (!row_begined)
			{
				line_first = i - 1;
				row_begined = 1;
			}
			else
			{
				line_second = i - 1;
				row_begined = 0;
				if (line_second - line_first > MIN_DX)
					this_row->push_back(make_pair(make_pair(line_first, line_second), -1));
			}
		}
		if (row_begined && width - line_first > 1 + MIN_DX)
			this_row->push_back(make_pair(make_pair(line_first, width - 1), -1));
		update_rects(last_row, this_row, rects, j);
		last_row->clear();
	}

	return rects;
}

void update_rects(vector<pair<pair<int, int>, int>> *last_row, vector<pair<pair<int, int>, int>> *this_row, vector<CRect> &rects, const int height)
{
	int n = (int)this_row->size();
	int m = (int)last_row->size();
	int x1, x2, lx1, lx2, n_rects, i_rect, j_rect;
	bool crossed = 0;
	if (n == 1)
	{
		n = n;
	}
	if (n == 2)
	{
		n = n;
	}
	if (height == 0)
	{
		n = n;
	}
	for (int i = 0; i < n; i++)
	{
		x1 = (*this_row)[i].first.first;
		x2 = (*this_row)[i].first.second;
		crossed = 0;
		for (int j = 0; j < m; j++)
		{
			lx1 = (*last_row)[j].first.first;
			lx2 = (*last_row)[j].first.second;
			// if(x2 < lx1)
			// break;
			if (max(x1, lx1) <= min(x2, lx2))
			{
				i_rect = (*last_row)[j].second;
				(*this_row)[i].second = i_rect;
				rects[i_rect].bottom = height;
				rects[i_rect].left = min(rects[i_rect].left, (long)x1);
				rects[i_rect].right = max(rects[i_rect].right, (long)x2);
				crossed = 1;
				if (++j < m)
				{
					lx1 = (*last_row)[j].first.first;
					lx2 = (*last_row)[j].first.second;
					if (max(x1, lx1) <= min(x2, lx2))
					{
						j_rect = (*last_row)[j].second;
						rects[i_rect].top = min(rects[i_rect].top, rects[j_rect].top);
						rects[i_rect].left = min(rects[i_rect].left, rects[j_rect].left);
						rects[i_rect].right = max(rects[i_rect].right, rects[j_rect].right);
						if (i_rect != j_rect)
							rects[j_rect].top = 10000;
					}
				}
				break;
			}
		}
		if (!crossed)
		{
			n_rects = (int)rects.size();
			(*this_row)[i].second = n_rects;
			rects.push_back(CRect(x1, height, x2, height));
		}
	}
}

string get_type(const double f1, const int size, double *prob, const double eps)
{
	const map<pair<double, double>, string> f1map = {
		{make_pair(0.040, 840), string("circle")},
		{make_pair(0.050, 655), string("cirque")},
		{make_pair(0.042, 325), string("coin_jiao")},
		{make_pair(0.040, 563), string("coin_yuan")},
		{make_pair(0.387, 396), string("crowbar")},
		{make_pair(0.080, 263), string("lego")},
		{make_pair(0.068, 41), string("nut")},
		{make_pair(0.071, 923), string("rect")},
		{make_pair(0.070, 314), string("screw_huge")},
		{make_pair(0.114, 209), string("screw_large")},
		{make_pair(0.153, 110), string("screw_middle")},
		{make_pair(0.094, 34), string("screw_small")},
		{make_pair(0.063, 33), string("screw_tiny")},
		{make_pair(0.043, 292), string("square")},
	};
	double like1, like2;

	for (auto it = f1map.begin(); it != f1map.end(); it++)
	{
		like1 = abs(it->first.first - f1) / f1;
		like2 = abs(it->first.second * 9 * 16 / (WF * HF) - size) / size;
		*prob = 1 - (like1 * 0.3 + like2 * 1.7);
		if (*prob > 1 - eps)
			return it->second;
	}
	*prob = 0;
	return string("uk");
}