// ----------------------------------------------------------------------------------------------------
//
// main�����̐���[main.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------------------------------------------------------------------
#include "main.h"
#include "manager.h"
#include "status.h"
#include <iomanip>
#include <cstdlib>
#include <crtdbg.h>
#include <iomanip>
//#include <libucrt.lib>

// ----------------------------------------------------------------------------------------------------
//
// �}�N���֐���`
//
// ----------------------------------------------------------------------------------------------------
#define CLASS_NAME "WindowClass"	// �E�B���h�E�N���X��
#define WINDOW_NAME "AnimalBobble"	// �E�B���h�E��(�L�����Z���\��)
#define ID_BUTTON000 (101)
#define SCREEN_WINDOWS (true)		// �X�N���[���̃E�B���h�E�T�C�Y

// ----------------------------------------------------------------------------------------------------
//
// �v���g�^�C�v�錾
//
// ----------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------
//
// �O���[�o���ϐ�
//
// ----------------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc
(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam); // �E�B���h�E�v���V�[�W���֐�

#ifdef _DEBUG
int			g_nCountFPS = 0;	// FPS�J�E���^
#endif

// ----------------------------------------------------------------------------------------------------
//
// ���C���֐�
//
// ----------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
#if ERROW_ACTION
	// ���������[�N�����ݒ�
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _DEBUG
	// �V�[�h�l�����Ԃɂ���
	srand((unsigned)time(NULL));

	// �ϐ��錾
	HWND hWnd;				// �E�B���h�E�n���h��
	MSG msg;				// ���b�Z�[�W���i�[����ϐ�

	DWORD dwCurrentTime;		// ���ݎ���
	DWORD dwExecLastTime;		// �Ō�ɏ�����������	
	DWORD dwFrameCount;
	DWORD dwFPSLastTime;
	CManager *pManager = NULL;			// �����_�����O�N���X

	// Window�N���X�̒ǉ��ݒ�
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEX�̃������T�C�Y�w��
		CS_CLASSDC,						// �\������E�B���h�E�̃X�^�C���ݒ�
		WindowProc,						// �E�B���h�E�v���V�[�W���̃A�h���X(�֐���)���w��
		0,								// �ʏ�͎w�肵�Ȃ��̂�0�w��
		0,								// �ʏ�͎w�肵�Ȃ��̂�0�w��
		hInstance,						// Windows�̈����̃C���X�^���X�n���h���w��
		LoadIcon(NULL,IDI_APPLICATION),	// �^�X�N�o�[�Ɏg�p����A�C�R���w��					
		LoadCursor(NULL,IDC_ARROW),		// �g�p����}�E�X�J�[�\���w��						
		(HBRUSH)(COLOR_WINDOW - 1),		// �E�B���h�E�N���C�A���g�̈�̐F�ݒ�
		NULL,							// ���j���[�w��
		CLASS_NAME,						// �E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)	// �������A�C�R���w��
	};


	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �E�B���h�E�N���X����
	hWnd = CreateWindowEx    // Ex�͊g��
	(
		0,					 // �g���E�B���h�E�X�^�C��
		CLASS_NAME,			 // �E�B���h�E�N���X��
		WINDOW_NAME,		 // �E�B���h�E��
		WS_OVERLAPPEDWINDOW, // �E�B���h�E�X�^�C��
		CW_USEDEFAULT,		 // �E�B���h�E����x���W				
		CW_USEDEFAULT,		 // �E�B���h�E����y���W				
		SCREEN_WIDTH,		 // �E�B���h�E��					
		SCREEN_HEIGHT,		 // �E�B���h�E����					
		NULL,				 // �e�E�B���h�E�n���h��
		NULL,				 // ���j���[�������͎q�E�B���h�EID
		hInstance,			 // �C���X�^���X�n���h��
		NULL				 // �E�B���h�E�쐬�f�[�^
	);

