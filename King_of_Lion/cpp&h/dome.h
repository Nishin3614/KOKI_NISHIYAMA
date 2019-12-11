// ------------------------------------------------------------------------------------------
//
// ドーム処理 [dome.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _DOME_H_
#define _DOME_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

// ========================
// タイプ
// ========================
typedef enum
{
	DOMETYPE_ = 0,	// ゴール地点
	DOMETYPE_MAX
} DOMETYPE;

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffDome = NULL;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	pIndexMeshField = NULL;	// インデックスバッファのポインタ
	D3DXVECTOR3 pos;								// 位置
	D3DXVECTOR3 move;								// 移動量
	D3DXVECTOR3 rot;								// 回転量
	D3DXVECTOR3 size;								// サイズ
	D3DXVECTOR3 OriginBlock;						// ブロック描画の原点
	D3DXCOLOR	col;								// カラー
	D3DXMATRIX  mtxWorldDome;						// ワールドマトリックス
	int	nNumberVertexMeshField;						// 総頂点数
	int	nNumIndex;									// 総インデックス
	int	nNumPolygon;								// 総ドーム
	int	nNumber;									// 頂点
	int	nBlock_Depth;								// 縦ブロック数
	int	nBlock_Width;								// 横ブロック数
	int nType;										// タイプ
	bool	bUse;									// 使用状態
} DOME;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitDome(void);
void UninitDome(void);
void UpdateDome(void);
void DrawDome(void);

// 使用中のものを初期化
void UseInitDome(void);

int SetDome(
	D3DXVECTOR3 pos,
	D3DXCOLOR	col,
	int nType);
void DeleteDome(int nIdx);

DOME *GetDome(void);
#endif
