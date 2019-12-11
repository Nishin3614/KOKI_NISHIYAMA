// ----------------------------------------
//
// チュートリアルUI処理の説明[tutorial_ui.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _TUTORIAL_UI_H_
#define _TUTORIAL_UI_H_	 // ファイル名を基準を決める

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

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CTutorial_ui : public CScene
{
public:
	/* 関数 */
	CTutorial_ui();
	~CTutorial_ui();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);		// 読み込み
	static void UnLoad(void);		// UnLoadする
	static CTutorial_ui * Create(void);	// 作成
protected:

private:
	/* 列挙型 */
	typedef enum
	{
		/* タイトルプレイ中には出現しないUI */
		TUTORIAL_UI_MANUAL = 0,
		TUTORIAL_UI_CLICK_R,
		TUTORIAL_UI_CLICK_L,
		TUTORIAL_UI_START,
		TUTORIAL_UI_MAX
	} TUTORIAL_UI;

	typedef enum
	{
		/* タイトルプレイ中には出現しないUI */
		TUTORIAL_STATE_FIRST = 0,
		TUTORIAL_STATE_SECOND,
		TUTORIAL_STATE_MAX
	} TUTORIAL_STATE;
	/* 関数 */
	void Various(int nCnt);											// 種類別処理
																	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex[TUTORIAL_UI_MAX];	// テクスチャー設定
	static D3DXVECTOR3 m_pos[TUTORIAL_UI_MAX];			// 位置情報
	static D3DXVECTOR2 m_size[TUTORIAL_UI_MAX];			// サイズ情報

	CScene_TWO *m_aScene_Two[TUTORIAL_UI_MAX];			// チュートリアル用ui2D
	TUTORIAL_STATE m_TutorialState;
	int		m_nSelect;									// 選択番号
	int		m_nCntCol;									// カラーカウント
	float	m_fAnim;									// アニメーション
	float	m_fMult;									// 倍数
	float	m_fAdd;										// 加算
};

#endif