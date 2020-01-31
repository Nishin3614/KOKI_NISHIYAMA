//*************************************************************************************************************
//
// �X�e�[�^�X�Ǘ�[status.h]
// Author : Nishiyama koki
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "status.h"
#include "player.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define EDITWINDOWCLASS_NAME	"PlayerEditorWindow"
#define EDITWINDOW_NAME			"PlayerEditor"
#define EDITWINDOW_SIZEX		(300)
#define EDITWINDOW_SIZEY		(500)

#define EDITITEM_SIZEX			(125)
#define EDITITEM_SIZEY			(20)

#define EDITBUTTON_ID			(1000)
#define DISPLAY_MAX				(36)


//-------------------------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK EditWindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//-------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
//-------------------------------------------------------------------------------------------------------------
WNDCLASSEX CStatus::m_wcex;
HWND CStatus::m_hWnd;
RECT CStatus::m_rect;

//-------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
//-------------------------------------------------------------------------------------------------------------
CStatus::CStatus()
{
}

//-------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^
//-------------------------------------------------------------------------------------------------------------
CStatus::~CStatus()
{
}

//-------------------------------------------------------------------------------------------------------------
// �E�B���h�E�̓o�^
//-------------------------------------------------------------------------------------------------------------
void CStatus::EditWindowRegister(HINSTANCE hInstance)
{
	m_wcex =
	{
		sizeof(WNDCLASSEX),				// WNDCLASSEX�̃������T�C�Y�̎w��
		CS_CLASSDC,						// �\������E�B���h�E�̃X�^�C����ݒ�
		EditWindowProcedure,			// �E�B���h�E�v���V�[�W���̃A�h���X�i�֐����j���w��
		0,								// �ʏ�͎g�p�͎g�p���Ȃ��̂�0�w��
		0,								// �ʏ�͎g�p�͎g�p���Ȃ��̂�0�w��
		hInstance,						// Windows�̈����̉@�X�^���h�n���h���w��
		LoadIcon(NULL,IDI_APPLICATION),	// �^�X�N�o�[�Ɏg�p����A�C�R���w��
		LoadCursor(NULL,IDC_ARROW),		// �g�p����}�E�X�J�[�\���w��
		(HBRUSH)BLACK_PEN,				// �E�B���h�E�N���C�A���g�̈�̐F�w��
		NULL,							// ���j���[���w��
		EDITWINDOWCLASS_NAME,					// �E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)	// �������A�C�R���w��
	};

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&m_wcex);
}

//-------------------------------------------------------------------------------------------------------------
// ����
//-------------------------------------------------------------------------------------------------------------
CStatus * CStatus::Create(void)
{
	return nullptr;
}

//-------------------------------------------------------------------------------------------------------------
// �E�B���h�E�̍쐬
//-------------------------------------------------------------------------------------------------------------
void CStatus::CreateEditWindow(HINSTANCE hInstance, int CmdShow)
{
	m_rect = { 0,0,EDITWINDOW_SIZEX,EDITWINDOW_SIZEY };
	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&m_rect, WS_OVERLAPPEDWINDOW, true);

	// �E�B���h�E�̐���
	m_hWnd = CreateWindowEx(0,			// �g���E�B���h�E�X�^�C��
		EDITWINDOWCLASS_NAME,					// �E�B���h�E�N���X��
		EDITWINDOW_NAME,				// �E�B���h�E�̖��O
		WS_EX_DLGMODALFRAME | WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME,		// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,								// �E�B���h�E����X���W
		CW_USEDEFAULT,								// �E�B���h�E����Y���W
		(m_rect.right - m_rect.left),					// �E�B���h�E�̕�
		(m_rect.bottom - m_rect.top),					// �E�B���h�E�̍���
		NULL,							// �e�E�B���h�E�̃n���h��
		NULL,							// ���j���[�������͎q�E�B���h�E�̃n���h��
		hInstance,						// �C���X�^���X�n���h��
		NULL);							// �E�B���h�E�쐬�f�[�^

										// �E�B���h�E�̕\��
	ShowWindow(m_hWnd, CmdShow);		// �w�肳�ꂽ�E�B���h�E�̕\���ݒ�
	UpdateWindow(m_hWnd);				// �E�B���h�E�̃N���C�A���g�̍X�V
}

//-------------------------------------------------------------------------------------------------------------
// �E�B���h�E�̓o�^����
//-------------------------------------------------------------------------------------------------------------
void CStatus::EditWindowUnregister(void)
{
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(EDITWINDOWCLASS_NAME, m_wcex.hInstance);

}

