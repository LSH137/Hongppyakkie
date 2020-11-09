
// ShowMusicScoreVer1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ShowMusicScoreVer1.h"
#include "ShowMusicScoreVer1Dlg.h"
#include "afxdialogex.h"
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <iostream>
#include <Windows.h>
#include <atlimage.h>
#include <cstring>
#include <sstream>
#include "C:\opencv-4.5.0\build\include\opencv2\imgproc\imgproc.hpp"
#include "C:\opencv-4.5.0\build\include\opencv2\highgui\highgui.hpp"
#include "C:\opencv-4.5.0\build\include\opencv2\objdetect\objdetect.hpp"
#include <opencv2/opencv.hpp>
#include <stdlib.h>
#include "stdafx.h"
#include <afxmsg_.h>

#define TRUE 1
#define FALSE 0
#define MaxPathLength 500
#define BufferSize 10

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

///////////////////////////////////////////////////////////////////
// 전역변수 입니다
wchar_t file_name[5010][MaxPathLength]; //세로크기, 가로크기 악보파일의 경로를 지정한다.
int page; // 악보 넘길때 쓰는 값
int max_page_num; //악보 장수를 저장하는 변수 
FILE* fp_log; //log file pointer
wchar_t* rawpath;
wchar_t* r_path;
bool play = FALSE;
//std::mutex mtx;
eyePoint eye_point;
CMutex mtx(FALSE, NULL);
////////////////////////////////////////////////////////////////////

//log 관리 함수
bool LogMessage(char message[])
{
	errno_t err;
	//mtx.lock();
	mtx.Lock();
	err = fopen_s(&fp_log, "log.txt", "a");

	if (fp_log != nullptr)
	{
		fprintf(fp_log, message);
		fprintf(fp_log, "\n");

		fclose(fp_log);
		mtx.Unlock();
		//mtx.unlock();
		return TRUE;
	}
	else
	{
		//mtx.unlock();
		mtx.Unlock();
		AfxMessageBox(_T("error occur while recording log"));
		return FALSE;
	}
}
// unicode 처리용 log 관리 함수 오버로딩
bool LogMessage(wchar_t message[])
{
	errno_t err;
	//mtx.lock();
	mtx.Lock();
	err = fopen_s(&fp_log, "log.txt", "a");

	if (fp_log != nullptr)
	{
		fwprintf(fp_log, message);
		fprintf(fp_log, "\n");

		fclose(fp_log);
		//mtx.unlock();
		mtx.Unlock();
		return TRUE;
	}
	else
	{
		//mtx.unlock();
		mtx.Unlock();
		AfxMessageBox(_T("error occur while recording log"));
		return FALSE;
	}
}

//현재 시간 반환 함수
tm GetTime()
{
	tm tm_time;
	char date_log[50];
	time_t temp = time(nullptr);

	localtime_s(&tm_time, &temp);

	sprintf_s(date_log, sizeof(date_log), "%04d.%02d.%02d | %02d : %02d : %02d", tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday, tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
	LogMessage(date_log);

	return tm_time;
}

//wchar_t 에서 char 로의 형변환 함수
void ConvertWCtoC(wchar_t* str, char* str_ch)
{
	//입력받은 wchar_t 변수의 길이를 구함
	int strSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	//char* 메모리 할당
	str_ch = new char[strSize];
	//형 변환
	WideCharToMultiByte(CP_ACP, 0, str, -1, str_ch, strSize, 0, 0);
}

void ConverCtoWC(char* str, wchar_t* str_w)
{
	//wchar_t형 변수 선언
	//wchar_t* pStr;
	//멀티 바이트 크기 계산 길이 반환
	int strSize = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, NULL);

	//wchar_t 메모리 할당
	str_w = new WCHAR[strSize];
	//형 변환
	MultiByteToWideChar(CP_ACP, 0, str, strlen(str) + 1, str_w, strSize);
}

