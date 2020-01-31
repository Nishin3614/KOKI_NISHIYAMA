#//*************************************************************************************************************
//
// ゲームUI処理[GameUI.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _GAMEUI_H_
#define _GAMEUI_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "Scene.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------
#define MAX_GAMEUI			(2)

//-------------------------------------------------------------------------------------------------------------
// クラス定義
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
		LPDIRECT3DTEXTURE9 pTexture;	// テクスチャ情報
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
	/* メンバ関数 */
	static int LoadNumber(const char *TextFile, const char  * SetItem);
	static void LoadTextureFile(const char * sFileName, PFILETAG TexName, const int &nNumMax);
	static bool Existenceofscript(FILE *pFile);

	void LoadUI(void);

	/* メンバ変数 */
	static TEXTURE_INFO			*m_pTextureinfo;		// テクスチャの情報
	static int					m_nNumTextureMax;		// テクスチャ最大数
	CScene_TWO					*m_pCScene2D;			// シーン2Dの情報
	int							m_nNumUI;				// UI数

	bool bUes;
};

#endif