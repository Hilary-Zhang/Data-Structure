// rainDlg.cpp : ʵ���ļ�
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

#define DEFAULT_RAIN_NUM 88//��ʼʱ�������
#define DEFAULT_BOTTOM_HEIGHT 70//�������
#define DEFAULT_DROPLET_MAX_X 450//�������ʱ���ĺ�����
#define DEFAULT_DROPLET_MAX_Y 40//�������ʱ����������
#define DEFAULT_DROPLET_MIN_LEN 5//����������С����
#define DEFAULT_DROPLET_MAX_LEN 25//�������ʱ���ĳ���
#define DEFAULT_DROPLET_MAX_ANGLE 450000//�������ʱ������б��
#define DEFAULT_DROPLET_MAX_SPEED 3//�������ʱ���������ٶ�
#define DEFAULT_DROPLET_MAX_THICK 2//�������ʱ���Ĵֶ�
#define MAX_COLOR 300//������ɫ��Χ[0,300]
#define RAIN_SPEED 70//Ĭ��ˢ������ٶ�,���>=10
#define ELLIPSE 8//������Բ��

// CrainDlg �Ի���
CrainDlg::CrainDlg(CWnd* pParent /*=NULL*/): CDialogEx(CrainDlg::IDD, pParent)
{
	//��ʼ��˫������
	struct dropletchain *p;
	m_rainnum=DEFAULT_RAIN_NUM;//�����ΪĬ�������
	pDropletchain=pDropletrear=NULL;
	for(int i=0;i<m_rainnum;i++)
	{
		p=new dropletchain;//����ǰ��������ڴ�ռ�
		p->data=new droplet;
		initDroplet(p->data);//��ʼ�����
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
	//�ͷ�����ռ�õ��ڴ�
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

// CrainDlg ��Ϣ�������
BOOL CrainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_slider.SetRange(10,100);//���������ٶȷ�Χ
	m_slider.SetPos(RAIN_SPEED);//���ó�ʼ�����ٶ�
	sndPlaySound(_T("rain.wav"),SND_ASYNC|SND_NOWAIT|SND_LOOP);//��������
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ��������Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó����⽫�ɿ���Զ���ɡ�
void CrainDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	isRunning=true;
	thread=AfxBeginThread(onThread,this);//������ͼ�߳�
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
HCURSOR CrainDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//��ʼ�����
void CrainDlg::initDroplet(droplet *droplet)
{
	droplet->x=rand()%DEFAULT_DROPLET_MAX_X;//�������������֮���������
	droplet->y=rand()%DEFAULT_DROPLET_MAX_Y;
	droplet->len=rand()%DEFAULT_DROPLET_MAX_LEN+DEFAULT_DROPLET_MIN_LEN;
	droplet->angle=rand()%DEFAULT_DROPLET_MAX_ANGLE/100000.0;
	droplet->speed=rand()%DEFAULT_DROPLET_MAX_SPEED+1;
	droplet->thick=rand()%DEFAULT_DROPLET_MAX_THICK+1;
	droplet->radius=1;
	droplet->color=RGB(rand()%MAX_COLOR,rand()%MAX_COLOR,rand()%MAX_COLOR);
}

UINT CrainDlg::onThread(LPVOID wParam)//�߳�
{
	CrainDlg *p=(CrainDlg *)wParam;//ǿ��ת��
	p->doThread();//ִ���߳�
	return 0;
}

void CrainDlg::doThread()//���߳�ִ�еķ���
{
	CBrush aBrush;//����,��Ҫ���������ɫ
	CPen aPen;//�ֱ�,��Ҫ����������
	CRect aRect;//����,
	CWnd * pWind=GetDlgItem(IDC_PIC);//��ȡ��ͼ����Դ
	CDC* pDC=pWind->GetDC();//����
	struct dropletchain *pRain;//������ȡ�������
	while(1)
	{
		//������
		aBrush.CreateSolidBrush(RGB(0,0,0));//���û��ʵ���ɫ,����յ���ɫ
		pWind->GetClientRect(&aRect);//��ȡ������С
		aRect.bottom-=DEFAULT_BOTTOM_HEIGHT;//�޸ĵײ��ĸ߶�,�ײ�����Ϊ���
		pDC->FillRect(aRect,&aBrush);//�����Ϸ����
		aBrush.DeleteObject();//���껭��ɾ��
		aBrush.CreateSolidBrush(RGB(9,128,104));//���ó�������ɫ
		aRect.top+=aRect.Height();//��������
		aRect.bottom+=DEFAULT_BOTTOM_HEIGHT;//�ײ��߶�����
		pDC->FillRect(aRect,&aBrush);//�����·�����
		aBrush.DeleteObject();//ɾ������
		pDC->SelectStockObject(NULL_BRUSH);//��DC���óɿ��Ļ���,�ڻ�����ʱ���Ի���������Բ
		pRain=pDropletchain;//��ָ����������ͷ
		while(pRain)
		{
			aPen.CreatePen(PS_SOLID,pRain->data->thick,pRain->data->color);//�����ֱ�
			pDC->SelectObject(&aPen);//������ֱ�������
			if(pRain->data->y>aRect.top)//����㵽���������
			{
				//������
				pDC->Ellipse(pRain->data->x-pRain->data->radius-ELLIPSE,pRain->data->y-pRain->data->radius,pRain->data->x+pRain->data->radius+ELLIPSE,pRain->data->y+pRain->data->radius);//������Բ
				pRain->data->radius++;//������Բ�뾶����
				if(pRain->data->radius>pRain->data->len||pRain->data->x+pRain->data->radius+ELLIPSE>=aRect.Width())
					initDroplet(pRain->data);//������뾶������㳤���������������
			}
			else
			{
				//�����
				pDC->MoveTo(pRain->data->x,pRain->data->y);//�ѻ����ƶ���ָ��������
				pDC->LineTo(pRain->data->x+(int)pRain->data->len*sin(pRain->data->angle),pRain->data->y+(int)pRain->data->len*cos(pRain->data->angle));//���ݽǶȺͳ��Ȼ��Ƴ�б��
				pRain->data->x+=pRain->data->speed;pRain->data->y+=pRain->data->speed;//������������,�������Ϊ���Ե�speed
				if(pRain->data->x+(int)pRain->data->len*sin(pRain->data->angle)>=aRect.Width()-pRain->data->thick)
					initDroplet(pRain->data);//��������Ҳ�����������������
			}
			aPen.DeleteObject();//ɾ���ֱ�
			pRain=pRain->next;//
		}
		Sleep(100-m_slider.GetPos());//����ѭ���������߳�,�γɶ���Ч��
	}
	pDC->ReleaseOutputDC();//�ͷ�DC��Դ
}

void CrainDlg::OnBnClickedButton1()//�˳���ť��ʵ��
{
	CDialogEx::OnCancel();
}

void CrainDlg::OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult)//������¼Ӽ���ť����Ϣ
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	if(pNMUpDown->iDelta>0)//�������
	{
		if(m_rainnum<=1)
		{
			AfxMessageBox(_T("ʣ��һ������~�����ټ�����~!"));
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
	else//�������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(thread!=NULL&&!GetExitCodeThread(thread,0))
	{
		if(isRunning)
		{
			m_command.SetWindowTextW(_T("�ָ�"));
			thread->SuspendThread();
			isRunning=false;
		}
		else
		{
			m_command.SetWindowTextW(_T("��ͣ"));
			thread->ResumeThread();
			isRunning=true;
		}
	}
}


