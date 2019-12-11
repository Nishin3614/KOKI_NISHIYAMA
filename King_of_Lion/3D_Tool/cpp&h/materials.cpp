// ------------------------------------------------------------------------------------------
//
// 素材処理 [materials.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "materials.h"
#include "input.h"
#include "shadow.h"
#include "wall.h"
#include "camera.h"
#include "floor.h"
#include "arrangement.h"
#include "mouse.h"
#include "game.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define TEXTURE_MATERIALS (1)
#define SIZE (0.5f)

// ------------------------------------------------------------------------------------------
// 構造体
// ------------------------------------------------------------------------------------------
// ---------- トライ敵 ---------- //
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
void MoveMaterials(void);
void RotMaterials(void);
void SetMaterials(int nType,D3DXVECTOR3 pos,D3DXVECTOR3 rot);
void Update_DestroyMaterials(void);
void Update_DispMaterials(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPD3DXMESH		g_paMeshMaterials[MATERIALSTYPE_MAX] = {};				// メッシュ情報へのポインタ
LPD3DXBUFFER	g_paBuffMatMaterials[MATERIALSTYPE_MAX] = {};			// マテリアル情報へのポインタ
DWORD			g_anNummatMaterials[MATERIALSTYPE_MAX];					// マテリアルの数
LPDIRECT3DTEXTURE9  g_paTextureMaterials[TEXTURE_MATERIALS] = {};	// テクスチャへのポインタ
MATERIALS_PARTS		g_materialsParts[MATERIALSTYPE_MAX];			// パーツ情報
MATERIALS			g_materials[ALL_MAX];						// 素材情報
int				g_nMaterialsNumType = 0;							// ナンバータイプ
int				g_nMaterialsIdxShadow = 0;							// 影のID

// 3D用のセーブファイル
const char g_cMatarialsFIleName_3D[5][128] =
{
	"data/SAVE/3D/Stage_1/materials.txt",
	"data/SAVE/3D/Stage_2/materials.txt",
	"data/SAVE/3D/Stage_3/materials.txt",
	"data/SAVE/3D/Stage_4/materials.txt",
	"data/SAVE/3D/Stage_5/materials.txt"
};

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
	char *paMaterialsFile[MATERIALSTYPE_MAX] =
	{
		{ "data/MODEL/materials/00_wood_green.x" },						// [緑木]00
		{ "data/MODEL/materials/00_wood_green.x" },						// [緑木]00
		{ "data/MODEL/materials/00_wood_green.x" },						// [緑木]00

	};

	// テクスチャーファイル名定義
	char *paTextureFile[TEXTURE_MATERIALS] =
	{
		{ "" }
	};

	// パーツの初期化
	for (nCntModelType = 0; nCntModelType < MATERIALSTYPE_MAX; nCntModelType++)
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
	for (nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		g_materials[nCntMaterials].pos = D3DXVECTOR3(200.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].posold = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].posKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rotLast = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rotbetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].rotKeyBetween = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_materials[nCntMaterials].nDrawType = 0;
		g_materials[nCntMaterials].nTexType = 0;
		g_materials[nCntMaterials].fLength = 0.0f;
		g_materials[nCntMaterials].bUse = false;
		g_materials[nCntMaterials].bDisp = false;
	}

	// 素材読込
	LoadMaterials();
	
	// 影の作成
	g_nMaterialsIdxShadow = SetShadow(g_materials[0].pos, g_materials[0].rot,D3DXVECTOR3(20.0f,0.0f,20.0f));
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitMaterials(void)
{
	for (int nCntMaterials = 0; nCntMaterials < MATERIALSTYPE_MAX; nCntMaterials++)
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
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();
	
	LONG		pMouseX = 0;
	LONG		pMouseY = 0;

	// マウスの情報取得
	pMouseX = GetMouseX();
	pMouseY = GetMouseY();

	for (int nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		// 現在地保存
		g_materials[nCntMaterials].posold = g_materials[nCntMaterials].pos;

		// 影の位置を設定
		SetPositionShadow(g_nMaterialsIdxShadow, g_materials[nCntMaterials].pos);
	}

	// 削除
	Update_DestroyMaterials();
	// 配置するときに表示
	Update_DispMaterials();

	// 範囲
	if (pMouseX < 1000.0f)
	{
		// 画面内を左クリックで、カーソルの位置に敵配置
		if (GetMousePressTrigger(0))
		{
			SetMaterials(0, pArrangement->pos, pArrangement->rot);
		}
	}

	// セーブ
	if (GetKeyboardSpeacePress(DIK_F7))
	{
		// 素材データの保存
		SaveMaterials();
	}
}

