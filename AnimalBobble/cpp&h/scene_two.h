// ----------------------------------------
//
// 2Dシーン処理の説明[scene_two.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _SCENE_TWO_H_
#define _SCENE_TWO_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "main.h"
#include "scene.h"

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
class CScene_TWO : public CScene
{
public:
	/* 関数 */
	CScene_TWO();
	CScene_TWO(ACTOR actor, LAYER layer);
	~CScene_TWO();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetLengh(D3DXVECTOR2 dist);
	void SetSize(D3DXVECTOR2 size);						// サイズ設定
	void SetPosition(D3DXVECTOR3 pos);					// 位置設定
	void SetRot(float frot);							// 回転設定
	void SetCol(D3DXCOLOR col);							// 色設定
	void SetTex(D3DXVECTOR2 first,D3DXVECTOR2 last);	// 表示テクスチャー設定
	D3DXVECTOR2 GetSize(void);							// サイズ情報取得
	D3DXVECTOR3 GetPosition(void);						// 位置情報取得
	float GetLengh(void);								// 長さ情報取得

	void BindTexture(LPDIRECT3DTEXTURE9 p_Tex);			// テクスチャー
	static CScene_TWO * Create();						// 作成

protected:
private:
	LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;					// 中心座標
	float m_rot;						// 角度
	D3DXVECTOR2 m_size;					// サイズ
	float m_fLengh;						// 長さ
	float m_fAngle;						// 角度
};

#endif