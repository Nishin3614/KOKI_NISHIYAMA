//*************************************************************************************************************
//
// �I�u�W�F�N�g�E�B���h�E�Ǘ�[objwindow.h]
// Author : Koki Nishiyama
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "objwindow.h"

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


//-------------------------------------------------------------------------------------------------------------
// �\����
//-------------------------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 *pos = NULL;
	D3DXVECTOR3 *size = NULL;
	int	*nBlock_H = NULL;
	int *nBlock_W = NULL;
} OBJWINDOWINFO;

//-------------------------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
//-------------------------------------------------------------------------------------------------------------
LRESULT CALLBACK EditWindowProcedure(
	HWND hWnd,
	UINT uMsg, 
	WPARAM wParam,
	LPARAM lParam
);
HWND SetWindowEditDisabled(
	HWND *hWndthis, 
	HWND hWnd, 
	int nID,
	INTEGER2 pos,
	char* name
);
HWND SetWindowEdit(
	HWND *hWndthis, 
	HWND hWnd, 
	int nID,
	INTEGER2 pos, 
	int *nValue = NULL,
	float *fValue = NULL
);

//-------------------------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
//-------------------------------------------------------------------------------------------------------------
WNDCLASSEX	g_objwindWcex;
HWND		g_objwindhWnd;
RECT		g_objwindrect;
OBJWINDOWINFO	g_objwind;

//-------------------------------------------------------------------------------------------------------------
// �ڍאݒ�
//-------------------------------------------------------------------------------------------------------------
void SetObjWindow(
	D3DXVECTOR3 *pos,	// �ʒu
	D3DXVECTOR3 *size,	// �T�C�Y
	int *nBlock_H,		// �c�u���b�N
	int *nBlock_W		// ���u���b�N
)
{
	g_objwind.pos = pos;
	g_objwind.size = size;
	g_objwind.nBlock_H = nBlock_H;
	g_objwind.nBlock_W = nBlock_W;
}

//-------------------------------------------------------------------------------------------------------------
// �E�B���h�E�̓o�^
//-------------------------------------------------------------------------------------------------------------
void EditWindowRegister(HINSTANCE hInstance)
{
	g_objwindWcex =
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
		EDITWINDOWCLASS_NAME,			// �E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)	// �������A�C�R���w��
	};

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&g_objwindWcex);
}

//-------------------------------------------------------------------------------------------------------------
// �E�B���h�E�̍쐬
//-------------------------------------------------------------------------------------------------------------
void CreateEditWindow(HINSTANCE hInstance, int CmdShow)
{
	g_objwindrect = { 0,0,EDITWINDOW_SIZEX,EDITWINDOW_SIZEY };
	// �w�肵���N���C�A���g�̈���m�ۂ��邽�߂ɕK�v�ȃE�B���h�E���W���v�Z
	AdjustWindowRect(&g_objwindrect, WS_OVERLAPPEDWINDOW, true);

	// �E�B���h�E�̐���
	g_objwindhWnd = CreateWindowEx(0,			// �g���E�B���h�E�X�^�C��
		EDITWINDOWCLASS_NAME,					// �E�B���h�E�N���X��
		EDITWINDOW_NAME,				// �E�B���h�E�̖��O
		WS_EX_DLGMODALFRAME | WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME,		// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,								// �E�B���h�E����X���W
		CW_USEDEFAULT,								// �E�B���h�E����Y���W
		(g_objwindrect.right - g_objwindrect.left),					// �E�B���h�E�̕�
		(g_objwindrect.bottom - g_objwindrect.top),					// �E�B���h�E�̍���
		NULL,							// �e�E�B���h�E�̃n���h��
		NULL,							// ���j���[�������͎q�E�B���h�E�̃n���h��
		hInstance,						// �C���X�^���X�n���h��
		NULL);							// �E�B���h�E�쐬�f�[�^

										// �E�B���h�E�̕\��
	ShowWindow(g_objwindhWnd, CmdShow);		// �w�肳�ꂽ�E�B���h�E�̕\���ݒ�
	UpdateWindow(g_objwindhWnd);				// �E�B���h�E�̃N���C�A���g�̍X�V
}

//-------------------------------------------------------------------------------------------------------------
// �E�B���h�E�̓o�^����
//-------------------------------------------------------------------------------------------------------------
void EditWindowUnregister(void)
{
	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(EDITWINDOWCLASS_NAME, g_objwindWcex.hInstance);
}

//-------------------------------------------------------------------------------------------------------------
// ���͖����G�f�B�^�̐ݒ�
//-------------------------------------------------------------------------------------------------------------
HWND SetWindowEditDisabled(
	HWND * hWndthis,
	HWND hWnd, 
	int nID, 
	INTEGER2 pos,
	char * name
)
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
HWND SetWindowEdit(
	HWND * hWndthis, 
	HWND hWnd, 
	int nID, 
	INTEGER2 pos,
	int * nValue, 
	float * fValue
)
{
	char string[24] = { "" };
	if (nValue != NULL)
	{
		snprintf(string, 24, "%d", *nValue);
	}
	else if (fValue != NULL)
	{
		snprintf(string, 24, "%.3f", *fValue);
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
LRESULT CALLBACK EditWindowProcedure(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
)
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

		/* �o�̓{�^�� */
		hWndOutputButton = CreateWindowEx(0,
			"BUTTON",
			" ",
			(WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON),
			10, 10 + (EDITITEM_SIZEY * 5),
			70, 30,								// X�AY�A���A����
			hWnd,											// �e�E�B���h�E
			(HMENU)EDITBUTTON_ID,
			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE)	// �C���X�^���X�n���h���i�A�v���̎��ʎq�j
			, NULL);
		// �W�����v�͂̐ݒ�
		SetWindowEditDisabled(&hWndJumpName, hWnd, 100,
			INTEGER2(10, 10), "�ʒu");
		SetWindowEdit(&hWndJump, hWnd, 101,
			INTEGER2(10 + EDITITEM_SIZEX, 10), NULL, NULL);

		SetWindowEdit(&hWndJumpRate, hWnd, 103,
			INTEGER2(10 + EDITITEM_SIZEX, 10 + EDITITEM_SIZEY), NULL, NULL);

		SetWindowEdit(&hWndRot, hWnd, 105,
			INTEGER2(10 + EDITITEM_SIZEX, 10 + EDITITEM_SIZEY * 2), NULL, NULL);

		// �o�̓{�^���ɕ\�����镶����
		SetWindowText(hWndOutputButton, "�o��");
	}
	GetWindowText(hWndJump, &aJumpData[0], 256);
	GetWindowText(hWndJumpRate, &aJumpRateData[0], 256);
	GetWindowText(hWndRot, &aRotData[0], 256);
	GetWindowText(hWndRotRate, &aRotRateData[0], 256);


	if (g_objwind.pos != NULL)
	{
		g_objwind.pos->x = (float)atof(aJumpData);		// �W�����v��
		g_objwind.pos->y = (float)atof(aJumpRateData);	// �W�����v�͂̏㏸��
		g_objwind.pos->z = (float)atof(aRotRateData);	// ��]��
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// ����̏������
}
