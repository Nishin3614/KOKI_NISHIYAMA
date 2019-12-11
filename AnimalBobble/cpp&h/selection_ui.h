// ----------------------------------------
//
// 選択UI処理の説明[selection_ui.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _SELECTION_UI_H_
#define _SELECTION_UI_H_	 // ファイル名を基準を決める

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

// ------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------
class CSelection_ui : public CScene
{
public:
	/* 関数 */
	CSelection_ui();
	~CSelection_ui();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);		// 読み込み
	static void UnLoad(void);		// UnLoadする
	static CSelection_ui * Create(void);	// 作成
protected:

private:
	/* 列挙型 */
	typedef enum
	{
		/* 選択プレイ中には出現しないUI */
		SELECTION_UI_BG = 0,
		SELECTION_UI_SINGLE,
		SELECTION_UI_DISCRIPTION,
		SELECTION_UI_MAX
	} SELECTION_UI;
	/* 関数 */

	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex[SELECTION_UI_MAX];	// テクスチャー設定
	static D3DXVECTOR3 m_pos[SELECTION_UI_MAX];			// 位置情報
	static D3DXVECTOR2 m_size[SELECTION_UI_MAX];			// サイズ情報
	float	m_fMult;												// 倍数
	float	m_fAdd;													// 加算
	int		m_nCntCol;												// カラーカウント
	CScene_TWO *m_aScene_Two[SELECTION_UI_MAX];			// ポーズ用ui2D
	CMark *m_mark;											// 目印
	int m_nSelect;									// 選択
};

#endif