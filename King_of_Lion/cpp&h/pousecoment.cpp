// ------------------------------------------------------------------------------------------
//
// ポーズコメント処理の説明[pousecoment.cpp]
// Author : Koki Nishiyama 
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// インクルードファイル
// ------------------------------------------------------------------------------------------
#include "title.h"

#include "input.h"

#include "joypad.h"

#include "fade.h"

#include "sound.h"

#include "pousecoment.h"

#include "arrow.h"

#include "game.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define POUSEUI_SIZE_X (300)					// ポーズコメント名サイズ（ｘ）
#define POUSEUI_SIZE_Y (300)					// ポーズコメント名サイズ（ｙ）

#define POUSECOMMENTUI_SIZE_X (500)					// ポーズコメント名サイズ（ｘ）
#define POUSECOMMENTUI_SIZE_Y (500)					// ポーズコメント名サイズ（ｙ）
#define UI_SIZE_X (200)							// エンター名サイズ（ｘ）
#define UI_SIZE_Y (200)							// エンター名サイズ（ｙ）

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void Update_PousePouseComment(void);
void Update_ColChangePouseComment(void);
void Update_VertexPouseComment(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9 g_pTexturePouseComent[POUSECOMMENT_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPouseComent = NULL;			// テクスチャへのポインタ
POUSECOMENT g_aPouseComent[POUSECOMMENT_MAX];					// ポーズコメント
int g_nCntPcoment = 0;

// ------------------------------------------------------------------------------------------
// ポーズコメント初期化処理
// ------------------------------------------------------------------------------------------
void InitPouseComent(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	VERTEX_2D *pVtx;			// 頂点情報の番地
	int nCntPouseComent;

	// テクスチャーファイル名定義
	char *paTextureFile[POUSECOMMENT_MAX] =
	{
		{ "data/TEXTURE/POUSE/00_pousecomment.png" },
		{ "data/TEXTURE/POUSE/01_GoGame.png" },
		{ "data/TEXTURE/POUSE/02_GoTitle.png" },
		{ "data/TEXTURE/POUSE/03_ReGame.png" }
	};

	// 初期化
	for (nCntPouseComent = 0; nCntPouseComent < POUSECOMMENT_MAX; nCntPouseComent++)
	{
		// ポーズコメントの位置
		if (nCntPouseComent == POUSECOMMENT_POUSE)
		{
			g_aPouseComent[nCntPouseComent].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 200.0f, 0.0f);
		}

		// ゲームコメントの位置
		else if(nCntPouseComent == POUSECOMMENT_GAME)
		{
			g_aPouseComent[nCntPouseComent].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.25f, 500.0f, 0.0f);
		}
		// タイトルコメントの位置
		else if (nCntPouseComent == POUSECOMMENT_TITLE)
		{
			g_aPouseComent[nCntPouseComent].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 500.0f, 0.0f);
		}

		// やり直すコメントの位置
		else if (nCntPouseComent == POUSECOMMENT_RESTART)
		{
			g_aPouseComent[nCntPouseComent].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.75f, 500.0f, 0.0f);
		}

		g_aPouseComent[nCntPouseComent].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPouseComent[nCntPouseComent].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aPouseComent[nCntPouseComent].nType = nCntPouseComent;
		g_aPouseComent[nCntPouseComent].nCntCol = 0;
		g_aPouseComent[nCntPouseComent].bUse = false;

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			paTextureFile[nCntPouseComent],
			&g_pTexturePouseComent[nCntPouseComent]);
	}

	// ポーズコメントカウントの初期化
	g_nCntPcoment = POUSECOMMENT_GAME;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * POUSECOMMENT_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffPouseComent,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffPouseComent->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntPouseComent = 0; nCntPouseComent < POUSECOMMENT_MAX; nCntPouseComent++)
	{
		if (nCntPouseComent == POUSECOMMENT_POUSE)
		{
			// 頂点の設定
			pVtx[0].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x - POUSECOMMENTUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y - POUSECOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// 一つ目の頂点

			pVtx[1].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x + POUSECOMMENTUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y - POUSECOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// 二つ目の頂点

			pVtx[2].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x - POUSECOMMENTUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y + POUSECOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// 三つ目の頂点

			pVtx[3].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x + POUSECOMMENTUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y + POUSECOMMENTUI_SIZE_Y * 0.5f,
				0.0f);	// 四つ目の頂点
		}
		
		else
		{
			// 頂点の設定
			pVtx[0].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x - POUSEUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y - POUSEUI_SIZE_Y * 0.5f,
				0.0f);	// 一つ目の頂点

			pVtx[1].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x + POUSEUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y - POUSEUI_SIZE_Y * 0.5f,
				0.0f);	// 二つ目の頂点

			pVtx[2].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x - POUSEUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y + POUSEUI_SIZE_Y * 0.5f,
				0.0f);	// 三つ目の頂点

			pVtx[3].pos = D3DXVECTOR3(
				g_aPouseComent[nCntPouseComent].pos.x + POUSEUI_SIZE_X * 0.5f,
				g_aPouseComent[nCntPouseComent].pos.y + POUSEUI_SIZE_Y * 0.5f,
				0.0f);	// 四つ目の頂点
		}

		// 頂点作成
		pVtx[0].rhw = 1.0f;									// 一つ目の頂点作成
		pVtx[1].rhw = 1.0f;									// 二つ目の頂点作成
		pVtx[2].rhw = 1.0f;									// 三つ目の頂点作成
		pVtx[3].rhw = 1.0f;									// 四つ目の頂点作成

		// カラー設定
		pVtx[0].col = g_aPouseComent[nCntPouseComent].col;	// 一つ目の色設定
		pVtx[1].col = g_aPouseComent[nCntPouseComent].col;	// 二つ目の色設定
		pVtx[2].col = g_aPouseComent[nCntPouseComent].col;	// 三つ目の色設定
		pVtx[3].col = g_aPouseComent[nCntPouseComent].col;	// 四つ目の色設定

		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				// 一つ目のテクスチャ
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				// 二つ目のテクスチャ
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				// 三つ目のテクスチャ
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				// 四つ目のテクスチャ

		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffPouseComent->Unlock();
}

