// ------------------------------------------------------------------------------------------
//
// 敵処理 [enemy.h]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#ifndef _ENEMY_H_
#define _ENEMY_H_

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
	D3DXMATRIX  mtxWorldEnemy;						// ワールドマトリックス
	D3DXVECTOR3 OriginEnemy;						// ブロック描画の原点
	int	nNumberVertexMeshField;						// 総頂点数
	int nNumIndex;									// 総インデックス
	int nNumPolygon;								// 総ポリゴン
	int nNumber;									// 頂点
	int nEnemy_Depth;								// 縦ブロック数
	int nEnemy_Width;								// 横ブロック数
	int nType;										// タイプ
	bool		bUse;								// 使用状態
	bool		bDisp;								// 表示状態
} ENEMY;

typedef struct
{
	D3DXVECTOR3 size;								// サイズ
	D3DXVECTOR3 rot;								// 回転量
	int nEnemy_Depth;								// 縦ブロック数
	int nEnemy_Width;								// 横ブロック数
	int nType;										// タイプ
} ENEMYSET;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

void SetEnemy(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nEnemy_Depth,
	int	nEnemy_Width,
	int	nType);

// 当たり判定
bool ColisionEnemy(
	D3DXVECTOR3 *pos,		// 現在のポジション
	D3DXVECTOR3 *posOld,	// 前回のポジション
	D3DXVECTOR3 *move,		// 移動量
	D3DXVECTOR3 *size,		// サイズ
	ENEMY		**ppEnemyRet);

void NotDispEnemy(void);	// 表示切替時に表示に

// セーブ
void SaveEnemy(void);

// ロード
void LoadEnemy(void);

ENEMY *GetEnemy(void);

#ifdef _DEBUG			
// 床の情報
//void DrawEnemyInfo(
//	D3DXVECTOR3 size,
//	int nEnemy_W,
//	int nEnemy_H,
//	int nType);
#endif

#endif
