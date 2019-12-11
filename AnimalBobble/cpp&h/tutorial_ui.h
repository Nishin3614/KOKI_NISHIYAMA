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

// ------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------
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
	/* 関数 */
																	
	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex;	// テクスチャー設定
	CScene_TWO *m_aScene_Two;			// チュートリアル用ui2D
	int m_nSelect;						// 選択番号
	float m_fAnim;						// アニメーション
};

#endif