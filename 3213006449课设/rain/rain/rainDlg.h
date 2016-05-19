// rainDlg.h : 头文件
//
#pragma once
#include "afxwin.h"

//雨点类型定义
typedef struct
{
	COLORREF color;//雨点颜色
	int x,y;//初始坐标
	int len;//雨点大小
	float angle;//雨点角度
	int speed;//落下速度
	int thick;//雨点粗细
	int radius;//涟漪半径
}droplet;//雨点类型

//雨的双向链表定义
struct dropletchain
{
	droplet *data;//数据域
	struct dropletchain *pre,*next;//指针域
};

// CrainDlg 对话框
class CrainDlg : public CDialogEx
{
// 构造
public:
	CrainDlg(CWnd* pParent = NULL);// 标准构造函数
	~CrainDlg();
// 对话框数据
	enum { IDD = IDD_RAIN_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	static UINT onThread(LPVOID wParam);//线程函数
	struct dropletchain *pDropletchain,*pDropletrear;//定义雨点链表的链表头和链表尾
	void initDroplet(droplet *droplet);//初始化雨点的方法
	CSliderCtrl m_slider;//绑定控制雨点速度的滑动条
	int m_rainnum;//存放雨点的数量
	CButton m_command;//绑定控制雨点暂停的按钮
	BOOL isRunning;//定义判断暂停的变量
	CWinThread *thread;//存放绘图线程的指针
public:
	void doThread();//画图的方法
	afx_msg void OnBnClickedButton1();//退出按钮
	afx_msg void OnBnClickedButton2();//暂停按钮
	afx_msg void OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult);//雨点数控制按钮
	afx_msg void OnReleasedcaptureSlider(NMHDR *pNMHDR, LRESULT *pResult);//雨点速度滑动条
};
