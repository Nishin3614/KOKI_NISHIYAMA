// ----------------------------------------
//
// �W���������̐���[forceline.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _FORCELINE_H_
#define _FORCELINE_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "main.h"
#include "scene_two.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------

// ------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------
class CForceline : public CScene_TWO
{
public:
	/* �֐� */
	CForceline();
	~CForceline();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	// �g�p���
	static void Use(bool bUse);
	static HRESULT Load(void);
	static void UnLoad(void);
	static CForceline * Create(void);		// �쐬
protected:

private:
	/* �ϐ� */
	static LPDIRECT3DTEXTURE9 m_pTex;
	static bool	m_bUse;			
};

#endif