// ------------------------------------------------------------------------------------------
//
// 素材処理 [materials.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "materials.h"
#include "input.h"
#include "shadow.h"
#include "bullet.h"
#include "wall.h"
#include "camera.h"
#include "floor.h"
#include "player.h"
#include "effect.h"
#include "Billboard.h"
#include "game.h"
#include "dome.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define TEXTURE_MATERIALS (1)
#define SIZE (0.5f)

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
// ---------- パーツ情報 ---------- //
typedef struct
{
	LPD3DXMESH		paMesh;				// メッシュ情報へのポインタ
	LPD3DXBUFFER	paBuffMat;			// マテリアル情報へのポインタ
	DWORD			nNumMat;			// マテリアルの数
	D3DXVECTOR3		size;				// サイズ
	D3DXVECTOR3		vtxMinMaterials;	// モデルの位置の最小値
	D3DXVECTOR3		vtxMaxMaterials;	// モデルの位置の最大値
} MATERIALS_PARTS;

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void Update_HitMaterials(int nCntMaterials);
void Update_PickMaterials(int nCntMaterials);
void Update_ColisionMaterials(int nCntMaterials);
void LoadMaterials(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPD3DXMESH			g_paMeshMaterials[MATERIALS_ITEM_MAX] = {};				// メッシュ情報へのポインタ
LPD3DXBUFFER		g_paBuffMatMaterials[MATERIALS_ITEM_MAX] = {};			// マテリアル情報へのポインタ
DWORD				g_anNummatMaterials[MATERIALS_ITEM_MAX];					// マテリアルの数

LPDIRECT3DTEXTURE9  g_paTextureMaterials[TEXTURE_MATERIALS] = {};	// テクスチャへのポインタ

MATERIALS_PARTS		g_materialsParts[MATERIALS_ITEM_MAX];			// パーツ情報

MATERIALS			g_materials[MATERIALS_MAX];						// 素材情報

int					g_nMaterialsNumType = 0;							// ナンバータイプ

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitMaterials(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// デバイスの取得
	CAMERA	*pCamera = GetCamera();				// カメラの取得
	int		nCntMaterials;						// プレイヤーカウント
	int		nCntModelType;						// モデルカウント
	int		nCntTexType;						// テクスチャーカウント

	// 素材ファイル名定義
	char *paMaterialsFile[MATERIALS_ITEM_MAX] =
	{
		// オブジェクト用 //
		{ "data/MODEL/materials/00_wood_green.x" },		// [緑木]00
		{ "data/MODEL/materials/03_wood_red.x" },		// [赤木]02
		{ "data/MODEL/materials/02_wood_orange.x" },	// [オレンジ木]01
														
		// アイテム用 //
		{ "data/MODEL/item/00_APPLE.x" },				// [リンゴ]00
		{ "data/MODEL/item/00_APPLE.x" }				// [マタタビ]01

	};

	// テクスチャーファイル名定義
	char *paTextureFile[TEXTURE_MATERIALS] =
	{
		{ "" }
	};

	// パーツの初期化
	for (nCntModelType = 0; nCntModelType < MATERIALS_ITEM_MAX; nCntModelType++)
	{
		g_materialsParts[nCntModelType].paBuffMat		= NULL;
		g_materialsParts[nCntModelType].paMesh			= NULL;
		g_materialsParts[nCntModelType].nNumMat			= 0;
		g_materialsParts[nCntModelType].size			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materialsParts[nCntModelType].vtxMaxMaterials	= D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
		g_materialsParts[nCntModelType].vtxMinMaterials = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);

		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			paMaterialsFile[nCntModelType],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_materialsParts[nCntModelType].paBuffMat,
			NULL,
			&g_materialsParts[nCntModelType].nNumMat,
			&g_materialsParts[nCntModelType].paMesh);

		// モデルの頂点座標の最大値・最小値の設定
		int nNumVertex;			// 頂点数
		DWORD sizeFVF;			// 頂点フォーマットのサイズ
		BYTE *pVertexBuffer;	// 頂点バッファへのポインタ

		// 頂点数を取得
		nNumVertex = g_materialsParts[nCntModelType].paMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_materialsParts[nCntModelType].paMesh->GetFVF());

		// 頂点バッファをロック
		g_materialsParts[nCntModelType].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		// 頂点数分繰り返す
		for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
		{
			// 番地情報のD3DXVE
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

			// -----------すべての頂点情報を比較して最小値・最大値を抜き出す---------- //
			// x //
			// 最小値
			if (g_materialsParts[nCntModelType].vtxMinMaterials.x > vtx.x)
			{
				g_materialsParts[nCntModelType].vtxMinMaterials.x = vtx.x;
			}

			// 最大値
			if (g_materialsParts[nCntModelType].vtxMaxMaterials.x < vtx.x)
			{
				g_materialsParts[nCntModelType].vtxMaxMaterials.x = vtx.x;
			}

			// y //
			// 最小値
			if (g_materialsParts[nCntModelType].vtxMinMaterials.y > vtx.y)
			{
				g_materialsParts[nCntModelType].vtxMinMaterials.y = vtx.y;
			}

			// 最大値
			if (g_materialsParts[nCntModelType].vtxMaxMaterials.y < vtx.y)
			{
				g_materialsParts[nCntModelType].vtxMaxMaterials.y = vtx.y;
			}

			// z //
			// 最小値
			if (g_materialsParts[nCntModelType].vtxMinMaterials.z > vtx.z)
			{
				g_materialsParts[nCntModelType].vtxMinMaterials.z = vtx.z;
			}

			// 最大値
			if (g_materialsParts[nCntModelType].vtxMaxMaterials.z < vtx.z)
			{
				g_materialsParts[nCntModelType].vtxMaxMaterials.z = vtx.z;
			}

			// サイズポインタを進める
			pVertexBuffer += sizeFVF;
		}

		// サイズ
		g_materialsParts[nCntModelType].size =
			g_materialsParts[nCntModelType].vtxMaxMaterials -
			g_materialsParts[nCntModelType].vtxMinMaterials;

		// アンロック
		g_materialsParts[nCntModelType].paMesh->UnlockVertexBuffer();
	}

	// テクスチャー呼び出し
	for (nCntTexType = 0; nCntTexType < TEXTURE_MATERIALS; nCntTexType++)
	{
		// テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice, paTextureFile[0], &g_paTextureMaterials[0]);
	}

	// 素材の初期設定
	for (nCntMaterials = 0; nCntMaterials < MATERIALS_MAX; nCntMaterials++)
	{
		// ボス状態
		if (GetGameStete() == GAMESTATE_BOSS)
		{
			// 使用中の影を消す
			if (g_materials[nCntMaterials].bUse == true)
			{
				DeleteShadow(g_materials[nCntMaterials].IdxShadow);
			}
		}

		g_materials[nCntMaterials].pos = D3DXVECTOR3(200.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rotbetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].size = g_materialsParts[0].size;
		g_materials[nCntMaterials].vtxMinMaterials = g_materialsParts[0].vtxMinMaterials;
		g_materials[nCntMaterials].vtxMaxMaterials = g_materialsParts[0].vtxMaxMaterials;
		g_materials[nCntMaterials].nDrawType = 0;
		g_materials[nCntMaterials].nTexType = 0;
		g_materials[nCntMaterials].nIdxDome = 0;
		g_materials[nCntMaterials].nSelect = 0;
		g_materials[nCntMaterials].fRadius = 50.0f;
		g_materials[nCntMaterials].bHit = false;
		g_materials[nCntMaterials].bUse = false;
		g_materials[nCntMaterials].bDisp = false;
	}

	// 素材読込
	LoadMaterials();
	
	// 素材の初期設定
	for (nCntMaterials = 0; nCntMaterials < MATERIALS_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse == true)
		{
			// 影の作成
			g_materials[nCntMaterials].IdxShadow = 
				SetShadow(
					g_materials[nCntMaterials].pos,
					D3DXVECTOR3(20.0f, 0.0f, 20.0f),
					D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitMaterials(void)
{
	for (int nCntMaterials = 0; nCntMaterials < MATERIALS_ITEM_MAX; nCntMaterials++)
	{
		// メッシュの開放
		if (g_materialsParts[nCntMaterials].paMesh != NULL)
		{
			g_materialsParts[nCntMaterials].paMesh->Release();
			g_materialsParts[nCntMaterials].paMesh = NULL;
		}	
		
		// マテリアルの開放
		if (g_materialsParts[nCntMaterials].paBuffMat != NULL)
		{
			g_materialsParts[nCntMaterials].paBuffMat->Release();
			g_materialsParts[nCntMaterials].paBuffMat = NULL;
		}

	}


	for (int nCntTexture = 0; nCntTexture < TEXTURE_MATERIALS; nCntTexture++)
	{
		// テクスチャーの開放
		if (g_paTextureMaterials[nCntTexture] != NULL)
		{
			g_paTextureMaterials[nCntTexture]->Release();
			g_paTextureMaterials[nCntTexture] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateMaterials(void)
{
	// 変数宣言
	int nCntMaterials;

	// 素材の初期設定
	for (nCntMaterials = 0; nCntMaterials < MATERIALS_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse == true)
		{
			// 現在地保存
			g_materials[nCntMaterials].posold = g_materials[nCntMaterials].pos;

			// アイテムの場合
			if (g_materials[nCntMaterials].nSelect == 1)
			{
				// アイテムを拾った時の処理
				Update_PickMaterials(nCntMaterials);

				// 叩いた時の処理
				Update_HitMaterials(nCntMaterials);
			}

			// 当たり判定
			Update_ColisionMaterials(nCntMaterials);

			// 影の位置を設定
			SetPositionShadow(
				g_materials[nCntMaterials].IdxShadow,
				g_materials[nCntMaterials].pos);
		}
	}
}

// ------------------------------------------------------------------------------------------
// 叩く処理
// ------------------------------------------------------------------------------------------
void Update_HitMaterials(int nCntMaterials)
{
	// 移動量
	g_materials[nCntMaterials].move.y -= 0.3f;

	// 移動量
	g_materials[nCntMaterials].move.x *= 0.5f;
	g_materials[nCntMaterials].move.z *= 0.5f;

	// 移動
	g_materials[nCntMaterials].pos +=
		g_materials[nCntMaterials].move;
}

// ------------------------------------------------------------------------------------------
// 拾った処理
// ------------------------------------------------------------------------------------------
void Update_PickMaterials(int nCntMaterials)
{
	// プレイヤー情報取得
	PLAYER *pPlayer = GetPlayer();

	// プレイヤーとアイテムの当たり判定
	if ((pPlayer->fLength * pPlayer->fLength) +
		(g_materials[nCntMaterials].fRadius *
			g_materials[nCntMaterials].fRadius) >=
			(pPlayer->pos.x - g_materials[nCntMaterials].pos.x) *
		(pPlayer->pos.x - g_materials[nCntMaterials].pos.x) +
		(pPlayer->pos.y - g_materials[nCntMaterials].pos.y) *
		(pPlayer->pos.y - g_materials[nCntMaterials].pos.y) +
		(pPlayer->pos.z - g_materials[nCntMaterials].pos.z) *
		(pPlayer->pos.z - g_materials[nCntMaterials].pos.z))
	{
		// プレイヤー回復
		RecoveryPlayer(g_materials[nCntMaterials].ability.nHPRecovery,
			g_materials[nCntMaterials].ability.nMPRecovery);

		// 未使用状態へ
		g_materials[nCntMaterials].bUse = false;
		g_materials[nCntMaterials].bDisp = false;

		// エフェクト
		float fAngle;
		float fYAngle;
		D3DXVECTOR3 move;
		D3DXCOLOR col;

		for (int nCntEffect = 0; nCntEffect < 100; nCntEffect++)
		{
			// X・Zの角度
			fAngle =
				float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// Yの角度
			fYAngle =
				float(rand() % 314) * 0.01f - float(rand() % 314) * 0.01f;

			// 移動方向
			move =
				D3DXVECTOR3(sinf(fAngle) * 5.0f,
					cosf(fYAngle) * 5.0f,
					cosf(fAngle) * 5.0f);

			// カラー
			col =
				D3DXCOLOR(
					0.5f + float(rand() % 50) / 100.0f,
					0.5f - float(rand() % 50) / 100.0f,
					0.0f,
					1.0f);

			// エフェクトの設定
			SetEffect(
				D3DXVECTOR3(													// 位置
					g_materials[nCntMaterials].pos.x,
					g_materials[nCntMaterials].pos.y,
					g_materials[nCntMaterials].pos.z),

				move,															// 移動量
				col,															// 色
				float(rand() % 10),												// サイズ
				-0,																// 減少サイズ
				0.0f,															// 重力
				10,																// エフェクトライフ
				0,																// タイプ
				0);
		}
	}
}

// ------------------------------------------------------------------------------------------
// 当たり判定処理
// ------------------------------------------------------------------------------------------
void Update_ColisionMaterials(int nCntMaterials)
{
	// 床の当たり判定 //
	if (ColisionFLOOR(&g_materials[nCntMaterials].pos,
		&g_materials[nCntMaterials].posold,
		&g_materials[nCntMaterials].move,
		&g_materials[nCntMaterials].size,
		&g_materials[nCntMaterials].pFloor))
	{

	}

}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawMaterials(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATERIAL		*pMat;				// 現在のマテリアル保存
	D3DMATERIAL9		matDef;				// マテリアルデータのポインタ

	// カウント素材
	for (int nCntMaterials = 0; nCntMaterials < MATERIALS_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_materials[nCntMaterials].mtxWorldMaterials);

			// 回転を反映
			// スケールを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_materials[nCntMaterials].rot.y, g_materials[nCntMaterials].rot.x, g_materials[nCntMaterials].rot.z);

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_materials[nCntMaterials].mtxWorldMaterials,
				&g_materials[nCntMaterials].mtxWorldMaterials, &mtxRot);

			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
				g_materials[nCntMaterials].pos.x, g_materials[nCntMaterials].pos.y, g_materials[nCntMaterials].pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_materials[nCntMaterials].mtxWorldMaterials,	// 1
				&g_materials[nCntMaterials].mtxWorldMaterials,					// 2
				&mtxTrans);									// 3

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_materials[nCntMaterials].mtxWorldMaterials);

			// 現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			// マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)g_materialsParts[g_materials[nCntMaterials].nDrawType].paBuffMat->GetBufferPointer();

			// テクスチャー設定
			pDevice->SetTexture(0, g_paTextureMaterials[g_materials[nCntMaterials].nTexType]);

			// カウントマテリアル
			for (int nCntMat = 0; nCntMat < (int)g_materialsParts[g_materials[nCntMaterials].nDrawType].nNumMat; nCntMat++)
			{
				// マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				// 描画
				g_materialsParts[g_materials[nCntMaterials].nDrawType].paMesh->DrawSubset(nCntMat);
			}

			// マテリアルをデフォルトに戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

// ------------------------------------------------------------------------------------------
// 当たり判定処理
// ------------------------------------------------------------------------------------------
int ColisionMaterials(
	D3DXVECTOR3 *pos,				// 現在のポジション
	D3DXVECTOR3 *posOld,			// 前回のポジション
	D3DXVECTOR3 *move,				// 移動量
	D3DXVECTOR3 *size,				// サイズ
	MATERIALS	**ppMaterialsRet	// 素材のポインターのポインター
)
{
	// 変数宣言
	int nLand = -1;		// どこの当たり判定か
	int  nCntMaterials;	// カウント床
							
	// 番地情報があったら
	if (ppMaterialsRet != NULL)
	{
		*ppMaterialsRet = NULL;		// 番地の中身の情報をヌルに
	}

	// 素材ループ
	for (nCntMaterials = 0; nCntMaterials < MATERIALS_MAX; nCntMaterials++)
	{
		// 使用状態
		if (g_materials[nCntMaterials].bUse == true)
		{
			// オブジェクトの当たり判定
			if (g_materials[nCntMaterials].nSelect == 0)
			{
				// 素材のY範囲
				if (pos->y + size->y * 0.5f > g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y&&
					pos->y - size->y * 0.5f < g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y)
				{
					// 素材のZ範囲
					if (pos->z + size->z * 0.5f > g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z&&
						pos->z - size->z * 0.5f < g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z)
					{
						// 当たり判定(左)
						if (pos->x + size->z * 0.5f > g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x&&
							posOld->x + size->z * 0.5f <= g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x)
						{
							// 番地の中身の情報に代入
							*ppMaterialsRet = &g_materials[nCntMaterials];

							// めり込んでいる
							nLand = 2;

							// 素材状の左に
							pos->x = g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x - size->z * 0.5f;

							// 移動量の初期化
							move->x = 0.0f;

						}

						// 当たり判定(右)
						else if (pos->x - size->z * 0.5f < g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x&&
							posOld->x - size->z * 0.5f >= g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x)
						{
							// 番地の中身の情報に代入
							*ppMaterialsRet = &g_materials[nCntMaterials];

							// めり込んでいる
							nLand = 2;

							// 素材状の左に
							pos->x = g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x + size->z * 0.5f;

							// 移動量の初期化
							move->x = 0.0f;
						}
					}

					// 素材のX範囲
					if (pos->x + size->x * 0.5f > g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x&&
						pos->x - size->x * 0.5f < g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x)
					{
						// 当たり判定(手前)
						if (pos->z + size->z * 0.5f > g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z&&
							posOld->z + size->z * 0.5f <= g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z)
						{
							// 番地の中身の情報に代入
							*ppMaterialsRet = &g_materials[nCntMaterials];

							// めり込んでいる
							nLand = 2;

							// 素材状の左に
							pos->z = g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z - size->z * 0.5f;

							// 移動量の初期化
							move->z = 0.0f;
						}

						// 当たり判定(奥)
						else if (pos->z - size->z * 0.5f < g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z&&
							posOld->z - size->z * 0.5f >= g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z)
						{
							// 番地の中身の情報に代入
							*ppMaterialsRet = &g_materials[nCntMaterials];

							// めり込んでいる
							nLand = 2;

							// 素材状の左に
							pos->z = 
								g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z +
								size->z * 0.5f + 0.1f;

							// 移動量の初期化
							move->z = 0.0f;
						}
					}
				}

				// 素材のZ範囲
				if (pos->z + size->z * 0.5f > g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z&&
					pos->z - size->z * 0.5f < g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z)
				{
					// 素材のX範囲
					if (pos->x + size->x * 0.5f > g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x&&
						pos->x - size->x * 0.5f < g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x)
					{
						// 当たり判定(下)
						if (pos->y + size->y * 0.5f > g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y&&
							posOld->y + size->y * 0.5f <= g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y)
						{
							// 番地の中身の情報に代入
							*ppMaterialsRet = &g_materials[nCntMaterials];

							// めり込んでいる
							nLand = 1;

							// 素材状の左に
							pos->y = g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y - size->y * 0.5f;

							// 移動量の初期化
							move->y = 0.0f;
						}

						// 当たり判定(上)
						else if (pos->y - size->y * 0.5f < g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y&&
							posOld->y - size->y * 0.5f >= g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y)
						{
							// 番地の中身の情報に代入
							*ppMaterialsRet = &g_materials[nCntMaterials];

							// めり込んでいる
							nLand = 0;

							// 素材状の左に
							pos->y = g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y + size->y * 0.5f;

							// 移動量の初期化
							move->y = 0.0f;
						}
					}
				}
			}
		}
	}
	return nLand;
}

// ------------------------------------------------------------------------------------------
// 設置処理
// ------------------------------------------------------------------------------------------
void SetMaterials(
	D3DXVECTOR3 pos,			// 位置
	D3DXVECTOR3 rot,			// 回転
	D3DXVECTOR3 move,			// 移動量
	int			nDrawType,		// 描画タイプ
	int			nHPRecovery,	// HP用
	int			nMPRecovery		// MP用
)
{
	// 変数宣言
	int nCntMaterials;

	// 素材ループ
	for (nCntMaterials = 0; nCntMaterials < MATERIALS_MAX; nCntMaterials++)
	{
		// 使用状態
		if (g_materials[nCntMaterials].bUse == false)
		{
			// 情報設定
			g_materials[nCntMaterials].pos = pos;										// 位置
			g_materials[nCntMaterials].rot = rot;										// 回転情報
			g_materials[nCntMaterials].move = move;										// 移動量
			g_materials[nCntMaterials].nDrawType = nDrawType;							// 描画タイプ
			g_materials[nCntMaterials].vtxMinMaterials = 								// 最小値
				g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMinMaterials;	
			g_materials[nCntMaterials].vtxMaxMaterials =								// 最大値
				g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMaxMaterials;	
			g_materials[nCntMaterials].ability.nHPRecovery = nHPRecovery;				// HP回復量
			g_materials[nCntMaterials].ability.nMPRecovery = nMPRecovery;				// MP回復量

			// 描画タイプがオブジェクトの場合
			if (g_materials[nCntMaterials].nDrawType < MATERIALS_OBJECT_MAX)
			{
				g_materials[nCntMaterials].nSelect = 0;
			}

			// 描画タイプがアイテムの場合
			else
			{
				g_materials[nCntMaterials].nSelect = 1;
			}
			g_materials[nCntMaterials].bUse = true;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 素材読み込み処理
// ------------------------------------------------------------------------------------------
void LoadMaterials(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad = 0;
	int nCntMaterials = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// チュートリアル用
	if (GetMode() == MODE_TUTORIAL)
	{
		// ファイル開
		pFile = fopen("data/SAVE/TUTORIAL/materials.txt", "r");
	}

	// ボス用
	else if (GetGameStete() == GAMESTATE_BOSS)
	{
		// ファイル開
		pFile = fopen("data/SAVE/GAME_BOSS/materials.txt", "r");
	}

	// それ以外
	else
	{
		// ファイル開
		pFile = fopen("data/SAVE/materials.txt", "r");
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
				else if (strcmp(cHeadText, "MATERIALSSET") == 0)
				{

					// エンドモデルセットが来るまでループ
					while (strcmp(cHeadText, "END_MATERIALSSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 親情報読み込み
						if (strcmp(cHeadText, "TYPE") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_materials[nCntMaterials].nDrawType);

							// サイズ
							g_materials[nCntMaterials].vtxMinMaterials =
								g_materialsParts[g_materials[nCntMaterials].nDrawType].size;

							// 最小値と最大値
							g_materials[nCntMaterials].vtxMinMaterials = 
								g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMinMaterials;
							g_materials[nCntMaterials].vtxMaxMaterials = 
								g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMaxMaterials;

							// 描画タイプがオブジェクトの場合
							if (g_materials[nCntMaterials].nDrawType < MATERIALS_OBJECT_MAX)
							{
								g_materials[nCntMaterials].nSelect = 0;
							}

							// 描画タイプがアイテムの場合
							else
							{
								g_materials[nCntMaterials].nSelect = 1;
							}
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_materials[nCntMaterials].pos.x,
								&g_materials[nCntMaterials].pos.y,
								&g_materials[nCntMaterials].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_materials[nCntMaterials].rot.x,
								&g_materials[nCntMaterials].rot.y,
								&g_materials[nCntMaterials].rot.z);
						}
					}

					// チュートリア
					if (GetMode() == MODE_TUTORIAL)
					{
						SetBillboard(D3DXVECTOR3(
							g_materials[nCntMaterials].pos.x + 100.0f,
							g_materials[nCntMaterials].pos.y + 50.0f ,
							g_materials[nCntMaterials].pos.z + 100.0f),
							D3DXVECTOR3(0.0f, 0.0f, 0.0f),
							D3DXVECTOR3(20.0f, 20.0f, 0.0f),
							BILLBOARDTYPE_TREE);
					}

					// 赤い木の場合
					if (g_materials[nCntMaterials].nDrawType == MATERIALS_OBJECT_01_WOOD_RED)
					{
						g_materials[nCntMaterials].nIdxDome  = 
							SetDome(g_materials[nCntMaterials].pos, 
								D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f),
								0);
					}
					// オレンジ木の場合
					if (g_materials[nCntMaterials].nDrawType == MATERIALS_OBJECT_02_WOOD_ORANGE)
					{
						g_materials[nCntMaterials].nIdxDome =
							SetDome(g_materials[nCntMaterials].pos,
								D3DXCOLOR(1.0f, 0.5f, 0.0f, 0.5f),
								0);
					}

					// 使用状態へ
					g_materials[nCntMaterials].bUse = true;

					// 表示状態へ
					g_materials[nCntMaterials].bDisp = true;

					// モデルの更新
					nCntMaterials++;
				}
			}
		}
		// ファイル閉
		fclose(pFile);
	}

	// 開けない
	else
	{
		MessageBox(NULL, "素材データの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}

}

// ------------------------------------------------------------------------------------------
// 素材情報処理
// ------------------------------------------------------------------------------------------
MATERIALS *GetMaterials(void)
{
	return &g_materials[0];
}