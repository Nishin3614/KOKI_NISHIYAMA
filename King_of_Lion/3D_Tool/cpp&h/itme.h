// ------------------------------------------------------------------------------------------
//
// アイテム処理 [item.h]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _ITME_H_
#define _ITME_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------

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
	D3DXMATRIX  mtxWorldItem;						// ワールドマトリックス
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
} ITME;

typedef struct
{
	D3DXVECTOR3 size;								// サイズ
	D3DXVECTOR3 rot;								// 回転量
	int nBlock_Depth;								// 縦ブロック数
	int nBlock_Width;								// 横ブロック数
	int nType;										// タイプ
} ITMESET;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

void SetItem(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nBlock_Depth,
	int	nBlock_Width,
	int	nType);

// 当たり判定
bool ColisionItem(
	D3DXVECTOR3 *pos,		// 現在のポジション
	D3DXVECTOR3 *posOld,	// 前回のポジション
	D3DXVECTOR3 *move,		// 移動量
	D3DXVECTOR3 *size,		// サイズ
	ITME		**ppItemRet);

void NotDispItem(void);	// 表示切替時に表示に

// セーブ
void SaveItem(void);

// ロード
void LoadItem(void);

ITME *GetItem(void);

#ifdef _DEBUG			
// 床の情報
//void DrawItemInfo(
//	D3DXVECTOR3 size,
//	int nBlock_W,
//	int nBlock_H,
//	int nType);
#endif

#endif
