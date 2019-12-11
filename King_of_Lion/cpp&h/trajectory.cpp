// ------------------------------------------------------------------------------------------
//
// 軌跡処理 [trajectory.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "trajectory.h"

#include "player.h"

#include "input.h"

#include "giraffe.h"

#include "king_rion.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define TRAJECTORY_PHONT "data/TEXTURE/soil.jpg"	// 軌跡の写真
#define TRAJECTORY_MAX (256)						// 軌跡の最大数
#define TRAJECTORY_TEXMAX (1)					// テクスチャー数

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
TRAJECTORY g_trajectory[TRAJECTORY_MAX];								// 軌跡
LPDIRECT3DTEXTURE9		g_apTextureTrajectory[TRAJECTORY_TEXMAX] = {};	// 頂点テクスチャーのポインタ

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitTrajectory(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	int nCntTrajectory;							// 軌跡のカウント
	int nCntMeshObit;							// 軌跡ポイントのカウント

												// 軌跡カウントの初期化
	for (nCntTrajectory = 0; nCntTrajectory < TRAJECTORY_MAX; nCntTrajectory++)
	{
		// 位置・オフセット・回転の初期設定
		g_trajectory[nCntTrajectory].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_trajectory[nCntTrajectory].ofset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_trajectory[nCntTrajectory].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// フレーム関係の初期設定
		g_trajectory[nCntTrajectory].nCntFram = 0;
		g_trajectory[nCntTrajectory].nStartFram = 0;
		g_trajectory[nCntTrajectory].nEndFram = 0;

		// タイプの初期設定
		g_trajectory[nCntTrajectory].nType = 0;

		// バッファの初期設定
		g_trajectory[nCntTrajectory].pVtxBuffTrajectory = NULL;

		// 軌跡ポイントの初期化
		for (nCntMeshObit = 0;
			nCntMeshObit < MAX_MESHOBIT;
			nCntMeshObit++)
		{
			g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0] = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
			g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1] = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
		}

		// 最大軌跡数の初期化
		g_trajectory[nCntTrajectory].nMaxMeshObit = 0;

		// 総頂点数・インデックス・軌跡の初期設定(計算)
		g_trajectory[nCntTrajectory].nNumberVertexMeshObit =
			g_trajectory[nCntTrajectory].nMaxMeshObit * 2;

		// ビルボード数
		g_trajectory[nCntTrajectory].nNumPolygon =
			g_trajectory[nCntTrajectory].nMaxMeshObit * 2 - 2;

		// 使用状態・表示状態の初期設定
		g_trajectory[nCntTrajectory].bUse = false;
		g_trajectory[nCntTrajectory].bDisp = false;
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TRAJECTORY_PHONT, &g_apTextureTrajectory[0]);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitTrajectory(void)
{
	for (int nCntTrajectory = 0; nCntTrajectory < TRAJECTORY_MAX; nCntTrajectory++)
	{
		// テクスチャの開放
		if (g_apTextureTrajectory[0] != NULL)
		{
			g_apTextureTrajectory[0]->Release();
			g_apTextureTrajectory[0] = NULL;
		}

		// 頂点バッファの開放
		if (g_trajectory[nCntTrajectory].pVtxBuffTrajectory != NULL)
		{
			g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Release();
			g_trajectory[nCntTrajectory].pVtxBuffTrajectory = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateTrajectory(void)
{
	// 変数宣言
	// 頂点情報の設定
	VERTEX_3D *pVtx;									// 頂点情報へのポイント
	int nCntTrajectory;
	D3DXVECTOR3 obit_ofset;								// 軌跡のオフセット

	int nCntMeshObit = 0;								// 軌跡ポイントのカウント1
	int nCntMeshObit2 = 0;								// 軌跡ポイントのカウント2

														// プレイヤー情報
	MODEL *pPlayerModel;								// プレイヤーのモデル情報
	D3DXVECTOR3 pPlayer_Pos = GetPlayer_Pos();			// プレイヤーの位置情報取得
	D3DXVECTOR3 pPlayer_PosOld = GetPlayer_PosOld();	// プレイヤーの前回の位置情報取得
	D3DXVECTOR3 pPlayer_Rot = GetPlayer_Rot();			// プレイヤーの回転情報取得

														// キリン情報
	GIRAFFE *pGiraffe;									// キリン情報

	KING_RION *pKing_rion;								// 百獣の王情報

	// 軌跡カウント
	for (nCntTrajectory = 0; nCntTrajectory < TRAJECTORY_MAX; nCntTrajectory++)
	{
		// 表示状態
		if (g_trajectory[nCntTrajectory].bDisp == true)
		{
			// フレーム数のUP
			g_trajectory[nCntTrajectory].nCntFram++;

			// フレーム数が開始フレーム数以上になったら
			if (g_trajectory[nCntTrajectory].nCntFram >=
				g_trajectory[nCntTrajectory].nStartFram)
			{
				// 軌跡の線の数
				nCntMeshObit2 = g_trajectory[nCntTrajectory].nMaxMeshObit - 1;

				// キャラクタータイプ
				switch (g_trajectory[nCntTrajectory].nType)
				{
				case TRAJECTORYTYPE_PLAYER:
					// 前回の引継ぎのためのループ
					for (nCntMeshObit = nCntMeshObit2 - 1;
						nCntMeshObit >= 0;
						nCntMeshObit--, nCntMeshObit2--)
					{
						// 1つ目の軌跡のポイント
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit2].pos[0] =
							g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0];

						// 2つ目の軌跡のポイント
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit2].pos[1] =
							g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1];
					}

					// プレイヤーのモデル情報の取得
					pPlayerModel = GetPlayerModel();

					// プレイヤーのモデル情報更新
					pPlayerModel += g_trajectory[nCntTrajectory].nParts;

					// 軌跡のオフセットを角度で設定
					obit_ofset.x = sinf(pPlayer_Rot.y) * g_trajectory[nCntTrajectory].ofset.z;
					obit_ofset.z = cosf(pPlayer_Rot.y) * g_trajectory[nCntTrajectory].ofset.z;

					// ポジションの獲得
					g_trajectory[nCntTrajectory].MeshObit[0].pos[0] =
						D3DXVECTOR3(pPlayerModel->mtxWorldModel._41,
							pPlayerModel->mtxWorldModel._42,
							pPlayerModel->mtxWorldModel._43);

					// ポジションの獲得
					g_trajectory[nCntTrajectory].MeshObit[0].pos[1] =
						D3DXVECTOR3(
							pPlayerModel->mtxWorldModel._41 +
							obit_ofset.x,
							pPlayerModel->mtxWorldModel._42,
							pPlayerModel->mtxWorldModel._43 +
							obit_ofset.z);


					// 頂点データの範囲をロックし、頂点バッファへのポインタ
					g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Lock(0, 0, (void **)&pVtx, 0);

					// 軌跡ポイントカウント
					for (nCntMeshObit = 0;
						nCntMeshObit < g_trajectory[nCntTrajectory].nMaxMeshObit;
						nCntMeshObit++)
					{
						// 頂点座標の設定
						// 1
						pVtx[0].pos =
							D3DXVECTOR3(
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].x,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].y,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].z);

						// 2
						pVtx[1].pos =
							D3DXVECTOR3(
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].x,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].y,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].z);

						// 頂点情報のポイント合わせ
						pVtx += 2;
					}

					// アンロック
					g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Unlock();

					// プレイヤーのモデル情報更新前に戻す
					pPlayerModel -= g_trajectory[nCntTrajectory].nParts;


					break;

				case TRAJECTORYTYPE_GIRAFFE:
					// 前回の引継ぎのためのループ
					for (nCntMeshObit = nCntMeshObit2 - 1;
						nCntMeshObit >= 0;
						nCntMeshObit--, nCntMeshObit2--)
					{
						// 1つ目の軌跡のポイント
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit2].pos[0] =
							g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0];

						// 2つ目の軌跡のポイント
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit2].pos[1] =
							g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1];
					}

					// プレイヤーのモデル情報の取得
					pGiraffe = GetGiraffe();

					// キリンの番号更新
					pGiraffe += g_trajectory[nCntTrajectory].nCntObject;

					// 軌跡のオフセットを角度で設定
					obit_ofset.x = sinf(pGiraffe->rot.y) * g_trajectory[nCntTrajectory].ofset.z;
					obit_ofset.z = cosf(pGiraffe->rot.y) * g_trajectory[nCntTrajectory].ofset.z;

					// ポジションの獲得
					g_trajectory[nCntTrajectory].MeshObit[0].pos[0] =
						D3DXVECTOR3(pGiraffe->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._41,
							pGiraffe->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._42,
							pGiraffe->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._43);

					// ポジションの獲得
					g_trajectory[nCntTrajectory].MeshObit[0].pos[1] =
						D3DXVECTOR3(
							pGiraffe->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._41 +
							obit_ofset.x,
							pGiraffe->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._42,
							pGiraffe->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._43 +
							obit_ofset.z);


					// 頂点データの範囲をロックし、頂点バッファへのポインタ
					g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Lock(0, 0, (void **)&pVtx, 0);

					// 軌跡ポイントカウント
					for (nCntMeshObit = 0;
						nCntMeshObit < g_trajectory[nCntTrajectory].nMaxMeshObit;
						nCntMeshObit++)
					{
						// 頂点座標の設定
						// 1
						pVtx[0].pos =
							D3DXVECTOR3(
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].x,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].y,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].z);

						// 2
						pVtx[1].pos =
							D3DXVECTOR3(
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].x,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].y,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].z) +
							g_trajectory[nCntTrajectory].ofset;

						// 頂点情報のポイント合わせ
						pVtx += 2;
					}

					// アンロック
					g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Unlock();

					// キリン情報更新前に戻す
					pGiraffe -= g_trajectory[nCntTrajectory].nCntObject;

					break;

				case TRAJECTORYTYPE_KING_RION:
					// 前回の引継ぎのためのループ
					for (nCntMeshObit = nCntMeshObit2 - 1;
						nCntMeshObit >= 0;
						nCntMeshObit--, nCntMeshObit2--)
					{
						// 1つ目の軌跡のポイント
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit2].pos[0] =
							g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0];

						// 2つ目の軌跡のポイント
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit2].pos[1] =
							g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1];
					}

					// プレイヤーのモデル情報の取得
					pKing_rion = GetKing_Rion();

					// キリンの番号更新
					pKing_rion += g_trajectory[nCntTrajectory].nCntObject;

					// 軌跡のオフセットを角度で設定
					obit_ofset.x = sinf(pKing_rion->rot.y) * g_trajectory[nCntTrajectory].ofset.z;
					obit_ofset.z = cosf(pKing_rion->rot.y) * g_trajectory[nCntTrajectory].ofset.z;

					// ポジションの獲得
					g_trajectory[nCntTrajectory].MeshObit[0].pos[0] =
						D3DXVECTOR3(pKing_rion->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._41,
							pKing_rion->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._42,
							pKing_rion->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._43);

					// ポジションの獲得
					g_trajectory[nCntTrajectory].MeshObit[0].pos[1] =
						D3DXVECTOR3(
							pKing_rion->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._41 +
							obit_ofset.x,
							pKing_rion->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._42,
							pKing_rion->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._43 +
							obit_ofset.z);


					// 頂点データの範囲をロックし、頂点バッファへのポインタ
					g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Lock(0, 0, (void **)&pVtx, 0);

					// 軌跡ポイントカウント
					for (nCntMeshObit = 0;
						nCntMeshObit < g_trajectory[nCntTrajectory].nMaxMeshObit;
						nCntMeshObit++)
					{
						// 頂点座標の設定
						// 1
						pVtx[0].pos =
							D3DXVECTOR3(
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].x,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].y,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].z);

						// 2
						pVtx[1].pos =
							D3DXVECTOR3(
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].x,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].y,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].z) +
							g_trajectory[nCntTrajectory].ofset;

						// 頂点情報のポイント合わせ
						pVtx += 2;
					}

					// アンロック
					g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Unlock();

					// キリン情報更新前に戻す
					pKing_rion -= g_trajectory[nCntTrajectory].nCntObject;

					break;

				default:
					break;
				}

				// フレーム数が終了カウント以上になったら
				if (g_trajectory[nCntTrajectory].nCntFram >=
					g_trajectory[nCntTrajectory].nEndFram)
				{
					// 初期化
					g_trajectory[nCntTrajectory].nCntFram = 0;
					g_trajectory[nCntTrajectory].bDisp = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawTrajectory(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

												// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;

	int nCntTrajectory;

	// 両面を描画
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// レンダーステート(加算合成処理)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntTrajectory = 0; nCntTrajectory < TRAJECTORY_MAX; nCntTrajectory++)
	{
		if (g_trajectory[nCntTrajectory].bDisp == true)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&g_trajectory[nCntTrajectory].mtxWorld);

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// 総合の入れ物
				g_trajectory[nCntTrajectory].rot.y, g_trajectory[nCntTrajectory].rot.x, g_trajectory[nCntTrajectory].rot.z);	// それぞれの回転量

																																// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_trajectory[nCntTrajectory].mtxWorld,	// 1
				&g_trajectory[nCntTrajectory].mtxWorld,					// 2
				&mtxRot);							// 3


													// 位置を反映 //
													// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,						// 総合の入れ物
				g_trajectory[nCntTrajectory].pos.x,
				g_trajectory[nCntTrajectory].pos.y,
				g_trajectory[nCntTrajectory].pos.z);	// それぞれの移動量

														// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_trajectory[nCntTrajectory].mtxWorld,	// 1
				&g_trajectory[nCntTrajectory].mtxWorld,					// 2
				&mtxTrans);							// 3

													// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_trajectory[nCntTrajectory].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_trajectory[nCntTrajectory].pVtxBuffTrajectory, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_apTextureTrajectory[0]);

			// 軌跡の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, g_trajectory[nCntTrajectory].nNumPolygon);
		}
	}

	// 通常ブレンド
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// 裏面(左回り)でカリングする
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

