// ----------------------------------------
//
// プレイ用UI処理の説明[play_element_ui.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _PLAY_ELEMENT_UI_H_
#define _PLAY_ELEMENT_UI_H_	 // ファイル名を基準を決める

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
class CPlay_element_ui : public CScene
{
public:
	/* 関数 */
	CPlay_element_ui();
	~CPlay_element_ui();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

	static HRESULT Load(void);		// 読み込み
	static void UnLoad(void);		// UnLoadする
	static CPlay_element_ui * Create(void);	// 作成
protected:

private:
	/* 列挙型 */
	typedef enum
	{
		/* ゲーム内のUI */
		PLAY_ELEMENT_UI_SCORE = 0,
		PLAY_ELEMENT_UI_TIME,
		PLAY_ELEMENT_UI_MAX
	} PLAY_ELEMENT_UI;
	/* 関数 */

	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex[PLAY_ELEMENT_UI_MAX];	// テクスチャー設定
	static D3DXVECTOR3 m_pos[PLAY_ELEMENT_UI_MAX];			// 位置情報
	static D3DXVECTOR2 m_size[PLAY_ELEMENT_UI_MAX];			// サイズ情報
	CScene_TWO *m_aScene_Two[PLAY_ELEMENT_UI_MAX];			// プレイ用UI2D
};

#endif