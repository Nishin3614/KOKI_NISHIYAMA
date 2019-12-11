// ------------------------------------------------------------------------------------------
//
// 壁処理 [wall.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _WALL_H_
#define _WALL_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define WALL_MAX (128)

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;		// 頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9	pIndex = NULL;			// インデックスバッファのポインタ
	D3DXVECTOR3 pos;								// 位置
	D3DXVECTOR3 move;								// 移動量
	D3DXVECTOR3 rot;								// 回転量
	D3DXVECTOR3 size;								// サイズ
	D3DXMATRIX  mtxWorldWall;						// ワールドマトリックス
	D3DXVECTOR3 OriginBlock;						// ブロック描画の原点
	int	nNumberVertexMeshField;						// 総頂点数
	int nNumIndex;									// 総インデックス
	int nNumPolygon;								// 総ポリゴン
	int nNumber;									// 頂点
	int nBlock_Depth;								// 縦ブロック数
	int nBlock_Width;								// 横ブロック数
	int nType;										// タイプ
	bool		bUse;								// 使用状態
	bool		bDisp;								// 表示状態

	// 当たり判定に使う変数
	D3DXVECTOR3	BiginPos;							// 原点とする
	D3DXVECTOR3 LastPos;							// 最終地点
	D3DXVECTOR3	Length;								// 壁の長さ
	D3DXVECTOR3 VecWall;							// 壁のベクトル
	float		fdistance;							// 距離
} WALL;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitWall(void);
void UninitWall(void);
void UpdateWall(void);
void DrawWall(void);

// 当たり判定
bool ColisionWall(
	D3DXVECTOR3 *pos,		// 現在のポジション
	D3DXVECTOR3 *posOld,	// 前回のポジション
	D3DXVECTOR3 *move,		// 移動量
	D3DXVECTOR3 *size,		// サイズ
	WALL		**ppWallRet);

WALL *GetWall(void);
#endif