// ------------------------------------------------------------------------------------------
// 軌跡の作成
// ------------------------------------------------------------------------------------------
int SetTrajectory(
	D3DXVECTOR3 ofset,
	int nParts,
	int MaxMeshObit,
	int StartFram,
	int EndFram,
	int	Type)
{
	// カウント軌跡
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	VERTEX_3D *pVtx;
	int	nCntTrajectory = 0;
	int	nCntMeshObit;

	// 軌跡カウント
	for (nCntTrajectory = 0; nCntTrajectory < TRAJECTORY_MAX; nCntTrajectory++)
	{
		if (g_trajectory[nCntTrajectory].bUse == false)
		{
			// オフセット
			g_trajectory[nCntTrajectory].ofset = ofset;

			// パーツ情報
			g_trajectory[nCntTrajectory].nParts = nParts;

			// 最大軌跡数
			g_trajectory[nCntTrajectory].nMaxMeshObit = MaxMeshObit;

			// 開始フレーム
			g_trajectory[nCntTrajectory].nStartFram = StartFram;

			// 終了フレーム
			g_trajectory[nCntTrajectory].nEndFram = EndFram;

			// タイプ
			g_trajectory[nCntTrajectory].nType = Type;

			// 総頂点数・インデックス・軌跡の初期設定(計算)
			g_trajectory[nCntTrajectory].nNumberVertexMeshObit =
				g_trajectory[nCntTrajectory].nMaxMeshObit * 2;

			// ビルボード数
			g_trajectory[nCntTrajectory].nNumPolygon =
				g_trajectory[nCntTrajectory].nMaxMeshObit * 2 - 2;

			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(
				sizeof(VERTEX_3D) * g_trajectory[nCntTrajectory].nNumberVertexMeshObit,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_trajectory[nCntTrajectory].pVtxBuffTrajectory,
				NULL);

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Lock(0, 0, (void **)&pVtx, 0);

			// 軌跡ポイントカウント
			for (nCntMeshObit = 0;
				nCntMeshObit < g_trajectory[nCntTrajectory].nMaxMeshObit;
				nCntMeshObit++)
			{
				// 頂点座標の設定
				// 1
				pVtx[0].pos =
					D3DXVECTOR3(
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].x,
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].y,
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].z);

				// 法線ベクトルの設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 色の設定
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				// テクスチャーの設定
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

				// 2
				pVtx[1].pos =
					D3DXVECTOR3(
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].x,
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].y,
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].z) +
					g_trajectory[nCntTrajectory].ofset;

				// 法線ベクトルの設定
				pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// 色の設定
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				// テクスチャーの設定
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);

				pVtx += 2;
			}

			// アンロック
			g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Unlock();

			// 使用状態
			g_trajectory[nCntTrajectory].bUse = true;

			// ループから抜け出す
			break;
		}
	}

	return nCntTrajectory;
}

