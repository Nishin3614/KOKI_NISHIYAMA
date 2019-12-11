// ------------------------------------------------------------------------------------------
//
// ゲーム処理の説明[game.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------

#include "game.h"
#include "input.h"
#include "joypad.h"
#include "mouse.h"
#include "camera.h"
#include "light.h"
#include "polygon.h"
#include "player.h"
#include "materials.h"
#include "shadow.h"
#include "wall.h"
#include "bullet.h"
#include "floor.h"
#include "dome.h"
#include "explosion.h"
#include "effect.h"
#include "giraffe.h"
#include "hemisphere.h"
#include "fade.h"
#include "pouse.h"
#include "pousecoment.h"
#include "arrow.h"
#include "trajectory.h"
#include "buffalo.h"
#include "hpgauge.h"
#include "item.h"
#include "king_rion.h"
#include "UI.h"
#include "time.h"
#include "Billboard.h"
#include "range.h"
#include "gamefade.h"
#include "select.h"
#include "selectcomment.h"
#include "ranking.h"
#include "enemy.h"
#include "score.h"
#include "water.h"
#include "shogira.h"
#include "prediction.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------
#define GAME1 "data/TEXTURE/BG001.png"	// 背景
#define GAME_LEFT_X (0)			// 左
#define GAME_ON_Y (0)			// 上
#define GAME_SIZE_X (1280)		// サイズ（ｘ）
#define GAME_SIZE_Y (720)		// サイズ（ｙ）
#define SET_ENEMY_X (1350.0F)		// ENEMYのXポジ

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
#ifdef _DEBUG
void Draw_Debug_PlayerPos(void);

void Draw_Debug_Camera(void);

void Draw_Debug_ShadowPos(void);

void Draw_Debug_GiraffeInfo(void);

void Draw_Debug_ATAN2F(void);
#endif

// ------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ------------------------------------------------------------------------------------------
GAMESTATE g_GameState = GAMESTATE_NONE;	// 状態
int g_nCntGameState = 0;				// カウント
#ifdef _DEBUG
LPD3DXFONT			g_pGameFont;			// フォントへのポインタ
int					g_nNumberDebug = 0;		// デバッグナンバー
#endif

// ------------------------------------------------------------------------------------------
//
// プレイヤー初期化処理
//
// ------------------------------------------------------------------------------------------

void InitGame(void)
{
#ifdef _DEBUG
	g_pGameFont = GetFont();
#endif
	// game初期化
	g_GameState = GAMESTATE_NORMAL;	// 状態設定
	g_nCntGameState = 0;			// カウント

	// ゲームフェードの初期化処理
	InitGameFade(g_GameState);

	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// 爆発の初期化
	InitExplosion();

	// エフェクト初期化
	InitEffect();

	// 軌跡の初期化処理
	InitTrajectory();

	// 弾の初期化処理
	InitBullet();

	// ビルボードの初期化処理
	InitBillboard();

	// 範囲の初期化処理
	InitRange();

	// ポリゴンの初期化処理
	InitFLOOR();

	// ドームの初期化
	InitDome();

	// 半球の初期化
	InitHemisphere();

	// 壁の初期化処理
	InitWall();

	// 影の初期化処理
	InitShadow();

	// 予測線の初期化処理
	InitPrediction();

	// マテリアルの初期化処理
	InitMaterials();

	// アイテムの初期化処理
	InitItem();

	// キリンの初期化処理
	InitGiraffe();

	// キリンの初期化処理
	InitShogira();

	// バッファローの初期化処理
	InitBuffalo();

	// 百獣の王の初期化処理
	InitKing_Rion();

	// プレイヤーの初期化処理
	InitPlayer();

	// 水の初期化処理
	InitWATER();

	// HPゲージの初期化処理
	InitHpGauge();

	// UIの初期化処理
	InitUi();

	// タイムの初期化処理
	InitTime();

	// 撃破数の初期化処理
	InitScore();

	// ポーズの初期化処理
	InitPouse();

	// ポーズコメントの初期化処理
	InitPouseComent();

	// 選択の初期化
	InitSelect();

	// 選択コメントの初期化
	InitSelectComment();

	// 選択コメントのタイプ
	SceneSelectComment(SELECTCOMMENTSCENE_AREA);

	// 矢印の初期化
	InitArrow();
}

