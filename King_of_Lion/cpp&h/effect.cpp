// ------------------------------------------------------------------------------------------
//
// エフェクト処理 [effect.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "effect.h"

#include "polygon.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define EFFECT_MAX (4096)								// エフェクトの最大数 
#define EFFECT_SIZE (20)							// サイズ
#define EFFECT_PHONT "data/TEXTURE/shadow000.jpg"	// エフェクトの写真

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	// 頂点テクスチャーのポインタ
EFFECT					g_aEffect[EFFECT_MAX];		// エフェクト情報

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntEffect;								// カウントエフェクト

												// 初期設定
	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aEffect[nCntEffect].fAlpha = 0.0f;
		g_aEffect[nCntEffect].fDestSize = 0;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].nType = 0;
		g_aEffect[nCntEffect].fG = 0;
		g_aEffect[nCntEffect].bUse = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, EFFECT_PHONT, &g_pTextureEffect);

	// 頂点情報の作成
	MakeVertexEffect(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitEffect(void)
{
	// テクスチャの開放
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	// 頂点バッファの開放
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateEffect(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポイント
	int nCntEffect;		// カウントエフェクト

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffEffect->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		// 使用状態
		if (g_aEffect[nCntEffect].bUse == true)
		{
			// 半径変化
			g_aEffect[nCntEffect].fSize += g_aEffect[nCntEffect].fDestSize;

			// 重力変化
			g_aEffect[nCntEffect].move.y += g_aEffect[nCntEffect].fG;

			// アルファ値変化
			g_aEffect[nCntEffect].col.a -= g_aEffect[nCntEffect].fAlpha;

			// ポジション変化
			g_aEffect[nCntEffect].pos += g_aEffect[nCntEffect].move;

			g_aEffect[nCntEffect].nLife--;

			if (g_aEffect[nCntEffect].nLife <= 0 || g_aEffect[nCntEffect].fSize <= 0)
			{
				g_aEffect[nCntEffect].bUse = false;
			}

			// 頂点情報の設定
			// 頂点座標の設定					
			pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fSize,
				g_aEffect[nCntEffect].fSize, 0.0f);		// 一つ目の頂点
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fSize,
				g_aEffect[nCntEffect].fSize, 0.0f);		// 二つ目の頂点
			pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fSize,
				-g_aEffect[nCntEffect].fSize, 0.0f);	// 三つ目の頂点
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fSize,
				-g_aEffect[nCntEffect].fSize, 0.0f);		// 四つ目の頂点

			// 頂点カラーの設定
			pVtx[0].col = g_aEffect[nCntEffect].col;	// 一つ目の色設定
			pVtx[1].col = g_aEffect[nCntEffect].col;	// 二つ目の色設定
			pVtx[2].col = g_aEffect[nCntEffect].col;	// 三つ目の色設定
			pVtx[3].col = g_aEffect[nCntEffect].col;	// 四つ目の色設定

		}
		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffEffect->Unlock();

}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawEffect(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	EFFECT *pEffect = &g_aEffect[0];			// エフェクト情報
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// 回転・位置の計算用変数
	int nCntEffect;								// カウントエフェクト

	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// Zテストの設定に変更
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);			// Zバッファへの追加書き込み書き込みを無効
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// 状況に応じてZテストを実行する(テストをパスする)

	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&pEffect->mtxWorldEffect);

			// 現在のビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// 逆行列
			g_aEffect[nCntEffect].mtxWorldEffect._11 = mtxView._11;
			g_aEffect[nCntEffect].mtxWorldEffect._12 = mtxView._21;
			g_aEffect[nCntEffect].mtxWorldEffect._13 = mtxView._31;

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
				pEffect->rot.y, pEffect->rot.x, pEffect->rot.z);	// それぞれの回転量

																	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&pEffect->mtxWorldEffect,	// 1
				&pEffect->mtxWorldEffect,					// 2
				&mtxRot);									// 3


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
				pEffect->pos.x, pEffect->pos.y, pEffect->pos.z);	// それぞれの移動量

																	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&pEffect->mtxWorldEffect,	// 1
				&pEffect->mtxWorldEffect,					// 2
				&mtxTrans);									// 3

															// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pEffect->mtxWorldEffect);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect);

			// エフェクトの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				nCntEffect * 4,
				2);
		}
	}

	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zテストの設定を通常へ
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// Zテスト無効
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// 状況に応じてZテストを実行する(新ピクセル <= 現在のピクセル)
}

