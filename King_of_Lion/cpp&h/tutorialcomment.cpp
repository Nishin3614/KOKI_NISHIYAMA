// ------------------------------------------------------------------------------------------
//
// コメント処理の説明[tutorialcomment.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// インクルードファイル
//
// ------------------------------------------------------------------------------------------
#include "title.h"
#include "input.h"
#include "joypad.h"
#include "fade.h"
#include "sound.h"
#include "tutorialcomment.h"
#include "title.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------
#define NAME_SIZE_X (500)		// タイトル名サイズ（ｘ）
#define NAME_SIZE_Y (300)		// タイトル名サイズ（ｙ）

#define ENTER_SIZE_X (300)		// エンター名サイズ（ｘ）
#define ENTER_SIZE_Y (150)		// エンター名サイズ（ｙ）

// ------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ------------------------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 g_pTextureTutorialComment[TUTORIALCOMMENTTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialComment = NULL;		// テクスチャへのポインタ
TUTORIALCOMMENT g_aTutorialComment[TUTORIALCOMMENTTYPE_MAX];							// コメント

// ------------------------------------------------------------------------------------------
//
// タイトル初期化処理
//
// ------------------------------------------------------------------------------------------

void InitTutorialComment(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntTutorialComment;

	// タイトルコメントファイル名定義
	char *paTutorialCommentFile[TUTORIALCOMMENTTYPE_MAX] =
	{
		{ "data/TEXTURE/TUTORIALS/.png" },	// 背景
		{ "data/TEXTURE/TUTORIALS/説明.png" },	// キーボード用の説明
		{ "data/TEXTURE/TUTORIALS/ジョイパッド説明.png" }	// ジョイパッド用の説明
	};

	// 初期化
	for (nCntTutorialComment = 0; nCntTutorialComment < TUTORIALCOMMENTTYPE_MAX; nCntTutorialComment++)
	{
		g_aTutorialComment[nCntTutorialComment].Type = TUTORIALCOMMENTTYPE_KEYBOARD;
		g_aTutorialComment[nCntTutorialComment].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTutorialComment[nCntTutorialComment].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
		g_aTutorialComment[nCntTutorialComment].fXSize = 0;
		g_aTutorialComment[nCntTutorialComment].fYSize = 0;
		g_aTutorialComment[nCntTutorialComment].bUse = false;

		D3DXCreateTextureFromFile(
			pDevice,
			paTutorialCommentFile[nCntTutorialComment],
			&g_pTextureTutorialComment[nCntTutorialComment]);
	}

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TUTORIALCOMMENTTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialComment, 
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffTutorialComment->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntTutorialComment = 0; nCntTutorialComment < TUTORIALCOMMENTTYPE_MAX; nCntTutorialComment++)
	{
		// 頂点の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x - g_aTutorialComment[nCntTutorialComment].fXSize / 2, 
			g_aTutorialComment[nCntTutorialComment].pos.y - g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x + g_aTutorialComment[nCntTutorialComment].fXSize / 2, 
			g_aTutorialComment[nCntTutorialComment].pos.y - g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x - g_aTutorialComment[nCntTutorialComment].fXSize / 2,
			g_aTutorialComment[nCntTutorialComment].pos.y + g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x + g_aTutorialComment[nCntTutorialComment].fXSize / 2,
			g_aTutorialComment[nCntTutorialComment].pos.y + g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// 四つ目の頂点
		
		// 頂点作成
		pVtx[0].rhw = 1.0f;									// 一つ目の頂点作成
		pVtx[1].rhw = 1.0f;									// 二つ目の頂点作成
		pVtx[2].rhw = 1.0f;									// 三つ目の頂点作成
		pVtx[3].rhw = 1.0f;									// 四つ目の頂点作成

		// カラー設定
		pVtx[0].col = g_aTutorialComment[nCntTutorialComment].col;	// 一つ目の色設定
		pVtx[1].col = g_aTutorialComment[nCntTutorialComment].col;	// 二つ目の色設定
		pVtx[2].col = g_aTutorialComment[nCntTutorialComment].col;	// 三つ目の色設定
		pVtx[3].col = g_aTutorialComment[nCntTutorialComment].col;	// 四つ目の色設定

		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				// 一つ目のテクスチャ
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				// 二つ目のテクスチャ
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				// 三つ目のテクスチャ
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				// 四つ目のテクスチャ

		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffTutorialComment->Unlock();

	// チュートリアルコメント設定
	// キーボード用コメント
	if (GetButtenTitle() == false)
	{
		SetTutorialComment(
			TUTORIALCOMMENTTYPE_KEYBOARD,
			D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			SCREEN_WIDTH, SCREEN_HEIGHT);
	}
	// ジョイパッド用コメント
	else
	{
		SetTutorialComment(
			TUTORIALCOMMENTTYPE_JOYPAD,
			D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f),
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			SCREEN_WIDTH, SCREEN_HEIGHT);
	}

	//// ジャンプコメント
	//SetTutorialComment(
	//	TUTORIALCOMMENTTYPE_JUMP,
	//	D3DXVECTOR3(620, 600, 0.0f),
	//	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
	//	250, 150);

	//// ポーズコメント
	//SetTutorialComment(TUTORIALCOMMENTTYPE_PAUSE, 
	//	D3DXVECTOR3(1180, 650, 0.0f), 
	//	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
	//	150, 100);

	//// コンボコメント
	//SetTutorialComment(TUTORIALCOMMENTTYPE_FRONT,
	//	D3DXVECTOR3(150, 400, 0.0f),
	//	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
	//	250, 150);

	//// 攻撃コメント
	//SetTutorialComment(TUTORIALCOMMENTTYPE_ATTACK,
	//	D3DXVECTOR3(1100, 400, 0.0f),
	//	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
	//	250, 150);

	//// 弾コメント
	//SetTutorialComment(TUTORIALCOMMENTTYPE_SHOT,
	//	D3DXVECTOR3(1100, 200, 0.0f),
	//	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f),
	//	250, 150);
}

