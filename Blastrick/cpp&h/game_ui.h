// ----------------------------------------
//
// ゲームUI処理の説明[game_ui.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _GAME_UI_H_
#define _GAME_UI_H_	 // ファイル名を基準を決める

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
class CNumber;

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CGame_ui : public CScene
{
public:
	/* 関数 */
	CGame_ui();
	~CGame_ui();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	CNumber * GetNumber(void);		// 番号取得
	static HRESULT Load(void);		// 読み込み
	static void UnLoad(void);		// UnLoadする
	static CGame_ui * Create(void);	// 作成
protected:

private:
	/* 列挙型 */
	typedef enum
	{
		/* ゲームプレイ中には出現しないUI */
		GAME_UI_START = 0,
		GAME_UI_STAGECLEAR,
		GAME_UI_GAMEOVER,
		GAME_UI_MAX
	} GAME_UI;
	/* 関数 */

	/* 変数 */
	static LPDIRECT3DTEXTURE9 m_pTex[GAME_UI_MAX];	// テクスチャー設定
	static D3DXVECTOR3 m_pos[GAME_UI_MAX];			// 位置情報
	static D3DXVECTOR2 m_size[GAME_UI_MAX];			// サイズ情報
	CScene_TWO *m_aScene_Two[GAME_UI_MAX];			// ポーズ用ui2D
	CNumber * m_pNumber;							// 番号
};

#endif