#if ERROW_ACTION
	// �R���\�[���Ăяo��
	AllocConsole();
	FILE *fp = NULL;
	freopen_s(&fp, "CONOUT$", "w", stdout);
	std::cout << "";
#endif 


	// �}�l�[�W���̃k���`�F�b�N
	if (pManager == NULL)
	{
		// �������̐���
		pManager = new CManager();	// �}�l�[�W���̐���
	}

	// ����������(�E�B���h�E�̐�����ɍs��)
	if (FAILED(pManager->Init(hWnd, SCREEN_WINDOWS,hInstance)))
	{
		return -1;	// ���s�����甲����
	}

#if ERROW_ACTION
#ifdef _DEBUG
	/*
	//------------------------------------
	// �T�u�E�B���h�E
	//------------------------------------
	// �N���X�̐���
	CStatus *pEditwindow = CStatus::Create();
	// �T�u�E�B���h�E�N���X�̓o�^
	pEditwindow->EditWindowRegister(hInstance);
	// �T�u�E�B���h�E�N���X�̐���
	pEditwindow->CreateEditWindow(hInstance, nCmdShow);
	*/
#endif
#endif
	// �E�B���h�E���� //
	ShowWindow(hWnd, nCmdShow); // �w�肳�ꂽ�E�B���h�E�̕\���ݒ�
	UpdateWindow(hWnd);			// �E�B���h�E�̃N���C�A���g�̈�X�V


	// ����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();		// �V�X�e���������~���b�P�ʂŎ擾

	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();		// �V�X�e���������~���b�P�ʂŎ擾

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}

			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);	// �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�ő���
			}
		}

		else
		{
			// DirectX�̏���
			dwCurrentTime = timeGetTime();		// �V�X�e���������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)	// 0.5�b���ƂɎ��s
			{
#ifdef _DEBUG
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);		// FPS�𑪒�
#endif

				dwFPSLastTime = dwCurrentTime;		// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;					// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))		// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

				// �X�V����
				pManager->Update();
				// �`�揈��
				pManager->Draw();

				dwFrameCount++;					// �J�E���g�����Z
			}
		}
	}

	// �I������
	pManager->Uninit();

	/* ��������j�� */
	// �}�l�[�W���̃k���`�F�b�N
	if (pManager != NULL)
	{
		// �}�l�[�W��
		delete pManager;
		pManager = NULL;
	}

#if ERROW_ACTION
#ifdef _DEBUG
	/*
	// �T�u�E�B���h�E�N���X�̔j��
	pEditwindow->EditWindowUnregister();
	*/
#endif
#endif
	//�E�B���h�E�N���X�̓o�^����� //
	UnregisterClass(CLASS_NAME, wcex.hInstance);

#if ERROW_ACTION
	// ���������[�N���|�[�g
	_CrtDumpMemoryLeaks();
#endif
	return (int)msg.wParam;
}

// ----------------------------------------------------------------------------------------------------
//
// �E�B���h�E�v���V�[�W���֐�
//
// ----------------------------------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	// ���N���b�N�� //
	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		break;		
	// �L�[�������ꂽ�Ƃ��̃��b�Z�[�W //
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			// �E�B���h�E��j��(WM_DESTROY����)
			DestroyWindow(hWnd);
		}
		break;
	// �~�I�� //
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	//�E�B���h�E�j�����b�Z�[�W //
	case WM_DESTROY:
		// WN_QUIT���b�Z�[�W��Ԃ��B�E�B���h�E���I��点��
		PostQuitMessage(0);
		break;
	// ���̑� //
	default:
		break;
	}

	// ����̏������ //
	return DefWindowProc(hWnd, uMsg, wParam, lParam); // �I��点��ȊO��msg
}

#if _DEBUG
int GetFPS(void)
{
	return g_nCountFPS;
}

void DispConsol()
{
	AllocConsole();

	FILE *fp = NULL;

	freopen_s(&fp, "CONOUT$", "w", stdout);

	/*
	cout << fixed;
	cout << setprecision(4);
	*/
}
#endif // _DEBUG
