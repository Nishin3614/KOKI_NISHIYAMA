// ------------------------------------------------------------------------------------------
//
// 半球処理 [hemisphere.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _HEMISPHERE_H_
#define _HEMISPHERE_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffHemisphere = NULL;	// 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9		pTextureHemisphere = NULL;	// 頂点テクスチャーのポインタ
	LPDIRECT3DINDEXBUFFER9	pIndexMeshField = NULL;			// インデックスバッファのポインタ
	D3DXVECTOR3 pos;										// 位置
	D3DXVECTOR3 move;										// 移動量
	D3DXVECTOR3 rot;										// 回転量
	D3DXVECTOR3 size;										// サイズ
	D3DXVECTOR3 OriginBlock;								// ブロック描画の原点
	D3DXMATRIX  mtxWorldHemisphere;						// ワールドマトリックス
	int	nNumberVertexMeshField;								// 総頂点数
	int nNumIndex;											// 総インデックス
	int nNumPolygon;										// 総半球
	int nNumber;											// 頂点
	int nBlock_Depth;										// 縦ブロック数
	int nBlock_Width;										// 横ブロック数
	float fRadius;											// ラジアン値
} HEMISPHERE;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitHemisphere(void);
void UninitHemisphere(void);
void UpdateHemisphere(void);
void DrawHemisphere(void);

HEMISPHERE *GetHemisphere(void);
#endif
