// ------------------------------------------------------------------------------------------
//
// カメラ処理 [camera.h]
// Author : KOKI_NISHIYAMA
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
	D3DXVECTOR3 move;			// 移動量
	D3DXVECTOR3 rot;			// 視点の回転量
	D3DXMATRIX	mtxProjection;	// プロジェクションマトリックス(テレビ視点)
	D3DXMATRIX	mtxView;		// ビューマトリックス(カメラマン視点)
	float		fLength;		// 視点と注視点の距離
	float		fY;				// yの位置

	// マウス用
	D3DXVECTOR2 mouse;			// マウス座標
	D3DXVECTOR2 mouseold;		// マウス座標保存
	D3DXVECTOR2 mouseDiff;		// 現在のマウス座標と過去のマウス座標の差
} CAMERA;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);

// カメラ設定
void SetCamera(void);

// カメラ情報
CAMERA *GetCamera(void);

// カメラの視点情報
D3DXVECTOR3 GetPosV(void);

// カメラの注視点情報
D3DXVECTOR3 GetPosR(void);

#endif
