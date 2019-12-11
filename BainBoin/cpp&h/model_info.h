//=============================================================================
//
// モデル情報処理 [model_info.h]
// Author : koki_nishiyama
//
//=============================================================================
#ifndef _MODEL_INFO_H_
#define _MODEL_INFO_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

//=============================================================================
// 前方宣言
//=============================================================================

//=============================================================================
// 構造体
//=============================================================================
// モデル情報
typedef struct
{
	LPD3DXMESH			pMesh;				// メッシュ情報へのポインタ
	LPD3DXBUFFER		pBuffMat;			// マテリアル情報へのポインタ
	LPDIRECT3DTEXTURE9	pTexture;			// テクスチャへのポインタ
	DWORD				nNumMat;			// マテリアルの数
	D3DXVECTOR3			vtxMinMaterials;	// モデル情報の位置の最小値
	D3DXVECTOR3			vtxMaxMaterials;	// モデル情報の位置の最大値
	D3DXVECTOR3			pos;				// 位置情報
	D3DXVECTOR3			posDest;			// 目的位置
	D3DXVECTOR3			rot;				// 回転情報
	D3DXVECTOR3			rotDest;			// 目的回転
	D3DXVECTOR3			size;				// サイズ
	D3DXVECTOR3			scal;				// スケール
	int					nParent;			// 親番号
} MODEL_INFO,*pMODEL_INFO;

// オフセットの情報
typedef struct
{
	D3DXVECTOR3			pos;		// 位置
	D3DXVECTOR3			rot;		// 回転
	int					nParent;	// 親番号
} MODEL_OFFSET;

//=============================================================================
// クラス
//=============================================================================
class CModel_info
{
public:
	/* 関数 */
	CModel_info();
	~CModel_info();
	// 開放
	void Unload(void);
	// 情報取得
	MODEL_INFO			&GetModel_info(void);	// モデル情報取得
	LPD3DXMESH			&GetMesh(void);			// メッシュ
	LPD3DXBUFFER		&GetBuffmat(void);		// マテリアル
	LPDIRECT3DTEXTURE9	&GetTex(void);			// テクスチャー
	DWORD				&GetNumMat(void);		// マテリアル数
	D3DXVECTOR3			&GetvtxMinMat(void);	// 頂点座標の最小値
	D3DXVECTOR3			&GetvtxMaxMat(void);	// 頂点座標の最大値
	D3DXVECTOR3			&GetPos(void);			// 位置
	D3DXVECTOR3			&GetRot(void);			// 回転
	int					&GetParent(void);		// 親
	// 読み込み
	void Load(									// モデル情報
		LPDIRECT3DDEVICE9 pDevice,
		const char* charModel
	);
	static void ModelOffset(					// モデルのオフセット設定
		MODEL_OFFSET * pModel_offset,
		const char * file_name
	);
protected:
private:
	MODEL_INFO			m_model_info;			// モデル情報
};

#endif
