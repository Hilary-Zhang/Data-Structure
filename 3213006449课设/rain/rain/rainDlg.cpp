// rainDlg.cpp : 实现文件
#include "stdafx.h"
#include "rain.h"
#include "rainDlg.h"
#include "afxdialogex.h"
#include <math.h>
#include <mmsystem.h>
#pragma comment(lib,"WINMM.LIB")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_RAIN_NUM 88//初始时雨点数量
#define DEFAULT_BOTTOM_HEIGHT 70//池塘深度
#define DEFAULT_DROPLET_MAX_X 450//生成雨点时最大的横坐标
#define DEFAULT_DROPLET_MAX_Y 40//生成雨点时最大的纵坐标
#define DEFAULT_DROPLET_MIN_LEN 5//生成雨点的最小长度
#define DEFAULT_DROPLET_MAX_LEN 25//生成雨点时最大的长度
#define DEFAULT_DROPLET_MAX_ANGLE 450000//生成雨点时最大的倾斜角
#define DEFAULT_DROPLET_MAX_SPEED 3//生成雨点时最大的下落速度
#define DEFAULT_DROPLET_MAX_THICK 2//生成雨点时最大的粗度
#define MAX_COLOR 300//最大的颜色范围[0,300]
#define RAIN_SPEED 70//默认刷新雨点速度,务必>=10
#define ELLIPSE 8//涟漪椭圆率

// CrainDlg 对话框
CrainDlg::CrainDlg(CWnd* pParent /*=NULL*/): CDialogEx(CrainDlg::IDD, pParent)
{
	//初始化双向链表
	struct dropletchain *p;
	m_rainnum=DEFAULT_RAIN_NUM;//雨点数为默认雨点数
	pDropletchain=pDropletrear=NULL;
	for(int i=0;i<m_rainnum;i++)
	{
		p=new dropletchain;//给当前结点申请内存空间
		p->data=new droplet;
		initDroplet(p->data);//初始化雨点
		p->next=NULL;
		p->pre=pDropletrear;
		if(pDropletchain==NULL)
			pDropletchain=p;
		else
			pDropletrear->next=p;
		pDropletrear=p;
	}
	
	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CrainDlg::~CrainDlg()
{
	//释放链表占用的内存
	struct dropletchain *p,*q;
	p=pDropletchain;
	while(p)
	{
		q=p->next;
		delete p->data;
		delete p;
		p=q;
	}
}

void CrainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Slider(pDX, IDC_SLIDER, m_speed);
	//  DDV_MinMaxInt(pDX, m_speed, 10, 300);
	DDX_Control(pDX, IDC_SLIDER, m_slider);
	//  DDX_Text(pDX, IDC_DROPNUM, m_dropnum);
	DDX_Control(pDX, IDC_BUTTON2, m_command);
}

BEGIN_MESSAGE_MAP(CrainDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CrainDlg::OnBnClickedButton1)
	ON_NOTIFY(UDN_DELTAPOS, IDD_SPIN, &CrainDlg::OnDeltaposSpin)
	ON_BN_CLICKED(IDC_BUTTON2, &CrainDlg::OnBnClickedButton2)
END_MESSAGE_MAP()

// CrainDlg 消息处理程序
BOOL CrainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	// TODO: 在此添加额外的初始化代码
	m_slider.SetRange(10,100);//设置雨点的速度范围
	m_slider.SetPos(RAIN_SPEED);//设置初始雨点的速度
	sndPlaySound(_T("rain.wav"),SND_ASYNC|SND_NOWAIT|SND_LOOP);//播声音放
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，这将由框架自动完成。
void CrainDlg::OnPaint()
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
	isRunning=true;
	thread=AfxBeginThread(onThread,this);//启动绘图线程
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CrainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//初始化雨点
void CrainDlg::initDroplet(droplet *droplet)
{
	droplet->x=rand()%DEFAULT_DROPLET_MAX_X;//雨点在最大横坐标之内随机产生
	droplet->y=rand()%DEFAULT_DROPLET_MAX_Y;
	droplet->len=rand()%DEFAULT_DROPLET_MAX_LEN+DEFAULT_DROPLET_MIN_LEN;
	droplet->angle=rand()%DEFAULT_DROPLET_MAX_ANGLE/100000.0;
	droplet->speed=rand()%DEFAULT_DROPLET_MAX_SPEED+1;
	droplet->thick=rand()%DEFAULT_DROPLET_MAX_THICK+1;
	droplet->radius=1;
	droplet->color=RGB(rand()%MAX_COLOR,rand()%MAX_COLOR,rand()%MAX_COLOR);
}

UINT CrainDlg::onThread(LPVOID wParam)//线程
{
	CrainDlg *p=(CrainDlg *)wParam;//强制转换
	p->doThread();//执行线程
	return 0;
}

