// ------------------------------------------------------------------------------------------
//
// �I����ʃw�b�_�[���� [selection.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _SELECTION_H_
#define _SELECTION_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
//
// �}�N����`
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �O���錾
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------
class CSelection
{
public:
	/* �֐� */
	CSelection();
	~CSelection();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
protected:
private:
};
#endif