//-------------------------------------------------------------------------------------------------------------
// �X�e�[�^�X�̓ǂݍ���
//-------------------------------------------------------------------------------------------------------------
void CStatus::StatusLoad(STATUS_INFO *Status)
{
	// �ϐ��錾
	FILE *pFile;		// �t�@�C���̃|�C���^
	int nCntError = 0;	// �G���[�\���J�E���g

	// �J�������ǂ���
	if ((pFile = fopen("data/status_manager.txt", "r")) != NULL)
	{
		// �f�[�^�̎擾
		while (fscanf(
			pFile,
			"%[^,],%d,%d,%f,%f,%f",
			&Status->cName,		// ���O
			&Status->nID,		// �ԍ�
			&Status->nType,		// �^�C�v
			&Status->fHp,		// HP
			&Status->fSpeed,	// �X�s�[�h
			&Status->fAttack	// �U����
		) != EOF)
		{

		}
		fclose(pFile);

	}
	else
	{
		MessageBox(NULL, "�X�e�[�^�X���t�@�C�����J���܂���ł���", "�x���I", MB_ICONWARNING);
	}
}

//-------------------------------------------------------------------------------------------------------------
// �X�e�[�^�X�o��
//-------------------------------------------------------------------------------------------------------------
void CStatus::StatusSave(void)
{
	/*
	// �ϐ��錾
	FILE *pFile;		// �t�@�C���̃|�C���^
	char cRead[128];	// �ǂݍ��ݗp
	char cComp[128];	// ��r�p
	char cEmpty[128];	// �v��Ȃ����̗p
	int nCntError = 0;	// �G���[�\���J�E���g

						// �J�������ǂ���
	if ((pFile = fopen("data/status_manager.txt", "r")) != NULL)
	{
		// �f�[�^�̎擾
		while (fscanf(pFile, "%[^,],%d,%d,%.3f,%.3f,%.3f",
			Status->cName,		// ���O
			Status->nID,		// �ԍ�
			Status->nType,		// �^�C�v
			Status->fHp,		// HP
			Status->fSpeed,		// �X�s�[�h
			Status->fAttack		// �U����
		) != EOF)
		{

		}
	}
	else
	{
		MessageBox(NULL, "�v���C���[���t�@�C�����J���܂���ł���", "�x���I", MB_ICONWARNING);
	}
	fclose(pFile);
	*/
}

