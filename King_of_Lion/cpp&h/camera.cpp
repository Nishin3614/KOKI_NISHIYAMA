// ------------------------------------------------------------------------------------------
//
// カメラ処理 [camera.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "camera.h"
#include "input.h"
#include "joypad.h"
#include "player.h"
#include "game.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void Update_Title_MoveCamera(void);
void Update_Tutorial_MoveCamera(void);
void Update_Game_MoveCamera(void);
void Update_Game_Compliance(PLAYER *pPlayer);
void Update_Game_ViewMove(PLAYER *pPlayer);
void Update_Clear_MoveCamera(void);
void Update_GameOver_MoveCamera(void);
void Update_Rank_MoveCamera(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
CAMERA g_Camera;	// カメラ情報

// ------------------------------------------------------------------------------------------
// カメラの初期化処理
// ------------------------------------------------------------------------------------------
void InitCamera(void)
{
	// タイトル時
	if (GetMode() == MODE_TITLE)
	{
		// カメラの初期化
		g_Camera.posV = D3DXVECTOR3(30.0f, 50.0f, 500.0f);
		g_Camera.posVDest = g_Camera.posV;
		g_Camera.posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.move = D3DXVECTOR3(1.0f, 3.0f, 3.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.fLength = 0;
		g_Camera.nCntTime = 0;
		g_Camera.bfront = false;
	}

	// チュートリアル時
	if (GetMode() == MODE_TUTORIAL)
	{
		// カメラの初期化
		g_Camera.posV = D3DXVECTOR3(0.0f, 150.0f, 300.0f);
		g_Camera.posVDest = g_Camera.posV;
		g_Camera.posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.fLength = 0;
		g_Camera.nCntTime = 0;
		g_Camera.bfront = false;
	}

	// ゲーム時
	else if (GetMode() == MODE_GAME)
	{
		// カメラの初期化
		g_Camera.posV = D3DXVECTOR3(0.0f, 75.0f, 750.0f);
		g_Camera.posVDest = g_Camera.posV;
		g_Camera.posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 500.0f);
		g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.fLength = 0;
		g_Camera.nCntTime = 0;
		g_Camera.bfront = false;
	}

	// クリア時
	else if (GetMode() == MODE_CLEAR)
	{
		// カメラの初期化
		g_Camera.posV = D3DXVECTOR3(-12.0f, 75.0f, 150.0f);
		g_Camera.posVDest = g_Camera.posV;
		g_Camera.posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.fLength = 0;
		g_Camera.nCntTime = 0;
		g_Camera.bfront = false;
	}

	// ゲームオーバー時
	else if (GetMode() == MODE_GAMEOVER)
	{
		// カメラの初期化
		g_Camera.posV = D3DXVECTOR3(-12.0f, 75.0f, 150.0f);
		g_Camera.posVDest = g_Camera.posV;
		g_Camera.posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.fLength = 0;
		g_Camera.nCntTime = 0;
		g_Camera.bfront = false;
	}

	// ランキング時
	else if (
		GetMode() == MODE_RANK ||
		GetMode() == MODE_STATI)
	{
		// カメラの初期化
		g_Camera.posV = D3DXVECTOR3(-12.0f, 75.0f, 150.0f);
		g_Camera.posVDest = g_Camera.posV;
		g_Camera.posVDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_Camera.move = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_Camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.fLength = 0;
		g_Camera.nCntTime = 0;
		g_Camera.bfront = false;
	}

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
	PLAYER * pPlayer = GetPlayer(); // プレイヤー情報取得

	// タイトル時
	if (GetMode() == MODE_TITLE)
	{
		// カメラ移動
		Update_Title_MoveCamera();
	}

	// チュートリアル時
	else if (GetMode() == MODE_TUTORIAL)
	{
		// カメラ移動
		Update_Game_MoveCamera();

		// 視点・注視点の移動
		Update_Game_ViewMove(pPlayer);
	}

	// ゲーム時
	else if (GetMode() == MODE_GAME)
	{
		// カメラ移動
		Update_Game_MoveCamera();

		// 視点・注視点の移動
		Update_Game_ViewMove(pPlayer);
	}

	// クリア時
	else if (GetMode() == MODE_CLEAR)
	{
		// カメラ移動
		Update_Clear_MoveCamera();
	}

	// ゲームオーバー時
	else if (GetMode() == MODE_GAMEOVER)
	{
		// カメラ移動
		Update_GameOver_MoveCamera();
	}

	// ランキング時
	else if (GetMode() == MODE_RANK ||
		GetMode() == MODE_STATI)
	{
		// カメラ移動
		Update_Rank_MoveCamera();
	}
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
		20000.0f);									// FarZ値(遠いビュー平面のZ値)初期値は1000

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
// カメラの位置情報処理
// ------------------------------------------------------------------------------------------
void CameraPosition(void)
{
	// ボス
	if (GetGameStete() == GAMESTATE_BOSS)
	{
		g_Camera.posV = D3DXVECTOR3(0.0f, 20.0f, 30.0f);
		g_Camera.posVDest = g_Camera.posV;
		g_Camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Camera.posRDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}

// ------------------------------------------------------------------------------------------
// タイトルのカメラ移動処理
// ------------------------------------------------------------------------------------------
void Update_Title_MoveCamera(void)
{
	// カメラ移動
	g_Camera.posR.z -= g_Camera.move.z;

	//// カメラの視点回転
	//g_Camera.rot.y -= 0.01f;

	// 視点更新
	g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
	g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
}

// ------------------------------------------------------------------------------------------
// チュートリアルのカメラ移動処理
// ------------------------------------------------------------------------------------------
void Update_Tutorial_MoveCamera(void)
{

}

// ------------------------------------------------------------------------------------------
// ゲームのカメラ移動処理
// ------------------------------------------------------------------------------------------
void Update_Game_MoveCamera(void)
{
	// 注視点軸に回転 //
	// 半時計回り
	if (GetKeyboardPress(DIK_L))
	{
		g_Camera.rot.y += 0.03f;

		// -3.14～3.14に回転差分を固定させる
		g_Camera.rot.y = Rot_One_Limit(g_Camera.rot.y);

		// 視点更新
		g_Camera.posVDest.x = g_Camera.posRDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
		g_Camera.posVDest.z = g_Camera.posRDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;

		// カウントタイム初期化
		g_Camera.nCntTime = 0;

		// 正面を向くをoffへ
		g_Camera.bfront = false;
	}

	// 時計回り
	else if (GetKeyboardPress(DIK_K))
	{
		g_Camera.rot.y -= 0.03f;

		// -3.14～3.14に回転差分を固定させる
		g_Camera.rot.y = Rot_One_Limit(g_Camera.rot.y);

		// 視点更新
		g_Camera.posVDest.x = g_Camera.posRDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
		g_Camera.posVDest.z = g_Camera.posRDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
	
		// カウントタイム初期化
		g_Camera.nCntTime = 0;

		// 正面を向くをoffへ
		g_Camera.bfront = false;
	}

	// ジョイパッド用 //
	// パッド用 //
	//GetGamepadStickLeft
	int nValueH, nValueV;	// ゲームパッドのスティック情報の取得用
	float fRot;			// 移動速度

	// ゲームパッドのスティック情報を取得
	GetGamepadStickRight(0, &nValueH, &nValueV);

	// ================================
	// プレイヤー移動
	// ================================
	// ゲームパッド移動
	if (nValueH != 0 || nValueV != 0)
	{
		// 速度の計算
		if (nValueH < 0)
		{
			fRot = (abs(nValueH) * 0.8f) / 32768.0f;
			g_Camera.rot.y -= fRot;

		}
		// 速度の計算
		else if (nValueH > 0)
		{
			fRot = (abs(nValueH) * 0.8f) / 32768.0f;
			g_Camera.rot.y += fRot;

		}

		// -3.14～3.14に回転差分を固定させる
		g_Camera.rot.y = Rot_One_Limit(g_Camera.rot.y);

		// 視点更新
		g_Camera.posVDest.x = g_Camera.posRDest.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
		g_Camera.posVDest.z = g_Camera.posRDest.z + cosf(g_Camera.rot.y) * g_Camera.fLength;

		// カウントタイム初期化
		g_Camera.nCntTime = 0;

		// 正面を向くをoffへ
		g_Camera.bfront = false;
	}
}

// ------------------------------------------------------------------------------------------
// ゲームの追従処理
// ------------------------------------------------------------------------------------------
void Update_Game_Compliance(PLAYER *pPlayer)
{
	// カウントタイムアップ
	g_Camera.nCntTime++;

	// 正面を向く
	if (g_Camera.bfront == true)
	{
		//-------------------- 視点移動 --------------------//
		// ゲームのカメラの回転の目的地
		g_Camera.rotDest.y = pPlayer->rot.y;

		// 回転差分差分
		g_Camera.rotBetween.y = (g_Camera.rotDest.y - g_Camera.rot.y);

		// -3.14～3.14に回転差分を固定させる
		g_Camera.rotBetween.y = Rot_One_Limit(g_Camera.rotBetween.y);

		// ゲームのカメラの回転の慣性
		g_Camera.rot.y += g_Camera.rotBetween.y * 0.1f;

		// -3.14～3.14に回転差分を固定させる
		g_Camera.rot.y = Rot_One_Limit(g_Camera.rot.y);
	}

	//-------------------- 注視点移動 --------------------//
	// プレイヤーが動いているとき
	if (pPlayer->move.x <= -0.1 || pPlayer->move.x >= 0.1f ||
		pPlayer->move.y <= -0.1 || pPlayer->move.y >= 0.1f ||
		pPlayer->move.z <= -0.1 || pPlayer->move.z >= 0.1f)
	{
		// カウントタイム初期化
		g_Camera.nCntTime = 0;

		// 正面を向くをoffへ
		g_Camera.bfront = false;
	}

	// プレイヤーが止まって1秒経過したら
	if (g_Camera.nCntTime >= 30)
	{
		g_Camera.bfront = true;
	}

	// -3.14～3.14に回転差分を固定させる
	g_Camera.rot.y = Rot_One_Limit(g_Camera.rot.y);
}

// ------------------------------------------------------------------------------------------
// 視点・注視点の移動処理
// ------------------------------------------------------------------------------------------
void Update_Game_ViewMove(PLAYER *pPlayer)
{
	//-------------------- 注視点移動 --------------------//
	// 目的の注視点
	g_Camera.posRDest.x = pPlayer->pos.x + sinf(g_Camera.rot.y) * 10.0f;
	g_Camera.posRDest.y = pPlayer->pos.y;
	g_Camera.posRDest.z = pPlayer->pos.z + cosf(g_Camera.rot.y) * 10.0f;

	// 目的地から現在地の距離(注視点)
	g_Camera.posRDiff = g_Camera.posRDest - g_Camera.posR;

	// 注視点移動
	g_Camera.posR.x += g_Camera.posRDiff.x * 0.9f;
	g_Camera.posR.y += g_Camera.posRDiff.y * 0.9f;
	g_Camera.posR.z += g_Camera.posRDiff.z * 0.9f;

	//-------------------- 視点移動 --------------------//
	// 目的の視点
	g_Camera.posVDest.x =
		g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
	g_Camera.posVDest.y =
		g_Camera.posR.y + 75.0f;
	g_Camera.posVDest.z =
		g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fLength;

	// 目的地から現在地の距離(注視点)
	g_Camera.posVDiff =
		g_Camera.posVDest - g_Camera.posV;

	// 注視点に慣性
	g_Camera.posV.x += g_Camera.posVDiff.x * 0.1f;
	g_Camera.posV.y += g_Camera.posVDiff.y * 0.1f;
	g_Camera.posV.z += g_Camera.posVDiff.z * 0.1f;
}

// ------------------------------------------------------------------------------------------
// クリアのカメラ移動処理
// ------------------------------------------------------------------------------------------
void Update_Clear_MoveCamera(void)
{
	// カメラの視点回転
	g_Camera.rot.y -= 0.01f;

	// 視点更新
	g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
	g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
}

// ------------------------------------------------------------------------------------------
// ゲームオーバーのカメラ移動処理
// ------------------------------------------------------------------------------------------
void Update_GameOver_MoveCamera(void)
{
	// カメラの視点回転
	g_Camera.rot.y -= 0.01f;

	// 視点更新
	g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
	g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
}

// ------------------------------------------------------------------------------------------
// ランキングのカメラ移動処理
// ------------------------------------------------------------------------------------------
void Update_Rank_MoveCamera(void)
{
	// カメラの視点回転
	g_Camera.rot.y -= 0.01f;

	// 視点更新
	g_Camera.posV.x = g_Camera.posR.x + sinf(g_Camera.rot.y) * g_Camera.fLength;
	g_Camera.posV.z = g_Camera.posR.z + cosf(g_Camera.rot.y) * g_Camera.fLength;
}

// ------------------------------------------------------------------------------------------
// カメラ情報処理
// ------------------------------------------------------------------------------------------
CAMERA *GetCamera(void)
{
	return &g_Camera;
}