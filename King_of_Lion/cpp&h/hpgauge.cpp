// ------------------------------------------------------------------------------------------
// 
// ゲージの処理 [HpGauge.cpp]
// Author:Koki Nishiyama
// 
// ------------------------------------------------------------------------------------------
#include "hpgauge.h"
#include "player.h"

// ------------------------------------------------------------------------------------------
// 
// マクロ定義
//
// ------------------------------------------------------------------------------------------
#define MAX_HPGAUGE				(10)
#define MAX_HPMULTI				(5.0f)
#define HPGAUGE_TEXTURE_NAME		"data/TEXTURE/hpgauge000.png"
#define HPGAUGE_SIZE_X			(500)
#define HPGAUGE_SIZE_Y			(20)
#define HPGAUGE_WHITE_X			(1280)
#define HPGAUGE_WHITE_Y			(40)

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------状態---------- //
typedef enum
{
	HPGAUGETYPE_WHITE = 0,
	HPGAUGETYPE_BLOCK,
	HPGAUGETYPE_RED,
	HPGAUGETYPE_GREEN,
	HPGAUGETYPE_MAX
} HPGAUGETYPE;

// ------------------------------------------------------------------------------------------
//
// 構造体定義
//
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3	pos;					// 座標
	D3DXCOLOR	col[HPGAUGETYPE_MAX];	// 色
	int			nType;					// 種類
	float		fSize[HPGAUGETYPE_MAX];	// 現在ゲージのサイズ
	float		fSizeLast;				// 目的ゲージのサイズ
	float		fBetSize;				// 差分サイズ
	float		fSize_y;				// ゲージのサイズ(y)
	bool		bUse[HPGAUGETYPE_MAX];	// 使ってるかどうか
} HPGAUGE;

// ------------------------------------------------------------------------------------------
// 
// グローバル変数
// 
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureHpGauge = NULL;	// テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHpGauge = NULL;	// 頂点バッファへのポインタ
HPGAUGE					g_aHpGauge[HPGAUGESYSTEM_MAX];

