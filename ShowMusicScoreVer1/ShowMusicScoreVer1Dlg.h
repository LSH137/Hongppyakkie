
// ShowMusicScoreVer1Dlg.h: 헤더 파일
//

#pragma once

#define MESSAGE_INCREASE_COUNT WM_USER

struct eyePoint
{
	int x;
	int y;
};

// CShowMusicScoreVer1Dlg 대화 상자
class CShowMusicScoreVer1Dlg : public CDialogEx
{
private:
	CRect m_image_rect;  // Picture Control의 위치를 기억할 변수
	CImage m_image;  // 사용자가 선택한 이미지 객체를 구성할 변수

	CRect m_rcMinimumDialog;
	void SetupForDynamicLayout();

// 생성입니다.
public:
	CShowMusicScoreVer1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	CWinThread* m_pThread = nullptr;
	bool m_isWorkingThread = 0;
	int m_nCount = 0;
	LRESULT NextPage(WPARAM wParam, LPARAM lParam);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOWMUSICSCOREVER1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedSubscore();
	afx_msg void OnStnClickedMainscore();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawProcess(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdrawVolume(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedSetting();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedRec();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnEnChangeEdit1();
};
