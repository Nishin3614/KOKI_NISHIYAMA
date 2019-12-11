// ------------------------------------------------------------------------------------------
//
// ビルボード処理 [billboard.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "Billboard.h"
#include "input.h"
#include "game.h"
#include "camera.h"
// 当たり判定用	//
#include "player.h"
#include "giraffe.h"
#include "buffalo.h"
#include "king_rion.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define BILLBOARD_MAX (128)
#define BILLBOARD_SIZE (10)
#define BILLBOARD_DISTANCE (0.0f)

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice);
void Update_MakeVerTexBillboard(void);
void LoadBillboard(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;						// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_paTextureBillboard[BILLBOARDTYPE_MAX] = {};	// 頂点テクスチャーのポインタ
BILLBOARD g_aBillboard[BILLBOARD_MAX];									// ビルボード

// テクスチャーファイル名定義
const char *g_paBillboardTexFile[BILLBOARDTYPE_MAX] =
{
	{ "data/TEXTURE/GAME/tree.png" },
	{ "data/TEXTURE/ui/敵を倒すとどうなるか.png" },
	{ "data/TEXTURE/ui/木をたたくとどうなるか.png" },
	{ "data/TEXTURE/ui/子ライオンからの説明.png" }
};

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		// 位置・回転の初期設定
		g_aBillboard[nCntBillboard].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBillboard[nCntBillboard].nType = BILLBOARDTYPE_WOOD;
		g_aBillboard[nCntBillboard].fAlpha = 1.0f;
		g_aBillboard[nCntBillboard].bUse = false;
		g_aBillboard[nCntBillboard].bDisp = false;
	}

	// ビルボードを読み込む
	LoadBillboard();

	// 頂点情報の作成
	MakeVertexBillboard(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitBillboard(void)
{
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARDTYPE_MAX; nCntBillboard++)
	{
		// テクスチャの開放
		if (g_paTextureBillboard[nCntBillboard] != NULL)
		{
			g_paTextureBillboard[nCntBillboard]->Release();
			g_paTextureBillboard[nCntBillboard] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffBillboard != NULL)
	{
		g_pVtxBuffBillboard->Release();
		g_pVtxBuffBillboard = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateBillboard(void)
{
	CAMERA		*pCamera = GetCamera();			// カメラ情報取得
	D3DXVECTOR3	pPlayer_Pos = GetPlayer_Pos();	// プレイヤー情報取得
	D3DXVECTOR3	diff[2];				// それぞれの頂点距離
	float		fDiff[2];				// 距離

	// カウントビルボード
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		// 使用状態
		if (g_aBillboard[nCntBillboard].bUse == true)
		{
			if (g_aBillboard[nCntBillboard].nType == BILLBOARDTYPE_WOOD)
			{
				// カメラの視点とビルボードの距離 //
				// 頂点それぞれの距離
				diff[0] = g_aBillboard[nCntBillboard].pos - pCamera->posV;

				// 距離の計算
				fDiff[0] = sqrtf(
					diff[0].x * diff[0].x +
					diff[0].y * diff[0].y +
					diff[0].z * diff[0].z);

				// カメラの視点とプレイヤーの距離 //
				// 頂点それぞれの距離
				diff[1] = pPlayer_Pos - pCamera->posV;

				// 距離の計算
				fDiff[1] = sqrtf(
					diff[1].x * diff[1].x +
					diff[1].y * diff[1].y +
					diff[1].z * diff[1].z);

				// プレイヤーのほうが奥に描画されているとき
				if (fDiff[0] - fDiff[1] < 0)
				{
					g_aBillboard[nCntBillboard].fAlpha = 0.8f;
				}

				// それ以外
				else
				{
					g_aBillboard[nCntBillboard].fAlpha = 1.0f;
				}
			}

			else
			{

			}
		}
	}

	// 頂点情報更新
	Update_MakeVerTexBillboard();
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	BILLBOARD *pBillboard = &g_aBillboard[0];

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;

	// ビューマトリックスの代入用
	D3DXMATRIX mtxView;

	// アルファテストの設定に変更
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテスト有効
	pDevice->SetRenderState(D3DRS_ALPHAREF, 150);				// アルファ値の基準設定(基準より大きいと描画する)
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 状況に応じてアルファテストを実行する(新ピクセル > 現在のピクセル)
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				// ライティングモード有効

	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++, pBillboard++)
	{
		if (pBillboard->bDisp)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&pBillboard->mtxWorldBillboard);

			// 現在のビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// 逆行列
			g_aBillboard[nCntBillboard].mtxWorldBillboard._11 = mtxView._11;
			g_aBillboard[nCntBillboard].mtxWorldBillboard._12 = mtxView._21;
			g_aBillboard[nCntBillboard].mtxWorldBillboard._13 = mtxView._31;
			g_aBillboard[nCntBillboard].mtxWorldBillboard._21 = mtxView._12;
			g_aBillboard[nCntBillboard].mtxWorldBillboard._22 = mtxView._22;
			g_aBillboard[nCntBillboard].mtxWorldBillboard._23 = mtxView._32;

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
				pBillboard->rot.y, pBillboard->rot.x, pBillboard->rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&pBillboard->mtxWorldBillboard,	// 1
				&pBillboard->mtxWorldBillboard,					// 2
				&mtxRot);							// 3

			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
				pBillboard->pos.x, pBillboard->pos.y, pBillboard->pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&pBillboard->mtxWorldBillboard,	// 1
				&pBillboard->mtxWorldBillboard,					// 2
				&mtxTrans);							// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pBillboard->mtxWorldBillboard);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_paTextureBillboard[g_aBillboard[nCntBillboard].nType]);

			// ビルボードの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBillboard * 4, 2);
		}
	}

	// アルファテストの設定を戻す
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);						// ライティングモード有効

}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexBillboard(LPDIRECT3DDEVICE9 pDevice)
{
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * 4 * BILLBOARD_MAX, 
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED, 
		&g_pVtxBuffBillboard,
		NULL);

	for (int nCntBillboardTexture = 0; nCntBillboardTexture < BILLBOARDTYPE_MAX; nCntBillboardTexture++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			g_paBillboardTexFile[nCntBillboardTexture],
			&g_paTextureBillboard[nCntBillboardTexture]);
	}

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);
			
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(
			-g_aBillboard[nCntBillboard].size.x,
			g_aBillboard[nCntBillboard].size.y, 0);	// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(
			g_aBillboard[nCntBillboard].size.x,
			g_aBillboard[nCntBillboard].size.y, 0);	// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(
			-g_aBillboard[nCntBillboard].size.x,
			-g_aBillboard[nCntBillboard].size.y, 0);	// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(
			g_aBillboard[nCntBillboard].size.x,
			-g_aBillboard[nCntBillboard].size.y, 0);	// 四つ目の頂点

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 一つ目の頂点
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 二つ目の頂点
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 三つ目の頂点
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 四つ目の頂点

		// カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 一つ目の色設定
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 二つ目の色設定
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 三つ目の色設定
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 四つ目の色設定

		// テクスチャー
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// 一つ目の色設定
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);	// 二つ目の色設定
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);	// 三つ目の色設定
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	// 四つ目の色設定

		// ポイント合わせ
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffBillboard->Unlock();
}