// ------------------------------------------------------------------------------------------
// 軌跡の発生
// ------------------------------------------------------------------------------------------
void OccurrenceTrajectory(int IdTrajectory, int nCnt)
{
	// プレイヤーのモデル情報
	MODEL *pPlayerModel;

	// キリンの情報
	GIRAFFE *pGiraffe;

	// 百獣の王の情報
	MODEL *pKing_Rion_Model;

	// 頂点情報
	VERTEX_3D *pVtx;

	// キャラクター番号
	g_trajectory[IdTrajectory].nCntObject = nCnt;

	// 初期化
	g_trajectory[IdTrajectory].nCntFram = 0;
	g_trajectory[IdTrajectory].bDisp = false;

	switch (g_trajectory[IdTrajectory].nType)
	{
	case TRAJECTORYTYPE_PLAYER:
		// プレイヤーのモデル情報の取得
		pPlayerModel = GetPlayerModel();

		// プレイヤーのモデル情報更新
		pPlayerModel += g_trajectory[IdTrajectory].nParts;

		// 頂点データの範囲をロックし、頂点バッファへのポインタ
		g_trajectory[IdTrajectory].pVtxBuffTrajectory->Lock(0, 0, (void **)&pVtx, 0);

		// 軌跡ポイントカウント
		for (int nCntMeshObit = 0;
			nCntMeshObit < g_trajectory[IdTrajectory].nMaxMeshObit;
			nCntMeshObit++)
		{
			// ポジションの獲得
			g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0] =
				D3DXVECTOR3(pPlayerModel->mtxWorldModel._41,
					pPlayerModel->mtxWorldModel._42,
					pPlayerModel->mtxWorldModel._43);


			// ポジションの獲得
			g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1] =
				D3DXVECTOR3(pPlayerModel->mtxWorldModel._41,
					pPlayerModel->mtxWorldModel._42,
					pPlayerModel->mtxWorldModel._43);


			// 頂点座標の設定
			// 1
			pVtx[0].pos =
				D3DXVECTOR3(
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].x,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].y,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].z);

			// 2
			pVtx[1].pos =
				D3DXVECTOR3(
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].x,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].y,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].z) +
				g_trajectory[IdTrajectory].ofset;

			// 頂点情報のポイント合わせ
			pVtx += 2;
		}
		break;
	case TRAJECTORYTYPE_GIRAFFE:
		// キリン情報の取得
		pGiraffe = GetGiraffe();

		// キリンの情報更新
		pGiraffe += g_trajectory[IdTrajectory].nCntObject;

		// 頂点データの範囲をロックし、頂点バッファへのポインタ
		g_trajectory[IdTrajectory].pVtxBuffTrajectory->Lock(0, 0, (void **)&pVtx, 0);

		// 軌跡ポイントカウント
		for (int nCntMeshObit = 0;
			nCntMeshObit < g_trajectory[IdTrajectory].nMaxMeshObit;
			nCntMeshObit++)
		{
			// ポジションの獲得
			g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0] =
				D3DXVECTOR3(pGiraffe->model[g_trajectory[IdTrajectory].nParts].mtxWorldModel._41,
					pGiraffe->model[g_trajectory[IdTrajectory].nParts].mtxWorldModel._42,
					pGiraffe->model[g_trajectory[IdTrajectory].nParts].mtxWorldModel._43);


			// ポジションの獲得
			g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1] =
				D3DXVECTOR3(pGiraffe->model[g_trajectory[IdTrajectory].nParts].mtxWorldModel._41,
					pGiraffe->model[g_trajectory[IdTrajectory].nParts].mtxWorldModel._42,
					pGiraffe->model[g_trajectory[IdTrajectory].nParts].mtxWorldModel._43);


			// 頂点座標の設定
			// 1
			pVtx[0].pos =
				D3DXVECTOR3(
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].x,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].y,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].z);

			// 2
			pVtx[1].pos =
				D3DXVECTOR3(
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].x,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].y,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].z) +
				g_trajectory[IdTrajectory].ofset;

			// 頂点情報のポイント合わせ
			pVtx += 2;
		}
		break;

	case TRAJECTORYTYPE_KING_RION:
		// 百獣の王のモデル情報の取得
		pKing_Rion_Model = GetKing_Rion_Model(nCnt);

		// 百獣の王のモデル更新
		pKing_Rion_Model += g_trajectory[IdTrajectory].nParts;

		// 頂点データの範囲をロックし、頂点バッファへのポインタ
		g_trajectory[IdTrajectory].pVtxBuffTrajectory->Lock(0, 0, (void **)&pVtx, 0);

		// 軌跡ポイントカウント
		for (int nCntMeshObit = 0;
			nCntMeshObit < g_trajectory[IdTrajectory].nMaxMeshObit;
			nCntMeshObit++)
		{
			// ポジションの獲得
			g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0] =
				D3DXVECTOR3(
					pKing_Rion_Model->mtxWorldModel._41,
					pKing_Rion_Model->mtxWorldModel._42,
					pKing_Rion_Model->mtxWorldModel._43);


			// ポジションの獲得
			g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1] =
				D3DXVECTOR3(
					pKing_Rion_Model->mtxWorldModel._41,
					pKing_Rion_Model->mtxWorldModel._42,
					pKing_Rion_Model->mtxWorldModel._43);

			// 頂点座標の設定
			// 1
			pVtx[0].pos =
				D3DXVECTOR3(
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].x,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].y,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].z);

			// 2
			pVtx[1].pos =
				D3DXVECTOR3(
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].x,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].y,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].z) +
				g_trajectory[IdTrajectory].ofset;

			// 頂点情報のポイント合わせ
			pVtx += 2;
		}
		break;
	}
	g_trajectory[IdTrajectory].bDisp = true;
}

// ------------------------------------------------------------------------------------------
// 軌跡の消去情報
// ------------------------------------------------------------------------------------------
void DeleteTrajectory(int nIdTrajectory)
{
	g_trajectory[nIdTrajectory].bUse = false;
	g_trajectory[nIdTrajectory].bDisp = false;
}

// ------------------------------------------------------------------------------------------
// 軌跡情報
// ------------------------------------------------------------------------------------------
TRAJECTORY *GetTrajectory(void)
{
	return &g_trajectory[0];
}