//-------------------------------------------------------------------------------------------------------------
// ���͖����G�f�B�^�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
HWND CStatus::SetWindowEditDisabled(HWND * hWndthis, HWND hWnd, int nID, INTEGER2 pos, char * name)
{
	*hWndthis = CreateWindowEx(0,
		"EDIT",
		name,
		(WS_CHILD | WS_VISIBLE | WS_DISABLED | ES_CENTER | WS_BORDER),
		pos.nMax,
		pos.nMin,
		EDITITEM_SIZEX,
		EDITITEM_SIZEY,
		hWnd,
		(HMENU)nID,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	return *hWndthis;
}

//-------------------------------------------------------------------------------------------------------------
// ���͗L���G�f�B�^�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
HWND CStatus::SetWindowEdit(
	HWND * hWndthis,
	HWND hWnd, 
	int nID, 
	INTEGER2 pos, 
	int * nValue, 
	float * fValue,
	char * cValue
)
{
	// �ϐ��錾
	char string[DISPLAY_MAX] = { "" };	// ������i�[
	if (nValue != nullptr)
	{
		snprintf(string, DISPLAY_MAX, "%d", *nValue);
	}
	else if (fValue != nullptr)
	{
		snprintf(string, DISPLAY_MAX, "%.3f", *fValue);
	}
	else if (cValue != nullptr)
	{
		strcpy(string, cValue);
	}
	*hWndthis = CreateWindowEx(0,
		"EDIT",
		string,
		(WS_CHILD | WS_VISIBLE | ES_RIGHT | WS_BORDER),
		pos.nMax,
		pos.nMin,
		EDITITEM_SIZEX,
		EDITITEM_SIZEY,
		hWnd,
		(HMENU)nID,
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
		NULL);
	return *hWndthis;
}

//-------------------------------------------------------------------------------------------------------------
// �E�B���h�E�v���V�[�W��
//-------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK EditWindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// �ϐ��錾
	static HWND hWndJumpName;			// �W�����v��
	static HWND hWndJump;				// �W�����v��
	static char aJumpData[64];			// �W�����v��

	static HWND hWndJumpRateName;		// �W�����v�͏㏸�l
	static HWND hWndJumpRate;			// �W�����v�͏㏸�l
	static char aJumpRateData[64];		// �W�����v�͏㏸�l

	static HWND hWndRotName;			// ��]��
	static HWND hWndRot;				// ��]��
	static char aRotData[64];			// ��]��

	static HWND hWndRotRateName;		// ��]�͏㏸�l
	static HWND hWndRotRate;			// ��]�͏㏸�l
	static char aRotRateData[64];		// ��]�͏㏸�l

	static HWND hWndOutputButton;		// �o�̓{�^��

	if (uMsg == WM_CREATE)
	{
		STATUS_INFO Status;
		// �X�e�[�^�X�̓ǂݍ���
		CStatus::StatusLoad(&Status);

		/* �o�̓{�^�� */
		hWndOutputButton = CreateWindowEx(0,
			"BUTTON",
			" ",
			(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON),
			10, 10 + (EDITITEM_SIZEY*5),
			70, 30,								// X�AY�A���A����
			hWnd,											// �e�E�B���h�E
			(HMENU)EDITBUTTON_ID,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE)	// �C���X�^���X�n���h���i�A�v���̎��ʎq�j
			, NULL);

		// �o�̓{�^���ɕ\�����镶����
		SetWindowText(hWndOutputButton, "�o��");

		// ���O�̐ݒ�
		CStatus::SetWindowEditDisabled(
			&hWndJumpName,					// ����̘g
			hWnd, 							// ���ׂĂ̘g
			100,							// �ԍ�
			INTEGER2(10, 10),				// �ʒu
			"���O");						// ���O
		CStatus::SetWindowEdit(&hWndJump, hWnd, 101,
			INTEGER2(10 + EDITITEM_SIZEX, 10), nullptr, nullptr,&Status.cName);

			// ID�㏸�l�̐ݒ�
		CStatus::SetWindowEditDisabled(&hWndJumpRateName, hWnd, 102,
			INTEGER2(10, 10+ EDITITEM_SIZEY), "ID");
		CStatus::SetWindowEdit(&hWndJumpRate, hWnd, 103,
			INTEGER2(10 + EDITITEM_SIZEX, 10 + EDITITEM_SIZEY), &Status.nID, nullptr,nullptr);


		// �^�C�v�̐ݒ�
		CStatus::SetWindowEditDisabled(&hWndJumpName, hWnd, 104,
			INTEGER2(10, 10 + EDITITEM_SIZEY * 2), "�^�C�v");
		CStatus::SetWindowEdit(&hWndRot, hWnd, 105,
			INTEGER2(10 + EDITITEM_SIZEX, 10 + EDITITEM_SIZEY*2), &Status.nType, nullptr);

		// HP�̐ݒ�
		CStatus::SetWindowEditDisabled(&hWndJumpRateName, hWnd, 106,
			INTEGER2(10, 10 + EDITITEM_SIZEY*3), "HP");
		CStatus::SetWindowEdit(&hWndRotRate, hWnd, 107,
			INTEGER2(10 + EDITITEM_SIZEX, 10 + EDITITEM_SIZEY*3), NULL, &Status.fHp,nullptr);
	}

	switch (uMsg)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == EDITBUTTON_ID)
		{
			// ���b�Z�[�W�{�b�N�X
			int nID = MessageBox(hWnd, "�o�͂��܂����H", "�o��", MB_YESNO);
			// ����YES��������
			if (nID == IDYES)
			{
				CStatus::StatusSave();
			}
		}
		break;
	}

	GetWindowText(hWndJump, &aJumpData[0], 256);
	GetWindowText(hWndJumpRate, &aJumpRateData[0], 256);
	GetWindowText(hWndRot, &aRotData[0], 256);
	GetWindowText(hWndRotRate, &aRotRateData[0], 256);

	/*
	CPlayer *pPlayer = NULL;

	for (int nCntlayer = 0; nCntlayer < CScene::GetMaxLayer(CScene::LAYER_3DOBJECT); nCntlayer++)
	{
		if (pPlayer = (CPlayer *)CScene::GetActor(CScene::ACTOR_PLAYER, CScene::LAYER_3DOBJECT, nCntlayer)) break;
	}

	if (pPlayer != NULL)
	{// �ϐ���`
		STATUS_INFO StatusInfo;										// �X�e�[�^�X�̏��

		StatusInfo.fJumpforce		= (float)atof(aJumpData);		// �W�����v��
		StatusInfo.fRateJumpforce	= (float)atof(aJumpRateData);	// �W�����v�͂̏㏸��
		StatusInfo.fRateRotforce	= (float)atof(aRotRateData);	// ��]��
		StatusInfo.fRotforce		= (float)atof(aRotData);		// ��]�͂̏㏸��

		// �X�e�[�^�X�̐ݒ�
		pPlayer->SetStatus(StatusInfo);
	}
	*/
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// ����̏������
}