// ------------------------------------------------------------------------------------------
// ビルボードの頂点情報更新
// ------------------------------------------------------------------------------------------
void Update_MakeVerTexBillboard(void)
{
	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);

	// カウントビルボード
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		// 使用状態の場合
		if (g_aBillboard[nCntBillboard].bUse == true)
		{
			// カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBillboard[nCntBillboard].fAlpha);	// 一つ目の色設定
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBillboard[nCntBillboard].fAlpha);	// 二つ目の色設定
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBillboard[nCntBillboard].fAlpha);	// 三つ目の色設定
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aBillboard[nCntBillboard].fAlpha);	// 四つ目の色設定
		}

		// 頂点情報更新
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffBillboard->Unlock();

}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void SetBillboard(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nType)
{
	VERTEX_3D * pVtx;	// 頂点情報

						// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);

	// 弾ループ
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		// 使用していない状態
		if (!g_aBillboard[nCntBillboard].bUse)
		{
			// 情報更新
			g_aBillboard[nCntBillboard].pos = pos;

			// サイズ
			g_aBillboard[nCntBillboard].size = size;

			// タイプ
			g_aBillboard[nCntBillboard].nType = nType;

			// 使用状態へ
			g_aBillboard[nCntBillboard].bUse = true;

			// 頂点情報の更新
			pVtx += nCntBillboard * 4;

			// 頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(
				-g_aBillboard[nCntBillboard].size.x,
				g_aBillboard[nCntBillboard].size.y, 0);	// 一つ目の頂点
			pVtx[1].pos = D3DXVECTOR3(
				g_aBillboard[nCntBillboard].size.x,
				g_aBillboard[nCntBillboard].size.y, 0);	// 二つ目の頂点
			pVtx[2].pos = D3DXVECTOR3(
				-g_aBillboard[nCntBillboard].size.x,
				-g_aBillboard[nCntBillboard].size.y, 0);	// 三つ目の頂点
			pVtx[3].pos = D3DXVECTOR3(
				g_aBillboard[nCntBillboard].size.x,
				-g_aBillboard[nCntBillboard].size.y, 0);	// 四つ目の頂点

															// カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// 一つ目の色設定
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// 二つ目の色設定
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// 三つ目の色設定
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// 四つ目の色設定

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// ビルボードの位置更新
// ------------------------------------------------------------------------------------------
void SetPositionBillboard(int nType, D3DXVECTOR3 pos)
{
	// 変数宣言
	VERTEX_3D *pVtx;			// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);

	// カウントビルボード
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		// 使用状態
		if (g_aBillboard[nCntBillboard].bUse == true)
		{
			// タイプ
			if (g_aBillboard[nCntBillboard].nType == nType)
			{
				// 情報更新
				g_aBillboard[nCntBillboard].pos = pos;


				g_aBillboard[nCntBillboard].bDisp = true;

				// カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 一つ目の色設定
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 二つ目の色設定
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 三つ目の色設定
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// 四つ目の色設定
			}
		}

		// ポイント合わせ
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffBillboard->Unlock();
}

// ------------------------------------------------------------------------------------------
// ビルボード消滅処理
// ------------------------------------------------------------------------------------------
void DeleteBillboard(int nType)
{
	// 変数宣言
	VERTEX_3D *pVtx;			// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffBillboard->Lock(0, 0, (void **)&pVtx, 0);

	// カウントビルボード
	for (int nCntBillboard = 0; nCntBillboard < BILLBOARD_MAX; nCntBillboard++)
	{
		// 使用状態
		if (g_aBillboard[nCntBillboard].bUse == true)
		{
			// タイプ
			if (g_aBillboard[nCntBillboard].nType == nType)
			{
				g_aBillboard[nCntBillboard].bDisp = false;

				// カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// 一つ目の色設定
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// 二つ目の色設定
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// 三つ目の色設定
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);	// 四つ目の色設定

				break;
			}
		}

		// ポイント合わせ
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffBillboard->Unlock();
}

// ------------------------------------------------------------------------------------------
// ビルボード読み込み処理
// ------------------------------------------------------------------------------------------
void LoadBillboard(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad = 0;		// カウントロード
	int nCntBillboard = 0;		// カウント床
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	// チュートリアル用
	if (GetMode() == MODE_TUTORIAL)
	{
		pFile = fopen("data/SAVE/TUTORIAL/billboard.txt", "r");
	}

	// ボス用
	else if (GetGameStete() == GAMESTATE_BOSS)
	{
		// ファイル開
		pFile = fopen("data/SAVE/GAME_BOSS/billboard.txt", "r");
	}

	// それ以外
	else
	{
		pFile = fopen("data/SAVE/billboard.txt", "r");
	}

	// 開けた
	if (pFile != NULL)
	{
		// スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);	// 一文を読み込む
			sscanf(cRaedText, "%s", &cHeadText);		// 比較用テクストに文字を代入
		}

		// スクリプトだったら
		if (strcmp(cHeadText, "SCRIPT") == 0)
		{
			// エンドスクリプトが来るまでループ
			while (strcmp(cHeadText, "END_SCRIPT") != 0)
			{
				fgets(cRaedText, sizeof(cRaedText), pFile);
				sscanf(cRaedText, "%s", &cHeadText);

				// モデルセットが来たら
				if (strcmp(cHeadText, "BILLBOARDSET") == 0)
				{

					// エンドモデルセットが来るまでループ
					while (strcmp(cHeadText, "END_BILLBOARDSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 種類情報読み込み
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, 
								&g_aBillboard[nCntBillboard].nType);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, 
								&g_aBillboard[nCntBillboard].pos.x, 
								&g_aBillboard[nCntBillboard].pos.y,
								&g_aBillboard[nCntBillboard].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_aBillboard[nCntBillboard].rot.x,
								&g_aBillboard[nCntBillboard].rot.y,
								&g_aBillboard[nCntBillboard].rot.z);
						}

						// サイズ情報読み込み
						else if (strcmp(cHeadText, "SIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, 
								&g_aBillboard[nCntBillboard].size.x, 
								&g_aBillboard[nCntBillboard].size.y,
								&g_aBillboard[nCntBillboard].size.z);
						}
					}

					// 使用状態・表示状態
					g_aBillboard[nCntBillboard].bUse = true;
					g_aBillboard[nCntBillboard].bDisp = true;

					// 床カウントの更新
					nCntBillboard++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "ビルボードデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// 弾情報
// ------------------------------------------------------------------------------------------
BILLBOARD *GetBillboard(void) 
{
	return &g_aBillboard[0];
}