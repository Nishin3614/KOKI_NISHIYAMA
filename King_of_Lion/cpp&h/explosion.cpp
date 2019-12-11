// ------------------------------------------------------------------------------------------
//
// 爆発処理 [explosion.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "explosion.h"
#include "polygon.h"
#include "player.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define EXPLOSION_MAX (128)								// 爆発の最大数 

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice);
void CollisionExplosion(int nCntExplosion);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;		// 頂点バッファへのポインタ
LPDIRECT3DTEXTURE9		g_pTextureExplosion[EXPLOSIONTYPE_MAX] = {};		// 頂点テクスチャーのポインタ
EXPLOSION				g_aExplosion[EXPLOSION_MAX];	// 爆発情報

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntExplosion;							// カウント爆発
	char cTexName[EXPLOSIONTYPE_MAX][36] =		// テクスチャー名
	{
		"data/TEXTURE/Thunder.png" ,
		"data/TEXTURE/bullet/hibana.png"
	};

	// 初期設定
	for (nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].size = D3DXVECTOR3(50.0f, 50.0f, 0.0f);
		g_aExplosion[nCntExplosion].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aExplosion[nCntExplosion].fSize = 0.0f;
		g_aExplosion[nCntExplosion].fAlpha = 0.0f;
		g_aExplosion[nCntExplosion].fDestSize = 0;
		g_aExplosion[nCntExplosion].fG = 0;
		g_aExplosion[nCntExplosion].nCounterAnim = 0;
		g_aExplosion[nCntExplosion].nPatternAnim = 0;
		g_aExplosion[nCntExplosion].nLife = 0;
		g_aExplosion[nCntExplosion].type = EXPLOSIONTYPE_THUNDER;
		g_aExplosion[nCntExplosion].nAttack = 0;
		g_aExplosion[nCntExplosion].bUse = false;
	}

	// テクスチャー読み込み
	for (int nCntTex = 0; nCntTex < EXPLOSIONTYPE_MAX; nCntTex++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, cTexName[nCntTex], &g_pTextureExplosion[nCntTex]);
	}

	// 頂点情報の作成
	MakeVertexExplosion(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitExplosion(void)
{
	for (int nCntTex = 0; nCntTex < EXPLOSIONTYPE_MAX; nCntTex++)
	{
		// テクスチャの開放
		if (g_pTextureExplosion[nCntTex] != NULL)
		{
			g_pTextureExplosion[nCntTex]->Release();
			g_pTextureExplosion[nCntTex] = NULL;
		}
	}

	// 頂点バッファの開放
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateExplosion(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポイント
	int nCntExplosion;	// カウント爆発

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++)
	{
		// 使用状態
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			g_aExplosion[nCntExplosion].nCounterAnim++;	// カウンター加算

			// 電撃テクスチャー
			if (g_aExplosion[nCntExplosion].type == EXPLOSIONTYPE_THUNDER)
			{
				// 当たり判定
				CollisionExplosion(nCntExplosion);
				if (g_aExplosion[nCntExplosion].nCounterAnim % 6 == 0)
				{
					// テクスチャ設定
					SetTexExplosion(pVtx,
						ANIMATION_SIX_FTEX * g_aExplosion[nCntExplosion].nPatternAnim,
						ANIMATION_SIX_FTEX + ANIMATION_SIX_FTEX * g_aExplosion[nCntExplosion].nPatternAnim);
					g_aExplosion[nCntExplosion].nPatternAnim = (g_aExplosion[nCntExplosion].nPatternAnim++) % ANIMATION_SIX;	// 6枚パターン創造

					// 爆発終了
					if (g_aExplosion[nCntExplosion].nPatternAnim % ANIMATION_SIX == 0)
					{
						g_aExplosion[nCntExplosion].bUse = false;
					}
				}
			}
			// 火花テクスチャー
			if (g_aExplosion[nCntExplosion].type == EXPLOSIONTYPE_HIBANA)
			{
				// 大きくする
				if (g_aExplosion[nCntExplosion].nCounterAnim <= 20)
				{
					g_aExplosion[nCntExplosion].fSize++;
				}
				// 小さくする
				else if(g_aExplosion[nCntExplosion].nCounterAnim <= 40)
				{
					g_aExplosion[nCntExplosion].fSize--;
				}
				// 爆発終了
				else
				{
					g_aExplosion[nCntExplosion].bUse = false;
				}

			}
		}
		// ポイント合わせ
		pVtx += 4;
	}

	// アンロック
	g_pVtxBuffExplosion->Unlock();
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawExplosion(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	EXPLOSION *pExplosion = &g_aExplosion[0];	// 爆発情報
	D3DXMATRIX mtxRot, mtxTrans, mtxView;		// 回転・位置の計算用変数
	int nCntExplosion;							// カウント爆発

	// Zテストの設定に変更
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);		// 状況に応じてZテストを実行する(テストをパスする)

	// アルファテストの設定に変更
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);		// アルファテスト有効
	pDevice->SetRenderState(D3DRS_ALPHAREF, 200);				// アルファ値の基準設定(基準より大きいと描画する)
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);	// 状況に応じてアルファテストを実行する(新ピクセル > 現在のピクセル)

	for (nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++, pExplosion++)
	{
		if (pExplosion->bUse)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&pExplosion->mtxWorldExplosion);

			// 現在のビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			// 逆行列
			g_aExplosion[nCntExplosion].mtxWorldExplosion._11 = mtxView._11;
			g_aExplosion[nCntExplosion].mtxWorldExplosion._12 = mtxView._21;
			g_aExplosion[nCntExplosion].mtxWorldExplosion._13 = mtxView._31;

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
				pExplosion->rot.y, pExplosion->rot.x, pExplosion->rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&pExplosion->mtxWorldExplosion,	// 1
				&pExplosion->mtxWorldExplosion,					// 2
				&mtxRot);									// 3

			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
				pExplosion->pos.x, pExplosion->pos.y, pExplosion->pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&pExplosion->mtxWorldExplosion,	// 1
				&pExplosion->mtxWorldExplosion,					// 2
				&mtxTrans);									// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &pExplosion->mtxWorldExplosion);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureExplosion[pExplosion->type]);

			// 爆発の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntExplosion * 4, 2);
		}
	}

	// Zテストの設定を通常へ
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);	// 状況に応じてZテストを実行する(新ピクセル <= 現在のピクセル)
															
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);	// アルファテストの設定を戻す

}

