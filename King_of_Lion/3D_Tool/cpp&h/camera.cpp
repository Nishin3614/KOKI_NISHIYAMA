// ------------------------------------------------------------------------------------------
//
// カメラ処理 [camera.cpp]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "camera.h"
#include "input.h"
#include "arrangement.h"
#include "mouse.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define CAMERA_MOVE (10.0f)
#define CAMERA_ROT (0.05f)

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MoveCamera(void);
void RotCamera(void);
void Update_Result(void);

// ------------------------------------------------------------------------------------------
// カメラの回転処理
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
CAMERA g_Camera;	// カメラ情報

// ------------------------------------------------------------------------------------------
// カメラの初期化処理
// ------------------------------------------------------------------------------------------
void InitCamera(void)
{
	// カメラの初期化
	g_Camera.posV = D3DXVECTOR3(0.0f, 400.0f, -300.0f);
	g_Camera.posVDest = g_Camera.posV;
	g_Camera.posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_Camera.move = D3DXVECTOR3(2.0f, 2.0f, 2.0f);
	g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Camera.fLength = 0;
	g_Camera.fY = 400.0f;
	g_Camera.mouse = D3DXVECTOR2(0.0f, 0.0f);
	g_Camera.mouseold = D3DXVECTOR2(0.0f, 0.0f);

	// 視点と注視点との距離計算
	D3DXVECTOR3 diffPos = g_Camera.posV - g_Camera.posR;

	// 長さ計算
	g_Camera.fLength = sqrtf(diffPos.x*diffPos.x + diffPos.z * diffPos.z);

}

// ------------------------------------------------------------------------------------------
// カメラの終了処理
// ------------------------------------------------------------------------------------------
void UninitCamera(void)
{
}

// ------------------------------------------------------------------------------------------
// カメラの更新処理
// ------------------------------------------------------------------------------------------
void UpdateCamera(void)
{
	// マウス座標を代入
	g_Camera.mouse = D3DXVECTOR2((float)GetMouseX(), (float)GetMouseY());	// マウスの座標

	// 現在と過去のマウス差
	g_Camera.mouseDiff = g_Camera.mouse - g_Camera.mouseold;

	// カメラ回転
	RotCamera();

	// カメラ移動
	MoveCamera();

	// カメラの追跡
	Update_Result();

	// 変数宣言
	g_Camera.mouseold = g_Camera.mouse;	// マウスの座標を保存
}

// ------------------------------------------------------------------------------------------
// カメラの設定処理
// ------------------------------------------------------------------------------------------
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// プロジェクションマトリックス //
	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxProjection);

	// プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(
		&g_Camera.mtxProjection,					// (テレビ視点)
		D3DXToRadian(45.0f),						// 視野角(画角)
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// アスペクト比
		10.0f,										// MearZ値(近いビュー平面のZ値)
		20000.0);									// FarZ値(遠いビュー平面のZ値)

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_Camera.mtxProjection);

	// ビューマトリックス //
	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_Camera.mtxView);

	// ビューマトリックスを作成
	D3DXMatrixLookAtLH(
		&g_Camera.mtxView,	// (カメラマン視点)
		&g_Camera.posV,		// カメラの視点
		&g_Camera.posR,		// カメラの注目点
		&g_Camera.posU);	// カメラの上方向ベクトル

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_Camera.mtxView);
}