int FileName(const wchar_t dir[150])
{
	typedef struct _wfinddatai64_t FILE_SEARCH;
	FILE_SEARCH file_search;

	intptr_t h_file;
	int name_index = 0;
	LogMessage("func: FileName()");

	if ((h_file = _wfindfirst64(dir, &file_search)) == -1L)
	{
		AfxMessageBox(_T("No files in current directory!"));
		LogMessage("No files in current directory!");
	}
	else
	{
		do
		{
			wcscpy_s(file_name[name_index], MaxPathLength, file_search.name);
			LogMessage(file_name[name_index]);
			name_index++;

		} while (_wfindnext64(h_file, &file_search) == 0);

		_findclose(h_file);
	}
	LogMessage("func: FileName() return");
	return name_index;
}

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
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

// CShowMusicScoreVer1Dlg 대화 상자
CShowMusicScoreVer1Dlg::CShowMusicScoreVer1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SHOWMUSICSCOREVER1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShowMusicScoreVer1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CShowMusicScoreVer1Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_STN_CLICKED(IDC_SubScore, &CShowMusicScoreVer1Dlg::OnStnClickedSubscore)
	ON_STN_CLICKED(IDC_MainScore, &CShowMusicScoreVer1Dlg::OnStnClickedMainscore)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CShowMusicScoreVer1Dlg::OnDeltaposSpin1)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROCESS, &CShowMusicScoreVer1Dlg::OnNMCustomdrawProcess)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_VOLUME, &CShowMusicScoreVer1Dlg::OnNMCustomdrawVolume)
	ON_BN_CLICKED(IDC_SETTING, &CShowMusicScoreVer1Dlg::OnBnClickedSetting)
	ON_BN_CLICKED(IDC_PAUSE, &CShowMusicScoreVer1Dlg::OnBnClickedPause)
	ON_BN_CLICKED(IDC_PLAY, &CShowMusicScoreVer1Dlg::OnBnClickedPlay)
	//ON_BN_CLICKED(IDC_REC, &CShowMusicScoreVer1Dlg::OnBnClickedRec)
	ON_MESSAGE(MESSAGE_INCREASE_COUNT, &CShowMusicScoreVer1Dlg::NextPage)
	ON_EN_CHANGE(IDC_EDIT1, &CShowMusicScoreVer1Dlg::OnEnChangeEdit1)
END_MESSAGE_MAP()

void detectEye(cv::Mat& im, cv::Mat& tpl, cv::Rect& rect, cv::CascadeClassifier* face_cascade, cv::CascadeClassifier* eye_cascade)
{
	std::vector<cv::Rect> faces, eyes;
	face_cascade->detectMultiScale(im, faces, 1.1, 2, 0 | cv::COLOR_BGR2GRAY, cv::Size(30, 30)); //CV_HAAR_SCALE_IMAGE

	for (int i = 0; i < faces.size(); i++)
	{
		cv::Mat face = im(faces[i]);
		eye_cascade->detectMultiScale(face, eyes, 1.1, 2, 0 | cv::COLOR_BGR2GRAY, cv::Size(20, 20)); //CV_HAAR_SCALE_IMAGE

		if (eyes.size())
		{
			rect = eyes[0] + cv::Point(faces[i].x, faces[i].y);
			//printf("x: %d, y: %d\n", faces[i].x, faces[i].y);

			tpl = im(rect);
		}
	}
	//return eyes.size();
}

void trackEye(cv::Mat& im, cv::Mat& tpl, cv::Rect& rect)
{
	cv::Size size(rect.width * 2, rect.height * 2);
	cv::Rect window(rect + size - cv::Point(size.width / 2, size.height / 2));

	window &= cv::Rect(0, 0, im.cols, im.rows);

	cv::Mat dst(window.width - tpl.rows + 1, window.height - tpl.cols + 1, CV_32FC1);
	cv::matchTemplate(im(window), tpl, dst, cv::TM_SQDIFF_NORMED); //CV_TM_SQDIFF_NORMED

	double minval, maxval;
	cv::Point minloc, maxloc;
	cv::minMaxLoc(dst, &minval, &maxval, &minloc, &maxloc);

	if (minval <= 0.2)
	{
		rect.x = window.x + minloc.x;
		rect.y = window.y + minloc.y;
	}
	else
		rect.x = rect.y = rect.width = rect.height = 0;
}

