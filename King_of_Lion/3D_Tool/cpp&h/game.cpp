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
#include "materials.h"
#include "shadow.h"
#include "wall.h"
#include "floor.h"
#include "giraffe.h"
#include "fade.h"
#include "arrangement.h"
#include "Billboard.h"
#include "seleob.h"
#include "debugproc.h"

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
void Update_ThirdGame(void);
#ifdef _DEBUG
void Update_Debuggame(void);
#endif // _DEBUG
void SaveGame(void);			// ゲームセーブ関数
void LoadGame(void);			// ゲームロード関数

// ------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ------------------------------------------------------------------------------------------
GAMESTATE g_GameState = GAMESTATE_3D;		// 状態
GAMESELECT g_GameSelect = GAMESELECT_FLOOR;	// 選択状態
int g_nCntGameState = 0;					// カウント
int g_GameStage = GAMESTAGE_1;	// ステージ情報

#ifdef _DEBUG
int					g_nNumberDebug = 4;		// デバッグナンバー
#endif

// ------------------------------------------------------------------------------------------
//
// プレイヤー初期化処理
//
// ------------------------------------------------------------------------------------------

void InitGame(void)
{
	// game初期化
	g_GameState = GAMESTATE_3D;			// 状態設定
	g_GameSelect = GAMESELECT_FLOOR;	// 選択状態
	g_nCntGameState = 0;			// カウント

	// カメラの初期化処理
	InitCamera();
	// ライトの初期化処理
	InitLight();
	// 配置の初期化処理
	InitArrangement();
	// ビルボードの初期化処理
	InitBillboard();
	// ポリゴンの初期化処理
	InitFloor();
	// 壁の初期化処理
	InitWall();
	// 影の初期化処理
	InitShadow();
	// マテリアルの初期化処理
	InitMaterials();
	// キリンの初期化処理
	InitGiraffe();
	// 選択オブの初期化処理
	InitSeleob();
}

// ------------------------------------------------------------------------------------------
//
// プレイヤー終了処理
//
// ------------------------------------------------------------------------------------------

void UninitGame(void)
{
	// カメラの終了処理
	UninitCamera();
	// ライトの終了処理
	UninitLight();
	// 配置の終了処理
	UninitArrangement();
	// ビルボードの終了処理
	UninitBillboard();
	// 3Dポリゴンの終了処理
	UninitFloor();
	// 壁の終了処理
	UninitWall();
	// 影の終了処理
	UninitShadow();
	// マテリアルの終了処理
	UninitMaterials();
	// キリンの終了処理
	UninitGiraffe();
	// 選択オブの終了処理
	UninitSeleob();
}

// ------------------------------------------------------------------------------------------
//
// プレイヤー更新処理
//
// ------------------------------------------------------------------------------------------

void UpdateGame(void)
{
#ifdef _DEBUG
	// デバッグ表示
	Update_Debuggame();
#endif // _DEBUG

	// カメラの更新処理
	UpdateCamera();
	// ライトの更新処理
	UpdateLight();
	// 配置の更新処理
	UpdateArrangement();
	// 3Dツール
	Update_ThirdGame();
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

	// 床の描画処理
	DrawFloor();

	// 壁の描画処理
	DrawWall();

	// マテリアルの描画処理
	DrawMaterials();

	// ビルボードの描画処理
	DrawBillboard();

	// キリンの描画処理
	DrawGiraffe();

	// 選択オブの描画処理
	DrawSeleob();
}

// ------------------------------------------------------------------------------------------
// 3DゲームUpdate処理
// ------------------------------------------------------------------------------------------
void Update_ThirdGame(void)
{
	// セーブ
	if (GetKeyboardSpeacePress(DIK_RETURN))
	{
		// 保存
		SaveGame();
	}
	// 配置するもの //
	// 床
	if (GetKeyboardSpeacePress(DIK_1))
	{
		// 非表示状態へ
		NotDisp_Game(GAMESELECT_FLOOR);
	}

	// 素材
	else if (GetKeyboardSpeacePress(DIK_2))
	{
		// 非表示状態へ
		NotDisp_Game(GAMESELECT_MATERIALS);
	}

	// キリン
	else if (GetKeyboardSpeacePress(DIK_3))
	{
		// 非表示状態へ
		NotDisp_Game(GAMESELECT_GIRAFFE);
	}

	// 壁
	else if (GetKeyboardSpeacePress(DIK_4))
	{
		// 非表示状態へ
		NotDisp_Game(GAMESELECT_WALL);
	}

	// ビルボード
	else if (GetKeyboardSpeacePress(DIK_5))
	{
		// 非表示状態へ
		NotDisp_Game(GAMESELECT_BILLBOARD);
	}

	// 何も表示させない
	else if (GetKeyboardSpeacePress(DIK_9))
	{
		// 非表示状態へ
		NotDisp_Game(GAMESELECT_SELECT);
	}

	// →クリック
	if (GetKeyboardSpeacePress(DIK_RIGHT))
	{
		// ゲームのセーブ
		SaveGame();
		// ステージカウントアップ
		g_GameStage++;

		// 限度を超えたら
		if (g_GameStage >= GAMESTAGE_MAX)
		{
			g_GameStage = GAMESTAGE_1;
		}

		// ゲームのロード
		LoadGame();
	}

	// ←クリック
	else if (GetKeyboardSpeacePress(DIK_LEFT))
	{
		// ゲームのセーブ
		SaveGame();

		// ステージカウントアップ
		g_GameStage--;

		// 限度を超えたら
		if (g_GameStage < 0)
		{
			g_GameStage = GAMESTAGE_5;
		}

		// ゲームのロード
		LoadGame();
	}
	switch (g_GameSelect)
	{
	case GAMESELECT_FLOOR:
		// ポリゴンの更新処理
		UpdateFloor();
		break;

	case GAMESELECT_MATERIALS:
		// マテリアルの更新処理
		UpdateMaterials();
		break;

	case GAMESELECT_GIRAFFE:
		// キリンの更新処理
		UpdateGiraffe();
		break;

	case GAMESELECT_WALL:
		// 壁の更新処理
		UpdateWall();
		break;

	case GAMESELECT_BILLBOARD:
		// 壁の更新処理
		UpdateBillboard();
		break;
	case GAMESELECT_SELECT:
		// 選択オブジェクトの更新処理
		UpdateSeleob();
		break;
	}
}