// ------------------------------------------------------------------------------------------
// カメラの移動処理
// ------------------------------------------------------------------------------------------
void MoveCamera(void)
{
	// 変数宣言
	float fAngle;

	// マウス回転(注意：yは↑が-、↓が+)
	// カーソルクリック
	if (GetMousePress(2))
	{
		// ベクトルの角度
		fAngle = atan2f(g_Camera.mouseDiff.x, g_Camera.mouseDiff.y);

		// 規定値に抑える
		fAngle = Rot_One_Limit(fAngle);

		// 動いているとき
		if (fAngle != 0)
		{
			g_Camera.posVDest.x += sinf(D3DX_PI - fAngle + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posVDest.y += cosf(D3DX_PI - fAngle) * CAMERA_MOVE;
			g_Camera.posVDest.z += cosf(D3DX_PI - fAngle + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
			g_Camera.posRDest.y += cosf(D3DX_PI - fAngle) * CAMERA_MOVE;
			g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
		}
	}

	//-------------------- x軸移動 --------------------//
	// 左
	if (GetKeyboardPress(DIK_A))
	{
		// 奥
		if (GetKeyboardPress(DIK_W))
		{
			g_Camera.posVDest.x += sinf(-D3DX_PI * 0.25f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posVDest.z += cosf(-D3DX_PI * 0.25f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
			g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;

		}

		// 前
		else if (GetKeyboardPress(DIK_S))
		{
			g_Camera.posVDest.x += sinf(-D3DX_PI * 0.75f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posVDest.z += cosf(-D3DX_PI * 0.75f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
			g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
		}

		else
		{
			g_Camera.posVDest.x += sinf(-D3DX_PI * 0.5f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posVDest.z += cosf(-D3DX_PI * 0.5f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
			g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
		}
	}

	// 右
	else if (GetKeyboardPress(DIK_D))
	{
		// 奥
		if (GetKeyboardPress(DIK_W))
		{
			g_Camera.posVDest.x += sinf(D3DX_PI * 0.25f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posVDest.z += cosf(D3DX_PI * 0.25f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
			g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;

		}

		// 前
		else if (GetKeyboardPress(DIK_S))
		{
			g_Camera.posVDest.x += sinf(D3DX_PI * 0.75f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posVDest.z += cosf(D3DX_PI * 0.75f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
			g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
		}

		else
		{
			g_Camera.posVDest.x += sinf(D3DX_PI * 0.5f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posVDest.z += cosf(D3DX_PI * 0.5f + g_Camera.rot.y) * CAMERA_MOVE;
			g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
			g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
		}
	}

	//-------------------- z軸移動 --------------------//
	// 奥
	else if (GetKeyboardPress(DIK_W))
	{
		g_Camera.posVDest.x += sinf(D3DX_PI * 0.0f + g_Camera.rot.y) * CAMERA_MOVE;
		g_Camera.posVDest.z += cosf(D3DX_PI * 0.0f + g_Camera.rot.y) * CAMERA_MOVE;
		g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
		g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
	}

	// 前
	else if (GetKeyboardPress(DIK_S))
	{
		g_Camera.posVDest.x += sinf(D3DX_PI * 1.0f + g_Camera.rot.y) * CAMERA_MOVE;
		g_Camera.posVDest.z += cosf(D3DX_PI * 1.0f + g_Camera.rot.y) * CAMERA_MOVE;
		g_Camera.posRDest.x = g_Camera.posVDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
		g_Camera.posRDest.z = g_Camera.posVDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
	}

	//-------------------- y軸移動 --------------------//
	// 上へ
	if (GetKeyboardPress(DIK_R))
	{
		g_Camera.posVDest.y += cosf(D3DX_PI * 0.0f) * CAMERA_MOVE;
	}

	// 下へ
	else if (GetKeyboardPress(DIK_F))
	{
		g_Camera.posVDest.y += cosf(D3DX_PI * 1.0f) * CAMERA_MOVE;
	}
}

// ------------------------------------------------------------------------------------------
// カメラの回転処理
// ------------------------------------------------------------------------------------------
void RotCamera(void)
{
	// マウス回転(注意：yは↑が-、↓が+)
	// 右クリック
	if (GetMousePress(1))
	{
		if (g_Camera.mouseDiff.x < 0)
		{
			// 回転
			g_Camera.rot.y -= CAMERA_ROT;
		}
		else if (g_Camera.mouseDiff.x > 0)
		{
			// 回転
			g_Camera.rot.y += CAMERA_ROT;
		}

		if (g_Camera.mouseDiff.y < 0)
		{
			// 回転
			g_Camera.posRDest.y += 10.0f;

		}
		else if (g_Camera.mouseDiff.y > 0)
		{
			// 回転
			g_Camera.posRDest.y -= 10.0f;

		}
		// 規定値に抑える
		g_Camera.rot.y = Rot_One_Limit(g_Camera.rot.y);

		// 注視点の位置
		g_Camera.posRDest.x = g_Camera.posV.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
		g_Camera.posRDest.z = g_Camera.posV.z + cosf(g_Camera.rot.y) * g_Camera.fLength;

	}


	// 規定値に抑える
	g_Camera.rot.y = Rot_One_Limit(g_Camera.rot.y);

	// yの位置を増減
	if (GetKeyboardSpeacePress(DIK_Q))
	{
		g_Camera.fY += 100.0f;
	}

	else if (GetKeyboardSpeacePress(DIK_Z))
	{
		g_Camera.fY -= 100.0f;
	}
}

// ------------------------------------------------------------------------------------------
// カメラの最終結果処理
// ------------------------------------------------------------------------------------------
void Update_Result(void)
{
	// 目的地から現在地の距離(注視点)
	g_Camera.posRDiff = g_Camera.posRDest - g_Camera.posR;

	// 注視点移動
	g_Camera.posR.x += g_Camera.posRDiff.x * 0.1f;
	g_Camera.posR.y += g_Camera.posRDiff.y * 0.1f;
	g_Camera.posR.z += g_Camera.posRDiff.z * 0.1f;

	// 目的地から現在地の距離(注視点)
	g_Camera.posVDiff = g_Camera.posVDest - g_Camera.posV;

	// 注視点に慣性
	g_Camera.posV.x += g_Camera.posVDiff.x * 0.1f;
	g_Camera.posV.y += g_Camera.posVDiff.y * 0.1f;
	g_Camera.posV.z += g_Camera.posVDiff.z * 0.1f;
}

// ------------------------------------------------------------------------------------------
// カメラ情報処理
// ------------------------------------------------------------------------------------------
CAMERA *GetCamera(void)
{
	return &g_Camera;
}


// ------------------------------------------------------------------------------------------
// カメラの視点情報処理
// ------------------------------------------------------------------------------------------
D3DXVECTOR3 GetPosV(void)
{
	return g_Camera.posV;
}

// ------------------------------------------------------------------------------------------
// カメラの注視点情報処理
// ------------------------------------------------------------------------------------------
D3DXVECTOR3 GetPosR(void)
{
	return g_Camera.posR;
}