// ------------------------------------------------------------------------------------------
//
// プレイヤー終了処理
//
// ------------------------------------------------------------------------------------------

void UninitGame(void)
{
	// ゲームフェードの終了処理
	UninitGameFade();

	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// 爆発の終了処理
	UninitExplosion();

	// エフェクトの終了処理
	UninitEffect();

	// 弾の終了処理
	UninitBullet();

	// ビルボードの初期化処理
	UninitBillboard();

	// 範囲の終了処理
	UninitRange();

	// 3Dポリゴンの終了処理
	UninitFLOOR();

	// ドームの終了処理
	UninitDome();

	// 半球の終了処理
	UninitHemisphere();

	// 壁の終了処理
	UninitWall();

	// 影の終了処理
	UninitShadow();

	// 予測線の終了処理
	UninitPrediction();

	// マテリアルの終了処理
	UninitMaterials();

	// プレイヤーの終了処理
	UninitPlayer();

	// アイテムの終了処理
	UninitItem();

	// キリンの終了処理
	UninitGiraffe();

	// キリンの終了処理
	UninitShogira();
	
	// バッファローの終了処理
	UninitBuffalo();

	// 百獣の王の終了処理
	UninitKing_Rion();

	// 水の終了処理
	UninitWATER();

	// 軌跡の終了処理
	UninitTrajectory();

	// HPゲージの終了処理
	UninitHpGauge();

	// UIの終了処理
	UninitUi();

	// タイムの終了処理
	UninitTime();

	// 撃破数の終了処理
	UninitScore();

	// 選択の終了処理
	UninitSelect();

	// 選択コメントの終了処理
	UninitSelectComment();

	// ポーズの終了処理
	UninitPouse();

	// ポーズコメントの終了処理
	UninitPouseComent();

	// ポーズ選択の終了処理
	UninitArrow();
}

// ------------------------------------------------------------------------------------------
//
// プレイヤー更新処理
//
// ------------------------------------------------------------------------------------------

void UpdateGame(void)
{
	if (g_GameState == GAMESTATE_NORMAL ||
		g_GameState == GAMESTATE_BOSS || 
		g_GameState == GAMESTATE_CLEAR || 
		g_GameState == GAMESTATE_GAMEOVER)
	{
		// カメラの更新処理
		UpdateCamera();

		// ライトの更新処理
		UpdateLight();

		// 爆発の更新処理
		UpdateExplosion();

		// エフェクトの更新処理
		UpdateEffect();

		// 弾の更新処理
		UpdateBullet();

		// ビルボードの更新処理
		UpdateBillboard();

		// 範囲の更新処理
		UpdateRange();

		// ポリゴンの更新処理
		UpdateFLOOR();

		// ドームの更新処理
		UpdateDome();

		// 半球の更新処理
		UpdateHemisphere();

		// 壁の更新処理
		UpdateWall();

		// 影の更新処理
		UpdateShadow();

		// 予測線の更新処理
		UpdatePrediction();

		// マテリアルの更新処理
		UpdateMaterials();

		// モデルの更新処理
		UpdatePlayer();

		// アイテムの更新処理
		UpdateItem();

		// キリンの更新処理
		UpdateGiraffe();

		// キリンの更新処理
		UpdateShogira();

		// バッファローの更新処理
		UpdateBuffalo();

		// 百獣の王の更新処理
		UpdateKing_Rion();

		// 水の更新処理
		UpdateWATER();

		// 軌跡の更新処理
		UpdateTrajectory();

		// HPゲージの更新処理
		UpdateHpGauge();

		// UIの更新処理
		UpdateUi();

		// タイムの更新処理
		UpdateTime();

		// 撃破数の更新処理
		UpdateScore();

		// ポーズへ
		if (GetKeyboardTrigger(DIK_P) ||
			GetJoyboardTrigger(0, JOYPADKEY_START))
		{
			SetGameState(GAMESTATE_POUSE);
		}
	}

	else if (g_GameState == GAMESTATE_POUSE)
	{
		// ポーズ背景
		UpdatePouse();

		// コメント
		UpdatePouseComent();

		// 選択
		UpdateArrow();

		if (GetKeyboardTrigger(DIK_P) ||
			GetJoyboardTrigger(0, JOYPADKEY_START))
		{
			SetGameState(GAMESTATE_NORMAL);
		}
	}
#ifdef _DEBUG
	// モデルの位置・回転情報
	if (GetKeyboardTrigger(DIK_1))
	{
		g_nNumberDebug = 0;
	}

	// カメラの位置情報
	else if (GetKeyboardTrigger(DIK_2))
	{
		g_nNumberDebug = 1;
	}

	// 影の位置情報
	else if (GetKeyboardTrigger(DIK_3))
	{
		g_nNumberDebug = 2;
	}

	// キリン情報
	else if (GetKeyboardTrigger(DIK_4))
	{
		g_nNumberDebug = 3;
	}

	else if (GetKeyboardTrigger(DIK_5))
	{
		g_nNumberDebug = 4;
	}
#endif

	switch (g_GameState)
	{
	case GAMESTATE_CLEAR:
		g_nCntGameState++;
		if (g_nCntGameState >= 30)
		{
			g_GameState = GAMESTATE_NONE;
			SetFade(MODE_CLEAR);
		}
		break;

	case GAMESTATE_GAMEOVER:
		g_nCntGameState++;
		if (g_nCntGameState >= 30)
		{
			g_GameState = GAMESTATE_NONE;
			SetFade(MODE_GAMEOVER);
		}
		break;

	case GAMESTATE_POUSE:
		break;

	case GAMESTATE_TITLE:
		g_GameState = GAMESTATE_NONE;

		SetFade(MODE_TITLE);
		break;

	case GAMESTATE_SELECT:
		// 選択の更新処理
		UpdateSelect();

		// 選択コメントの更新処理
		UpdateSelectComment();

		// 矢印の更新処理
		UpdateArrow();

		break;
	default:
		break;
	}

	// ゲームフェードの更新処理
	UpdateGameFade();
}

