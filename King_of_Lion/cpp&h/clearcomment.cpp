// ------------------------------------------------------------------------------------------
//
// クリアコメント画面処理の説明[clearcomment.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------
#include "clearcomment.h"

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
	CLEARCOMMENTTYPE_CLEAR = 0,
	CLEARCOMMENTTYPE_THANK,
	CLEARCOMMENTTYPE_MAX
} CLEARCOMMENTTYPE;

// ------------------------------------------------------------------------------------------
//
// 構造体
//
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR2 pos;	// 位置
	D3DXVECTOR2 size;	// サイズ
} CLEARCOM;

// ------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ------------------------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 g_pTextureClearComment[CLEARCOMMENTTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffClearComment = NULL;					// バッファへのポインタ
CLEARCOM g_ClearCom[CLEARCOMMENTTYPE_MAX];								// クリアこめ変数

// テクスチャーファイル名定義
const char *g_paClearTexFile[CLEARCOMMENTTYPE_MAX] =
{
	{ "data/TEXTURE/CLEAR/Clear_name.png" },
	{ "data/TEXTURE/CLEAR/Thank_You_for_Playing2.png" }
};


// ------------------------------------------------------------------------------------------
//
// クリアコメント初期化処理
//
// ------------------------------------------------------------------------------------------

void InitClearComment(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイス
	VERTEX_2D *pVtx;			// 頂点情報へのポイント

	// テクスチャの読み込み
	pDevice = GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * CLEARCOMMENTTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, 
		D3DPOOL_MANAGED, 
		&g_pVtxBuffClearComment,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffClearComment->Lock(0, 0, (void **)&pVtx, 0);

	// クリアコメントカウント
	for (int nCntCom = 0; nCntCom < CLEARCOMMENTTYPE_MAX; nCntCom++)
	{
		// テクスチャー作成
		D3DXCreateTextureFromFile(
			pDevice,
			g_paClearTexFile[nCntCom],
			&g_pTextureClearComment[nCntCom]);

		// クリアのコメント
		if (nCntCom == CLEARCOMMENTTYPE_CLEAR)
		{
			g_ClearCom[nCntCom].pos = D3DXVECTOR2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
			g_ClearCom[nCntCom].size = D3DXVECTOR2(350.0f, 250.0f);
		}

		// サンキューのコメント
		else if (nCntCom == CLEARCOMMENTTYPE_THANK)
		{
			g_ClearCom[nCntCom].pos = D3DXVECTOR2(900.0f, 600.0f);
			g_ClearCom[nCntCom].size = D3DXVECTOR2(500.0f, 130.0f);
		}

		// 頂点の設定
		pVtx[0].pos = D3DXVECTOR3(
			g_ClearCom[nCntCom].pos.x - g_ClearCom[nCntCom].size.x * 0.5f,
			g_ClearCom[nCntCom].pos.y - g_ClearCom[nCntCom].size.y * 0.5f,
			0.0f);
		pVtx[1].pos = D3DXVECTOR3(
			g_ClearCom[nCntCom].pos.x + g_ClearCom[nCntCom].size.x * 0.5f, 
			g_ClearCom[nCntCom].pos.y - g_ClearCom[nCntCom].size.y * 0.5f,
			0.0f);
		pVtx[2].pos = D3DXVECTOR3(
			g_ClearCom[nCntCom].pos.x - g_ClearCom[nCntCom].size.x * 0.5f,
			g_ClearCom[nCntCom].pos.y + g_ClearCom[nCntCom].size.y * 0.5f,
			0.0f);
		pVtx[3].pos = D3DXVECTOR3(
			g_ClearCom[nCntCom].pos.x + g_ClearCom[nCntCom].size.x * 0.5f, 
			g_ClearCom[nCntCom].pos.y + g_ClearCom[nCntCom].size.y * 0.5f,
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
	g_pVtxBuffClearComment->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// クリアコメント終了処理
//
// ------------------------------------------------------------------------------------------
void UninitClearComment(void)
{
	// テクスチャの開放
	// クリアコメントカウント
	for (int nCntCom = 0; nCntCom < CLEARCOMMENTTYPE_MAX; nCntCom++)
	{
		if (g_pTextureClearComment[nCntCom] != NULL)
		{
			g_pTextureClearComment[nCntCom]->Release();
			g_pTextureClearComment[nCntCom] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffClearComment != NULL)
	{
		g_pVtxBuffClearComment->Release();
		g_pVtxBuffClearComment = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// クリアコメント更新処理
//
// ------------------------------------------------------------------------------------------
void UpdateClearComment(void)
{
}

// ------------------------------------------------------------------------------------------
//
// クリアコメント描画処理
//
// ------------------------------------------------------------------------------------------

void DrawClearComment(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;	// デバイス

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffClearComment, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// クリアコメントカウント
	for (int nCntCom = 0; nCntCom < CLEARCOMMENTTYPE_MAX; nCntCom++)
	{
		// テクスチャの設定
		pDevice->SetTexture(0, g_pTextureClearComment[nCntCom]);

		// クリアコメント描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 
			nCntCom * NUM_VERTEX, 
			NUM_POLYGON);
	}
}