// ------------------------------------------------------------------------------------------
// 
// 初期化処理
// 
// ------------------------------------------------------------------------------------------
void InitHpGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_2D *pVtx;
	int nCntType;		// タイプカウント
	int nCntHpGauge;	// HPカウント

	// ゲージの情報の初期化
	for (nCntHpGauge = 0; nCntHpGauge < HPGAUGESYSTEM_MAX; nCntHpGauge++)
	{
		g_aHpGauge[nCntHpGauge].nType = 0;								// 種類
		g_aHpGauge[nCntHpGauge].fSizeLast = HPGAUGE_SIZE_X;				// 目的サイズ
		g_aHpGauge[nCntHpGauge].fBetSize = 0;							// 差分サイズ
		g_aHpGauge[nCntHpGauge].fSize_y = 10;							// サイズ(Y)

		// それぞれの長さ
		for (nCntType = 0; nCntType < HPGAUGETYPE_MAX; nCntType++)
		{
			g_aHpGauge[nCntHpGauge].fSize[nCntType] = g_aHpGauge[nCntHpGauge].fSizeLast;	// サイズ(X)
			g_aHpGauge[nCntHpGauge].bUse[nCntType] = true;									// 使ってるかどうか
		}

		// ゲージの色
		g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_WHITE] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);	// 白枠
		g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_BLOCK] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);	// 黒のゲージ
		g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_RED] = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);	// 赤のゲージ

		// HPゲージ
		if (nCntHpGauge == HPGAUGESYSTEM_HP)
		{
			g_aHpGauge[nCntHpGauge].pos = D3DXVECTOR3(50.0f, 30.0f, 0.0f);						// 座標

			g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_GREEN] = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);	// 緑のゲージ
		}

		// MPゲージ
		else
		{
			g_aHpGauge[nCntHpGauge].pos = D3DXVECTOR3(50.0f, 50.0f, 0.0f);						// 座標

			g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_GREEN] = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);	// 青のゲージ

			g_aHpGauge[nCntHpGauge].bUse[HPGAUGETYPE_WHITE] = false;							// 使ってるかどうか
		}
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		HPGAUGE_TEXTURE_NAME,
		&g_pTextureHpGauge);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * HPGAUGESYSTEM_MAX * HPGAUGETYPE_MAX - 1,	// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,			// 頂点フォーマット
		D3DPOOL_MANAGED,
		&g_pVtxBuffHpGauge,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffHpGauge->Lock(0, 0, (void**)&pVtx, 0);
	
	for (nCntHpGauge = 0; nCntHpGauge < HPGAUGESYSTEM_MAX; nCntHpGauge++)
	{
		// 白枠作成 //
		// 頂点の座標
		pVtx[0].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(-100.0f, -40.0f, 0.0f);
		pVtx[1].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(HPGAUGE_WHITE_X, -40.0f, 0.0f);
		pVtx[2].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(-100.0f, HPGAUGE_WHITE_Y, 0.0f);
		pVtx[3].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(HPGAUGE_WHITE_X, HPGAUGE_WHITE_Y, 0.0f);

		// 頂点の同次座標	
		pVtx[0].rhw = 1.0f;									// 1.0fで固定
		pVtx[1].rhw = 1.0f;									// 1.0fで固定
		pVtx[2].rhw = 1.0f;									// 1.0fで固定
		pVtx[3].rhw = 1.0f;									// 1.0fで固定

		// 頂点の色
		pVtx[0].col = g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_WHITE];				// (赤,緑,青,透明度)
		pVtx[1].col = g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_WHITE];				// (赤,緑,青,透明度)
		pVtx[2].col = g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_WHITE];				// (赤,緑,青,透明度)
		pVtx[3].col = g_aHpGauge[nCntHpGauge].col[HPGAUGETYPE_WHITE];				// (赤,緑,青,透明度)

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				// (テクスチャのUV座標)
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				// (テクスチャのUV座標)
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				// (テクスチャのUV座標)
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				// (テクスチャのUV座標)

		pVtx += 4;

		for (nCntType = HPGAUGETYPE_BLOCK; nCntType < HPGAUGETYPE_MAX; nCntType++)
		{
			// 頂点の座標
			pVtx[0].pos = g_aHpGauge[nCntHpGauge].pos;
			pVtx[1].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(g_aHpGauge[nCntHpGauge].fSize[nCntType], 0.0f, 0.0f);
			pVtx[2].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(0.0f, g_aHpGauge[nCntHpGauge].fSize_y, 0.0f);
			pVtx[3].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(g_aHpGauge[nCntHpGauge].fSize[nCntType], g_aHpGauge[nCntHpGauge].fSize_y, 0.0f);

			// 頂点の同次座標	
			pVtx[0].rhw = 1.0f;									// 1.0fで固定
			pVtx[1].rhw = 1.0f;									// 1.0fで固定
			pVtx[2].rhw = 1.0f;									// 1.0fで固定
			pVtx[3].rhw = 1.0f;									// 1.0fで固定

			// ゲージ本体
			if (nCntType == HPGAUGETYPE_GREEN)
			{
				// 頂点の色
				pVtx[0].col = g_aHpGauge[nCntHpGauge].col[nCntType] + D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.5f);				// (赤,緑,青,透明度)
				pVtx[1].col = g_aHpGauge[nCntHpGauge].col[nCntType];				// (赤,緑,青,透明度)
				pVtx[2].col = g_aHpGauge[nCntHpGauge].col[nCntType] + D3DXCOLOR(0.0f, 0.0f, 0.0f, -0.5f);				// (赤,緑,青,透明度)
				pVtx[3].col = g_aHpGauge[nCntHpGauge].col[nCntType];				// (赤,緑,青,透明度)

			}

			// 赤・黒のゲージ
			else 
			{
				// 頂点の色
				pVtx[0].col = g_aHpGauge[nCntHpGauge].col[nCntType];				// (赤,緑,青,透明度)
				pVtx[1].col = g_aHpGauge[nCntHpGauge].col[nCntType];				// (赤,緑,青,透明度)
				pVtx[2].col = g_aHpGauge[nCntHpGauge].col[nCntType];				// (赤,緑,青,透明度)
				pVtx[3].col = g_aHpGauge[nCntHpGauge].col[nCntType];				// (赤,緑,青,透明度)
			}

			// テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);				// (テクスチャのUV座標)
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);				// (テクスチャのUV座標)
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);				// (テクスチャのUV座標)
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);				// (テクスチャのUV座標)

			pVtx += 4;
		}
	}

	// 頂点データをアンロックする
	g_pVtxBuffHpGauge->Unlock();
}

