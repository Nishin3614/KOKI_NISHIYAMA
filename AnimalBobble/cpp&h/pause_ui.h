// ----------------------------------------
//
// ポーズ用PAUSE_UI処理の説明[pause_ui.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _PAUSE_UI_H_
#define _PAUSE_UI_H_	 // ファイル名を基準を決める

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
class CPause_ui : public CScene
{
public:
	/* 関数 */
	CPause_ui();
	~CPause_ui();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);		// 読み込み
	static void UnLoad(void);		// UnLoadする
	static CPause_ui * Create(void);	// 作成
protected:

private:
	/* 列挙型 */
	typedef enum
	{
		PAUSE_UI_BG = 0,
		PAUSE_UI_PAUSE,
		PAUSE_UI_GAME,
		PAUSE_UI_TITLE,
		PAUSE_UI_RESTART,
		PAUSE_UI_MAX
	} PAUSE_UI;
	/* 関数 */

	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex[PAUSE_UI_MAX];	// テクスチャー設定
	static D3DXVECTOR3 m_pos[PAUSE_UI_MAX];			// 位置情報
	static D3DXVECTOR2 m_size[PAUSE_UI_MAX];		// サイズ情報
	static int m_nSelect;							// 選択
	float	m_fMult;								// 倍数
	float	m_fAdd;									// 加算
	int		m_nCntCol;								// カラーカウント
	CMark * m_mark;									// マーク
	CScene_TWO *m_aScene_Two[PAUSE_UI_MAX];			// ポーズ用ui2D
};

#endif