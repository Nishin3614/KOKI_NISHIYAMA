// ------------------------------------------------------------------------------------------
//
// 壁処理 [wall.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "wall.h"

#include "input.h"

#include "game.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define WALL_PHONT "data/TEXTURE/woods.png"

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice);
bool ColisionWall(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, 
	D3DXVECTOR3 * move, D3DXVECTOR3 * size, WALL ** ppWallRet);

void LoadWall(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9		g_pTextureWall = NULL;	// 頂点テクスチャーのポインタ

WALL g_awall[WALL_MAX];							// 壁

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 位置・回転の初期設定
	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		g_awall[nCntWall].pos			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_awall[nCntWall].rot			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_awall[nCntWall].move			= D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_awall[nCntWall].size			= D3DXVECTOR3(100.0f, 100.0f, 0.0f);
		g_awall[nCntWall].nBlock_Depth	= 2;
		g_awall[nCntWall].nBlock_Width	= 2;

		// 原点の計算
		g_awall[nCntWall].OriginBlock	= D3DXVECTOR3(
			g_awall[nCntWall].size.x * -0.5f * g_awall[nCntWall].nBlock_Width,
			g_awall[nCntWall].size.y * g_awall[nCntWall].nBlock_Depth,
			0.0f);

		// 総頂点の計算
		g_awall[nCntWall].nNumberVertexMeshField = 
			(g_awall[nCntWall].nBlock_Depth + 1) *
			(g_awall[nCntWall].nBlock_Width + 1);

		// インデックスの計算
		g_awall[nCntWall].nNumIndex = 
			(g_awall[nCntWall].nBlock_Width + 1) * 2 *
			g_awall[nCntWall].nBlock_Depth + 
			2 * (g_awall[nCntWall].nBlock_Depth - 1);

		// ポリゴンの計算
		g_awall[nCntWall].nNumPolygon = 
			g_awall[nCntWall].nBlock_Depth * 
			g_awall[nCntWall].nBlock_Width * 2 + 
			4 * (g_awall[nCntWall].nBlock_Depth - 1);

		g_awall[nCntWall].nNumber = 0;

		// 使用・表示状態の初期化
		g_awall[nCntWall].bUse = false;
		g_awall[nCntWall].bDisp = false;


		// 当たり判定用 // 
		// 原点
		g_awall[nCntWall].BiginPos = g_awall[nCntWall].OriginBlock;

		// 最終地点
		g_awall[nCntWall].LastPos =
			g_awall[nCntWall].OriginBlock * (float)g_awall[nCntWall].nBlock_Width;

		// 壁の長さ
		g_awall[nCntWall].Length = g_awall[nCntWall].LastPos - g_awall[nCntWall].BiginPos;

		// 距離
		g_awall[nCntWall].fdistance =
			sqrtf(g_awall[nCntWall].Length.x * g_awall[nCntWall].Length.x +
				g_awall[nCntWall].Length.z * g_awall[nCntWall].Length.z);

		//// 正規化してベクトルを求める
		//D3DXVec3Normalize(&g_awall[nCntWall].VecWall, &g_awall[nCntWall].Length);
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, WALL_PHONT, &g_pTextureWall);

	// 壁のデータ読込
	LoadWall();

	// 頂点情報の作成
	MakeVertexWall(pDevice);
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitWall(void)
{
	// テクスチャの開放
	if (g_pTextureWall != NULL)
	{
		g_pTextureWall->Release();
		g_pTextureWall = NULL;
	}

	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		// 頂点バッファの開放
		if (g_awall[nCntWall].pVtxBuff != NULL)
		{
			g_awall[nCntWall].pVtxBuff->Release();
			g_awall[nCntWall].pVtxBuff = NULL;
		}

		// 頂点インデックスの開放
		if (g_awall[nCntWall].pIndex != NULL)
		{
			g_awall[nCntWall].pIndex->Release();
			g_awall[nCntWall].pIndex = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateWall(void)
{

}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawWall(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans,mtxView;

	// カウント壁
	for (int nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		if (g_awall[nCntWall].bUse)
		{
			// ワールドマトリックスの初期化(単位行列にするため)
			D3DXMatrixIdentity(&g_awall[nCntWall].mtxWorldWall);

			// 回転を反映 //
			// ヨーピッチロールによる回転行列作成(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,											// 総合の入れ物
				g_awall[nCntWall].rot.y, g_awall[nCntWall].rot.x, g_awall[nCntWall].rot.z);	// それぞれの回転量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_awall[nCntWall].mtxWorldWall,	// 1
				&g_awall[nCntWall].mtxWorldWall,				// 2
				&mtxRot);							// 3


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,												// 総合の入れ物
				g_awall[nCntWall].pos.x, g_awall[nCntWall].pos.y, g_awall[nCntWall].pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_awall[nCntWall].mtxWorldWall,	// 1
				&g_awall[nCntWall].mtxWorldWall,				// 2
				&mtxTrans);							// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_awall[nCntWall].mtxWorldWall);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_awall[nCntWall].pVtxBuff, 0, sizeof(VERTEX_3D));

			// インデックスバッファをデータストリームを設定
			pDevice->SetIndices(g_awall[nCntWall].pIndex);

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall);

			// ポリゴンの描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_awall[nCntWall].nNumberVertexMeshField, 0, g_awall[nCntWall].nNumPolygon);
		}
	}
}

