#//*************************************************************************************************************
//
// �Q�[��UI����[GameUI.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _GAMEUI_H_
#define _GAMEUI_H_

//-------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"

//-------------------------------------------------------------------------------------------------------------
// �}�N����`
//-------------------------------------------------------------------------------------------------------------
#define MAX_GAMEUI			(2)

//-------------------------------------------------------------------------------------------------------------
// �N���X��`
//-------------------------------------------------------------------------------------------------------------
class CScene_TWO;
class CGameUI :public CScene
{
public:

	typedef struct
	{
		char string[64];
	} FILETAG, *PFILETAG;

	typedef struct
	{
		LPDIRECT3DTEXTURE9 pTexture;	// �e�N�X�`�����
	} TEXTURE_INFO;

	CGameUI();
	~CGameUI();
	static HRESULT Load(void);
	static void Unload(void);
	static CGameUI *Create(void);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
private:
	/* �����o�֐� */
	static int LoadNumber(const char *TextFile, const char  * SetItem);
	static void LoadTextureFile(const char * sFileName, PFILETAG TexName, const int &nNumMax);
	static bool Existenceofscript(FILE *pFile);

	void LoadUI(void);

	/* �����o�ϐ� */
	static TEXTURE_INFO			*m_pTextureinfo;		// �e�N�X�`���̏��
	static int					m_nNumTextureMax;		// �e�N�X�`���ő吔
	CScene_TWO					*m_pCScene2D;			// �V�[��2D�̏��
	int							m_nNumUI;				// UI��

	bool bUes;
};

#endif