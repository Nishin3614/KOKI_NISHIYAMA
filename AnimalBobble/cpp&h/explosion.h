// ----------------------------------------
//
// 爆発処理の説明[explosion.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "main.h"
#include "scene_two.h"
#include "bullet.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

// ------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------
class CExplosion : public CScene_TWO
{
public:
	/* 関数 */
	CExplosion();
	~CExplosion();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CExplosion * Create(D3DXVECTOR3 pos, CBullet::TYPE nType);	// 作成
	static HRESULT Load(void);									// 読み込み
	static void UnLoad(void);									// 読み込んだ情報を破棄
protected:

private:
	static LPDIRECT3DTEXTURE9 m_pTex[CBullet::TYPE_MAX];	// テクスチャー情報
	D3DXVECTOR3 m_pos;							// 位置
	D3DXCOLOR m_col;							// 色
	int	m_nLife;								// 体力
	CBullet::TYPE m_type;						// タイプ
};
#endif