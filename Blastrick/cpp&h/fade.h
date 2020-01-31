// ----------------------------------------
//
// 画面遷移処理の説明[fade.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _FADE_H_
#define _FADE_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "manager.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define MAX_FADE (10)

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CFade
{
public:
	typedef enum
	{
		FADE_NONE = 0,		// 何もない状態
		FADE_IN,			// フェードイン処理
		FADE_OUT,			// フェードアウト処理
		FADE_MAX
	} FADE;

	/* 関数 */
	CFade();
	~CFade();
	void Init(CManager::MODE Mode);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetFade(CManager::MODE Mode);	// 画面遷移設定
	FADE GetFade(void);					// 画面遷移情報取得

	static HRESULT Load(void);
	static void UnLoad(void);
protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex;	// テクスチャー
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	FADE					m_fade;		// フェード状態
	CManager::MODE			m_modeNext;	// 次の画面（モード）
	D3DXCOLOR				m_col;		// フェード色
};

#endif