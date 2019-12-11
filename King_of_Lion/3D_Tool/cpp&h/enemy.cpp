// ------------------------------------------------------------------------------------------
//
// 敵処理 [enemy.cpp]
// Author : KOKI_NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "enemy.h"
#include "mouse.h"
#include "input.h"
#include "arrangement.h"
#include "game.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define ENEMY_PHONT "data/TEXTURE/woods.png"
#define ENEMY_SIZE (10)

// ------------------------------------------------------------------------------------------
// 列挙型
// ------------------------------------------------------------------------------------------
// ---------モーションセット---------- //
typedef enum
{
	ENEMYTYPE_00_ROAD1 = 0,
	ENEMYTYPE_MAX
} ENEMYTYPE;
// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice);
bool ColisionEnemy(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, 
	D3DXVECTOR3 * move, D3DXVECTOR3 * size, ENEMY ** ppEnemyRet);

void Update_ChangeEnemy(void);
void Update_DispEnemy(void);
void Update_DestroyEnemy(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureEnemy = NULL;	// 頂点テクスチャーのポインタ

ENEMY g_aenemy[ALL_MAX];							// 敵
ENEMYSET g_EnemySet;							// 配置情報

// 3D用のセーブファイル
const char g_cEnemyFIleName_3D[5][128] =
{
	"data/SAVE/3D/Stage_1/enemy.txt",
	"data/SAVE/3D/Stage_2/enemy.txt",
	"data/SAVE/3D/Stage_3/enemy.txt",
	"data/SAVE/3D/Stage_4/enemy.txt",
	"data/SAVE/3D/Stage_5/enemy.txt"
};

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	for (int nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		g_aenemy[nCntEnemy].pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aenemy[nCntEnemy].rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aenemy[nCntEnemy].move			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_aenemy[nCntEnemy].size			= D3DXVECTOR3(100.0f, 100.0f, 0.0f);
		g_aenemy[nCntEnemy].nEnemy_Depth	= 1;
		g_aenemy[nCntEnemy].nEnemy_Width	= 1;

		// 原点の計算
		g_aenemy[nCntEnemy].OriginEnemy	= D3DXVECTOR3(
			g_aenemy[nCntEnemy].size.x * -0.5f * g_aenemy[nCntEnemy].nEnemy_Width,
			g_aenemy[nCntEnemy].size.y * g_aenemy[nCntEnemy].nEnemy_Depth,
			0.0f);

		// 総頂点の計算
		g_aenemy[nCntEnemy].nNumberVertexMeshField = 
			(g_aenemy[nCntEnemy].nEnemy_Depth + 1) *
			(g_aenemy[nCntEnemy].nEnemy_Width + 1);

		// インデックスの計算
		g_aenemy[nCntEnemy].nNumIndex = 
			(g_aenemy[nCntEnemy].nEnemy_Width + 1) * 2 *
			g_aenemy[nCntEnemy].nEnemy_Depth + 
			2 * (g_aenemy[nCntEnemy].nEnemy_Depth - 1);

		// ポリゴンの計算
		g_aenemy[nCntEnemy].nNumPolygon = 
			g_aenemy[nCntEnemy].nEnemy_Depth * 
			g_aenemy[nCntEnemy].nEnemy_Width * 2 + 
			4 * (g_aenemy[nCntEnemy].nEnemy_Depth - 1);

		g_aenemy[nCntEnemy].nNumber = 0;

		// 使用・表示状態の初期化
		g_aenemy[nCntEnemy].bUse = false;
		g_aenemy[nCntEnemy].bDisp = false;
	}

	// 配置情報の初期化
	g_EnemySet.nEnemy_Depth = 1;
	g_EnemySet.nEnemy_Width = 1;
	g_EnemySet.nType = 0;
	g_EnemySet.size = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	g_EnemySet.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, ENEMY_PHONT, &g_pTextureEnemy);

	// 敵のデータ読込
	LoadEnemy();

	// 頂点情報の作成
	MakeVertexEnemy(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitEnemy(void)
{
	// テクスチャの開放
	if (g_pTextureEnemy != NULL)
	{
		g_pTextureEnemy->Release();
		g_pTextureEnemy = NULL;
	}

	for (int nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		// 頂点バッファの開放
		if (g_aenemy[nCntEnemy].pVtxBuff != NULL)
		{
			g_aenemy[nCntEnemy].pVtxBuff->Release();
			g_aenemy[nCntEnemy].pVtxBuff = NULL;
		}

		// 頂点インデックスの開放
		if (g_aenemy[nCntEnemy].pIndex != NULL)
		{
			g_aenemy[nCntEnemy].pIndex->Release();
			g_aenemy[nCntEnemy].pIndex = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateEnemy(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	LONG		pMouseX = 0;

	// マウスの情報取得
	pMouseX = GetMouseX();

	// 削除処理
	Update_DestroyEnemy();

	// 床の変化処理
	Update_ChangeEnemy();

	// 配置時の場所把握
	Update_DispEnemy();

	// 範囲
	if (pMouseX < 1000.0f)
	{
		// 画面内を左クリックで、カーソルの位置に敵配置
		if (GetMousePressTrigger(0))
		{
			SetEnemy(
				pArrangement->pos,
				pArrangement->rot,
				g_EnemySet.size,
				g_EnemySet.nEnemy_Depth,
				g_EnemySet.nEnemy_Width,
				g_EnemySet.nType);
		}
	}
}

// ------------------------------------------------------------------------------------------
// 床の変化処理
// ------------------------------------------------------------------------------------------
void Update_ChangeEnemy(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	// 縦のブロック数を増減
	if (GetKeyboardSpeacePress(DIK_U))
	{
		g_EnemySet.nEnemy_Depth++;
	}

	else if (GetKeyboardSpeacePress(DIK_J))
	{
		g_EnemySet.nEnemy_Depth--;
	}

	// 横のブロック数を増減
	if (GetKeyboardSpeacePress(DIK_Y))
	{
		g_EnemySet.nEnemy_Width++;
	}

	else if (GetKeyboardSpeacePress(DIK_H))
	{
		g_EnemySet.nEnemy_Width--;
	}

	// 縦のサイズを増減
	if (GetKeyboardSpeacePress(DIK_O))
	{
		g_EnemySet.size.y += ENEMY_SIZE;
	}

	else if (GetKeyboardSpeacePress(DIK_L))
	{
		g_EnemySet.size.y -= ENEMY_SIZE;
	}

	// 横のサイズを増減
	if (GetKeyboardSpeacePress(DIK_I))
	{
		g_EnemySet.size.x += ENEMY_SIZE;
	}

	else if (GetKeyboardSpeacePress(DIK_K))
	{
		g_EnemySet.size.x -= ENEMY_SIZE;
	}

	// 回転量を増減
	if (GetKeyboardSpeacePress(DIK_T))
	{
		g_EnemySet.rot.y += 1.57f;

		// -3.14～3.14に回転量を固定させる
		if (g_EnemySet.rot.y < -D3DX_PI) g_EnemySet.rot.y += D3DX_PI * 2;		// -3.14未満

		else if (g_EnemySet.rot.y > D3DX_PI) g_EnemySet.rot.y += -D3DX_PI * 2;	// 3.14超過

	}

	else if (GetKeyboardSpeacePress(DIK_G))
	{
		g_EnemySet.rot.y -= 1.57f;

		// -3.14～3.14に回転量を固定させる
		if (g_EnemySet.rot.y < -D3DX_PI) g_EnemySet.rot.y += D3DX_PI * 2;		// -3.14未満

		else if (g_EnemySet.rot.y > D3DX_PI) g_EnemySet.rot.y += -D3DX_PI * 2;	// 3.14超過
	}

	// タイプの変化
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_EnemySet.nType++;

		// 限界値まで行ったら
		if (g_EnemySet.nType >= ENEMYTYPE_MAX)
		{
			// 初期化
			g_EnemySet.nType = 0;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 配置の時の場所把握処理
// ------------------------------------------------------------------------------------------
void Update_DispEnemy(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
												// 変数宣言
	int nCountDirect;
	int nCountWidth;

	int nCntEnemy;

	for (nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		if (g_aenemy[nCntEnemy].bUse == false)
		{
			g_aenemy[nCntEnemy].nType = g_EnemySet.nType;

			// 使用状態・表示状態
			g_aenemy[nCntEnemy].bDisp = true;

			// 配置の位置情報を取得
			g_aenemy[nCntEnemy].pos = pArrangement->pos;

			// 回転情報を取得
			g_aenemy[nCntEnemy].rot = pArrangement->rot;

			if (g_aenemy[nCntEnemy].nEnemy_Depth != g_EnemySet.nEnemy_Depth ||
				g_aenemy[nCntEnemy].nEnemy_Width != g_EnemySet.nEnemy_Width ||
				g_aenemy[nCntEnemy].size != g_EnemySet.size)
			{
				g_aenemy[nCntEnemy].nEnemy_Depth = g_EnemySet.nEnemy_Depth;
				g_aenemy[nCntEnemy].nEnemy_Width = g_EnemySet.nEnemy_Width;
				g_aenemy[nCntEnemy].size = g_EnemySet.size;

				// 原点の計算
				g_aenemy[nCntEnemy].OriginEnemy = D3DXVECTOR3(
					g_aenemy[nCntEnemy].size.x * -0.5f * g_aenemy[nCntEnemy].nEnemy_Width,
					g_aenemy[nCntEnemy].size.y * g_aenemy[nCntEnemy].nEnemy_Depth,
					0.0f);

				// 総頂点数・インデックス・ポリゴンの設定(計算)
				g_aenemy[nCntEnemy].nNumberVertexMeshField =
					(g_aenemy[nCntEnemy].nEnemy_Depth + 1) *
					(g_aenemy[nCntEnemy].nEnemy_Width + 1);

				g_aenemy[nCntEnemy].nNumIndex =
					(g_aenemy[nCntEnemy].nEnemy_Width + 1) * 2 *
					g_aenemy[nCntEnemy].nEnemy_Depth + 2 *
					(g_aenemy[nCntEnemy].nEnemy_Depth - 1);

				g_aenemy[nCntEnemy].nNumPolygon =
					g_aenemy[nCntEnemy].nEnemy_Depth * g_aenemy[nCntEnemy].nEnemy_Width * 2 +
					4 * (g_aenemy[nCntEnemy].nEnemy_Depth - 1);

				// 頂点バッファの生成
				pDevice->CreateVertexBuffer(
					sizeof(VERTEX_3D) * g_aenemy[nCntEnemy].nNumberVertexMeshField,
					D3DUSAGE_WRITEONLY,
					FVF_VERTEX_3D,
					D3DPOOL_MANAGED,
					&g_aenemy[nCntEnemy].pVtxBuff, NULL);

				// インデックスバッファの生成
				pDevice->CreateIndexBuffer(
					sizeof(WORD) * g_aenemy[nCntEnemy].nNumIndex,
					D3DUSAGE_WRITEONLY,
					D3DFMT_INDEX16,
					D3DPOOL_MANAGED,
					&g_aenemy[nCntEnemy].pIndex,
					NULL);

				// 頂点情報の設定
				VERTEX_3D *pVtx;	// 頂点情報へのポイント

									// インデックス情報の設定
				WORD * pIdx;		// インデックスデータへのポインタを取得

				// 頂点データの範囲をロックし、頂点バッファへのポインタ
				g_aenemy[nCntEnemy].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

				//頂点設定 //
				//行ループ
				for (nCountDirect = 0; nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth + 1; nCountDirect++)
				{
					// 列ループ
					for (nCountWidth = 0; nCountWidth < g_aenemy[nCntEnemy].nEnemy_Width + 1; nCountWidth++)
					{
						// 頂点座標の設定
						pVtx[0].pos =
							D3DXVECTOR3(
								g_aenemy[nCntEnemy].OriginEnemy.x + 
								(g_aenemy[nCntEnemy].size.x * nCountWidth),
								g_aenemy[nCntEnemy].OriginEnemy.y - 
								(g_aenemy[nCntEnemy].size.y * nCountDirect),
								0.0f);

						// 法線ベクトルの設定
						pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

						// 色の設定
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						// テクスチャーの設定
						pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

						// ポイント合わせ
						pVtx++;
					}
				}
				// アンロック
				g_aenemy[nCntEnemy].pVtxBuff->Unlock();

				// 頂点データの範囲をロックし、頂点バッファへのポインタ
				g_aenemy[nCntEnemy].pIndex->Lock(0, 0, (void **)&pIdx, 0);

				// 縦ブロック個数
				for (nCountDirect = 0; nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth; nCountDirect++)
				{
					// ２回目のループ以降
					if (nCountDirect >= 1)
					{
						// 縮退ポリゴン分の頂点追加
						pIdx[0] = 
							nCountDirect * 
							(g_aenemy[nCntEnemy].nEnemy_Width + 1) +
							g_aenemy[nCntEnemy].nEnemy_Width + 1;

						// インデックスのポイント合わせ
						pIdx++;
					}

					// 横ブロックの頂点数
					for (nCountWidth = 0; nCountWidth < g_aenemy[nCntEnemy].nEnemy_Width + 1; nCountWidth++)
					{
						// 描画順番のインデックス
						pIdx[0] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + nCountWidth + g_aenemy[nCntEnemy].nEnemy_Width + 1;
						pIdx[1] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + nCountWidth;

						// インデックスのポイント合わせ
						pIdx += 2;
					}

					// 縮退ポリゴンを作る必要がある場合
					if (nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth - 1)
					{
						// 縮退ポリゴン分の頂点追加
						pIdx[0] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + g_aenemy[nCntEnemy].nEnemy_Width;

						// インデックスのポイント合わせ
						pIdx++;
					}
				}

				// アンロック
				g_aenemy[nCntEnemy].pIndex->Unlock();
			}

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 削除処理
// ------------------------------------------------------------------------------------------
void Update_DestroyEnemy(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	for (int nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		if (g_aenemy[nCntEnemy].bUse == true && g_aenemy[nCntEnemy].bDisp == true)
		{
			// ブロックのX範囲
			if (pArrangement->pos.x >
				g_aenemy[nCntEnemy].pos.x - sinf(g_aenemy[nCntEnemy].rot.y) * g_aenemy[nCntEnemy].size.x * 0.5f * g_aenemy[nCntEnemy].nEnemy_Width&&
				pArrangement->pos.x <
				g_aenemy[nCntEnemy].pos.x + sinf(g_aenemy[nCntEnemy].rot.y) * g_aenemy[nCntEnemy].size.x * 0.5f * g_aenemy[nCntEnemy].nEnemy_Width&&
				pArrangement->pos.z >
				g_aenemy[nCntEnemy].pos.z - cosf(g_aenemy[nCntEnemy].rot.y) * g_aenemy[nCntEnemy].size.z * 0.5f * g_aenemy[nCntEnemy].nEnemy_Depth&&
				pArrangement->pos.z <
				g_aenemy[nCntEnemy].pos.z + cosf(g_aenemy[nCntEnemy].rot.y) * g_aenemy[nCntEnemy].size.z * 0.5f * g_aenemy[nCntEnemy].nEnemy_Depth)
			{
				// 削除処理
				if (GetKeyboardSpeacePress(DIK_B))
				{
					g_aenemy[nCntEnemy].bUse = false;
					g_aenemy[nCntEnemy].bDisp = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans,mtxView;

	// カウント敵
	for (int nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		if (g_aenemy[nCntEnemy].bDisp == true)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&g_aenemy[nCntEnemy].mtxWorldEnemy);

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,											// 総合の入れ物
				g_aenemy[nCntEnemy].rot.y, g_aenemy[nCntEnemy].rot.x, g_aenemy[nCntEnemy].rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_aenemy[nCntEnemy].mtxWorldEnemy,	// 1
				&g_aenemy[nCntEnemy].mtxWorldEnemy,				// 2
				&mtxRot);							// 3


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,												// 総合の入れ物
				g_aenemy[nCntEnemy].pos.x, g_aenemy[nCntEnemy].pos.y, g_aenemy[nCntEnemy].pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_aenemy[nCntEnemy].mtxWorldEnemy,	// 1
				&g_aenemy[nCntEnemy].mtxWorldEnemy,				// 2
				&mtxTrans);							// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aenemy[nCntEnemy].mtxWorldEnemy);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_aenemy[nCntEnemy].pVtxBuff, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームを設定
			pDevice->SetIndices(g_aenemy[nCntEnemy].pIndex);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEnemy);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_aenemy[nCntEnemy].nNumberVertexMeshField,
				0,
				g_aenemy[nCntEnemy].nNumPolygon);
		}
	}

#ifdef _DEBUG	
	//if (GetGameSelect() == GAMESELECT_ENEMY)
	//{
	//	// 床の情報表示
	//	DrawEnemyInfo(
	//		g_EnemySet.size,
	//		g_EnemySet.nEnemy_Width,
	//		g_EnemySet.nEnemy_Depth,
	//		g_EnemySet.nType);
	//}
#endif

}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexEnemy(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	int nCntEnemy;
	int nCountDirect;
	int nCountWidth;

	for (nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		if (g_aenemy[nCntEnemy].bUse == true)
		{
			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aenemy[nCntEnemy].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_aenemy[nCntEnemy].pVtxBuff, NULL);

			// インデックスバッファの生成
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_aenemy[nCntEnemy].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aenemy[nCntEnemy].pIndex,
				NULL);

			// 頂点情報の設定
			VERTEX_3D *pVtx;	// 頂点情報へのポイント

			// インデックス情報の設定
			WORD * pIdx;		// インデックスデータへのポインタを取得

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_aenemy[nCntEnemy].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

			//頂点設定 //
			//行ループ
			for (nCountDirect = 0; nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth + 1; nCountDirect++)
			{
				// 列ループ
				for (nCountWidth = 0; nCountWidth < g_aenemy[nCntEnemy].nEnemy_Width + 1; nCountWidth++)
				{
					// 頂点座標の設定
					pVtx[0].pos =
						D3DXVECTOR3(
							g_aenemy[nCntEnemy].OriginEnemy.x + 
							(g_aenemy[nCntEnemy].size.x * nCountWidth),
							g_aenemy[nCntEnemy].OriginEnemy.y -
							(g_aenemy[nCntEnemy].size.y * nCountDirect),
							0.0f);

					// 法線ベクトルの設定
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// 色の設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// テクスチャーの設定
					pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

					// ポイント合わせ
					pVtx++;
				}
			}
			// アンロック
			g_aenemy[nCntEnemy].pVtxBuff->Unlock();

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_aenemy[nCntEnemy].pIndex->Lock(0, 0, (void **)&pIdx, 0);

			// 縦ブロック個数
			for (nCountDirect = 0; nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth; nCountDirect++)
			{
				// ２回目のループ以降
				if (nCountDirect >= 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + g_aenemy[nCntEnemy].nEnemy_Width + 1;

					// インデックスのポイント合わせ
					pIdx++;
				}

				// 横ブロックの頂点数
				for (nCountWidth = 0; nCountWidth < g_aenemy[nCntEnemy].nEnemy_Width + 1; nCountWidth++)
				{
					// 描画順番のインデックス
					pIdx[0] =
						nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + 
						nCountWidth + g_aenemy[nCntEnemy].nEnemy_Width + 1;

					pIdx[1] =
						nCountDirect *
						(g_aenemy[nCntEnemy].nEnemy_Width + 1) + nCountWidth;

					// インデックスのポイント合わせ
					pIdx += 2;
				}

				// 縮退ポリゴンを作る必要がある場合
				if (nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth - 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + g_aenemy[nCntEnemy].nEnemy_Width;

					// インデックスのポイント合わせ
					pIdx++;
				}
			}

			// アンロック
			g_aenemy[nCntEnemy].pIndex->Unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------
// 敵の当たり判定情報
// ------------------------------------------------------------------------------------------
bool ColisionEnemy(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size, ENEMY ** ppEnemyRet)
{
	// 変数宣言
	bool bLand = false;		// めり込んでいるかいないか
	int  nCntEnemy;			// カウント床

	// 番地情報があったら
	if (ppEnemyRet != NULL)
	{
		*ppEnemyRet = NULL;		// 番地の中身の情報をヌルに
	}

	// ブロックループ
	for (nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		// 使用状態
		if (g_aenemy[nCntEnemy].bUse == true)
		{
			
			// ブロックのY範囲
			if (pos->y - size->y < g_aenemy[nCntEnemy].pos.y + g_aenemy[nCntEnemy].size.y&&
				pos->y + size->y > g_aenemy[nCntEnemy].pos.y)
			{
				// ブロックのZ範囲
				if (pos->z + size->z > g_aenemy[nCntEnemy].pos.z - (g_aenemy[nCntEnemy].size.x * sinf(g_aenemy[nCntEnemy].rot.y)) &&	// z方向のサイズ
					pos->z - size->z < g_aenemy[nCntEnemy].pos.z + (g_aenemy[nCntEnemy].size.x * sinf(g_aenemy[nCntEnemy].rot.y)))	// z方向のサイズ)
				{
					// 当たり判定(左)
					if (pos->x + size->z >
						g_aenemy[nCntEnemy].pos.x&&
						posOld->x + size->z <=
						g_aenemy[nCntEnemy].pos.x)
					{
						// 番地の中身の情報に代入
						*ppEnemyRet = &g_aenemy[nCntEnemy];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->x = g_aenemy[nCntEnemy].pos.x - size->z;

						// 移動量の初期化
						move->x = 0.0f;
					}
				}

					// ブロックのZ範囲
				if (pos->z + size->z > g_aenemy[nCntEnemy].pos.z + (g_aenemy[nCntEnemy].size.x * sinf(g_aenemy[nCntEnemy].rot.y)) &&	// z方向のサイズ
					pos->z - size->z < g_aenemy[nCntEnemy].pos.z -(g_aenemy[nCntEnemy].size.x * sinf(g_aenemy[nCntEnemy].rot.y)))	// z方向のサイズ)
				{

					// 当たり判定(右)
					if (pos->x - size->z <
						g_aenemy[nCntEnemy].pos.x&&
						posOld->x - size->z >=
						g_aenemy[nCntEnemy].pos.x)
					{
						// 番地の中身の情報に代入
						*ppEnemyRet = &g_aenemy[nCntEnemy];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->x = g_aenemy[nCntEnemy].pos.x + size->z;

						// 移動量の初期化
						move->x = 0.0f;
					}
				}

				// ブロックのX範囲
				if (pos->x + size->z > g_aenemy[nCntEnemy].pos.x - (g_aenemy[nCntEnemy].size.x * cosf(g_aenemy[nCntEnemy].rot.y)) &&	// z方向のサイズ
					pos->x - size->z < g_aenemy[nCntEnemy].pos.x + (g_aenemy[nCntEnemy].size.x * cosf(g_aenemy[nCntEnemy].rot.y)))	// z方向のサイズ)
				{
					// 当たり判定(手前)
					if (pos->z + size->z > g_aenemy[nCntEnemy].pos.z&&
						posOld->z + size->z <= g_aenemy[nCntEnemy].pos.z)
					{
						// 番地の中身の情報に代入
						*ppEnemyRet = &g_aenemy[nCntEnemy];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->z = g_aenemy[nCntEnemy].pos.z - size->z;

						// 移動量の初期化
						move->z = 0.0f;
					}
				}

					// ブロックのX範囲
				if (pos->x + size->z > g_aenemy[nCntEnemy].pos.x + (g_aenemy[nCntEnemy].size.x * cosf(g_aenemy[nCntEnemy].rot.y)) &&	// z方向のサイズ
					pos->x - size->z < g_aenemy[nCntEnemy].pos.x - (g_aenemy[nCntEnemy].size.x * cosf(g_aenemy[nCntEnemy].rot.y)))	// z方向のサイズ)
				{

					// 当たり判定(奥)
					if (pos->z - size->z < g_aenemy[nCntEnemy].pos.z&&
						posOld->z - size->z >= g_aenemy[nCntEnemy].pos.z)
					{
						// 番地の中身の情報に代入
						*ppEnemyRet = &g_aenemy[nCntEnemy];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->z = g_aenemy[nCntEnemy].pos.z + size->z;

						// 移動量の初期化
						move->z = 0.0f;
					}
				}
			}
		}
	}

	return bLand;
}

// ------------------------------------------------------------------------------------------
// 配置場所がわかるために描画していたものを非表示に処理
// ------------------------------------------------------------------------------------------
void NotDispEnemy(void)
{
	int nCntEnemy;

	// 床のカウント
	for (nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		// 配置用の床
		if (g_aenemy[nCntEnemy].bUse == false &&
			g_aenemy[nCntEnemy].bDisp == true)
		{
			g_aenemy[nCntEnemy].bDisp = false;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// セーブ処理
// ------------------------------------------------------------------------------------------
void SaveEnemy(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int		nCntEnemy = 0;
	int		nCntDataLoad = 0;
	int		nCntBug = 0;			// バグ対策カウント
	char	cWriteText[512];		// 文字として書き込み用
	char	cHeadText[128];			// 文字の格納
	char	cEqual[8] = { "=" };	// イコール用

	char	cSharpEnter[8];			// コメント１
	char	cComment[64];			// コメント２
	char	cEnter[8];				// コメント３

									// よく使われる文字
	strcpy(cSharpEnter, "#\n");
	strcpy(cComment, "#=============================\n");
	strcpy(cEnter, "\n");

	// 床の種類種類
	char *cEnemyType[ENEMYTYPE_MAX] =
	{
		{ "    ----------[00]:敵1----------" },				// 00_道１
	};

	// 床データの種類
	char *cEnemyData[] =
	{
		{ "TYPE" },			// タイプ
		{ "POS" },			// 位置
		{ "ROT" },			// 回転
		{ "BLOCK_DEPTH" },	// 縦ブロック
		{ "BLOCK_WIDTH" },	// 横ブロック
		{ "XSIZE" },		// Ｘサイズ
		{ "YSIZE" },		// Zサイズ
		{ "END_ENEMYSET" }	// エンドフロアー
	};

	// ファイル開
	pFile = fopen(g_cEnemyFIleName_3D[GetGameStage()], "w");

	// 開けた
	if (pFile != NULL)
	{
		//------------------ コメント ------------------//
		strcpy(cWriteText, cComment);

		strcat(cWriteText, cSharpEnter);

		strcat(cWriteText, "# セーブモデル\n");

		strcat(cWriteText, "# Author : koki nishiyama\n");

		strcat(cWriteText, cSharpEnter);

		strcat(cWriteText, cComment);

		strcat(cWriteText, cEnter);

		fputs(cWriteText, pFile);

		//------------------ コメント終了 ------------------//

		strcpy(cWriteText, "SCRIPT");

		fputs(cWriteText, pFile);

		// スクリプトが通った
		if (strcmp(cWriteText, "SCRIPT") == 0)
		{
			// エンドスクリプトが来るまでループ
			while (strcmp(cWriteText, "END_SCRIPT") != 0)
			{
				// 床の最大数ならセーブ
				if (nCntEnemy < ALL_MAX - 1)
				{
					if (g_aenemy[nCntEnemy].bUse == true)
					{
						fputs(cEnter, pFile);

						// モデルのパーツの名前を代入
						strcpy(cWriteText, cEnemyType[g_aenemy[nCntEnemy].nType]);
						fputs(cWriteText, pFile);
						fputs(cEnter, pFile);

						// エンドスクリプトではない
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    ENEMYSET\n");
							fputs(cWriteText, pFile);

							// エンドモデルセットが来るまでループ
							while (strcmp(cWriteText, "END_ENEMYSET") != 0)
							{
								// 文字列をコピーする
								strcpy(cWriteText, cEnemyData[nCntDataLoad]);

								// タイプ情報保存
								if (strcmp(cWriteText, "TYPE") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual, g_aenemy[nCntEnemy].nType);
									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 位置情報保存
								else if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_aenemy[nCntEnemy].pos.x,
										g_aenemy[nCntEnemy].pos.y,
										g_aenemy[nCntEnemy].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 回転情報保存
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_aenemy[nCntEnemy].rot.x,
										g_aenemy[nCntEnemy].rot.y,
										g_aenemy[nCntEnemy].rot.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 縦ブロック数情報保存
								else if (strcmp(cWriteText, "BLOCK_DEPTH") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual,
										g_aenemy[nCntEnemy].nEnemy_Depth);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 横ブロック数情報保存
								else if (strcmp(cWriteText, "BLOCK_WIDTH") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual,
										g_aenemy[nCntEnemy].nEnemy_Width);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// Ｘサイズ情報保存
								else if (strcmp(cWriteText, "XSIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f\n", &cWriteText, &cEqual,
										g_aenemy[nCntEnemy].size.x);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// Zサイズ情報保存
								else if (strcmp(cWriteText, "YSIZE") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f\n", &cWriteText, &cEqual,
										g_aenemy[nCntEnemy].size.y);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// エンド床セット
								else if (strcmp(cWriteText, "END_ENEMYSET") == 0)
								{
									sprintf(cHeadText, "    %s", cWriteText);
									fputs(cHeadText, pFile);

									// カウントデータ初期化
									nCntDataLoad = 0;
								}
							}
						}
					}
					// モデルパーツ変更
					nCntEnemy++;
				}

				// エンドスクリプト
				else
				{
					fputs(cEnter, pFile);

					strcpy(cWriteText, "END_SCRIPT");

					fputs(cWriteText, pFile);
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	else
	{
		MessageBox(NULL, "モデルデータの書き込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}

// ------------------------------------------------------------------------------------------
// 敵読み込み処理
// ------------------------------------------------------------------------------------------
void LoadEnemy(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad = 0;		// カウントロード
	int nCntEnemy = 0;		// カウント床
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// もし表示しているものがあったら
	for (nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		if (g_aenemy[nCntEnemy].bUse == true)
		{
			g_aenemy[nCntEnemy].bUse = false;
			g_aenemy[nCntEnemy].bDisp = false;
		}
	}

	// カウント初期化
	nCntEnemy = 0;

	// ファイル開
	pFile = fopen(g_cEnemyFIleName_3D[GetGameStage()], "r");

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

				// 改行だったら
				if (strcmp(cHeadText, "\n") == 0)
				{
				}

				// モデルセットが来たら
				else if (strcmp(cHeadText, "ENEMYSET") == 0)
				{

					// エンドモデルセットが来るまでループ
					while (strcmp(cHeadText, "END_ENEMYSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 種類情報読み込み
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_aenemy[nCntEnemy].nType);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_aenemy[nCntEnemy].pos.x, &g_aenemy[nCntEnemy].pos.y, &g_aenemy[nCntEnemy].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_aenemy[nCntEnemy].rot.x, &g_aenemy[nCntEnemy].rot.y, &g_aenemy[nCntEnemy].rot.z);
						}

						// 縦ブロック情報読み込み
						else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_aenemy[nCntEnemy].nEnemy_Depth);
						}

						// 横ブロック情報読み込み
						else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_aenemy[nCntEnemy].nEnemy_Width);
						}

						// xサイズ情報読み込み
						else if (strcmp(cHeadText, "XSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_aenemy[nCntEnemy].size.x);
						}

						// yサイズ情報読み込み
						else if (strcmp(cHeadText, "YSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_aenemy[nCntEnemy].size.y);
						}

					}

					// 原点の計算
					g_aenemy[nCntEnemy].OriginEnemy = D3DXVECTOR3(
						g_aenemy[nCntEnemy].size.x * -0.5f * g_aenemy[nCntEnemy].nEnemy_Width,
						g_aenemy[nCntEnemy].size.y * g_aenemy[nCntEnemy].nEnemy_Depth,
						0.0f);

					// 総頂点数・インデックス・ポリゴンの設定(計算)
					g_aenemy[nCntEnemy].nNumberVertexMeshField =
						(g_aenemy[nCntEnemy].nEnemy_Depth + 1) * 
						(g_aenemy[nCntEnemy].nEnemy_Width + 1);

					g_aenemy[nCntEnemy].nNumIndex = 
						(g_aenemy[nCntEnemy].nEnemy_Width + 1) * 2 * 
						g_aenemy[nCntEnemy].nEnemy_Depth + 2 *
						(g_aenemy[nCntEnemy].nEnemy_Depth - 1);

					g_aenemy[nCntEnemy].nNumPolygon =
						g_aenemy[nCntEnemy].nEnemy_Depth * g_aenemy[nCntEnemy].nEnemy_Width * 2 + 
						4 * (g_aenemy[nCntEnemy].nEnemy_Depth - 1);

					// 使用状態・表示状態
					g_aenemy[nCntEnemy].bUse = true;
					g_aenemy[nCntEnemy].bDisp = true;

					// 床カウントの更新
					nCntEnemy++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "モデルデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}

}

// ------------------------------------------------------------------------------------------
// 床の設置
// ------------------------------------------------------------------------------------------
void SetEnemy(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 rot,
	D3DXVECTOR3 size,
	int nEnemy_Depth,
	int	nEnemy_Width,
	int	nType)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	int nCntEnemy;		// カウント床
	int nCountDirect;	// 縦ブロックのカウント
	int nCountWidth;	// 横ブロックのカウント

						// 床の設置をする
	for (nCntEnemy = 0; nCntEnemy < ALL_MAX; nCntEnemy++)
	{
		// 未使用の床
		if (g_aenemy[nCntEnemy].bUse == false)
		{
			// 情報を変数に代入
			g_aenemy[nCntEnemy].pos = pos;
			g_aenemy[nCntEnemy].rot = rot;
			g_aenemy[nCntEnemy].size = size;
			g_aenemy[nCntEnemy].nEnemy_Depth = nEnemy_Depth;
			g_aenemy[nCntEnemy].nEnemy_Width = nEnemy_Width;
			g_aenemy[nCntEnemy].nType = nType;

			// 原点の計算
			g_aenemy[nCntEnemy].OriginEnemy = D3DXVECTOR3(
				g_aenemy[nCntEnemy].size.x * -0.5f * g_aenemy[nCntEnemy].nEnemy_Width,
				g_aenemy[nCntEnemy].size.y * g_aenemy[nCntEnemy].nEnemy_Depth,
				0.0f);

			// 総頂点の計算
			g_aenemy[nCntEnemy].nNumberVertexMeshField =
				(g_aenemy[nCntEnemy].nEnemy_Depth + 1) *
				(g_aenemy[nCntEnemy].nEnemy_Width + 1);

			// インデックスの計算
			g_aenemy[nCntEnemy].nNumIndex =
				(g_aenemy[nCntEnemy].nEnemy_Width + 1) * 2 *
				g_aenemy[nCntEnemy].nEnemy_Depth +
				2 * (g_aenemy[nCntEnemy].nEnemy_Depth - 1);

			// ポリゴンの計算
			g_aenemy[nCntEnemy].nNumPolygon =
				g_aenemy[nCntEnemy].nEnemy_Depth *
				g_aenemy[nCntEnemy].nEnemy_Width * 2 +
				4 * (g_aenemy[nCntEnemy].nEnemy_Depth - 1);

			g_aenemy[nCntEnemy].nNumber = 0;

			// 使用状態・表示状態
			g_aenemy[nCntEnemy].bUse = true;
			g_aenemy[nCntEnemy].bDisp = true;

			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aenemy[nCntEnemy].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_aenemy[nCntEnemy].pVtxBuff, NULL);

			// インデックスバッファの生成
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_aenemy[nCntEnemy].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_aenemy[nCntEnemy].pIndex,
				NULL);

			// 頂点情報の設定
			VERTEX_3D *pVtx;	// 頂点情報へのポイント

								// インデックス情報の設定
			WORD * pIdx;		// インデックスデータへのポインタを取得

								// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_aenemy[nCntEnemy].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

			//頂点設定 //
			//行ループ
			for (nCountDirect = 0; nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth + 1; nCountDirect++)
			{
				// 列ループ
				for (nCountWidth = 0; nCountWidth < g_aenemy[nCntEnemy].nEnemy_Width + 1; nCountWidth++)
				{
					// 頂点座標の設定
					pVtx[0].pos =
						D3DXVECTOR3(
							g_aenemy[nCntEnemy].OriginEnemy.x + (g_aenemy[nCntEnemy].size.x * nCountWidth),
							g_aenemy[nCntEnemy].OriginEnemy.y - (g_aenemy[nCntEnemy].size.y * nCountDirect),
							0.0f);

					// 法線ベクトルの設定
					pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

					// 色の設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					// テクスチャーの設定
					pVtx[0].tex = D3DXVECTOR2(1.0f * nCountWidth, 1.0f * nCountDirect);

					// ポイント合わせ
					pVtx++;
				}
			}
			// アンロック
			g_aenemy[nCntEnemy].pVtxBuff->Unlock();

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_aenemy[nCntEnemy].pIndex->Lock(0, 0, (void **)&pIdx, 0);

			// 縦ブロック個数
			for (nCountDirect = 0; nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth; nCountDirect++)
			{
				// ２回目のループ以降
				if (nCountDirect >= 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + g_aenemy[nCntEnemy].nEnemy_Width + 1;

					// インデックスのポイント合わせ
					pIdx++;
				}

				// 横ブロックの頂点数
				for (nCountWidth = 0; nCountWidth < g_aenemy[nCntEnemy].nEnemy_Width + 1; nCountWidth++)
				{
					// 描画順番のインデックス
					pIdx[0] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + nCountWidth + g_aenemy[nCntEnemy].nEnemy_Width + 1;
					pIdx[1] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + nCountWidth;

					// インデックスのポイント合わせ
					pIdx += 2;
				}

				// 縮退ポリゴンを作る必要がある場合
				if (nCountDirect < g_aenemy[nCntEnemy].nEnemy_Depth - 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] = nCountDirect * (g_aenemy[nCntEnemy].nEnemy_Width + 1) + g_aenemy[nCntEnemy].nEnemy_Width;

					// インデックスのポイント合わせ
					pIdx++;
				}
			}

			// アンロック
			g_aenemy[nCntEnemy].pIndex->Unlock();
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 敵情報
// ------------------------------------------------------------------------------------------
ENEMY *GetEnemy(void)
{
	return &g_aenemy[0];
}