// ------------------------------------------------------------------------------------------
// エフェクト基本情報の設定
// ------------------------------------------------------------------------------------------
void SetInformation(
	D3DXVECTOR3 pos,			// 位置
	D3DXCOLOR col,				// 色
	EFFECTAPPEARANCE Appearnce,	// 出現時
	EFFECTMOVETYPE Movetype,	// 移動タイプ
	int nMaxEffect,				// 最大数
	float fSize,				// サイズ
	float fSpeed, 				// 速さ
	int nLife					// ライフ
)
{
	// 変数宣言
	D3DXVECTOR3 Effectpos = D3DXVECTOR3(0.0f,0.0f,0.0f);
	D3DXCOLOR	Effectcol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	float		EffectfSize = 0;

	for (int nCntEffect = 0; nCntEffect < nMaxEffect; nCntEffect++)
	{
		switch (Appearnce)
		{
		case EFFECTAPPEARANCE_NORMAL:
			break;
		case EFFECTAPPEARANCE_RAND:
			break;
		case EFFECTAPPEARANCE_RING:
			SetMove(pos, col,
				Movetype,
				fSize, fSpeed, nLife);

			break;
		}

		switch (Movetype)
		{
		case EFFECTMOVETYPE_NORMAL:
			SetMove(pos, col,
				Movetype,
				fSize, fSpeed, nLife);
			break;
		case EFFECTMOVETYPE_UP:
			SetMove(pos, col,
				Movetype,
				fSize, fSpeed, nLife);
			break;
		case EFFECTMOVETYPE_DOWN:
			SetMove(pos, col,
				Movetype,
				fSize, fSpeed, nLife);
			break;
		case EFFECTMOVETYPE_G:
			SetMove(pos, col,
				Movetype,
				fSize, fSpeed, nLife);
			break;
		case EFFECTMOVETYPE_RAND:
			SetMove(pos, col,
				Movetype,
				fSize, fSpeed, nLife);
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// エフェクト移動の設定
// ------------------------------------------------------------------------------------------
void SetMove(
	D3DXVECTOR3 pos,			// 位置
	D3DXCOLOR col,				// 色
	EFFECTMOVETYPE Movetype,	// 移動タイプ
	float fSize, 				// サイズ
	float fSpeed,				// 速さ
	int nLife					// ライフ
)
{
}

// ------------------------------------------------------------------------------------------
// エフェクトの設定
// ------------------------------------------------------------------------------------------
void SetEffect(
	D3DXVECTOR3 pos,	// 位置
	D3DXVECTOR3 move,	// 移動
	D3DXCOLOR col,		// 色
	float fSize, 		// サイズ
	float fDestSize,	// サイズ変更
	float fG,			// 重力
	int nLife,			// ライフ
	int nType,			// タイプ
	int nMaxEffect		// 最大数
)
{
	// 変数宣言
	POLYGON *pPolygon = GetPolygon();	// ポリゴン情報
	int nCntEffect;		// カウントバレット

						// ブロックループ
	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		// 使用していない状態
		if (!g_aEffect[nCntEffect].bUse)
		{
			// 情報更新
			g_aEffect[nCntEffect].pos = pos;
			g_aEffect[nCntEffect].move = move;
			g_aEffect[nCntEffect].col = col;
			g_aEffect[nCntEffect].fSize = fSize;
			g_aEffect[nCntEffect].fDestSize = fDestSize;
			g_aEffect[nCntEffect].fG = fG;
			g_aEffect[nCntEffect].nLife = nLife;
			g_aEffect[nCntEffect].nType = nType;
			g_aEffect[nCntEffect].bUse = true;
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexEffect(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntEffect;		// カウントエフェクト

						// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EFFECT_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffEffect, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

						// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffEffect->Lock(0, 0, (void **)&pVtx, 0);

	// 初期設定
	for (nCntEffect = 0; nCntEffect < EFFECT_MAX; nCntEffect++)
	{
		// 頂点座標の設定					
		pVtx[0].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fSize, g_aEffect[nCntEffect].fSize, 0.0f);		// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fSize, g_aEffect[nCntEffect].fSize, 0.0f);		// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(-g_aEffect[nCntEffect].fSize, -g_aEffect[nCntEffect].fSize, 0.0f);	// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].fSize, -g_aEffect[nCntEffect].fSize, 0.0f);		// 四つ目の頂点

																										// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 一つ目の頂点
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 二つ目の頂点
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 三つ目の頂点
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);	// 四つ目の頂点

														// カラー
		pVtx[0].col = g_aEffect[nCntEffect].col;	// 一つ目の色設定
		pVtx[1].col = g_aEffect[nCntEffect].col;	// 二つ目の色設定
		pVtx[2].col = g_aEffect[nCntEffect].col;	// 三つ目の色設定
		pVtx[3].col = g_aEffect[nCntEffect].col;	// 四つ目の色設定

													// テクスチャー
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	// 一つ目の色設定
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);	// 二つ目の色設定
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);	// 三つ目の色設定
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);	// 四つ目の色設定

												// ポイント合わせ
		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffEffect->Unlock();
}

// ------------------------------------------------------------------------------------------
// エフェクトの情報
// ------------------------------------------------------------------------------------------
EFFECT *GetEffect(void)
{
	return &g_aEffect[0];
}