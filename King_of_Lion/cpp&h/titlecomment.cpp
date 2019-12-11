// ------------------------------------------------------------------------------------------
//
// コメント処理の説明[titlecomment.cpp]
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
#include "titlecomment.h"

// ------------------------------------------------------------------------------------------
//
// マクロ定義
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// プロトタイプ宣言
//
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
//
// グローバル変数
//
// ------------------------------------------------------------------------------------------

LPDIRECT3DTEXTURE9 g_pTextureTitleComment[TITLECOMMENTTYPE_MAX] = {};	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleComment = NULL;		// テクスチャへのポインタ
TITLECOMMENT g_aTitleComment[TITLECOMMENTTYPE_MAX];							// コメント

// ------------------------------------------------------------------------------------------
//
// タイトル初期化処理
//
// ------------------------------------------------------------------------------------------

void InitTitleComment(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntTitleComment;

	// タイトルコメントファイル名定義
	char *paTitleCommentFile[TITLECOMMENTTYPE_MAX] =
	{
		{ "data/TEXTURE/TITLE/Title_Log.png" },		// タイトル名
		{ "data/TEXTURE/TITLE/00_rion_shadow.png" },	// ライオン
	};

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * TITLECOMMENTTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED, 
		&g_pVtxBuffTitleComment,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffTitleComment->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntTitleComment = 0; nCntTitleComment < TITLECOMMENTTYPE_MAX; nCntTitleComment++)
	{
		// タイトル名
		if (nCntTitleComment == TITLECOMMENTTYPE_NAME)
		{
			g_aTitleComment[nCntTitleComment].pos = D3DXVECTOR3(640.0f, -350.0f, 0.0f);
			g_aTitleComment[nCntTitleComment].move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
			g_aTitleComment[nCntTitleComment].size = D3DXVECTOR2(350.0f, 350.0f);
			g_aTitleComment[nCntTitleComment].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		// ライオン
		else if (nCntTitleComment == TITLECOMMENTTYPE_RION)
		{
			g_aTitleComment[nCntTitleComment].pos = D3DXVECTOR3(1000.0f, 200.0f, 0.0f);
			g_aTitleComment[nCntTitleComment].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aTitleComment[nCntTitleComment].size = D3DXVECTOR2(0.0f, 0.0f);
			g_aTitleComment[nCntTitleComment].tex = D3DXVECTOR2(0.0f, 0.0f);
		}

		g_aTitleComment[nCntTitleComment].bUse = true;

		// 目的地に到着しているかどうか
		g_aTitleComment[nCntTitleComment].bPut = false;

		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			paTitleCommentFile[nCntTitleComment],
			&g_pTextureTitleComment[nCntTitleComment]);

		// 頂点の設定
		pVtx[0].pos = D3DXVECTOR3(
			g_aTitleComment[nCntTitleComment].pos.x - g_aTitleComment[nCntTitleComment].size.x,
			g_aTitleComment[nCntTitleComment].pos.y - g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(
			g_aTitleComment[nCntTitleComment].pos.x + g_aTitleComment[nCntTitleComment].size.x,
			g_aTitleComment[nCntTitleComment].pos.y - g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(
			g_aTitleComment[nCntTitleComment].pos.x - g_aTitleComment[nCntTitleComment].size.x,
			g_aTitleComment[nCntTitleComment].pos.y + g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(
			g_aTitleComment[nCntTitleComment].pos.x + g_aTitleComment[nCntTitleComment].size.x,
			g_aTitleComment[nCntTitleComment].pos.y + g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// 四つ目の頂点
		
		// 頂点作成
		pVtx[0].rhw = 1.0f;									// 一つ目の頂点作成
		pVtx[1].rhw = 1.0f;									// 二つ目の頂点作成
		pVtx[2].rhw = 1.0f;									// 三つ目の頂点作成
		pVtx[3].rhw = 1.0f;									// 四つ目の頂点作成

		// カラー設定
		pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0);	// 一つ目の色設定
		pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0);	// 二つ目の色設定
		pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0);	// 三つ目の色設定
		pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0);	// 四つ目の色設定

		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(
			0.0f, 
			0.0f);				// 一つ目のテクスチャ

		pVtx[1].tex = D3DXVECTOR2(
			g_aTitleComment[nCntTitleComment].tex.x,
			0.0f);				// 二つ目のテクスチャ

		pVtx[2].tex = D3DXVECTOR2(
			0.0f, 
			g_aTitleComment[nCntTitleComment].tex.y);				// 三つ目のテクスチャ

		pVtx[3].tex = D3DXVECTOR2(
			g_aTitleComment[nCntTitleComment].tex.x,
			g_aTitleComment[nCntTitleComment].tex.y);				// 四つ目のテクスチャ

		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffTitleComment->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// タイトル終了処理
//
// ------------------------------------------------------------------------------------------

void UninitTitleComment(void)
{
	int nCntTitleComment;

	// テクスチャの開放
	for (nCntTitleComment = 0; nCntTitleComment < TITLECOMMENTTYPE_MAX; nCntTitleComment++)
	{
		if (g_pTextureTitleComment[nCntTitleComment] != NULL)
		{
			g_pTextureTitleComment[nCntTitleComment]->Release();
			g_pTextureTitleComment[nCntTitleComment] = NULL;
		}
	}
	// 頂点バッファの開放
	if (g_pVtxBuffTitleComment != NULL)
	{
		g_pVtxBuffTitleComment->Release();
		g_pVtxBuffTitleComment = NULL;
	}
}

// ------------------------------------------------------------------------------------------
//
// タイトル更新処理
//
// ------------------------------------------------------------------------------------------

void UpdateTitleComment(void)
{
	VERTEX_2D *pVtx;	// 頂点情報へのポイント
	int nCntTitleComment;
	static int nCntColor = 0;

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffTitleComment->Lock(0, 0, (void **)&pVtx, 0);

	// カウントタイトルコメント
	for (nCntTitleComment = 0; nCntTitleComment < TITLECOMMENTTYPE_MAX; nCntTitleComment++)
	{
		// 目的地についているかどうか
		if (g_aTitleComment[nCntTitleComment].bPut == false)
		{
			// タイトル名
			if (nCntTitleComment == TITLECOMMENTTYPE_NAME)
			{
				g_aTitleComment[nCntTitleComment].pos.y += g_aTitleComment[nCntTitleComment].move.y;

				if (g_aTitleComment[nCntTitleComment].pos.y >= TITLECOMENT_ON_Y)
				{
					g_aTitleComment[nCntTitleComment].pos.y = TITLECOMENT_ON_Y;

					// 目的地に到着しているかどうか
					g_aTitleComment[nCntTitleComment].bPut = true;
				}

				if (GetKeyboardTrigger(DIK_RETURN) ||
					GetKeyboardTrigger(DIK_J) ||
					GetJoyboardTrigger(0, JOYPADKEY_B))
				{
					g_aTitleComment[nCntTitleComment].pos.y = TITLECOMENT_ON_Y;

					// 目的地に到着しているかどうか
					g_aTitleComment[nCntTitleComment].bPut = true;
				}

				// 頂点の設定
				pVtx[0].pos = D3DXVECTOR3(
					g_aTitleComment[nCntTitleComment].pos.x - g_aTitleComment[nCntTitleComment].size.x,
					g_aTitleComment[nCntTitleComment].pos.y - g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// 一つ目の頂点
				pVtx[1].pos = D3DXVECTOR3(
					g_aTitleComment[nCntTitleComment].pos.x + g_aTitleComment[nCntTitleComment].size.x,
					g_aTitleComment[nCntTitleComment].pos.y - g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// 二つ目の頂点
				pVtx[2].pos = D3DXVECTOR3(
					g_aTitleComment[nCntTitleComment].pos.x - g_aTitleComment[nCntTitleComment].size.x,
					g_aTitleComment[nCntTitleComment].pos.y + g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// 三つ目の頂点
				pVtx[3].pos = D3DXVECTOR3(
					g_aTitleComment[nCntTitleComment].pos.x + g_aTitleComment[nCntTitleComment].size.x,
					g_aTitleComment[nCntTitleComment].pos.y + g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// 四つ目の頂点
			}

			// ライオン
			if (nCntTitleComment == TITLECOMMENTTYPE_RION)
			{
				// タイトル名が目的地に到達したら
				if (g_aTitleComment[TITLECOMMENTTYPE_NAME].bPut == true)
				{
					// テクスチャが徐々に出現する
					g_aTitleComment[nCntTitleComment].tex.x += 0.01f;
					g_aTitleComment[nCntTitleComment].tex.y += 0.01f;

					// サイズが徐々に大きくなる
					g_aTitleComment[nCntTitleComment].size.x += 1.0f;
					g_aTitleComment[nCntTitleComment].size.y += 1.0f;


					// テクスチャを規定値に
					if (g_aTitleComment[nCntTitleComment].tex.y >= 1.0f ||
						g_aTitleComment[nCntTitleComment].tex.x >= 1.0f)
					{
						g_aTitleComment[nCntTitleComment].tex.x = 1.0f;

						g_aTitleComment[nCntTitleComment].tex.y = 1.0f;

						// 目的のテクスチャになったら
						g_aTitleComment[nCntTitleComment].bPut = true;
					}

					// 頂点の設定
					pVtx[0].pos = D3DXVECTOR3(
						g_aTitleComment[nCntTitleComment].pos.x - g_aTitleComment[nCntTitleComment].size.x,
						g_aTitleComment[nCntTitleComment].pos.y - g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// 一つ目の頂点
					pVtx[1].pos = D3DXVECTOR3(
						g_aTitleComment[nCntTitleComment].pos.x + g_aTitleComment[nCntTitleComment].size.x,
						g_aTitleComment[nCntTitleComment].pos.y - g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// 二つ目の頂点
					pVtx[2].pos = D3DXVECTOR3(
						g_aTitleComment[nCntTitleComment].pos.x - g_aTitleComment[nCntTitleComment].size.x,
						g_aTitleComment[nCntTitleComment].pos.y + g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// 三つ目の頂点
					pVtx[3].pos = D3DXVECTOR3(
						g_aTitleComment[nCntTitleComment].pos.x + g_aTitleComment[nCntTitleComment].size.x,
						g_aTitleComment[nCntTitleComment].pos.y + g_aTitleComment[nCntTitleComment].size.y, 0.0f);	// 四つ目の頂点

					// テクスチャの設定
					pVtx[0].tex = D3DXVECTOR2(
						0.0f,
						0.0f);				// 一つ目のテクスチャ

					pVtx[1].tex = D3DXVECTOR2(
						g_aTitleComment[nCntTitleComment].tex.x,
						0.0f);				// 二つ目のテクスチャ

					pVtx[2].tex = D3DXVECTOR2(
						0.0f,
						g_aTitleComment[nCntTitleComment].tex.y);				// 三つ目のテクスチャ

					pVtx[3].tex = D3DXVECTOR2(
						g_aTitleComment[nCntTitleComment].tex.x,
						g_aTitleComment[nCntTitleComment].tex.y);				// 四つ目のテクスチャ
				}
			}
		}
		// ポイント合わせ
		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffTitleComment->Unlock();
}

// ------------------------------------------------------------------------------------------
//
// タイトル描画処理
//
// ------------------------------------------------------------------------------------------
void DrawTitleComment(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTitleComment;

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffTitleComment, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	// テクスチャの設定
	for (nCntTitleComment = 0; nCntTitleComment < TITLECOMMENTTYPE_MAX; nCntTitleComment++)
	{
		if (g_aTitleComment[nCntTitleComment].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureTitleComment[nCntTitleComment]);

			// タイトル描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitleComment * 4, 2);
		}
	}
}

// ------------------------------------------------------------------------------------------
//
// タイトル取得処理
//
// ------------------------------------------------------------------------------------------

TITLECOMMENT *GetTitleComment(int nCntTitleComment)
{
	return &g_aTitleComment[nCntTitleComment];
}