// ------------------------------------------------------------------------------------------
// 爆発の設定
// ------------------------------------------------------------------------------------------
void SetExplosion(
	D3DXVECTOR3 pos,	// 位置
	EXPLOSIONTYPE type,	// タイプ
	float fSize, 		// サイズ
	int nAttack			// 攻撃力
)
{
	// 変数宣言
	POLYGON *pPolygon = GetPolygon();	// ポリゴン情報
	int nCntExplosion;		// カウントバレット

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);

	// ブロックループ
	for (nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++)
	{
		// 使用していない状態
		if (!g_aExplosion[nCntExplosion].bUse)
		{
			// 情報更新
			g_aExplosion[nCntExplosion].pos = pos;

			g_aExplosion[nCntExplosion].nAttack = 5 + nAttack;
			g_aExplosion[nCntExplosion].type = type;
			g_aExplosion[nCntExplosion].fSize = fSize;
			g_aExplosion[nCntExplosion].nPatternAnim = 0;
			g_aExplosion[nCntExplosion].nCounterAnim = 0;
			g_aExplosion[nCntExplosion].bUse = true;
			// ポイント更新
			pVtx += nCntExplosion * 4;
			SetPosExplosion(pVtx,nCntExplosion);

			break;
		}
	}

	// アンロック
	g_pVtxBuffExplosion->Unlock();
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexExplosion(LPDIRECT3DDEVICE9 pDevice)
{
	int nCntExplosion;		// カウント爆発

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * EXPLOSION_MAX, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffExplosion, NULL);

	// 頂点情報の設定
	VERTEX_3D *pVtx;	// 頂点情報へのポイント

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	g_pVtxBuffExplosion->Lock(0, 0, (void **)&pVtx, 0);

	// 初期設定
	for (nCntExplosion = 0; nCntExplosion < EXPLOSION_MAX; nCntExplosion++)
	{
		// 頂点座標の設定					
		pVtx[0].pos = D3DXVECTOR3(-g_aExplosion[nCntExplosion].fSize, g_aExplosion[nCntExplosion].fSize, 0.0f);		// 一つ目の頂点
		pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].fSize, g_aExplosion[nCntExplosion].fSize, 0.0f);		// 二つ目の頂点
		pVtx[2].pos = D3DXVECTOR3(-g_aExplosion[nCntExplosion].fSize, -g_aExplosion[nCntExplosion].fSize, 0.0f);	// 三つ目の頂点
		pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].fSize, -g_aExplosion[nCntExplosion].fSize, 0.0f);		// 四つ目の頂点

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 一つ目の頂点
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 二つ目の頂点
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 三つ目の頂点
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);	// 四つ目の頂点

		// カラー
		pVtx[0].col = g_aExplosion[nCntExplosion].col;	// 一つ目の色設定
		pVtx[1].col = g_aExplosion[nCntExplosion].col;	// 二つ目の色設定
		pVtx[2].col = g_aExplosion[nCntExplosion].col;	// 三つ目の色設定
		pVtx[3].col = g_aExplosion[nCntExplosion].col;	// 四つ目の色設定

														// テクスチャー
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);		// 一つ目の色設定
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);	// 二つ目の色設定
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);		// 三つ目の色設定
		pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);	// 四つ目の色設定

		// ポイント合わせ
		pVtx += 4;
	}
	// アンロック
	g_pVtxBuffExplosion->Unlock();
}

