// rainDlg.h : ͷ�ļ�
//
#pragma once
#include "afxwin.h"

//������Ͷ���
typedef struct
{
	COLORREF color;//�����ɫ
	int x,y;//��ʼ����
	int len;//����С
	float angle;//���Ƕ�
	int speed;//�����ٶ�
	int thick;//����ϸ
	int radius;//�����뾶
}droplet;//�������

//���˫��������
struct dropletchain
{
	droplet *data;//������
	struct dropletchain *pre,*next;//ָ����
};

// CrainDlg �Ի���
class CrainDlg : public CDialogEx
{
// ����
public:
	CrainDlg(CWnd* pParent = NULL);// ��׼���캯��
	~CrainDlg();
// �Ի�������
	enum { IDD = IDD_RAIN_DIALOG };
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	static UINT onThread(LPVOID wParam);//�̺߳���
	struct dropletchain *pDropletchain,*pDropletrear;//����������������ͷ������β
	void initDroplet(droplet *droplet);//��ʼ�����ķ���
	CSliderCtrl m_slider;//�󶨿�������ٶȵĻ�����
	int m_rainnum;//�����������
	CButton m_command;//�󶨿��������ͣ�İ�ť
	BOOL isRunning;//�����ж���ͣ�ı���
	CWinThread *thread;//��Ż�ͼ�̵߳�ָ��
public:
	void doThread();//��ͼ�ķ���
	afx_msg void OnBnClickedButton1();//�˳���ť
	afx_msg void OnBnClickedButton2();//��ͣ��ť
	afx_msg void OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult);//��������ư�ť
	afx_msg void OnReleasedcaptureSlider(NMHDR *pNMHDR, LRESULT *pResult);//����ٶȻ�����
};
