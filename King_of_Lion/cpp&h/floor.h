// ------------------------------------------------------------------------------------------
//
// 床処理 [floor.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _FLOOR_H_
#define _FLOOR_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define FLOOR_MAX (128)						// 床の最大数

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffFLOOR = NULL;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	pIndexMeshField = NULL;		// インデックスバッファのポインタ
	D3DXVECTOR3 pos;									// 位置
	D3DXVECTOR3 move;									// 移動量
	D3DXVECTOR3 rot;									// 回転量
	D3DXVECTOR3 size;									// サイズ
	D3DXVECTOR3 OriginBlock;							// ブロック描画の原点
	D3DXMATRIX  mtxWorldFLOOR;							// ワールドマトリックス
	int	nNumberVertexMeshField;							// 総頂点数
	int nNumIndex;										// 総インデックス
	int nNumPolygon;									// 総床
	int nNumber;										// 頂点
	int nBlock_Depth;									// 縦ブロック数
	int nBlock_Width;									// 横ブロック数
	int nType;											// 種類
	bool bUse;											// 使用状態
	bool bDisp;											// 表示状態
} FLOOR;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitFLOOR(void);
void UninitFLOOR(void);
void UpdateFLOOR(void);
void DrawFLOOR(void);

// 当たり判定
bool ColisionFLOOR(
	D3DXVECTOR3 *pos,			// 現在のポジション
	D3DXVECTOR3 *posOld,		// 前回のポジション
	D3DXVECTOR3 *move,			// 移動量
	D3DXVECTOR3 *size,			// サイズ
	FLOOR		**ppFloorRet	// 床のアドレス情報のアドレス
);

// 床の情報
FLOOR *GetFLOOR(void);
#endif
