// ------------------------------------------------------------------------------------------
//
// モデル処理 [model.h]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------

// ---------- トライ敵 ---------- //
typedef struct
{
	LPD3DXMESH		paMesh;				// メッシュ情報へのポインタ
	LPD3DXBUFFER	paBuffMat;			// マテリアル情報へのポインタ
	DWORD			nNumMat;			// マテリアルの数
	D3DXMATRIX		mtxWorldModel;		// ワールドマトリックス
	D3DXVECTOR3		pos;				// 位置
	D3DXVECTOR3		posLast;			// 行きたい位置
	D3DXVECTOR3		posKeyBetween;		// キー間の距離
	D3DXVECTOR3		posOrigin;			// 初期位置
	D3DXVECTOR3		rot;				// 現在回転量
	D3DXVECTOR3		rotLast;			// 行きたい方向
	D3DXVECTOR3		rotKeyBetween;		// キー間の回転量
	D3DXVECTOR3		size;				// サイズ
	D3DXVECTOR3		vtxMinMaterials;	// モデルの位置の最小値
	D3DXVECTOR3		vtxMaxMaterials;	// モデルの位置の最大値
	int				nIdxModelParent;	// 親モデルのインデックス
	int				nFram;				// フレーム
	int				nCntKeySet;			// キーセットカウント
} MODEL;

// ---------- キー要素 ---------- //
typedef struct 
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
} KEY;

// ---------- キー情報 ---------- //
typedef struct
{
	int nFram;
	KEY key[15];	// パーツ数
} KEY_INFO;
#endif