UINT TurnPage(LPVOID param)
{
	CRect m_image_rect;
	cv::CascadeClassifier face_cascade;
	cv::CascadeClassifier eye_cascade;
	eyePoint buffer[BufferSize] = { 0 };
	char log[50] = {0};
	int index = 0;
	FILE* fp_eye_movement_data = nullptr;
	errno_t err = 0;

	err = fopen_s(&fp_eye_movement_data, "eyemovementdata.csv", "a");

	LogMessage("----------func: TurnPage()----------");

	CShowMusicScoreVer1Dlg* pMain = (CShowMusicScoreVer1Dlg*)param;

	while (pMain->m_isWorkingThread)
	{
		Sleep(30);
		if (play)
		{
			if (fp_eye_movement_data != nullptr)
			{
				// Load the cascade classifiers
				// Make sure you point the XML files to the right path, or 
				// just copy the files from [OPENCV_DIR]/data/haarcascades directory
				face_cascade.load("C:\\opencv-4.5.0\\sources\\data\\haarcascades\\haarcascade_frontalface_alt2.xml");
				eye_cascade.load("C:\\opencv-4.5.0\\sources\\data\\haarcascades\\haarcascade_eye.xml");

				// Open webcam
				cv::VideoCapture cap(0);

				// Check if everything is ok
				if (face_cascade.empty() || eye_cascade.empty() || !cap.isOpened())
					return 1;

				// Set video to 320x240
				cap.set(cv::CAP_PROP_FRAME_WIDTH, 320);
				cap.set(cv::CAP_PROP_FRAME_HEIGHT, 240);

				cv::Mat frame, eye_tpl;
				cv::Rect eye_bb;

				while (cv::waitKey(15) != 'q' && play)
				{
					cap >> frame;
					if (frame.empty())
						break;

					// Flip the frame horizontally, Windows users might need this
					cv::flip(frame, frame, 1);

					// Convert to grayscale and 
					// adjust the image contrast using histogram equalization
					cv::Mat gray;
					cv::cvtColor(frame, gray, 7); // CV_RGB2GRAY

					if (eye_bb.width == 0 && eye_bb.height == 0)
					{
						// Detection stage
						// Try to detect the face and the eye of the user
						detectEye(gray, eye_tpl, eye_bb, &face_cascade, &eye_cascade);
					}
					else
					{
						// Tracking stage with template matching
						trackEye(gray, eye_tpl, eye_bb);

						// Draw bounding rectangle for the eye
						cv::rectangle(frame, eye_bb, CV_RGB(0, 255, 0));
						fprintf(fp_eye_movement_data, "%d, %d\n", eye_bb.x, eye_bb.y);
						buffer[index % BufferSize].x = eye_bb.x;
						buffer[index % BufferSize].y = eye_bb.y;
						Sleep(200);

						sprintf_s(log, sizeof(log), "(%d, %d) -> (%d, %d)", buffer[(index - 1) % BufferSize].x, buffer[(index - 1) % BufferSize].y, buffer[index % BufferSize].x, buffer[index % BufferSize].y);
						LogMessage(log);

						if (buffer[(index - 1) % BufferSize].x - buffer[index % BufferSize].x > 3 && buffer[(index - 1) % BufferSize].y - buffer[index % BufferSize].y > 5) //&& sample1.y - sample2.y > 15
						{
							SendMessage(pMain->m_hWnd, MESSAGE_INCREASE_COUNT, NULL, NULL);
						}
						index++;
						//printf("x: %d, y: %d\n", eye_bb.x, eye_bb.y);
					}

					//Display video
					//cv::imshow("video", frame);
				}

				fclose(fp_eye_movement_data);
			}
			else
				printf("error occur while making data file\n");

		}
		LogMessage("----------func: TurnPage() end----------");

	}

	return 0;

}

