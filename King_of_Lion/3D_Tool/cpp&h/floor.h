// ------------------------------------------------------------------------------------------
//
// ポリゴン処理 [floor.h]
// Author : KOKI_NISHIYAMA
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

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------


// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffFLOOR = NULL;	// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	pIndexMeshField = NULL;		// インデックスバッファのポインタ
	D3DXVECTOR3 pos;									// 位置
	D3DXVECTOR3 move;									// 移動量
	D3DXVECTOR3 rot;									// 回転量
	D3DXVECTOR3 size;									// サイズ
	D3DXVECTOR3 OriginBlock;							// ブロック描画の原点
	D3DXMATRIX  mtxWorldFLOOR;							// ワールドマトリックス
	D3DXCOLOR	col;									// カラー
	int	nNumberVertexMeshField;							// 総頂点数
	int nNumIndex;										// 総インデックス
	int nNumPolygon;									// 総ポリゴン
	int nNumber;										// 頂点
	int nBlock_Depth;									// 縦ブロック数
	int nBlock_DepthOld;								// 前回の縦ブロック数
	int nBlock_Width;									// 横ブロック数
	int nBlock_WidthOld;								// 前回の横ブロック数
	int nType;											// 種類
	bool bUse;											// 使用状態
	bool bDisp;											// 表示状態
	bool bSelect;										// 選択状態
} FLOOR;

typedef struct
{
	D3DXVECTOR3 size;									// サイズ
	int nBlock_Depth;									// 縦ブロック数
	int nBlock_Width;									// 横ブロック数
	int nType;											// タイプ
} FLOORSET;


// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitFloor(void);
void UninitFloor(void);
void UpdateFloor(void);
void DrawFloor(void);
// 床の設置
void SetFloor(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nBlock_Depth,
	int	nBlock_Width,
	int	nType);

// 当たり判定
bool ColisionFloor(
	D3DXVECTOR3 *pos,		// 現在のポジション
	D3DXVECTOR3 *posOld,	// 前回のポジション
	D3DXVECTOR3 *move,		// 移動量
	D3DXVECTOR3 *size,		// サイズ
	FLOOR		**ppFloorRet);

// 表示切替時に表示に
void NotDispFLoor(void);	

// セーブ
void SaveFloor(void);
// ロード
void LoadFloor(void);

// 選択
bool SeleFloor(int nCntFloor);

// 床の情報取得
FLOOR *GetFloor(void);

#endif
