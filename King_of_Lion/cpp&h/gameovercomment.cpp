// ------------------------------------------------------------------------------------------
//
// ゲームオーバーコメント処理の説明[gameovercomment.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------

#include "gameovercomment.h"
#include "input.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"

// ------------------------------------------------------------------------------------------
//
// 列挙型
//
// ------------------------------------------------------------------------------------------
// ---------タイプ---------- //
typedef enum
{
	GAMEOVERCOMMENTTYPE_GAMEOVER = 0,
	GAMEOVERCOMMENTTYPE_MAX
} GAMEOVERCOMMENTTYPE;

// ------------------------------------------------------------------------------------------
//
// 構造体
//
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR2 pos;	// 位置
	D3DXVECTOR2 size;	// サイズ
} GAMEOVERCOM;

// ------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ------------------------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 g_pTextureGameoverComment[GAMEOVERCOMMENTTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameoverComment = NULL;					// テクスチャへのポインタ
GAMEOVERCOM g_GameoverCom[GAMEOVERCOMMENTTYPE_MAX];							// クリアこめ変数

// テクスチャーファイル名定義
const char *g_paGameoverTexFile[GAMEOVERCOMMENTTYPE_MAX] =
{
	{ "data/TEXTURE/GAMEOVER/Game_Over.png" },
};

// ------------------------------------------------------------------------------------------
//
// プレイヤー初期化処理
//
// ------------------------------------------------------------------------------------------

void InitGameoverComment(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D *pVtx;	// 頂点情報へのポイント

	// テクスチャの読み込み
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * GAMEOVERCOMMENTTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameoverComment,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffGameoverComment->Lock(0, 0, (void **)&pVtx, 0);

	// クリアコメントカウント
	for (int nCntCom = 0; nCntCom < GAMEOVERCOMMENTTYPE_MAX; nCntCom++)
	{
		// テクスチャー作成
		D3DXCreateTextureFromFile(
			pDevice,
			g_paGameoverTexFile[nCntCom],
			&g_pTextureGameoverComment[nCntCom]);

		// クリアのコメント
		if (nCntCom == GAMEOVERCOMMENTTYPE_GAMEOVER)
		{
			g_GameoverCom[nCntCom].pos = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
			g_GameoverCom[nCntCom].size = D3DXVECTOR2(400.0f, 250.0f);
		}

		// 頂点の設定
		pVtx[0].pos = D3DXVECTOR3(
			g_GameoverCom[nCntCom].pos.x - g_GameoverCom[nCntCom].size.x * 0.5f,
			g_GameoverCom[nCntCom].pos.y - g_GameoverCom[nCntCom].size.y * 0.5f,
			0.0f);
		pVtx[1].pos = D3DXVECTOR3(
			g_GameoverCom[nCntCom].pos.x + g_GameoverCom[nCntCom].size.x * 0.5f,
			g_GameoverCom[nCntCom].pos.y - g_GameoverCom[nCntCom].size.y * 0.5f,
			0.0f);
		pVtx[2].pos = D3DXVECTOR3(
			g_GameoverCom[nCntCom].pos.x - g_GameoverCom[nCntCom].size.x * 0.5f,
			g_GameoverCom[nCntCom].pos.y + g_GameoverCom[nCntCom].size.y * 0.5f,
			0.0f);
		pVtx[3].pos = D3DXVECTOR3(
			g_GameoverCom[nCntCom].pos.x + g_GameoverCom[nCntCom].size.x * 0.5f,
			g_GameoverCom[nCntCom].pos.y + g_GameoverCom[nCntCom].size.y * 0.5f,
			0.0f);

		// 頂点作成
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// カラー設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += NUM_VERTEX;
	}

	// アンロック
	g_pVtxBuffGameoverComment->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// プレイヤー終了処理
//
// ------------------------------------------------------------------------------------------

void UninitGameoverComment(void)
{
	// テクスチャの開放
	for (int nCntGameoverComment = 0; nCntGameoverComment < GAMEOVERCOMMENTTYPE_MAX; nCntGameoverComment++)
	{
		if (g_pTextureGameoverComment[nCntGameoverComment] != NULL)
		{
			g_pTextureGameoverComment[nCntGameoverComment]->Release();
			g_pTextureGameoverComment[nCntGameoverComment] = NULL;
		}
	}
	// 頂点バッファの開放
	if (g_pVtxBuffGameoverComment != NULL)
	{
		g_pVtxBuffGameoverComment->Release();
		g_pVtxBuffGameoverComment = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// プレイヤー更新処理
//
// ------------------------------------------------------------------------------------------

void UpdateGameoverComment(void)
{
}

// ------------------------------------------------------------------------------------------
//
// プレイヤー描画処理
//
// ------------------------------------------------------------------------------------------

void DrawGameoverComment(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffGameoverComment, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	for (int nCntGameoverComment = 0; nCntGameoverComment < GAMEOVERCOMMENTTYPE_MAX; nCntGameoverComment++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureGameoverComment[nCntGameoverComment]);

		// プレイヤー描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntGameoverComment * 4, 2);
	}
}