/*
UINT ThreadForCounting(LPVOID param)
{
	CShowMusicScoreVer1Dlg* pMain = (CShowMusicScoreVer1Dlg*)param;

	while (pMain->m_isWorkingThread)
	{
		Sleep(30);

		PostMessage(pMain->m_hWnd, MESSAGE_INCREASE_COUNT, NULL, NULL);
	}

	return 0;
}
*/
// CShowMusicScoreVer1Dlg 메시지 처리기

BOOL CShowMusicScoreVer1Dlg::OnInitDialog()
{
	LogMessage("========================================");
	GetTime();
	CDialogEx::OnInitDialog();
	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	SetupForDynamicLayout();

	// 그림 출력에 사용하기 위해 Picture Control의 위치를 얻는다.
	GetDlgItem(IDC_MainScore)->GetWindowRect(m_image_rect);
	// GetWindowRect로 좌표를 얻으면 캡션과 테두리 영역이 포함되기
	// 때문에 해당 영역을 제외시킨다.
	ScreenToClient(m_image_rect);

	

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CShowMusicScoreVer1Dlg::SetupForDynamicLayout()
{
	// 동적 레이아웃 활성화.
	this->EnableDynamicLayout();

	/* 이동 옵션 */
	// 1. 이동하지 않음.
	auto move_none = CMFCDynamicLayout::MoveSettings{};
	// 2. X,Y 방향으로 100% 비율 이동.
	auto move_both_100 = CMFCDynamicLayout::MoveSettings{};
	move_both_100.m_nXRatio = 100;
	move_both_100.m_nYRatio = 100;
	// 3. X 방향으로만 100% 비율 이동.
	auto move_x_100 = CMFCDynamicLayout::MoveSettings{};
	move_x_100.m_nXRatio = 100;
	// 4. Y 방향으로만 100% 비율 이동.
	auto move_y_100 = CMFCDynamicLayout::MoveSettings{};
	move_y_100.m_nYRatio = 100;


	/* 크기 조정 옵션 */
	// 1. 크기 조정하지 않음.
	auto size_none = CMFCDynamicLayout::SizeSettings{};
	// 2. X,Y 방향으로 100% 비율 크기 조정.
	auto size_both_100 = CMFCDynamicLayout::SizeSettings{};
	size_both_100.m_nXRatio = 100;
	size_both_100.m_nYRatio = 100;
	// 3. X 방향으로만 100% 비율 크기 조정.
	auto size_x_100 = CMFCDynamicLayout::SizeSettings{};
	size_x_100.m_nXRatio = 100;
	// 4. Y 방향으로만 100% 비율 크기 조정.
	auto size_y_100 = CMFCDynamicLayout::SizeSettings{};
	size_y_100.m_nYRatio = 100;

	// Dialog의 동적 레이아웃 포인터 획득.
	auto manager = this->GetDynamicLayout();
	// 동적 레이아웃 생성.
	manager->Create(this);
	// 동적 레이아웃에 등록할 컨트롤 추가.

	/*
	manager->AddItem(IDC_MainScore, move_both_100, size_both_100);
	manager->AddItem(IDC_SubScore, move_both_100, size_both_100);
	manager->AddItem(IDC_SPIN1, move_both_100, size_x_100);
	manager->AddItem(IDC_PROCESS, move_both_100, size_both_100);
	manager->AddItem(IDC_VOLUME, move_both_100, size_both_100);
	manager->AddItem(IDC_STATIC, move_both_100, size_both_100);
	manager->AddItem(IDC_SETTING, move_both_100, size_both_100);
	manager->AddItem(IDC_PROCESS, move_both_100, size_both_100);
	manager->AddItem(IDC_PAUSE, move_both_100, size_both_100);
	manager->AddItem(IDC_PLAY, move_both_100, size_both_100);
	manager->AddItem(IDC_REC, move_both_100, size_both_100);
	*/

}

void CShowMusicScoreVer1Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CShowMusicScoreVer1Dlg::OnPaint()
{
	LogMessage("----------class: OnPaint----------");

	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	if (IsIconic())
	{
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//mtx.lock();
		if (!m_image.IsNull())
		{ // 그림이 로딩되었는지 체크한다.
			// 이미지가 원본 크기와 다르게 출력될때 어떤 방식으로 이미지를
			// 확대하거나 축소할 것인지를 결정한다.
			dc.SetStretchBltMode(COLORONCOLOR);
			// 그림을 Picture Control 크기로 화면에 출력한다.
			//mtx.lock();
			m_image.Draw(dc, m_image_rect);
			//mtx.unlock();
		}
		else
			LogMessage("!m_image.IsNull() is false");

		CDialogEx::OnPaint();
		//mtx.unlock();
	}

	LogMessage("----------class: OnPaint end----------");
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CShowMusicScoreVer1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CShowMusicScoreVer1Dlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = m_rcMinimumDialog.Width();
	lpMMI->ptMinTrackSize.y = m_rcMinimumDialog.Height();
}

void CShowMusicScoreVer1Dlg::OnStnClickedSubscore()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CShowMusicScoreVer1Dlg::OnStnClickedMainscore()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CShowMusicScoreVer1Dlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	wchar_t path[MaxPathLength];
	char log[10];

	LogMessage("----------class: OnDeltaposSpin1----------");
	sprintf_s(log, sizeof(log), "page: %d", page);
	LogMessage(log);

	LogMessage("r_path: ");
	LogMessage(r_path);

	if (pNMUpDown->iDelta < 0)
	{
		LogMessage("이전 페이지");
		page--;
		if (page < 0)
		{
			AfxMessageBox(_T("첫장 입니다"));
			LogMessage("첫장 입니다");
			page = 0;
		}
		else
		{
			SetDlgItemInt(IDC_EDIT1, page + 1);
			swprintf_s(path, MaxPathLength, L"%s\\%s", r_path, file_name[page]);
			LogMessage(path);
			//mtx.lock();
			m_image.Destroy();
			m_image.Load(path); // 파일을 읽어서 비트맵 객체를 구성한다.
			// Picture Control 위치의 화면을 갱신해서 그림을 보이게 한다.
			InvalidateRect(m_image_rect, FALSE);
			//mtx.unlock();
		}
	}
	else
	{
		LogMessage("다음 페이지");
		page++;
		if (page > max_page_num)
		{
			AfxMessageBox(_T("마지막 장입니다"));
			LogMessage("마지막 장입니다");
			page = max_page_num;
			SetDlgItemInt(IDC_EDIT1, page + 1);
		}
		else
		{
			SetDlgItemInt(IDC_EDIT1, page + 1);
			swprintf_s(path, MaxPathLength, L"%s\\%s", r_path, file_name[page]);
			LogMessage(path);
			//mtx.lock();
			m_image.Destroy();
			m_image.Load(path); // 파일을 읽어서 비트맵 객체를 구성한다.
			// Picture Control 위치의 화면을 갱신해서 그림을 보이게 한다.
			InvalidateRect(m_image_rect, FALSE);
			//mtx.unlock();
		}
	}

	*pResult = 0;

	LogMessage("----------class: OnDeltaposSpin1 end----------");
}

