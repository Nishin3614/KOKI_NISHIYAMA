// ------------------------------------------
//
// �|�[�Y�w�b�_�[���� [pause.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------
#ifndef _PAUSE_H_
#define _PAUSE_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------
#include "main.h"

// ------------------------------------------
//
// �}�N����`
//
// ------------------------------------------

// ------------------------------------------
//
// �O���錾
//
// ------------------------------------------

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CPause
{
public:
	/* �֐� */
	CPause();
	~CPause();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	static void SetPause(bool bPause);	// �|�[�Y�̏�Ԑݒ�
	static bool GetPause(void);			// �|�[�Y�̏�Ԏ擾
protected:
private:
	static bool m_bPause;
};
#endif
