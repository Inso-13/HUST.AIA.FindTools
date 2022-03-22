
// demoDlg.h: 头文件
//

#pragma once
#include <vector>
#include "MVGigE.h"
#include "MVImage.h"

#include <stdio.h>
#include <io.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include "opencv.hpp"
#include "imgproc.hpp"
#include "highgui.hpp"
// #include "GigECamera_Types.h"
#define LINEWIDTH 3
#define MIN_DX 0 
#define GET_F get_f1
#define WF 8
// #define WF 16
#define HF 8
#define MIN_RECT (1000/(WF*HF))
#define TEST_IMG_PATH "J:\\images3\\all_hard_0002.bmp"
// #define TEST_IMG_PATH "J:\\images3\\all_0002.bmp"
#define TEST_IMG_DIR_PATH "J:\\images3\\"

// #define SHOW_1C

// using namespace cv;
using namespace std;

// CdemoDlg 对话框
class CdemoDlg : public CDialogEx
{
	// 构造
public:
	//定义相机句柄对象
	HANDLE m_hCam;
	//定义图像处理对象
	MVImage m_image_3C_raw;
	MVImage m_image_1C;
	HDC m_hdc;
	//定义像素格式对象
	MV_PixelFormatEnums m_PixelFormat;
	//采集标识
	bool m_bRun;
	//通过回调函数获取图像数据信息
	int OnStreamCB(MV_IMAGE_INFO *pInfo);
	//画图
	void DrawImage();
	//关闭相机、释放资源
	void OnClose();
	//处理图像
	void ProcessImage();
	void PreProcessImage(const uchar lower_r, const uchar lower_g, const uchar lower_b, const uchar upper_r, const uchar upper_g, const uchar upper_b);
	CdemoDlg(CWnd *pParent = nullptr); // 标准构造函数
	int m_height;
	int m_width;
	int m_height_raw;
	int m_width_raw;
	uchar *m_imagePtr_3C_raw;
	uchar *m_imagePtr_1C;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum
	{
		IDD = IDD_DEMO_DIALOG
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV 支持

	void type_clean(CRect *rect);
	// void convert_to_1C(const uchar lower_r, const uchar lower_g, const uchar lower_b, const uchar upper_r, const uchar upper_g,const  uchar upper_b);
	// void dilate(const int kx,const int ky);
	// void erode(const int kx,const int ky);
	// void get_bound();


	int get_m(const int p, const int q, const CRect &rect) const;
	int get_u(const int p, const int q, const CRect &rect) const;
	int get_size(const CRect& rect) const;

	inline double get_n(const int p, const int q, const CRect &rect) const;
	inline double get_f1(const CRect &rect) const;
	inline double get_f2(const CRect &rect) const;
	inline double get_f3(const CRect &rect) const;
	inline double get_f4(const CRect &rect) const;
	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpencam();
	afx_msg void OnBnClickedStartgrab();
	afx_msg void OnBnClickedClosecam();
	afx_msg void OnBnClickedTest();
};

int __stdcall StreamCB(MV_IMAGE_INFO *pInfo, ULONG_PTR nUserVal);

int pow_int(const int x,const int a);
string get_type(const double f1,const int size,double* prob, const double eps=0.15);

void update_rects(vector<pair<pair<int, int>, int>> *last_row, vector<pair<pair<int, int>, int>> *this_row, vector<CRect> &rects, const int height);

inline uchar get_pixel_3C(const uchar *imagePtr, const int &x, const int &y, const int channel, const int width);
inline uchar get_pixel_1C(const uchar *imagePtr, const int &x, const int &y, const int width);
inline void set_pixel_1C(uchar *imagePtr, const int &x, const int &y, const uchar val, const int width);
vector<CRect> get_rects(const int &width, const int &height,uchar *imagePtr_1C);
void erode_to(uchar *imagePtr_1C, int width, CRect rect, uchar *temp_image, int kernel=3);
inline int round4(int x);