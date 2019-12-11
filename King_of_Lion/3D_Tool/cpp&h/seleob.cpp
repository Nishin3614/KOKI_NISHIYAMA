// ------------------------------------------------------------------------------------------
//
// オブの選択処理 [seleob.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "seleob.h"
#include "input.h"
#include "camera.h"
#include "mouse.h"
#include "floor.h"
#include "materials.h"
#include "giraffe.h"
#include "wall.h"
#include "Billboard.h"
#include "game.h"

#ifdef _DEBUG
#include "debugproc.h"
#endif // _DEBUG


// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define TEXTURE_SELEOB (1)
#define SELEOB_MOVE (2.0f)
#define SELEOB_SIZE_X (100.0f)
#define SELEOB_SIZE_Y (200.0f)

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexSeleOb(LPDIRECT3DDEVICE9 pDevice);

#ifdef _DEBUG
void Debug_SeleOb(void);
#endif // _DEBUG
// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSeleOb = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureSeleOb = NULL;	// 頂点テクスチャーのポインタ
SELEOB g_SeleOb;

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitSeleob(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 素材ファイル名定義
	char *paSeleObFile[1] =
	{
		{ "data/MODEL/矢印.x" }						// [緑木]00
	};


	// 初期化
	g_SeleOb.Screenpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SeleOb.Worldpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_SeleOb.bUse = false;
	g_SeleOb.type = SELEOBTYPE_NONE;
	g_SeleOb.nNumber = 0;

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,"", &g_pTextureSeleOb);

	// 頂点作成
	MakeVertexSeleOb(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitSeleob(void)
{
	// テクスチャーの開放
	if (g_pTextureSeleOb != NULL)
	{
		g_pTextureSeleOb->Release();
		g_pTextureSeleOb = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateSeleob(void)
{
	CAMERA		*pCamera = GetCamera();						// カメラ情報取得
	GIRAFFE		*pGiraffe = GetGiraffe();					// キリン情報取得
	FLOOR		*pFloor = GetFloor();						// 床情報取得
	WALL		*pWall = GetWall();							// 壁情報取得
	BILLBOARD	*pBillboard = GetBillboard();				// ビルボード情報取得
	MATERIALS	*pMaterials = GetMaterials();				// マテリアル情報取得
	float MouseposX = (float)GetMouseX();					// マウスのx座標を取得
	float MouseposZ = (float)GetMouseY();					// マウスのy座標を取得
	D3DXVECTOR3 WorldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// ワールド座標

	// 頂点情報の設定
	for (int nCntSeleob = ALL_MAX; nCntSeleob >= 0; nCntSeleob--)
	{
		// マテリアル
		if (pMaterials[nCntSeleob].bUse == true && pMaterials[nCntSeleob].bDisp == true)
		{
			// マウスの点をスクリーン座標からワールド座標へ(オブジェクトの存在する平面)
			CalcScreenToXZ(
				&WorldPos,
				MouseposX,
				MouseposZ,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				&pCamera->mtxView,
				&pCamera->mtxProjection,
				pMaterials[nCntSeleob].pos + D3DXVECTOR3(0.0f, pMaterials[nCntSeleob].size.y / 2, 0.0f)
			);

			// 円の当たり判定
			if (Collision_Circle(
				WorldPos, 4.0f,
				pMaterials[nCntSeleob].pos +
				D3DXVECTOR3(0.0f, pMaterials[nCntSeleob].size.y / 2, 0.0f), pMaterials[nCntSeleob].size.x))
			{
				// 左クリックで選択
				if (GetMousePressTrigger(0))
				{
					g_SeleOb.Worldpos = pMaterials[nCntSeleob].pos + D3DXVECTOR3(0.0, 20.0f, 0.0f);	// 方向オブジェクト座標
					g_SeleOb.type = SELEOBTYPE_MATERIALS;							// 選択状態
					g_SeleOb.nNumber = nCntSeleob;								// 変更可能な選択オブジェクトの番号
					g_SeleOb.bUse = true;										// 方向オブジェクト出現
					return;
				}
			}
			else
			{
				// 選択解除
				if (GetMousePressTrigger(0))
				{
					g_SeleOb.type = SELEOBTYPE_NONE;	// 選択状態解除
					g_SeleOb.nNumber = 0;				// 変更可能な選択オブジェクトの番号を初期化
					g_SeleOb.bUse = false;				// 方向オブジェクトを未使用状態へ
				}
			}
		}
	}
	// 頂点情報の設定
	for (int nCntSeleob = ALL_MAX; nCntSeleob >= 0; nCntSeleob--)
	{

		// キリン(エネミー)
		if (pGiraffe[nCntSeleob].bUse == true && pGiraffe[nCntSeleob].bDisp == true)
		{
			// マウスの点をスクリーン座標からワールド座標へ(オブジェクトの存在する平面)
			CalcScreenToXZ(
				&WorldPos,
				MouseposX,
				MouseposZ,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				&pCamera->mtxView,
				&pCamera->mtxProjection,
				pGiraffe[nCntSeleob].pos + D3DXVECTOR3(0.0f, pGiraffe[nCntSeleob].size.y / 2, 0.0f)
			);

			// 円の当たり判定
			if (Collision_Circle(
				WorldPos, 4.0f,
				pGiraffe[nCntSeleob].pos + D3DXVECTOR3(0.0f, pGiraffe[nCntSeleob].size.y / 2, 0.0f), pGiraffe[nCntSeleob].size.x))
			{
				// 左クリックで選択
				if (GetMousePressTrigger(0))
				{
					g_SeleOb.Worldpos = pGiraffe[nCntSeleob].pos + D3DXVECTOR3(0.0, pGiraffe[nCntSeleob].size.y, 0.0f);	// 方向オブジェクト座標
					g_SeleOb.type = SELEOBTYPE_GIRAFFE;							// 選択状態
					g_SeleOb.nNumber = nCntSeleob;								// 変更可能な選択オブジェクトの番号
					g_SeleOb.bUse = true;										// 方向オブジェクト出現
					return;
				}
			}
			else
			{
				// 選択解除
				if (GetMousePressTrigger(0))
				{
					g_SeleOb.type = SELEOBTYPE_NONE;	// 選択状態解除
					g_SeleOb.nNumber = 0;				// 変更可能な選択オブジェクトの番号を初期化
					g_SeleOb.bUse = false;				// 方向オブジェクトを未使用状態へ
				}
			}
		}
	}
	// 頂点情報の設定
	for (int nCntSeleob = ALL_MAX; nCntSeleob >= 0; nCntSeleob--)
	{
		// 床
		if (pFloor[nCntSeleob].bUse == true && pFloor[nCntSeleob].bDisp == true)
		{
			// マウスの点をスクリーン座標からワールド座標へ(オブジェクトの存在する平面)
			CalcScreenToXZ(
				&WorldPos,
				MouseposX,
				MouseposZ,
				SCREEN_WIDTH,
				SCREEN_HEIGHT,
				&pCamera->mtxView,
				&pCamera->mtxProjection,
				pFloor[nCntSeleob].pos
			);

			// 判定
			if (SquareBottom_Judg(
				pFloor[nCntSeleob].pos,
				pFloor[nCntSeleob].size,
				pFloor[nCntSeleob].rot,
				pFloor[nCntSeleob].nBlock_Width,
				pFloor[nCntSeleob].nBlock_Depth,
				WorldPos))
			{
				// 左クリックで選択
				if (GetMousePressTrigger(0))
				{
					g_SeleOb.Worldpos = pFloor[nCntSeleob].pos + D3DXVECTOR3(0.0, 20.0f, 0.0f);	// 方向オブジェクト座標
					g_SeleOb.type = SELEOBTYPE_FLOOR;							// 選択状態
					g_SeleOb.nNumber = nCntSeleob;								// 変更可能な選択オブジェクトの番号
					g_SeleOb.bUse = true;										// 方向オブジェクト出現
					return;
				}
			}
			else
			{
				// 選択解除
				if (GetMousePressTrigger(0))
				{
					g_SeleOb.type = SELEOBTYPE_NONE;	// 選択状態解除
					g_SeleOb.nNumber = 0;				// 変更可能な選択オブジェクトの番号を初期化
					g_SeleOb.bUse = false;				// 方向オブジェクトを未使用状態へ
				}
			}
		}
	}
	// 選択状態にあるオブジェクト
	if (g_SeleOb.type == SELEOBTYPE_MATERIALS)
	{
		// 選択位置
		g_SeleOb.Worldpos = pMaterials[g_SeleOb.nNumber].pos + D3DXVECTOR3(0.0, pMaterials[g_SeleOb.nNumber].size.y, 0.0f);	// 方向オブジェクト座標

		// 選択したオブジェクトの設定
		if (!SeleMaterials(g_SeleOb.nNumber))
		{
			g_SeleOb.bUse = false;
			g_SeleOb.type = SELEOBTYPE_NONE;
		}
	}

	// 選択状態にあるオブジェクト
	else if (g_SeleOb.type == SELEOBTYPE_GIRAFFE)
	{
		// 選択位置
		g_SeleOb.Worldpos = pGiraffe[g_SeleOb.nNumber].pos + D3DXVECTOR3(0.0, pGiraffe[g_SeleOb.nNumber].size.y, 0.0f);	// 方向オブジェクト座標

		// 選択したオブジェクトの設定
		if (!SeleGiraffe(g_SeleOb.nNumber))
		{
			g_SeleOb.bUse = false;
			g_SeleOb.type = SELEOBTYPE_NONE;
		}
	}

	// 選択状態にあるオブジェクト
	else if (g_SeleOb.type == SELEOBTYPE_FLOOR)
	{
		// 選択位置
		g_SeleOb.Worldpos = pFloor[g_SeleOb.nNumber].pos + D3DXVECTOR3(0.0, 20.0f, 0.0f);	// 方向オブジェクト座標

		// 選択したオブジェクトの設定
		if (!SeleFloor(g_SeleOb.nNumber))
		{
			g_SeleOb.bUse = false;
			g_SeleOb.type = SELEOBTYPE_NONE;
		}
	}

#ifdef _DEBUG
	// デバッグ表示
	Debug_SeleOb();
#endif // _DEBUG

}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawSeleob(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice;
	FLOOR	*pFloor = GetFloor();						// 床情報取得

	// デバイスを取得する
	pDevice = GetDevice();

	// 頂点バッファをデバイスのデータストリームにバインド
	pDevice->SetStreamSource(0, g_pVtxBuffSeleOb, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// 使用状態
	if (g_SeleOb.bUse == true)
	{
		// テクスチャー情報
		pDevice->SetTexture(0, g_pTextureSeleOb);
		// 描画処理
		for (int nCntDraw = 0; nCntDraw < SELEOBDICTION_MAX; nCntDraw++)
		{
			// タイトル描画
			//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDraw * 4, 2);
		}
	}
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexSeleOb(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	VERTEX_2D *pVtx;					// 頂点情報へのポイント
	int nCntSeleOb;						// カウント
	D3DXCOLOR col[SELEOBDICTION_MAX] =	// カラー
	{
		D3DXCOLOR(1.0f,0.0f,0.0f,1.0f),	// X(赤)
		D3DXCOLOR(0.0f,1.0f,0.0f,1.0f),	// Y(緑)
		D3DXCOLOR(0.0f,0.0f,1.0f,1.0f),	// Z(青)
	};

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_2D) * 4 * SELEOBDICTION_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D, 
		D3DPOOL_MANAGED,
		&g_pVtxBuffSeleOb,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffSeleOb->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntSeleOb = 0; nCntSeleOb < SELEOBDICTION_MAX; nCntSeleOb++)
	{
		// 頂点の設定
		pVtx[0].pos = D3DXVECTOR3(g_SeleOb.Worldpos.x - SELEOB_SIZE_X, g_SeleOb.Worldpos.y - SELEOB_SIZE_Y, 0.0f);	// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(g_SeleOb.Worldpos.x + SELEOB_SIZE_X, g_SeleOb.Worldpos.y - SELEOB_SIZE_Y, 0.0f);	// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(g_SeleOb.Worldpos.x - SELEOB_SIZE_X, g_SeleOb.Worldpos.y + SELEOB_SIZE_Y, 0.0f);	// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(g_SeleOb.Worldpos.x + SELEOB_SIZE_X, g_SeleOb.Worldpos.y + SELEOB_SIZE_Y, 0.0f);	// 四つ目の頂点

		// 頂点作成
		pVtx[0].rhw = 1.0f;	// 一つ目の頂点作成
		pVtx[1].rhw = 1.0f;	// 二つ目の頂点作成
		pVtx[2].rhw = 1.0f;	// 三つ目の頂点作成
		pVtx[3].rhw = 1.0f;	// 四つ目の頂点作成

		// カラー設定
		pVtx[0].col = col[nCntSeleOb];	// 一つ目の色設定
		pVtx[1].col = col[nCntSeleOb];	// 二つ目の色設定
		pVtx[2].col = col[nCntSeleOb];	// 三つ目の色設定
		pVtx[3].col = col[nCntSeleOb];	// 四つ目の色設定

		// テクスチャの設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// 一つ目のテクスチャ
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);	// 二つ目のテクスチャ
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);	// 三つ目のテクスチャ
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	// 四つ目のテクスチャ

		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffSeleOb->Unlock();
}

#ifdef _DEBUG
// ------------------------------------------------------------------------------------------
// デバッグ表示
// ------------------------------------------------------------------------------------------
void Debug_SeleOb(void)
{
	PrintDebug("/*----------<選択状態>----------*/\n");
	PrintDebug("オブジェクト削除:BACKSPACE\n");
}
#endif // _DEBUG
