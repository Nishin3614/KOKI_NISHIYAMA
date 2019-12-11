// ----------------------------------------
//
// 3Dシーン処理の説明[scene_three.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _SCENE_THREE_H_
#define _SCENE_THREE_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CScene_THREE : public CScene
{
public:
	/* 関数 */
	CScene_THREE();
	CScene_THREE(ACTOR actor, LAYER layer);
	~CScene_THREE();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetSize(D3DXVECTOR3 size);						// サイズ設定
	void SetPos(D3DXVECTOR3 pos);						// 位置設定
	void SetRot(D3DXVECTOR3 rot);						// 回転設定
	void SetCol(D3DXCOLOR col);							// 色設定
	void SetTex(D3DXVECTOR2 first,D3DXVECTOR2 last);	// 表示テクスチャー設定
	D3DXVECTOR3 GetSize(void);							// サイズ取得
	D3DXVECTOR3 GetRot(void);							// 回転取得
	D3DXVECTOR3 GetPos(void);							// 位置取得
	void BindTexture(LPDIRECT3DTEXTURE9 p_Tex);			// テクスチャー
	void SetBillboard(bool bBillboard);					// ビルボード状態
	static CScene_THREE * Create(bool bBillboard);		// 作成
	float GetHeight(D3DXVECTOR3 pos);					// 高さ取得
protected:
private:
	LPDIRECT3DTEXTURE9 m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// 頂点バッファへのポインタ
	D3DXVECTOR3 m_pos;					// 中心座標
	D3DXVECTOR3 m_OriginBlock;			// 初期配置
	D3DXMATRIX  m_mtxWorld;				// ワールドマトリックス
	D3DXVECTOR3 m_size;					// サイズ
	D3DXVECTOR3 m_rot;					// 角度
	float m_fLengh;						// 長さ
	float m_fAngle;						// 角度
	bool m_bBillboard;					// ビルボード状態
};

#endif