// ------------------------------------------------------------------------------------------
// 
// 終了処理
// 
// ------------------------------------------------------------------------------------------
void UninitHpGauge(void)
{
	
	// テクスチャの開放
	if(g_pTextureHpGauge != NULL)
	{
		g_pTextureHpGauge->Release();
		g_pTextureHpGauge = NULL;
	}
	
	// 頂点バッファの開放
	if(g_pVtxBuffHpGauge != NULL)
	{
		g_pVtxBuffHpGauge->Release();
		g_pVtxBuffHpGauge = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 
// 更新処理
// 
// ------------------------------------------------------------------------------------------
void UpdateHpGauge(void)
{
	VERTEX_2D * pVtx;	// 頂点情報
	int nCntHpGauge;	// HPゲージカウント

	// 頂点データの範囲をロックし、頂点バッファへのポインタを取得
	g_pVtxBuffHpGauge->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntHpGauge = 0; nCntHpGauge < HPGAUGESYSTEM_MAX; nCntHpGauge++)
	{
		// プレイヤーのHPが変わったら
		if (g_aHpGauge[nCntHpGauge].fSize[HPGAUGETYPE_RED] != g_aHpGauge[nCntHpGauge].fSizeLast)
		{
			// 差分分割加算
			g_aHpGauge[nCntHpGauge].fSize[HPGAUGETYPE_RED] -= g_aHpGauge[nCntHpGauge].fBetSize;

			// 目的のサイズ量に合わせる
			if (g_aHpGauge[nCntHpGauge].fSize[HPGAUGETYPE_RED] < g_aHpGauge[nCntHpGauge].fSizeLast + MAX_HPMULTI)
			{
				g_aHpGauge[nCntHpGauge].fSize[HPGAUGETYPE_RED] = g_aHpGauge[nCntHpGauge].fSizeLast;
			}
		}

		pVtx += 4;

		// カウントタイプ
		for (int nCntType = HPGAUGETYPE_BLOCK; nCntType < HPGAUGETYPE_MAX; nCntType++)
		{
			// 頂点の座標
			pVtx[0].pos = g_aHpGauge[nCntHpGauge].pos;
			pVtx[1].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(g_aHpGauge[nCntHpGauge].fSize[nCntType], 0.0f, 0.0f);
			pVtx[2].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(0.0f, g_aHpGauge[nCntHpGauge].fSize_y, 0.0f);
			pVtx[3].pos = g_aHpGauge[nCntHpGauge].pos + D3DXVECTOR3(g_aHpGauge[nCntHpGauge].fSize[nCntType], g_aHpGauge[nCntHpGauge].fSize_y, 0.0f);

			pVtx += 4;
		}
	}
	// 頂点データをアンロックする
	g_pVtxBuffHpGauge->Unlock();
}

// ------------------------------------------------------------------------------------------
// 
// 描画処理
// 
// ------------------------------------------------------------------------------------------
void DrawHpGauge(void)
{
	int nCntHpGauge;
	int nCntType;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffHpGauge, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, g_pTextureHpGauge);

	for (nCntHpGauge = 0; nCntHpGauge < HPGAUGESYSTEM_MAX; nCntHpGauge++)
	{
		for (nCntType = 0; nCntType < HPGAUGETYPE_MAX; nCntType++)
		{
			if (g_aHpGauge[nCntHpGauge].bUse[nCntType] == true)
			{
				// ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,				// プリミティブの種類
					(nCntHpGauge * HPGAUGETYPE_MAX * 4) + nCntType * 4,		// 開始するインデックス(頂点)  
					2);					// ポリゴンの枚数
			}

		}
	}
}

// ------------------------------------------------------------------------------------------
// 
// HPゲージの変化
// 
// ------------------------------------------------------------------------------------------
void ChangeHpGauge(
	int nCntHpGauge,	// カウント
	float fLife,		// ライフ
	float fLastLife		// 上限ライフ
)
{
	// サイズの更新
	g_aHpGauge[nCntHpGauge].fSizeLast = MAX_HPMULTI * fLastLife;

	// 現在のサイズに代入(緑)
	g_aHpGauge[nCntHpGauge].fSize[HPGAUGETYPE_GREEN] = g_aHpGauge[nCntHpGauge].fSizeLast;

	// 差分分割
	g_aHpGauge[nCntHpGauge].fBetSize = 
		(g_aHpGauge[nCntHpGauge].fSizeLast - g_aHpGauge[nCntHpGauge].fSize[HPGAUGETYPE_BLOCK]) /
		(fLastLife - fLife);
}

// ------------------------------------------------------------------------------------------
// 
// 最大HPゲージの変化
// 
// ------------------------------------------------------------------------------------------
void ChangeMaxHpGauge(
	int nCntHpGauge,	// カウント
	float fMaxHP		// 最大HP
)
{
	// 最大HPゲージの変更
	g_aHpGauge[nCntHpGauge].fSize[HPGAUGETYPE_BLOCK] = MAX_HPMULTI * fMaxHP;
}