// ------------------------------------------------------------------------------------------
// 保存処理
// ------------------------------------------------------------------------------------------
void SaveGame(void)
{
	// 素材データの保存
	SaveMaterials();

	// 床データの保存
	SaveFloor();

	// キリンデータの保存
	SaveGiraffe();

	// 壁の保存
	SaveWall();

	// ビルボードの保存
	SaveBillboard();
}

// ------------------------------------------------------------------------------------------
// ロード処理関数
// ------------------------------------------------------------------------------------------
void LoadGame(void)
{
	// 素材データの保存
	LoadMaterials();

	// 床データの保存
	LoadFloor();

	// キリンデータの保存
	LoadGiraffe();

	// 壁の保存
	LoadWall();

	// ビルボードの保存
	LoadBillboard();
}

// ------------------------------------------------------------------------------------------
// 非表示状態へ処理
// ------------------------------------------------------------------------------------------
void NotDisp_Game(GAMESELECT select)
{
	// 素材を非表示状態へ
	NotDisp_Materials();

	// キリンを非表示状態へ
	NotDispGiraffe();

	// 床を非表示状態へ
	NotDispFLoor();

	// 壁を非表示状態へ
	NotDispWall();

	// ビルボードを非表示状態へ
	NotDispBillboard();

	g_GameSelect = select;
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

// ------------------------------------------------------------------------------------------
//
// ゲーム選択の設定
//
// ------------------------------------------------------------------------------------------

void SetGameSelect(GAMESELECT select)
{
	g_GameSelect = select;
}

// ------------------------------------------------------------------------------------------
//
// ゲーム選択の取得
//
// ------------------------------------------------------------------------------------------

GAMESELECT GetGameSelect(void)
{
	return g_GameSelect;
}

// ------------------------------------------------------------------------------------------
//
// ステージ情報の取得
//
// ------------------------------------------------------------------------------------------
int GetGameStage(void)
{
	return g_GameStage;
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------
// デバッグ表示
// ------------------------------------------------------------------------------------------
void Update_Debuggame(void)
{
	// 変数宣言
	// 情報取得
	ARRANGEMENT *pArrangement = GetArrangement();	// 配置				// モデル取得
	CAMERA		*pCamera = GetCamera();				// カメラ

	// -------------------基本情報-------------------- //
	PrintDebug("/*---------<基本操作>----------*/\n");
	PrintDebug("[マウスの右クリック]:カメラ回転\n");
	PrintDebug("[マウスのホイールクリック]:カメラ移動\n");
	PrintDebug("[マウスの左クリック]:選択・配置\n");
	PrintDebug("[A][D]:カメラX軸移動 / [W][S]:カメラZ軸移動\n");
	PrintDebug("[F][R]:カメラY軸移動\n");
	PrintDebug("[E][C]:オブジェクトのy座標変更\n");
	PrintDebug("[←][→]:セーブ場所切り替え\n");
	PrintDebug("[1]:床	[2]:モデル\n");
	PrintDebug("[3]:敵	[4]:壁\n");
	PrintDebug("[9]:選択モード\n");
	PrintDebug("/*------------------------------*/\n");
	PrintDebug("オートセーブ\n", g_GameStage);
	PrintDebug("現在のセーブ場所[%d/5]\n",g_GameStage);
	PrintDebug("現在のモード[%d]\n",g_GameSelect);
	PrintDebug("選択位置情報->(%0.3f,%0.3f,%0.3f)\n",
		pArrangement->pos.x,
		pArrangement->pos.y,
		pArrangement->pos.z
	);
	PrintDebug("選択回転情報->(%0.3f,%0.3f,%0.3f)\n",
		pArrangement->rot.x,
		pArrangement->rot.y,
		pArrangement->rot.z
	);
	// -------------------座標情報-------------------- //

}
#endif // _DEBUG