// ------------------------------------------------------------------------------------------
//
// プレイヤー描画処理
//
// ------------------------------------------------------------------------------------------

void DrawGame(void)
{
	// カメラの設定
	SetCamera();

	// ポリゴンの描画処理
	DrawFLOOR();

	// 壁の描画処理
	DrawWall();

	// ドームの描画処理
	DrawDome();

	// 半球の描画処理
	DrawHemisphere();

	// 弾の描画処理
	DrawBullet();

	// ビルボードの描画処理
	DrawBillboard();

	// 範囲の終了処理
	DrawRange();

	// 爆発の描画処理
	DrawExplosion();

	// エフェクトの描画処理
	DrawEffect();

	// 影の描画処理
	DrawShadow();

	// 予測線の描画処理
	DrawPrediction();

	// マテリアルの描画処理
	DrawMaterials();

	// モデルの描画処理
	DrawPlayer();

	// アイテムの描画処理
	DrawItem();

	// キリンの描画処理
	DrawGiraffe();

	// キリンの描画処理
	DrawShogira();

	// バッファローのの描画処理
	DrawBuffalo();

	// 百獣の王の描画処理
	DrawKing_Rion();

	// 軌跡の描画処理
	DrawTrajectory();

	// 水の描画処理
	DrawWATER();

	// HPゲージの描画処理
	DrawHpGauge();

	// UIの描画処理
	DrawUi();
	
	// タイムの描画処理
	DrawTime();

	// 撃破数の描画処理
	DrawScore();

	switch (g_GameState)
	{
	case GAMESTATE_POUSE:
		// ポーズ背景
		DrawPouse();

		// コメント
		DrawPouseComent();

		// 選択
		DrawArrow();

		break;

	case GAMESTATE_SELECT:
		// 選択の描画処理
		DrawSelect();

		// 選択コメントの描画処理
		DrawSelectComment();

		// 矢印の描画処理
		DrawArrow();
		break;

	}

	// ゲームフェードの描画処理
	DrawGameFade();

#ifdef _DEBUG
	// モデル
	if (g_nNumberDebug == 0)
	{
		// モデル情報表示
		Draw_Debug_PlayerPos();
	}

	// カメラの位置情報
	else if (g_nNumberDebug == 1)
	{
		// カメラ情報表示
		Draw_Debug_Camera();
	}

	// 影の位置情報
	else if (g_nNumberDebug == 2)
	{
		// 影情報表示
		Draw_Debug_ShadowPos();
	}

	// キリン情報
	else if (g_nNumberDebug == 3)
	{
		// キリン情報表示
		Draw_Debug_GiraffeInfo();
	}

	else if (g_nNumberDebug == 4)
	{
		// アークタンジェント実験表示
		Draw_Debug_ATAN2F();
	}
#endif
}

