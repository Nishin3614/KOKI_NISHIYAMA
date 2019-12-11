// ------------------------------------------------------------------------------------------
//
// 水処理 [water.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _WATER_H_
#define _WATER_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define WATER_MAX (16)						// 水の最大数

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffWATER = NULL;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	pIndexMeshField = NULL;		// インデックスバッファのポインタ
	D3DXVECTOR3 pos;									// 位置
	D3DXVECTOR3 move;									// 移動量
	D3DXVECTOR3 rot;									// 回転量
	D3DXVECTOR3 size;									// サイズ
	D3DXVECTOR3 OriginBlock;							// ブロック描画の原点
	D3DXMATRIX  mtxWorldWATER;							// ワールドマトリックス
	int	nNumberVertexMeshField;							// 総頂点数
	int nNumIndex;										// 総インデックス
	int nNumPolygon;									// 総水
	int nNumber;										// 頂点
	int nBlock_Depth;									// 縦ブロック数
	int nBlock_Width;									// 横ブロック数
	int nType;											// 種類
	bool bUse;											// 使用状態
	bool bDisp;											// 表示状態
} WATER;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitWATER(void);
void UninitWATER(void);
void UpdateWATER(void);
void DrawWATER(void);

// 当たり判定
bool ColisionWATER(
	D3DXVECTOR3 *pos,		// 現在のポジション
	D3DXVECTOR3 *posOld,	// 前回のポジション
	D3DXVECTOR3 *move,		// 移動量
	D3DXVECTOR3 *size,		// サイズ
	WATER		**ppWaterRet);

// 水の情報
WATER *GetWATER(void);
#endif
