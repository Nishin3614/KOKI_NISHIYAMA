//*************************************************************************************************************
//
// �G�f�B�b�g�E�B���h�E����[Editwindow.h]
// Author : Nishiyama koki
//
//*************************************************************************************************************
#ifndef _STATUS_H_
#define _STATUS_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "main.h"
#include "Calculation.h"

//-------------------------------------------------------------------------------------------------------------
// �\����
//-------------------------------------------------------------------------------------------------------------
typedef struct
{
	char cName;
	int nID;
	int nType;
	float fHp;
	float fSpeed;
	float fAttack;
} STATUS_INFO;

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class CStatus
{
public:
	CStatus();
	~CStatus();
	// ����
	static CStatus *Create(void);								// �X�e�[�^�X
	void CreateEditWindow(HINSTANCE hInstance, int CmdShow);	// �E�B���h�E�̐���
	void EditWindowRegister(HINSTANCE hInstance);				// �E�B���h�E�N���X�̓o�^
	void EditWindowUnregister(void);							// �E�B���h�E�N���X�̔j��
	// ���[�h�E�Z�[�u
	static void StatusLoad(STATUS_INFO *Status);				// �X�e�[�^�X�̓ǂݍ���
	static void StatusSave(void);								// �X�e�[�^�X�Z�[�u
	// �ݒ�
	static HWND SetWindowEditDisabled(							// ���O�̘g
		HWND *hWndthis,											// ����̘g
		HWND hWnd, 												// ���ׂĂ̘g
		int nID, 												// �ԍ�
		INTEGER2 pos, 											// �ʒu
		char* name);											// ���O
	static HWND SetWindowEdit(									// ���l���͗p�̘g
		HWND *hWndthis,											// ����̘g
		HWND hWnd,												// ���ׂĂ̘g
		int nID,												// �ԍ�
		INTEGER2 pos,											// �ʒu
		int *nValue = NULL,										// �����^
		float *fValue = NULL,									// ���������_�^
		char *cValue = NULL										// ������^
	);

private:
	/* �ϐ� */
	static WNDCLASSEX	m_wcex;	// �E�B���h�E�N���X
	static HWND			m_hWnd;	// �n���h���E�B���h�E
	static RECT			m_rect;	// ����

	STATUS_INFO			m_Status;	// �X�e�[�^�X���
};















#endif