// ------------------------------------------------------------------------------------------
//
// ゲームステートの設定
//
// ------------------------------------------------------------------------------------------

void SetGameState(GAMESTATE state)
{
	g_GameState = state;
	g_nCntGameState = 0;

	// ボス状態だったら
	if (g_GameState == GAMESTATE_BOSS)
	{
		// カメラデータの位置情報
		CameraPosition();

		// プレイヤーデータの位置情報
		PlayerPosition();

		// ビルボードの初期化処理
		InitBillboard();

		// 範囲の初期化処理
		InitRange();

		// ドームの初期化処理
		InitDome();

		// ポリゴンの初期化処理
		InitFLOOR();

		// 壁の初期化処理
		InitWall();

		// マテリアルの初期化処理
		InitMaterials();

		// アイテムの初期化処理
		InitItem();

		// キリンの初期化処理
		InitGiraffe();

		// キリンの初期化処理
		InitShogira();

		// バッファローの初期化処理
		InitBuffalo();

		// 百獣の王の初期化処理
		InitKing_Rion();
	}
}

// ------------------------------------------------------------------------------------------
//
// ゲームステートの取得
//
// ------------------------------------------------------------------------------------------

GAMESTATE GetGameStete(void)
{
	return g_GameState;
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------
// モデルのポジション処理
// ------------------------------------------------------------------------------------------
void Draw_Debug_PlayerPos(void)
{
	PLAYER *pPlayer = GetPlayer();	// モデル取得

	RECT rect = { 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT };	// 表示場所・範囲
	char aStr[128];										// 文字格納
	char aAllStr[1024];									// 一括に格納

														//-------------------- コメント --------------------//
														// 変数に文字を当てえる
	sprintf(&aStr[0], "//------------------------------//\n");

	// 一括変数へ
	strcpy(&aAllStr[0], &aStr[0]);


	// 変数に文字を当てえる
	sprintf(&aStr[0], "// [Q][A]:X軸移動\t[Z]:X軸リセット//\n");

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "// [W][S]:Y軸移動\t[X]:Y軸リセット//\n");

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "// [E][D]:Z軸移動\t[C]:Z軸リセット//\n");

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "//------------------------------//\n");

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "<現在のポジション>\n");

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);
	//-------------------- コメント終了 --------------------//


	//-------------------- 回転表示 --------------------//
	// カウントデバッグ
	for (int nCntDebug = 0; nCntDebug < PLAYERTYPE_MAX; nCntDebug++)
	{
		// 変数に文字を当てる
		sprintf(&aStr[0], "[%d]:(%0.3f,%0.3f,%0.3f)\n",
			nCntDebug,
			pPlayer->model[nCntDebug].rot.x,
			pPlayer->model[nCntDebug].rot.y,
			pPlayer->model[nCntDebug].rot.z);

		// 一括変数へ
		strcat(&aAllStr[0], &aStr[0]);
	}
	//-------------------- 回転表示終了 --------------------//

	// テキスト描画
	g_pGameFont->DrawText(NULL, &aAllStr[0], -10, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

// ------------------------------------------------------------------------------------------
// カメラのポジション処理
// ------------------------------------------------------------------------------------------
void Draw_Debug_Camera(void)
{
	CAMERA *pCamera;
	RECT rect = { 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT };	// 表示場所・範囲
	char aStr[64];										// 文字格納
	char aAllStr[1024];									// 一括格納

	// カメラ情報取得
	pCamera = GetCamera();

	//-------------------- コメント --------------------//
	// 変数に文字を当てえる
	sprintf(&aStr[0], "<現在のポジション>\n");

	// 一括変数へ
	strcpy(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "[posV](%0.3f,%0.3f,%0.3f)\n", pCamera->posV.x, pCamera->posV.y, pCamera->posV.z);

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "[posR](%0.3f,%0.3f,%0.3f)\n", pCamera->posR.x, pCamera->posR.y, pCamera->posR.z);

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "[posU](%0.3f,%0.3f,%0.3f)\n", pCamera->posU.x, pCamera->posU.y, pCamera->posU.z);

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	//-------------------- コメント --------------------//
	// 変数に文字を当てえる
	sprintf(&aStr[0], "<現在の回転方向>\n");

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "[rot](%0.3f,%0.3f,%0.3f)\n", pCamera->rot.x, pCamera->rot.y, pCamera->rot.z);

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// テキスト描画
	g_pGameFont->DrawText(NULL, &aAllStr[0], -10, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

// ------------------------------------------------------------------------------------------
// 影のポジション処理
// ------------------------------------------------------------------------------------------
void Draw_Debug_ShadowPos(void)
{
	SHADOW *pShadow;									// 影の情報
	RECT rect = { 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT };	// 表示場所・範囲
	char aStr[64];										// 文字格納
	char aAllStr[256];									// 一括格納

														// カメラ情報取得
	pShadow = GetShadow();

	//-------------------- コメント --------------------//
	// 変数に文字を当てえる
	sprintf(&aStr[0], "<現在のポジション>\n");

	// 一括変数へ
	strcpy(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "[posV](%0.3f,%0.3f,%0.3f)\n", pShadow->pos.x, pShadow->pos.y, pShadow->pos.z);

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// テキスト描画
	g_pGameFont->DrawText(NULL, &aAllStr[0], -10, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

// ------------------------------------------------------------------------------------------
// 影のポジション処理
// ------------------------------------------------------------------------------------------
void Draw_Debug_GiraffeInfo(void)
{
	GIRAFFE *pGiraffe;
	RECT rect = { 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT };	// 表示場所・範囲
	char aStr[64];										// 文字格納
	char aAllStr[1024];									// 一括格納

	// カメラ情報取得
	pGiraffe = GetGiraffe();

	//-------------------- コメント --------------------//
	// 変数に文字を当てえる
	sprintf(&aStr[0], "<現在のポジション>\n");

	// 一括変数へ
	strcpy(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "[pos](%0.3f,%0.3f,%0.3f)\n", pGiraffe->pos.x, pGiraffe->pos.y, pGiraffe->pos.z);

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "<現在の回転方向>\n");

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "[rot](%0.3f,%0.3f,%0.3f)\n", pGiraffe->rot.x, pGiraffe->rot.y, pGiraffe->rot.z);

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// テキスト描画
	g_pGameFont->DrawText(NULL, &aAllStr[0], -10, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

// ------------------------------------------------------------------------------------------
// アークタンジェントの実験処理
// ------------------------------------------------------------------------------------------
void Draw_Debug_ATAN2F(void)
{
	KING_RION * pKing_Rion = GetKing_Rion();			// 百獣の王の情報取得
	PLAYER * pPlayer = GetPlayer();						// プレイヤー情報取得
	D3DXVECTOR3 diffpos;								// 百獣の王とプレイヤーまでの距離
	RECT rect = { 0, 50, SCREEN_WIDTH, SCREEN_HEIGHT };	// 表示場所・範囲
	char aStr[64];										// 文字格納
	char aAllStr[1024];									// 一括格納
	float	fAngle;										// 百獣の王から見てプレイヤーがいる角度
	
	// 計算
	diffpos = pPlayer->pos - pKing_Rion->pos;	// プレイヤーと百獣の王との距離
	fAngle = (atan2f(diffpos.x, diffpos.z));	// 百獣の王からプレイヤーの角度

	//-------------------- コメント --------------------//
	// 変数に文字を当てえる
	sprintf(&aStr[0], "<現在のポジション>\n");

	// 一括変数へ
	strcpy(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "[百獣の王のpos](%0.3f,%0.3f,%0.3f)\n", 
		pKing_Rion->pos.x, pKing_Rion->pos.y, pKing_Rion->pos.z);

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "[主人公のpos](%0.3f,%0.3f,%0.3f)\n",
		pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "[距離](%0.3f,%0.3f,%0.3f)\n", diffpos.x, diffpos.y, diffpos.z);

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "[百獣の王のrot](%0.3f,%0.3f,%0.3f)\n",
		pKing_Rion->rotLast.x, pKing_Rion->rotLast.y, pKing_Rion->rotLast.z);

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// 変数に文字を当てえる
	sprintf(&aStr[0], "[atan2f](%0.3f)\n", fAngle);

	// 一括変数へ
	strcat(&aAllStr[0], &aStr[0]);

	// テキスト描画
	g_pGameFont->DrawText(NULL, &aAllStr[0], -10, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
#endif // _DEBUG