// ------------------------------------------------------------------------------------------
// ポーズコメント終了処理
// ------------------------------------------------------------------------------------------
void UninitPouseComent(void)
{
	int nCntPouseComent;

	// テクスチャの開放
	for (nCntPouseComent = 0; nCntPouseComent < POUSECOMMENT_MAX; nCntPouseComent++)
	{
		if (g_pTexturePouseComent[nCntPouseComent] != NULL)
		{
			g_pTexturePouseComent[nCntPouseComent]->Release();
			g_pTexturePouseComent[nCntPouseComent] = NULL;
		}
	}
	// 頂点バッファの開放
	if (g_pVtxBuffPouseComent != NULL)
	{
		g_pVtxBuffPouseComent->Release();
		g_pVtxBuffPouseComent = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// ポーズコメント更新処理
// ------------------------------------------------------------------------------------------

void UpdatePouseComent(void)
{
	// 選択処理
	Update_PousePouseComment();

	// カラーの変化処理
	Update_ColChangePouseComment();

	// 頂点情報処理
	Update_VertexPouseComment();
}

// ------------------------------------------------------------------------------------------
// 選択処理
// ------------------------------------------------------------------------------------------
void Update_PousePouseComment(void)
{
	// スティックでの操作
	static int nCntStickLeft = 0;
	static int nCntStickRight = 0;

	// ジョイパッド用 //
	int nValueH, nValueV;	// ゲームパッドのスティック情報の取得用

	// ゲームパッドのスティック情報を取得
	GetGamepadStickLeft(0, &nValueH, &nValueV);

	// ゲームパッド移動
	if (nValueH != 0 || nValueV != 0)
	{
		// 速度の計算
		if (nValueH < 0)
		{
			nCntStickLeft++;
		}
		// 速度の計算
		else if (nValueH > 0)
		{
			nCntStickRight++;
		}
	}
	else
	{
		nCntStickLeft = 0;
		nCntStickRight = 0;
	}

	// カウント増減
	if (GetKeyboardTrigger(DIK_D) ||
		GetJoyboardTrigger(0, JOYPADKEY_RIGHT) ||
		nCntStickRight == 1)
	{
		// 決定音
		PlaySound(SOUND_LABEL_SE_ENTER);

		// 前回のα情報を通常へ
		g_aPouseComent[g_nCntPcoment].col.a = 1.0f;

		// 前回のカラーカウントを通常へ
		g_aPouseComent[g_nCntPcoment].nCntCol = 0;

		// 頂点情報処理
		Update_VertexPouseComment();

		// カウントアップ
		g_nCntPcoment++;

		// スティックのカウント初期化
		nCntStickLeft = 0;
		nCntStickRight = 0;

		// カウントが限界点を超えたら
		if (g_nCntPcoment >= POUSECOMMENT_MAX)
		{
			g_nCntPcoment = POUSECOMMENT_GAME;
		}
	}
	else if (GetKeyboardTrigger(DIK_A) ||
		GetJoyboardTrigger(0, JOYPADKEY_LEFT) ||
		nCntStickLeft == 1)
	{
		// 決定音
		PlaySound(SOUND_LABEL_SE_ENTER);

		// 前回のα情報を通常へ
		g_aPouseComent[g_nCntPcoment].col.a = 1.0f;

		// 前回のカラーカウントを通常へ
		g_aPouseComent[g_nCntPcoment].nCntCol = 0;

		// 頂点情報処理
		Update_VertexPouseComment();

		// カウントダウン
		g_nCntPcoment--;

		// カウントが限界点を下回ったら
		if (g_nCntPcoment < POUSECOMMENT_GAME)
		{
			g_nCntPcoment = POUSECOMMENT_MAX - 1;
		}
	}

	if (GetKeyboardTrigger(DIK_RETURN) ||
		GetKeyboardTrigger(DIK_J) ||
		GetJoyboardTrigger(0, JOYPADKEY_B) ||
		GetJoyboardTrigger(0, JOYPADKEY_A))
	{
		// ゲーム選択時
		if (g_nCntPcoment == POUSECOMMENT_GAME)
		{
			// 決定音
			PlaySound(SOUND_LABEL_SE_ENTER);

			SetGameState(GAMESTATE_NORMAL);
		}

		// タイトル選択時
		else if (g_nCntPcoment == POUSECOMMENT_TITLE)
		{
			// 決定音
			PlaySound(SOUND_LABEL_SE_ENTER);

			SetGameState(GAMESTATE_TITLE);
		}

		// やり直し選択時
		else if (g_nCntPcoment == POUSECOMMENT_RESTART)
		{
			// 決定音
			PlaySound(SOUND_LABEL_SE_ENTER);

			SetFade(MODE_GAME);
		}
	}

	// 矢印の位置情報更新
	ArrowPosition(g_aPouseComent[g_nCntPcoment].pos);
}

// ------------------------------------------------------------------------------------------
// カラー変化処理
// ------------------------------------------------------------------------------------------
void Update_ColChangePouseComment(void)
{
	// カウントアップ
	g_aPouseComent[g_nCntPcoment].nCntCol++;

	if (g_aPouseComent[g_nCntPcoment].nCntCol % 20 == 0)
	{
		// カラー設定
		g_aPouseComent[g_nCntPcoment].col.a = 1.0f;	// 一つ目の色設定
	}

	else if (g_aPouseComent[g_nCntPcoment].nCntCol % 10 == 0)
	{
		// カラー設定
		g_aPouseComent[g_nCntPcoment].col.a = 0.0f;	// 一つ目の色設定
	}
}

// ------------------------------------------------------------------------------------------
// 頂点情報処理
// ------------------------------------------------------------------------------------------
void Update_VertexPouseComment(void)
{
	VERTEX_2D *pVtx;			// 頂点情報

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffPouseComent->Lock(0, 0, (void **)&pVtx, 0);

	// ポイント合わせ
	pVtx += 4 * g_nCntPcoment;

	// カラー設定
	pVtx[0].col = g_aPouseComent[g_nCntPcoment].col;	// 一つ目の色設定
	pVtx[1].col = g_aPouseComent[g_nCntPcoment].col;	// 二つ目の色設定
	pVtx[2].col = g_aPouseComent[g_nCntPcoment].col;	// 三つ目の色設定
	pVtx[3].col = g_aPouseComent[g_nCntPcoment].col;	// 四つ目の色設定

	// アンロック
	g_pVtxBuffPouseComent->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// ポーズコメント描画処理
//
// ------------------------------------------------------------------------------------------
void DrawPouseComent(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	int nCntPouseComent;

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffPouseComent, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	// テクスチャの設定
	for (nCntPouseComent = 0; nCntPouseComent < POUSECOMMENT_MAX; nCntPouseComent++)
	{
			pDevice->SetTexture(0, g_pTexturePouseComent[nCntPouseComent]);

			// ポーズコメント描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPouseComent * 4, 2);
	}
}

// ------------------------------------------------------------------------------------------
// ポーズコメント取得処理
// ------------------------------------------------------------------------------------------
int GetPouseCommentPouse(void)
{
	return g_nCntPcoment;
}

// ------------------------------------------------------------------------------------------
//
// ポーズコメント取得処理
//
// ------------------------------------------------------------------------------------------
POUSECOMENT *GetPouseComent(void)
{
	return &g_aPouseComent[0];
}