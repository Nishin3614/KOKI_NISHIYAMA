// ------------------------------------------------------------------------------------------
//
// カメラ処理 [camera.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#define _CRT_SECURE_NO_WARNINGS

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// カメラの構造体
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 posV;			// 視点
	D3DXVECTOR3 posVDest;		// 目的視点
	D3DXVECTOR3 posVDiff;		// 目的視点から視点
	D3DXVECTOR3 posR;			// 注視点
	D3DXVECTOR3 posRDest;		// 目標注視点
	D3DXVECTOR3 posRDiff;		// 目的注視点から注視点
	D3DXVECTOR3 posU;			// 上方向ベクトル
	D3DXVECTOR3 between;		// プレイヤーとの距離 
	D3DXVECTOR3 last;			// 
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// 視点の回転量
	D3DXVECTOR3 rotDest;		// 視点の目的回転地点
	D3DXVECTOR3 rotDiff;		// 目的回転地点から現在回転地点
	D3DXVECTOR3 rotBetween;		// 目的回転地点と現在回転地点の差分
	D3DXMATRIX	mtxProjection;	// プロジェクションマトリックス(テレビ視点)
	D3DXMATRIX	mtxView;		// ビューマトリックス(カメラマン視点)
	float		fLength;		// 視点と注視点の距離
	int			nCntTime;		// 正面に向くのにかかる時間
	bool		bfront;			// 正面に向くか向かないか
} CAMERA;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

// カメラ設定
void SetCamera(void);

void CameraPosition(void);

// カメラ情報
CAMERA *GetCamera(void);
#endif
