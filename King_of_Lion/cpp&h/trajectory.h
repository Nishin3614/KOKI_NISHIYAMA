// ------------------------------------------------------------------------------------------
//
// 軌跡処理 [trajectory.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _TRAJECTORY_H_
#define _TRAJECTORY_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define MAX_MESHOBIT (100)							// 軌跡のポリゴン数

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// 軌跡のタイプ //
enum
{
	TRAJECTORYTYPE_PLAYER = 0,
	TRAJECTORYTYPE_GIRAFFE,
	TRAJECTORYTYPE_BUFFALO,
	TRAJECTORYTYPE_KING_RION,
	TRAJECTORYTYPE_MAX
};

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
// ---------- 軌跡の線 ---------- //
typedef struct
{
	D3DXVECTOR3 pos[2];									// 位置
} MESH_OBIT;

// ---------- 軌跡 ---------- //
typedef struct
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffTrajectory = NULL;	// 頂点バッファへのポインタ
	D3DXVECTOR3 pos;									// 位置
	D3DXVECTOR3 ofset;									// オフセット
	D3DXVECTOR3 rot;									// 回転量
	D3DXMATRIX  mtxWorld;								// ワールドマトリックス
	MESH_OBIT	MeshObit[MAX_MESHOBIT];					// 軌跡のポイント
	int	nNumberVertexMeshObit;							// 総頂点数
	int nNumPolygon;									// 総軌跡
	int nNumber;										// 頂点
	int	nType;											// タイプ
	int nParts;											// モデルのパーツ番号
	int	nMaxMeshObit;									// 最大軌跡数
	int	nCntFram;										// フレーム数
	int	nStartFram;										// 開始フレーム数
	int nEndFram;										// 終了フレーム数
	int	nCntObject;										// どのオブジェクトか
	bool bUse;											// 使用状態
	bool bDisp;											// 表示状態
} TRAJECTORY;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitTrajectory(void);
void UninitTrajectory(void);
void UpdateTrajectory(void);
void DrawTrajectory(void);

// 軌跡の設定
int SetTrajectory(
	D3DXVECTOR3 ofset,
	int nParts,
	int MaxMeshObit,
	int StartFram,
	int EndFram,
	int Type);

// 軌跡の発生
void OccurrenceTrajectory(
	int IdTrajectory,
	int	nCntObject);

// 軌跡の消去
void DeleteTrajectory(int nIdTrajectory);

TRAJECTORY *GetTrajectory(void);
#endif
