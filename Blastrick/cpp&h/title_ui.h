// ----------------------------------------
//
// タイトルUI処理の説明[title_ui.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _TITLE_UI_H_
#define _TITLE_UI_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene_two.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

// ----------------------------------------
//
// 前方宣言
//
// ----------------------------------------
class CMark;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CTitle_ui : public CScene
{
public:
	/* 関数 */
	CTitle_ui();
	~CTitle_ui();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	static HRESULT Load(void);		// 読み込み
	static void UnLoad(void);		// UnLoadする
	static CTitle_ui * Create(void);	// 作成
protected:

private:
	/* 列挙型 */
	typedef enum
	{
		/* タイトルプレイ中には出現しないUI */
		TITLE_UI_NAME = 0,
		TITLE_UI_CLICK,
		TITLE_UI_MAX
	} TITLE_UI;
	/* 関数 */
	void Various(int nCnt);											// 種類別処理
																	
	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex[TITLE_UI_MAX];	// テクスチャー設定
	static D3DXVECTOR3 m_pos[TITLE_UI_MAX];			// 位置情報
	static D3DXVECTOR2 m_size[TITLE_UI_MAX];			// サイズ情報
	float	m_fMult;												// 倍数
	float	m_fAdd;													// 加算
	int		m_nCntCol;												// カラーカウント
	CScene_TWO *m_aScene_Two[TITLE_UI_MAX];			// ポーズ用ui2D
};

#endif