// ----------------------------------------
//
// 試し背景処理の説明[tlybg.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _TLYBG_H_
#define _TLYBG_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "main.h"
#include "scene_two.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define MAX_TLYBG (1)

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CTlyBg : public CScene
{
public:
	/* 関数 */
	CTlyBg();
	~CTlyBg();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void UnLoad(void);
	static CTlyBg * Create(CManager::MODE mode);	// 作成
protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex[CManager::MODE_MAX][MAX_TLYBG];
	static CManager::MODE m_mode;									// モード
	static D3DXVECTOR3 m_pos[CManager::MODE_MAX][MAX_TLYBG];			// 位置情報
	static D3DXVECTOR2 m_size[CManager::MODE_MAX][MAX_TLYBG];			// サイズ情報
	CScene_TWO *m_aScene_Two[MAX_TLYBG];
};

#endif