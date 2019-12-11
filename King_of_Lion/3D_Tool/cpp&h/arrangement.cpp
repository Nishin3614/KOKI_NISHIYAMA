// ------------------------------------------------------------------------------------------
//
// 配置処理 [arrangement.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "arrangement.h"
#include "input.h"
#include "camera.h"
#include "mouse.h"

#ifdef _DEBUG
#include "debugproc.h"
#endif // _DEBUG

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define ARRANGEMENT_MOVE (2.0f)				// 移動量
#define ARRANGEMENT_MOVE_Y (50)				// yの移動量
#define ARRANGEMENT_ROUND (100)				// 四捨五入範囲
#define ARRANGEMENT_ROUND_Y (50)			// yの値四捨五入範囲
#define ARRANGEMENT_ROT_ROUND (D3DX_PI / 2)	// 角度の四捨五入範囲

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void Update_RotArrangement(void);
void Update_MoveArrangement(void);
void Update_RoundOffArrangement(void);

#ifdef _DEBUG
void Debug_Arrangement(void);
#endif // _DEBUG

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
ARRANGEMENT				g_arrangement;				// 配置情報

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitArrangement(void)
{
	// 配置の初期設定
	g_arrangement.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_arrangement.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitArrangement(void)
{

}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateArrangement(void)
{
	// 回転
	Update_RotArrangement();
	// 移動
	Update_MoveArrangement();
	// 四捨五入
	Update_RoundOffArrangement();
}

// ------------------------------------------------------------------------------------------
// 回転処理
// ------------------------------------------------------------------------------------------
void Update_RotArrangement(void)
{
	CAMERA		*pCamera = GetCamera();						// カメラ情報取得
	float MouseposX = (float)GetMouseX();					// マウスのx座標を取得
	float MouseposZ = (float)GetMouseY();					// マウスのy座標を取得
	D3DXVECTOR3 WorldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ワールド座標

	g_arrangement.rot = pCamera->rot;
	// マウスの点をスクリーン座標からワールド座標へ(オブジェクトの存在する平面)
	CalcScreenToXZ(
		&g_arrangement.pos,
		MouseposX,
		MouseposZ,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		&pCamera->mtxView,
		&pCamera->mtxProjection,
		D3DXVECTOR3(0.0f, g_arrangement.pos.y, 0.0f)	// 平面を決める(Y座標を変える)
	);
}

// ------------------------------------------------------------------------------------------
// 移動処理
// ------------------------------------------------------------------------------------------
void Update_MoveArrangement(void)
{
	CAMERA *pCamera = GetCamera();	// カメラ情報取得

	if (GetKeyboardPress(DIK_A))
	{
		g_arrangement.pos.x += sinf(-D3DX_PI * 0.5f + pCamera->rot.y) * ARRANGEMENT_MOVE;
		g_arrangement.pos.z += cosf(-D3DX_PI * 0.5f + pCamera->rot.y) * ARRANGEMENT_MOVE;
	}

	if (GetKeyboardPress(DIK_D))
	{
		g_arrangement.pos.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y) * ARRANGEMENT_MOVE;
		g_arrangement.pos.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y) * ARRANGEMENT_MOVE;
	}

	if (GetKeyboardPress(DIK_W))
	{
		g_arrangement.pos.x += sinf(D3DX_PI * 0.0f + pCamera->rot.y) * ARRANGEMENT_MOVE;
		g_arrangement.pos.z += cosf(D3DX_PI * 0.0f + pCamera->rot.y) * ARRANGEMENT_MOVE;
	}

	if (GetKeyboardPress(DIK_S))
	{
		g_arrangement.pos.x += sinf(D3DX_PI * 1.0f + pCamera->rot.y) * ARRANGEMENT_MOVE;
		g_arrangement.pos.z += cosf(D3DX_PI * 1.0f + pCamera->rot.y) * ARRANGEMENT_MOVE;
	}
	
	if (GetKeyboardPress(DIK_E))
	{
		g_arrangement.pos.y += ARRANGEMENT_MOVE;
	}

	if (GetKeyboardPress(DIK_C))
	{
		g_arrangement.pos.y -= ARRANGEMENT_MOVE;
	}
}

// ------------------------------------------------------------------------------------------
// 四捨五入
// ------------------------------------------------------------------------------------------
void Update_RoundOffArrangement(void)
{
	// 位置 //
	// x
	g_arrangement.pos.x /= ARRANGEMENT_ROUND;
	g_arrangement.pos.x = roundf(g_arrangement.pos.x);
	g_arrangement.pos.x *= ARRANGEMENT_ROUND;
	// y
	g_arrangement.pos.y /= ARRANGEMENT_ROUND_Y;
	g_arrangement.pos.y = roundf(g_arrangement.pos.y);
	g_arrangement.pos.y *= ARRANGEMENT_ROUND_Y;
	// z
	g_arrangement.pos.z /= ARRANGEMENT_ROUND;
	g_arrangement.pos.z = roundf(g_arrangement.pos.z);
	g_arrangement.pos.z *= ARRANGEMENT_ROUND;
	// 角度 //
	// x
	g_arrangement.rot.x /= ARRANGEMENT_ROT_ROUND;
	g_arrangement.rot.x = roundf(g_arrangement.rot.x);
	g_arrangement.rot.x *= ARRANGEMENT_ROT_ROUND;
	// y
	g_arrangement.rot.y /= ARRANGEMENT_ROT_ROUND;
	g_arrangement.rot.y = roundf(g_arrangement.rot.y);
	g_arrangement.rot.y *= ARRANGEMENT_ROT_ROUND;
	// z
	g_arrangement.rot.z /= ARRANGEMENT_ROT_ROUND;
	g_arrangement.rot.z = roundf(g_arrangement.rot.z);
	g_arrangement.rot.z *= ARRANGEMENT_ROT_ROUND;
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------
// デバッグ表示処理
// ------------------------------------------------------------------------------------------
void Debug_Arrangement(void)
{
}
#endif // _DEBUG

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawArrangement(void)
{

}

// ------------------------------------------------------------------------------------------
// 配置情報処理
// ------------------------------------------------------------------------------------------
ARRANGEMENT *GetArrangement(void)
{
	return &g_arrangement;
}