// ------------------------------------------------------------------------------------------
//
// タイトル終了処理
//
// ------------------------------------------------------------------------------------------

void UninitTutorialComment(void)
{
	int nCntTutorialComment;

	// テクスチャの開放
	for (nCntTutorialComment = 0; nCntTutorialComment < TUTORIALCOMMENTTYPE_MAX; nCntTutorialComment++)
	{
		if (g_pTextureTutorialComment[nCntTutorialComment] != NULL)
		{
			g_pTextureTutorialComment[nCntTutorialComment]->Release();
			g_pTextureTutorialComment[nCntTutorialComment] = NULL;
		}
	}
	// 頂点バッファの開放
	if (g_pVtxBuffTutorialComment != NULL)
	{
		g_pVtxBuffTutorialComment->Release();
		g_pVtxBuffTutorialComment = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// タイトル更新処理
//
// ------------------------------------------------------------------------------------------

void UpdateTutorialComment(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntTutorialComment;
	static int nCntColor = 0;

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffTutorialComment->Lock(0, 0, (void **)&pVtx, 0);

	// カウントチュートリアル
	for (nCntTutorialComment = 0; nCntTutorialComment < TUTORIALCOMMENTTYPE_MAX; nCntTutorialComment++)
	{
		// タイプごとに表示
		switch (g_aTutorialComment[nCntTutorialComment].Type)
		{
			// 移動コメント
		case TUTORIALCOMMENTTYPE_KEYBOARD:

			break;
		}
		// メモリー合わせ
		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffTutorialComment->Unlock();

}

// ------------------------------------------------------------------------------------------
//
// タイトル描画処理
//
// ------------------------------------------------------------------------------------------
void DrawTutorialComment(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTutorialComment;

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialComment, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	// テクスチャの設定
	for (nCntTutorialComment = 0; nCntTutorialComment < TUTORIALCOMMENTTYPE_MAX; nCntTutorialComment++)
	{
		if (g_aTutorialComment[nCntTutorialComment].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureTutorialComment[g_aTutorialComment[nCntTutorialComment].Type]);

			// タイトル描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTutorialComment * 4, 2);
		}
	}
}

// ------------------------------------------------------------------------------------------
//
// タイトル設定処理
//
// ------------------------------------------------------------------------------------------
void SetTutorialComment(TUTORIALCOMMENTTYPE Type, D3DXVECTOR3 pos, D3DXCOLOR col, float fXSize, float fYSize) 
{
	// 変数宣言
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntTutorialComment;

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffTutorialComment->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntTutorialComment = 0; nCntTutorialComment < TUTORIALCOMMENTTYPE_MAX; nCntTutorialComment++)
	{
		// 使用していない時
		if (g_aTutorialComment[nCntTutorialComment].bUse == false)
		{
			g_aTutorialComment[nCntTutorialComment].Type = Type;
			g_aTutorialComment[nCntTutorialComment].pos = pos;
			g_aTutorialComment[nCntTutorialComment].col = col;
			g_aTutorialComment[nCntTutorialComment].fXSize = fXSize;
			g_aTutorialComment[nCntTutorialComment].fYSize = fYSize;
			g_aTutorialComment[nCntTutorialComment].bUse = true;

			// データのポインタを合わせる
			pVtx += nCntTutorialComment * 4;

			// タイプごとに表示
			switch (g_aTutorialComment[nCntTutorialComment].Type)
			{
				// 移動コメント
			case TUTORIALCOMMENTTYPE_KEYBOARD:

				break;
			}
			// 頂点の設定
			pVtx[0].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x - g_aTutorialComment[nCntTutorialComment].fXSize / 2,
				g_aTutorialComment[nCntTutorialComment].pos.y - g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// 一つ目の頂点
			pVtx[1].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x + g_aTutorialComment[nCntTutorialComment].fXSize / 2,
				g_aTutorialComment[nCntTutorialComment].pos.y - g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// 二つ目の頂点
			pVtx[2].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x - g_aTutorialComment[nCntTutorialComment].fXSize / 2,
				g_aTutorialComment[nCntTutorialComment].pos.y + g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// 三つ目の頂点
			pVtx[3].pos = D3DXVECTOR3(g_aTutorialComment[nCntTutorialComment].pos.x + g_aTutorialComment[nCntTutorialComment].fXSize / 2,
				g_aTutorialComment[nCntTutorialComment].pos.y + g_aTutorialComment[nCntTutorialComment].fYSize / 2, 0.0f);	// 四つ目の頂点

			// カラー設定
			pVtx[0].col = g_aTutorialComment[nCntTutorialComment].col;	// 一つ目の色設定
			pVtx[1].col = g_aTutorialComment[nCntTutorialComment].col;	// 二つ目の色設定
			pVtx[2].col = g_aTutorialComment[nCntTutorialComment].col;	// 三つ目の色設定
			pVtx[3].col = g_aTutorialComment[nCntTutorialComment].col;	// 四つ目の色設定

			// for分を抜ける
			break;
		}
	}
	// アンロック
	g_pVtxBuffTutorialComment->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// タイトル取得処理
//
// ------------------------------------------------------------------------------------------

TUTORIALCOMMENT *GetTutorialComment(void)
{
	return &g_aTutorialComment[0];
}