void CrainDlg::doThread()//由线程执行的方法
{
	CBrush aBrush;//画笔,主要用来填充颜色
	CPen aPen;//钢笔,主要用来画线条
	CRect aRect;//矩形,
	CWnd * pWind=GetDlgItem(IDC_PIC);//获取画图框资源
	CDC* pDC=pWind->GetDC();//画布
	struct dropletchain *pRain;//用来读取雨点链表
	while(1)
	{
		//画背景
		aBrush.CreateSolidBrush(RGB(0,0,0));//设置画笔的颜色,即天空的颜色
		pWind->GetClientRect(&aRect);//获取画布大小
		aRect.bottom-=DEFAULT_BOTTOM_HEIGHT;//修改底部的高度,底部向上为天空
		pDC->FillRect(aRect,&aBrush);//绘制上方天空
		aBrush.DeleteObject();//用完画笔删除
		aBrush.CreateSolidBrush(RGB(9,128,104));//设置池塘的颜色
		aRect.top+=aRect.Height();//顶部下移
		aRect.bottom+=DEFAULT_BOTTOM_HEIGHT;//底部高度下移
		pDC->FillRect(aRect,&aBrush);//绘制下方池塘
		aBrush.DeleteObject();//删除画笔
		pDC->SelectStockObject(NULL_BRUSH);//把DC设置成空心画笔,在画涟漪时可以画出空心椭圆
		pRain=pDropletchain;//把指针移向链表头
		while(pRain)
		{
			aPen.CreatePen(PS_SOLID,pRain->data->thick,pRain->data->color);//创建钢笔
			pDC->SelectObject(&aPen);//用这个钢笔来画画
			if(pRain->data->y>aRect.top)//若雨点到达池塘顶部
			{
				//画涟漪
				pDC->Ellipse(pRain->data->x-pRain->data->radius-ELLIPSE,pRain->data->y-pRain->data->radius,pRain->data->x+pRain->data->radius+ELLIPSE,pRain->data->y+pRain->data->radius);//画出椭圆
				pRain->data->radius++;//涟漪椭圆半径增大
				if(pRain->data->radius>pRain->data->len||pRain->data->x+pRain->data->radius+ELLIPSE>=aRect.Width())
					initDroplet(pRain->data);//如果雨点半径大于雨点长度则重新生成雨点
			}
			else
			{
				//画雨点
				pDC->MoveTo(pRain->data->x,pRain->data->y);//把画笔移动到指定的坐标
				pDC->LineTo(pRain->data->x+(int)pRain->data->len*sin(pRain->data->angle),pRain->data->y+(int)pRain->data->len*cos(pRain->data->angle));//根据角度和长度绘制出斜线
				pRain->data->x+=pRain->data->speed;pRain->data->y+=pRain->data->speed;//雨点的坐标增大,增大幅度为各自的speed
				if(pRain->data->x+(int)pRain->data->len*sin(pRain->data->angle)>=aRect.Width()-pRain->data->thick)
					initDroplet(pRain->data);//如果超出右侧界限则重新生成雨点
			}
			aPen.DeleteObject();//删除钢笔
			pRain=pRain->next;//
		}
		Sleep(100-m_slider.GetPos());//在死循环中休眠线程,形成动画效果
	}
	pDC->ReleaseOutputDC();//释放DC资源
}

void CrainDlg::OnBnClickedButton1()//退出按钮的实现
{
	CDialogEx::OnCancel();
}

void CrainDlg::OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult)//点击上下加减按钮的消息
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta>0)//减少雨点
	{
		if(m_rainnum<=1)
		{
			AfxMessageBox(_T("剩下一滴雨啦~不能再减少啦~!"));
		}
		else
		{
			struct dropletchain *p;
			p=pDropletrear->pre;
			p->next=NULL;
			delete pDropletrear->data;
			delete pDropletrear;
			pDropletrear=p;
			m_rainnum--;
		}
	}
	else//增加雨点
	{
		struct dropletchain *p;
		p=new dropletchain;
		p->data=new droplet;
		initDroplet(p->data);
		pDropletrear->next=p;
		p->pre=pDropletrear;
		p->next=NULL;
		pDropletrear=p;
		m_rainnum++;
	}
	*pResult=0;
}

void CrainDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if(thread!=NULL&&!GetExitCodeThread(thread,0))
	{
		if(isRunning)
		{
			m_command.SetWindowTextW(_T("恢复"));
			thread->SuspendThread();
			isRunning=false;
		}
		else
		{
			m_command.SetWindowTextW(_T("暂停"));
			thread->ResumeThread();
			isRunning=true;
		}
	}
}


