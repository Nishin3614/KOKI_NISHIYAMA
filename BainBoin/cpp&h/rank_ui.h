// ----------------------------------------
//
// ランク用RANK_UI処理の説明[rank_ui.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _RANK_UI_H_
#define _RANK_UI_H_	 // ファイル名を基準を決める

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
#define MAX_RANK_UI (7)

// ----------------------------------------
//
// 前方宣言
//
// ----------------------------------------

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CRank_ui : public CScene
{
public:
	/* 関数 */
	CRank_ui();
	~CRank_ui();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);		// 読み込み
	static void UnLoad(void);		// UnLoadする
	static CRank_ui * Create(void);	// 作成
protected:

private:
	/* 列挙型 */

	/* 関数 */

	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex[MAX_RANK_UI];	// テクスチャー設定
	static D3DXVECTOR3 m_pos[MAX_RANK_UI];			// 位置情報
	static D3DXVECTOR2 m_size[MAX_RANK_UI];			// サイズ情報
	static int m_nSelect;							// 選択
	int	m_nCntCol;									// カラーカウント
	CScene_TWO *m_aScene_Two[MAX_RANK_UI];			// ランク用ui2D
};

#endif