// ------------------------------------------------------------------------------------------
// 削除処理
// ------------------------------------------------------------------------------------------
void Update_DestroyMaterials(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	for (int nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse == true && g_materials[nCntMaterials].bDisp == true)
		{
			// ブロックのX範囲
			if (pArrangement->pos.x >
				g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x&&
				pArrangement->pos.x <
				g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x&&
				pArrangement->pos.z >
				g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z&&
				pArrangement->pos.z <
				g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z)
			{
				// 削除処理
				if (GetKeyboardSpeacePress(DIK_B))
				{
					g_materials[nCntMaterials].bUse = false;
					g_materials[nCntMaterials].bDisp = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 素材を置くときに表示処理
// ------------------------------------------------------------------------------------------
void Update_DispMaterials(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	int nCntMaterials;

	for (nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse == false)
		{
			g_materials[nCntMaterials].bDisp = true;

			g_materials[nCntMaterials].pos = pArrangement->pos;

			g_materials[nCntMaterials].rot = pArrangement->rot;
			break;
		}
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
	for (int nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bDisp == true)
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
bool ColisionMaterials(
	D3DXVECTOR3 * pos,
	D3DXVECTOR3 * posOld,
	D3DXVECTOR3 * move,
	D3DXVECTOR3 * size,
	MATERIALS ** ppMaterialsRet)
{
	// 変数宣言
	bool bLand = false;		// めり込んでいるかいないか
	int  nCntMaterials;			// カウント床
							// 番地情報があったら
	if (ppMaterialsRet != NULL)
	{
		*ppMaterialsRet = NULL;		// 番地の中身の情報をヌルに
	}

	// ブロックループ
	for (nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{

		// 使用状態
		if (g_materials[nCntMaterials].bUse == true)
		{

			// ブロックのY範囲
			if (pos->y + size->y * 0.5f > g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y&&
				pos->y - size->y * 0.5f < g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y)
			{
				// ブロックのZ範囲
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
						bLand = true;

						// ブロック状の左に
						pos->x = g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x - size->z * 0.5f;

						// 移動量の初期化
						move->x = 0.0f;
					}

					// 当たり判定(右)
					else if (pos->x - size->z * 0.5f< g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x&&
						posOld->x - size->z * 0.5f >= g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x)
					{
						// 番地の中身の情報に代入
						*ppMaterialsRet = &g_materials[nCntMaterials];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->x = g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x + size->z * 0.5f;

						// 移動量の初期化
						move->x = 0.0f;
					}
				}

				// ブロックのX範囲
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
						bLand = true;

						// ブロック状の左に
						pos->z = g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z - size->z * 0.5f;

						// 移動量の初期化
						move->z = 0.0f;
					}

					// 当たり判定(奥)
					else if (pos->z - size->z * 0.5f < g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z&&
						posOld->z - size->z * 0.5f >= g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z )
					{
						// 番地の中身の情報に代入
						*ppMaterialsRet = &g_materials[nCntMaterials];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->z = g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z + size->z * 0.5f;

						// 移動量の初期化
						move->z = 0.0f;
					}
				}
			}

			// ブロックのZ範囲
			if (pos->z + size->z * 0.5f > g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMinMaterials.z&&
				pos->z - size->z * 0.5f < g_materials[nCntMaterials].pos.z + g_materials[nCntMaterials].vtxMaxMaterials.z)
			{
				// ブロックのX範囲
				if (pos->x + size->x * 0.5f > g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMinMaterials.x&&
					pos->x - size->x * 0.5f < g_materials[nCntMaterials].pos.x + g_materials[nCntMaterials].vtxMaxMaterials.x)
				{
					// 当たり判定(下
					if (pos->y + size->y * 0.5f > g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y&&
						posOld->y + size->y * 0.5f <= g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y)
					{
						// 番地の中身の情報に代入
						*ppMaterialsRet = &g_materials[nCntMaterials];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->y = g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMinMaterials.y - size->y * 0.5f;

						// 移動量の初期化
						move->y = 0.0f;
					}

					// 当たり判定(上)
					else if (pos->y - size->y * 0.5f< g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y&&
						posOld->y - size->y * 0.5f >= g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y)
					{
						// 番地の中身の情報に代入
						*ppMaterialsRet = &g_materials[nCntMaterials];

						// めり込んでいる
						bLand = true;

						// ブロック状の左に
						pos->y = g_materials[nCntMaterials].pos.y + g_materials[nCntMaterials].vtxMaxMaterials.y + size->y * 0.5f;

						// 移動量の初期化
						move->y = 0.0f;
					}
				}
			}
		}
	}
	return bLand;
}

// ------------------------------------------------------------------------------------------
// 移動処理
// ------------------------------------------------------------------------------------------
void MoveMaterials(void)
{

}

// ------------------------------------------------------------------------------------------
// 回転処理
// ------------------------------------------------------------------------------------------
void RotMaterials(void)
{

}

// ------------------------------------------------------------------------------------------
// 素材設置処理
// ------------------------------------------------------------------------------------------
void SetMaterials(int nType,D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// カウント素材
	for (int nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse == false)
		{
			// 設定を代入
			g_materials[nCntMaterials].pos = pos;
			g_materials[nCntMaterials].rot = rot;
			g_materials[nCntMaterials].nDrawType = nType;

			// 最大値
			g_materials[nCntMaterials].vtxMaxMaterials =
				g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMaxMaterials;

			// 最小値
			g_materials[nCntMaterials].vtxMinMaterials =
				g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMinMaterials;

			// サイズ
			g_materials[nCntMaterials].size =
				g_materialsParts[g_materials[nCntMaterials].nDrawType].size;

			g_materials[nCntMaterials].bUse = true;
			g_materials[nCntMaterials].bDisp = true;
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 配置場所がわかるために描画していたものを非表示に処理
// ------------------------------------------------------------------------------------------
void NotDispMaterials(void)
{
	int nCntMaterials;

	for (nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse == false &&
			g_materials[nCntMaterials].bDisp == true)
		{
			g_materials[nCntMaterials].bDisp = false;

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
	
	// もし表示しているものがあったら
	for (nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		if (g_materials[nCntMaterials].bUse == true)
		{
			g_materials[nCntMaterials].bUse = false;
			g_materials[nCntMaterials].bDisp = false;
		}
	}

	// カウント初期化
	nCntMaterials = 0;

	// ファイル開
	pFile = fopen(g_cMatarialsFIleName_3D[GetGameStage()], "r");

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
								// 最大値
								g_materials[nCntMaterials].vtxMaxMaterials =
									g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMaxMaterials;

								// 最小値
								g_materials[nCntMaterials].vtxMinMaterials =
									g_materialsParts[g_materials[nCntMaterials].nDrawType].vtxMinMaterials;

								// サイズ
								g_materials[nCntMaterials].size = 
									g_materialsParts[g_materials[nCntMaterials].nDrawType].size;
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
// 選択しているオブジェクトを設定
// ------------------------------------------------------------------------------------------
bool SeleMaterials(int nCntMaterials)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	ARRANGEMENT	*pArrangement = GetArrangement();

	// 移動
	if (GetMousePress(0))
	{
		// 配置の位置情報を取得
		g_materials[nCntMaterials].pos = pArrangement->pos;
	}

	// タイプの変化
	if (GetKeyboardSpeacePress(DIK_1))
	{
		g_materials[nCntMaterials].nDrawType++;

		// 限界値まで行ったら
		if (g_materials[nCntMaterials].nDrawType >= MATERIALSTYPE_MAX)
		{
			// 初期化
			g_materials[nCntMaterials].nDrawType = 0;
		}
	}

	// 削除
	if (GetKeyboardSpeacePress(DIK_BACK))
	{
		g_materials[nCntMaterials].bDisp = false;
		g_materials[nCntMaterials].bUse = false;
	}

	return g_materials[nCntMaterials].bUse;
}

// ------------------------------------------------------------------------------------------
// セーブ情報処理
// ------------------------------------------------------------------------------------------
void SaveMaterials(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int		nCntMaterials = 0;
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
	char *cMaterialsType[MATERIALSTYPE_MAX] =
	{
		{ "    ----------[00]:木----------" },				// [木]00
		{ "    ----------[00]:木----------" },				// [木]00
		{ "    ----------[00]:木----------" },				// [木]00
	};

	// 床データの種類
	char *cMaterialsData[] =
	{
		{ "TYPE" },				// タイプ
		{ "POS" },				// 位置
		{ "ROT" },				// 回転
		{ "END_MATERIALSSET" }	// エンドフロアー
	};

	// セーブ用 //
	// ファイル開
	pFile = fopen(g_cMatarialsFIleName_3D[GetGameStage()], "w");

	// 開けた
	if (pFile != NULL)
	{
		//------------------ コメント ------------------//
		strcpy(cWriteText, cComment);

		strcat(cWriteText, cSharpEnter);

		strcat(cWriteText, "# セーブ素材\n");

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
				if (nCntMaterials < ALL_MAX - 1)
				{
					if (g_materials[nCntMaterials].bUse == true)
					{
						fputs(cEnter, pFile);

						// 素材の名前を代入
						strcpy(cWriteText, cMaterialsType[g_materials[nCntMaterials].nDrawType]);
						fputs(cWriteText, pFile);
						fputs(cEnter, pFile);

						// エンドスクリプトではない
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    MATERIALSSET\n");
							fputs(cWriteText, pFile);

							// エンドモデルセットが来るまでループ
							while (strcmp(cWriteText, "END_MATERIALSSET") != 0)
							{
								// 文字列をコピーする
								strcpy(cWriteText, cMaterialsData[nCntDataLoad]);

								// タイプ情報保存
								if (strcmp(cWriteText, "TYPE") == 0)
								{
									sprintf(cHeadText, "        %s %s %d\n", &cWriteText, &cEqual, g_materials[nCntMaterials].nDrawType);
									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 位置情報保存
								else if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_materials[nCntMaterials].pos.x,
										g_materials[nCntMaterials].pos.y,
										g_materials[nCntMaterials].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 回転情報保存
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_materials[nCntMaterials].rot.x,
										g_materials[nCntMaterials].rot.y,
										g_materials[nCntMaterials].rot.z);

									fputs(cHeadText, pFile);

									// カウントデータ初期化
									nCntDataLoad++;
								}

								// 回転情報保存
								else if (strcmp(cWriteText, "END_MATERIALSSET") == 0)
								{
									sprintf(cHeadText, "    %s", &cWriteText);

									fputs(cHeadText, pFile);

									// カウントデータ初期化
									nCntDataLoad = 0;
								}

							}
						}
					}
					// 素材カウント変更
					nCntMaterials++;
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
		MessageBox(NULL, "素材データの書き込みに失敗しました", "警告！", MB_ICONWARNING);
		// ファイル閉
		fclose(pFile);
	}
}

// ------------------------------------------------------------------------------------------
// 配置時にわかりやすく表示させていたものを非表示に処理
// ------------------------------------------------------------------------------------------
void NotDisp_Materials(void)
{
	int nCntMaterials;

	// 素材のカウント
	for (nCntMaterials = 0; nCntMaterials < ALL_MAX; nCntMaterials++)
	{
		// 今使っているもの
		if (g_materials[nCntMaterials].bUse == false &&
			g_materials[nCntMaterials].bDisp == true)
		{
			g_materials[nCntMaterials].bDisp = false;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 素材情報処理
// ------------------------------------------------------------------------------------------
MATERIALS *GetMaterials(void)
{
	return &g_materials[0];
}