// ------------------------------------------------------------------------------------------
// 頂点情報の作成
// ------------------------------------------------------------------------------------------
void MakeVertexWall(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	int nCntWall;
	int nCountDirect;
	int nCountWidth;

	for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		if (g_awall[nCntWall].bUse == true)
		{
			// 頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_awall[nCntWall].nNumberVertexMeshField,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_awall[nCntWall].pVtxBuff, NULL);

			// インデックスバッファの生成
			pDevice->CreateIndexBuffer(sizeof(WORD) * g_awall[nCntWall].nNumIndex,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_awall[nCntWall].pIndex,
				NULL);

			// 頂点情報の設定
			VERTEX_3D *pVtx;	// 頂点情報へのポイント

			// インデックス情報の設定
			WORD * pIdx;		// インデックスデータへのポインタを取得

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_awall[nCntWall].pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);

			//頂点設定 //
			//行ループ
			for (nCountDirect = 0; nCountDirect < g_awall[nCntWall].nBlock_Depth + 1; nCountDirect++)
			{
				// 列ループ
				for (nCountWidth = 0; nCountWidth < g_awall[nCntWall].nBlock_Width + 1; nCountWidth++)
				{
					// 頂点座標の設定
					pVtx[0].pos =
						D3DXVECTOR3(
							g_awall[nCntWall].OriginBlock.x + (g_awall[nCntWall].size.x * nCountWidth),
							g_awall[nCntWall].OriginBlock.y - (g_awall[nCntWall].size.y * nCountDirect),
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
			g_awall[nCntWall].pVtxBuff->Unlock();

			// 頂点データの範囲をロックし、頂点バッファへのポインタ
			g_awall[nCntWall].pIndex->Lock(0, 0, (void **)&pIdx, 0);

			// 縦ブロック個数
			for (nCountDirect = 0; nCountDirect < g_awall[nCntWall].nBlock_Depth; nCountDirect++)
			{
				// ２回目のループ以降
				if (nCountDirect >= 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + g_awall[nCntWall].nBlock_Width + 1;

					// インデックスのポイント合わせ
					pIdx++;
				}

				// 横ブロックの頂点数
				for (nCountWidth = 0; nCountWidth < g_awall[nCntWall].nBlock_Width + 1; nCountWidth++)
				{
					// 描画順番のインデックス
					pIdx[0] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + nCountWidth + g_awall[nCntWall].nBlock_Width + 1;
					pIdx[1] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + nCountWidth;

					// インデックスのポイント合わせ
					pIdx += 2;
				}

				// 縮退ポリゴンを作る必要がある場合
				if (nCountDirect < g_awall[nCntWall].nBlock_Depth - 1)
				{
					// 縮退ポリゴン分の頂点追加
					pIdx[0] = nCountDirect * (g_awall[nCntWall].nBlock_Width + 1) + g_awall[nCntWall].nBlock_Width;

					// インデックスのポイント合わせ
					pIdx++;
				}
			}

			// アンロック
			g_awall[nCntWall].pIndex->Unlock();
		}
	}
}

// ------------------------------------------------------------------------------------------
// 壁の当たり判定情報
// ------------------------------------------------------------------------------------------
bool ColisionWall(D3DXVECTOR3 * pos, D3DXVECTOR3 * posOld, D3DXVECTOR3 * move, D3DXVECTOR3 * size, WALL ** ppWallRet)
{
	// 変数宣言
	bool bLand = false;		// めり込んでいるかいないか
	int  nCntWall;			// カウント床

	// ベクトルの当たり判定
	D3DXVECTOR3 PlayerOldVec;	// 前回のプレイヤーのブロックの始点からのベクトル
	D3DXVECTOR3 PlayerVec;		// プレイヤーのブロックの始点からのベクトル

	//// ブロックループ
	//for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	//{
	//	// 使用状態
	//	if (g_awall[nCntWall].bUse == true)
	//	{
	//		// 前回のプレイヤーのベクトル
	//		PlayerOldVec = D3DXVECTOR3(
	//			posOld->x - g_awall[nCntWall].pos.x,
	//			posOld->y - g_awall[nCntWall].pos.y,
	//			posOld->z - g_awall[nCntWall].pos.z);

	//		// 
	//		// プレイヤーのベクトル
	//		PlayerVec = D3DXVECTOR3(
	//			pos->x - g_awall[nCntWall].pos.x,
	//			pos->y - g_awall[nCntWall].pos.y,
	//			pos->z - g_awall[nCntWall].pos.z);

	//		//// 正規化してベクトルを求める
	//		//D3DXVec3Normalize(&PlayerVec, &PlayerVec);

	//		//// 正規化してベクトルを求める
	//		//D3DXVec3Normalize(&PlayerOldVec, &PlayerOldVec);

	//		// ブロックのY範囲
	//		if (pos->y - size->y < g_awall[nCntWall].pos.y + g_awall[nCntWall].size.y &&
	//			pos->y + size->y > g_awall[nCntWall].pos.y)
	//		{
	//			// 外積(前回-,現在+)
	//			if ((PlayerOldVec.z * g_awall[nCntWall].Length.x) -
	//				(PlayerOldVec.x * g_awall[nCntWall].Length.z) < 0 &&	// 前回の外積が-
	//				(PlayerVec.z * g_awall[nCntWall].Length.x) -
	//				(PlayerVec.x * g_awall[nCntWall].Length.z) > 0)			// 現在の外積が+
	//			{
	//				// ブロック状の左に
	//				pos->x = 0.0f;

	//				// ブロック状の左に
	//				pos->y = 0.0f;

	//				// ブロック状の左に
	//				pos->z = 0.0f;
	//			}

	//			// 外積(前回+,現在-)
	//			else if ((PlayerOldVec.z * g_awall[nCntWall].Length.x) -
	//				(PlayerOldVec.x * g_awall[nCntWall].Length.z) > 0 &&	// 前回の外積が+
	//				(PlayerVec.z * g_awall[nCntWall].Length.x) -
	//				(PlayerVec.x * g_awall[nCntWall].Length.z) < 0)			// 現在の外積が-
	//			{
	//				// ブロック状の左に
	//				pos->x = 0.0f;

	//				// ブロック状の左に
 //					pos->y = 0.0f;

	//				// ブロック状の左に
	//				pos->z = 0.0f;

	//			}
	//		}
	//	}
	//}





	// 番地情報があったら
	if (ppWallRet != NULL)
	{
		*ppWallRet = NULL;		// 番地の中身の情報をヌルに
	}

	// ブロックループ
	for (nCntWall = 0; nCntWall < WALL_MAX; nCntWall++)
	{
		// 使用状態
		if (g_awall[nCntWall].bUse == true)
		{
			// ブロックのY範囲
			if (pos->y - size->y < g_awall[nCntWall].pos.y + g_awall[nCntWall].size.y &&
				pos->y + size->y > g_awall[nCntWall].pos.y)
			{
				// ブロックのZ範囲
				if (pos->z + size->z > 
					g_awall[nCntWall].pos.z -
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width *sinf(g_awall[nCntWall].rot.y)) &&	// z方向のサイズ
					pos->z - size->z < 
					g_awall[nCntWall].pos.z +
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width *sinf(g_awall[nCntWall].rot.y)))	// z方向のサイズ)
				{
					// 当たり判定(左)
					if (pos->x + size->z >
						g_awall[nCntWall].pos.x&&
						posOld->x + size->z <=
						g_awall[nCntWall].pos.x)
					{
						// 番地の中身の情報に代入
						*ppWallRet = &g_awall[nCntWall];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->x = g_awall[nCntWall].pos.x - size->z;

						// 移動量の初期化
						move->x = 0.0f;
					}
				}

					// ブロックのZ範囲
				if (pos->z + size->z > 
					g_awall[nCntWall].pos.z + 
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width * sinf(g_awall[nCntWall].rot.y)) &&	// z方向のサイズ
					pos->z - size->z <
					g_awall[nCntWall].pos.z -
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width * sinf(g_awall[nCntWall].rot.y)))	// z方向のサイズ)
				{

					// 当たり判定(右)
					if (pos->x - size->z <
						g_awall[nCntWall].pos.x&&
						posOld->x - size->z >=
						g_awall[nCntWall].pos.x)
					{
						// 番地の中身の情報に代入
						*ppWallRet = &g_awall[nCntWall];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->x = g_awall[nCntWall].pos.x + size->z;

						// 移動量の初期化
						move->x = 0.0f;
					}
				}

				// ブロックのX範囲
				if (pos->x + size->z >
					g_awall[nCntWall].pos.x - 
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width * cosf(g_awall[nCntWall].rot.y)) &&	// z方向のサイズ
					pos->x - size->z <
					g_awall[nCntWall].pos.x + 
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width*cosf(g_awall[nCntWall].rot.y)))	// z方向のサイズ)
				{
					// 当たり判定(手前)
					if (pos->z + size->z > g_awall[nCntWall].pos.z&&
						posOld->z + size->z <= g_awall[nCntWall].pos.z)
					{
						// 番地の中身の情報に代入
						*ppWallRet = &g_awall[nCntWall];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->z = g_awall[nCntWall].pos.z - size->z;

						// 移動量の初期化
						move->z = 0.0f;
					}
				}

					// ブロックのX範囲
				if (pos->x + size->z > 
					g_awall[nCntWall].pos.x + 
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width * cosf(g_awall[nCntWall].rot.y)) &&	// z方向のサイズ
					pos->x - size->z < 
					g_awall[nCntWall].pos.x - 
					(g_awall[nCntWall].size.x * 0.5f * g_awall[nCntWall].nBlock_Width * cosf(g_awall[nCntWall].rot.y)))	// z方向のサイズ)
				{

					// 当たり判定(奥)
					if (pos->z - size->z < g_awall[nCntWall].pos.z&&
						posOld->z - size->z >= g_awall[nCntWall].pos.z)
					{
						// 番地の中身の情報に代入
						*ppWallRet = &g_awall[nCntWall];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->z = g_awall[nCntWall].pos.z + size->z;

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
// 壁読み込み処理
// ------------------------------------------------------------------------------------------
void LoadWall(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad = 0;		// カウントロード
	int nCntWall = 0;		// カウント床
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	// チュートリアル用
	if (GetMode() == MODE_TUTORIAL)
	{
		pFile = fopen("data/SAVE/TUTORIAL/wall.txt", "r");
	}

	// ボス用
	else if (GetGameStete() == GAMESTATE_BOSS)
	{
		// ファイル開
		pFile = fopen("data/SAVE/GAME_BOSS/wall.txt", "r");
	}

	// それ以外
	else
	{
		pFile = fopen("data/SAVE/wall.txt", "r");
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

				// 改行だったら
				if (strcmp(cHeadText, "\n") == 0)
				{
				}

				// モデルセットが来たら
				else if (strcmp(cHeadText, "WALLSET") == 0)
				{

					// エンドモデルセットが来るまでループ
					while (strcmp(cHeadText, "END_WALLSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 種類情報読み込み
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_awall[nCntWall].nType);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_awall[nCntWall].pos.x, &g_awall[nCntWall].pos.y, &g_awall[nCntWall].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie, &g_awall[nCntWall].rot.x, &g_awall[nCntWall].rot.y, &g_awall[nCntWall].rot.z);
						}

						// 縦ブロック情報読み込み
						else if (strcmp(cHeadText, "BLOCK_DEPTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_awall[nCntWall].nBlock_Depth);
						}

						// 横ブロック情報読み込み
						else if (strcmp(cHeadText, "BLOCK_WIDTH") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_awall[nCntWall].nBlock_Width);
						}

						// xサイズ情報読み込み
						else if (strcmp(cHeadText, "XSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_awall[nCntWall].size.x);
						}

						// yサイズ情報読み込み
						else if (strcmp(cHeadText, "YSIZE") == 0)
						{
							sscanf(cRaedText, "%s %s %f", &cDie, &cDie, &g_awall[nCntWall].size.y);
						}

					}

					// 原点の計算
					g_awall[nCntWall].OriginBlock = D3DXVECTOR3(
						g_awall[nCntWall].size.x * -0.5f * g_awall[nCntWall].nBlock_Width,
						g_awall[nCntWall].size.y * g_awall[nCntWall].nBlock_Depth,
						0.0f);

					// 総頂点数・インデックス・ポリゴンの設定(計算)
					g_awall[nCntWall].nNumberVertexMeshField = (g_awall[nCntWall].nBlock_Depth + 1) * (g_awall[nCntWall].nBlock_Width + 1);
					g_awall[nCntWall].nNumIndex = (g_awall[nCntWall].nBlock_Width + 1) * 2 * g_awall[nCntWall].nBlock_Depth + 2 * (g_awall[nCntWall].nBlock_Depth - 1);
					g_awall[nCntWall].nNumPolygon = g_awall[nCntWall].nBlock_Depth * g_awall[nCntWall].nBlock_Width * 2 + 4 * (g_awall[nCntWall].nBlock_Depth - 1);

					// 使用状態・表示状態
					g_awall[nCntWall].bUse = true;
					g_awall[nCntWall].bDisp = true;


					// 当たり判定用 // 
					// 原点
					g_awall[nCntWall].BiginPos = g_awall[nCntWall].OriginBlock;

					// 最終地点
					g_awall[nCntWall].LastPos =
						g_awall[nCntWall].OriginBlock * (float)g_awall[nCntWall].nBlock_Width;

					// 壁の長さ
					g_awall[nCntWall].Length = g_awall[nCntWall].LastPos - g_awall[nCntWall].BiginPos;

					// 距離
					g_awall[nCntWall].fdistance =
						sqrtf(g_awall[nCntWall].Length.x * g_awall[nCntWall].Length.x +
							g_awall[nCntWall].Length.z * g_awall[nCntWall].Length.z);

					//// 正規化してベクトルを求める
					//D3DXVec3Normalize(&g_awall[nCntWall].VecWall, &g_awall[nCntWall].Length);

					// 床カウントの更新
					nCntWall++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "壁データの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}

}

// ------------------------------------------------------------------------------------------
// 壁情報
// ------------------------------------------------------------------------------------------
WALL *GetWall(void)
{
	return &g_awall[0];
}