// ------------------------------------------------------------------------------------------
// 当たり判定情報
// ------------------------------------------------------------------------------------------
void CollisionExplosion(int nCntExplosion)
{
	// プレイヤー情報取得
	PLAYER *pPlayer = GetPlayer();

	if (pPlayer->status.bColition == false)
	{
		if (SquareBottom_Judg(g_aExplosion[nCntExplosion].pos,
			D3DXVECTOR3(g_aExplosion[nCntExplosion].fSize, g_aExplosion[nCntExplosion].fSize, g_aExplosion[nCntExplosion].fSize),
			D3DXVECTOR3(0.0f, 0.0f, 0.0f),
			1, 1, pPlayer->pos))
		{
			// バッファローのダメージ処理
			HitPlayer(g_aExplosion[nCntExplosion].nAttack);
		}
	}
}

// ------------------------------------------------------------------------------------------
// 爆発の情報
// ------------------------------------------------------------------------------------------
EXPLOSION *GetExplosion(void)
{
	return &g_aExplosion[0];
}

// ------------------------------------------------------------------------------------------
// テクスチャー設定
// ------------------------------------------------------------------------------------------
void SetTexExplosion(
	VERTEX_3D *pVtx,	// 位置頂点情報
	float fBeginTex,	// 初めのテクスチャー
	float fEndTex		// 最後のテクスチャー
)
{
	// テクスチャー
	pVtx[0].tex = D3DXVECTOR2(fBeginTex, 0.0f);	// 一つ目の色設定
 	pVtx[1].tex = D3DXVECTOR2(fEndTex, 0.0f);	// 二つ目の色設定
	pVtx[2].tex = D3DXVECTOR2(fBeginTex, 1.0f);	// 三つ目の色設定
	pVtx[3].tex = D3DXVECTOR2(fEndTex, 1.0f);	// 四つ目の色設定
}

// ------------------------------------------------------------------------------------------
// 頂点座標の設定
// ------------------------------------------------------------------------------------------
void SetPosExplosion(
	VERTEX_3D *pVtx,	// 位置頂点情報
	int nCntExplosion	// カウント
)
{
	// 頂点座標の設定					
	pVtx[0].pos = D3DXVECTOR3(-g_aExplosion[nCntExplosion].fSize, g_aExplosion[nCntExplosion].fSize, 0.0f);		// 一つ目の頂点
	pVtx[1].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].fSize, g_aExplosion[nCntExplosion].fSize, 0.0f);		// 二つ目の頂点
	pVtx[2].pos = D3DXVECTOR3(-g_aExplosion[nCntExplosion].fSize, -g_aExplosion[nCntExplosion].fSize, 0.0f);	// 三つ目の頂点
	pVtx[3].pos = D3DXVECTOR3(g_aExplosion[nCntExplosion].fSize, -g_aExplosion[nCntExplosion].fSize, 0.0f);		// 四つ目の頂点
}