void CShowMusicScoreVer1Dlg::OnNMCustomdrawProcess(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CShowMusicScoreVer1Dlg::OnNMCustomdrawVolume(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CShowMusicScoreVer1Dlg::OnBnClickedSetting() //Select score
{
	LogMessage("----------class: OnBnClickedSetting----------");

	wchar_t path[MaxPathLength];
	CString name;
	wchar_t* name_wch;
	//char dir[150];
	char log[50];
	// 파일 선택하기
	CString strPathName;
	CString strTemp;
	CString str = _T("Image Files(*.jpg, *.png, *.bmp) |*.jpg; *.png; *.bmp|"); // 이미지 파일 표시
	// _T("Excel 파일 (*.xls, *.xlsx) |*.xls; *.xlsx|"); 와 같이 확장자를 제한하여 표시할 수 있음
	CFileDialog dlg(TRUE, _T("*.dat"), NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, str, this);

	if (dlg.DoModal() == IDOK)
	{
		strPathName = dlg.GetPathName();
		rawpath = T2W(strPathName.GetBuffer());

		
		//ConvertWCtoC(rawpath, dir);

		if (m_image.IsNull()) {  // 이미 그림이 읽혀졌는지 체크한다.
			m_image.Load(rawpath); // 파일을 읽어서 비트맵 객체를 구성한다.
			// Picture Control 위치의 화면을 갱신해서 그림을 보이게 한다.
			InvalidateRect(m_image_rect, FALSE);
		}
		name = PathFindFileName(rawpath);
		name_wch = T2W(name.GetBuffer());

		PathRemoveFileSpec(rawpath);

		r_path = new wchar_t[MaxPathLength];
		wcscpy_s(r_path, MaxPathLength, rawpath);
		LogMessage("r_path: ");
		LogMessage(r_path);

		swprintf_s(path, MaxPathLength, L"%s\\*.jpg", rawpath);
		max_page_num = FileName(path);
		LogMessage("raw path: ");
		LogMessage(rawpath);
		LogMessage("path: ");
		LogMessage(path);

		sprintf_s(log, sizeof(log), "파일 개수: %d", max_page_num);
		LogMessage(log);

		// 선택한 페이지가 1페이지가 아닐 수도 있음. 이때 page 값을 맞춰줘야 함.
		for (page = 0; page < max_page_num; page++)
		{
			if (wcscmp(name_wch, file_name[page]) == 0)
				break;
		}
		SetDlgItemInt(IDC_EDIT1, page + 1);
		sprintf_s(log, sizeof(log), "현재 page: %d", page);
		LogMessage(log);
		

		//AfxMessageBox(rawpath);
		// 파일 경로를 가져와 사용할 경우, Edit Control에 값 저장
		//SetDlgItemText(IDC_EDIT1, strPathName);
	}

	LogMessage("----------class: OnBnClickedSetting end----------");
}

void CShowMusicScoreVer1Dlg::OnBnClickedPause()
{
	LogMessage("----------class: OnBnClickedPause----------");
	play = FALSE;
	m_isWorkingThread = false;

	WaitForSingleObject(m_pThread->m_hThread, 5000);
	LogMessage("play = FALSE");
	LogMessage("----------class: OnBnClickedPause end----------");
}

void CShowMusicScoreVer1Dlg::OnBnClickedPlay()
{
	//wchar_t path[MaxPathLength];
	//CShowMusicScoreVer1Dlg obj;

	LogMessage("----------class: OnBnClickedPlay----------");
	LogMessage("play = TRUE");
	play = TRUE;

	m_isWorkingThread = true;

	m_pThread = AfxBeginThread(TurnPage, this);

	//std::thread turn_Page(&CShowMusicScoreVer1Dlg::TurnPage, &obj);
	//turn_Page.detach();

	
	
	LogMessage("----------class: OnBnClickedPlay end----------");
}

void CShowMusicScoreVer1Dlg::OnBnClickedRec()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

LRESULT CShowMusicScoreVer1Dlg::NextPage(WPARAM wParam, LPARAM lParam) // change page
{
	char log[50];
	wchar_t path[MaxPathLength];

	LogMessage("----------func: NextPage()----------");

	if (play == TRUE)
	{
		page++;
		sprintf_s(log, sizeof(log), "Next page - page: %d", page);
		LogMessage(log);

		if (page > max_page_num)
		{
			AfxMessageBox(_T("마지막 장입니다"));
			LogMessage("마지막 장입니다");
			page = max_page_num;
		}
		else
		{
			SetDlgItemInt(IDC_EDIT1, page + 1);
			swprintf_s(path, MaxPathLength, L"%s\\%s", r_path, file_name[page]);
			LogMessage(path);
			m_image.Destroy();
			m_image.Load(path); // 파일을 읽어서 비트맵 객체를 구성한다.
			// Picture Control 위치의 화면을 갱신해서 그림을 보이게 한다.
			InvalidateRect(m_image_rect, FALSE);
		}

		//SetDlgItemText(IDC_STATIC_COUNT, text);
	}
	else
		LogMessage("play == FALSE");

	LogMessage("----------func: NextPage() end----------");
	return 0;
}

void CShowMusicScoreVer1Dlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
