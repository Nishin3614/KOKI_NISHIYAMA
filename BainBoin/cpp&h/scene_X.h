// ----------------------------------------
//
// Xシーン処理の説明[scene_X.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _SCENE_X_H_
#define _SCENE_X_H_	 // ファイル名を基準を決める

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
#include "scene.h"
#include "model_info.h"

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
class CScene_X : public CScene
{
public:
	/* 関数 */
	CScene_X();
	CScene_X(ACTOR actor, LAYER layer);
	~CScene_X();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	// 情報共有
	void BindTexture(LPDIRECT3DTEXTURE9 p_Tex);		// テクスチャー
	void BindModel(MODEL_INFO const &model_info);	// モデル
	static CScene_X * Create();						// 作成
	// 設定
	void SetPos(D3DXVECTOR3 pos);					// 位置
	void SetRot(D3DXVECTOR3 size);					// 回転
	// 取得
	D3DXVECTOR3 GetPos(void);						// 位置
	D3DXVECTOR3 GetRot(void);						// 回転

protected:
private:
	/* 関数 */
	/* 変数 */
	MODEL_INFO		m_model_info;	// モデル情報
	D3DXMATRIX		m_mtxWorld;		// ワールドマトリックス
	LPDIRECT3DTEXTURE9	pTexture[30];			// テクスチャへのポインタ
};

#endif