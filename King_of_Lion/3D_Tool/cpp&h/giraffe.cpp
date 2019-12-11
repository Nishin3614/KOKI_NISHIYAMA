// ------------------------------------------------------------------------------------------
//
// モデル処理 [giraffe.cpp]
// Author : KOKI NISHIYAMA
//
// ------------------------------------------------------------------------------------------
#include "giraffe.h"
#include "input.h"
#include "shadow.h"
#include "wall.h"
#include "camera.h"
#include "floor.h"
#include "materials.h"
#include "arrangement.h"
#include "mouse.h"
#include "game.h"

// ------------------------------------------------------------------------------------------
// マクロ定義
// ------------------------------------------------------------------------------------------
#define TEXTURE_GIRAFFE (1)

// ------------------------------------------------------------------------------------------
// プロトタイプ宣言
// ------------------------------------------------------------------------------------------
void Init_PartsGiraffe(void);
void Init_GiraffeInfo(void);
void Init_MotionGiraffe(void);
void SetGiraffe(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void Update_DispGiraffe(void);
void LoadGiraffeModel(void);
void LoadGiraffeMosion(void);

// ------------------------------------------------------------------------------------------
// グローバル変数
// ------------------------------------------------------------------------------------------
LPDIRECT3DTEXTURE9  g_paTextureGiraffe[TEXTURE_GIRAFFE] = {};	// テクスチャへのポインタ
GIRAFFE				g_giraffe[ALL_MAX];							// キリン情報
MODEL				g_giraffeModel[GIRAFFETYPE_MAX];			// モデル情報
MOSION_GIRAFFE		g_giraffeKey[GIRAFFEMOSIONSET_MAX];			// モーション情報
D3DXVECTOR3			g_gireffe_vtxMax;							// 最大値
D3DXVECTOR3			g_gireffe_vtxMin;							// 最小値
D3DXVECTOR3			g_gireffe_size;								// サイズ

// 3D用のセーブファイル
const char g_cGiraffeFIleName_3D[5][128] =
{
	"data/SAVE/3D/Stage_1/giraffe.txt",
	"data/SAVE/3D/Stage_2/giraffe.txt",
	"data/SAVE/3D/Stage_3/giraffe.txt",
	"data/SAVE/3D/Stage_4/giraffe.txt",
	"data/SAVE/3D/Stage_5/giraffe.txt"
};

// ------------------------------------------------------------------------------------------
// 初期化処理
// ------------------------------------------------------------------------------------------
void InitGiraffe(void)
{
	// キリンパーツ情報
	Init_PartsGiraffe();

	// キリン情報
	Init_GiraffeInfo();

	// キリンモーション情報
	Init_MotionGiraffe();
}

// ------------------------------------------------------------------------------------------
// 終了処理
// ------------------------------------------------------------------------------------------
void UninitGiraffe(void)
{
	// モデルデータの開放
	for (int nCntModel = 0; nCntModel < GIRAFFETYPE_MAX; nCntModel++)
	{
		// メッシュの開放
		if (g_giraffeModel[nCntModel].paMesh != NULL)
		{
			g_giraffeModel[nCntModel].paMesh->Release();
			g_giraffeModel[nCntModel].paMesh = NULL;
		}

		// マテリアルの開放
		if (g_giraffeModel[nCntModel].paBuffMat != NULL)
		{
			g_giraffeModel[nCntModel].paBuffMat->Release();
			g_giraffeModel[nCntModel].paBuffMat = NULL;
		}
	}

	// モデルテクスチャの開放
	for (int nCntTexture = 0; nCntTexture < TEXTURE_GIRAFFE; nCntTexture++)
	{
		// テクスチャーの開放
		if (g_paTextureGiraffe[nCntTexture] != NULL)
		{
			g_paTextureGiraffe[nCntTexture]->Release();
			g_paTextureGiraffe[nCntTexture] = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 更新処理
// ------------------------------------------------------------------------------------------
void UpdateGiraffe(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	// 配置するときに表示
	Update_DispGiraffe();

	// 画面内を左クリックで、カーソルの位置に敵配置
	if (GetMousePressTrigger(0))
	{
		SetGiraffe(pArrangement->pos, pArrangement->rot);
	}

	// セーブ
	if (GetKeyboardSpeacePress(DIK_F7))
	{
		// 素材データの保存
		SaveGiraffe();
	}
}

// ------------------------------------------------------------------------------------------
// 素材を置くときに表示処理
// ------------------------------------------------------------------------------------------
void Update_DispGiraffe(void)
{
	// 配置情報
	ARRANGEMENT *pArrangement = GetArrangement();

	int nCntGiraffe;

	for (nCntGiraffe = 0; nCntGiraffe < ALL_MAX; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse == false)
		{
			g_giraffe[nCntGiraffe].bDisp = true;

			g_giraffe[nCntGiraffe].pos = pArrangement->pos;

			g_giraffe[nCntGiraffe].rot = pArrangement->rot;
			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// 描画処理
// ------------------------------------------------------------------------------------------
void DrawGiraffe(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();
	
	D3DXMATRIX			mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATERIAL		*pMat;				// 現在のマテリアル保存
	D3DMATERIAL9		matDef;				// マテリアルデータのポインタ
	int nCntGiraffe;
	int nCntModel;

	// カウントモデル
	for (nCntGiraffe = 0; nCntGiraffe < ALL_MAX; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bDisp)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_giraffe[nCntGiraffe].mtxWorldGiraffe);

			// 回転を反映
			// スケールを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot,
				g_giraffe[nCntGiraffe].rot.y, 
				g_giraffe[nCntGiraffe].rot.x, 
				g_giraffe[nCntGiraffe].rot.z);

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
			D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].mtxWorldGiraffe,
				&g_giraffe[nCntGiraffe].mtxWorldGiraffe, &mtxRot);


			// 位置を反映 //
			// 平行移動行列作成(オフセット)
			D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
				g_giraffe[nCntGiraffe].pos.x, 
				g_giraffe[nCntGiraffe].pos.y, 
				g_giraffe[nCntGiraffe].pos.z);	// それぞれの移動量

			// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
			D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].mtxWorldGiraffe,	// 1
				&g_giraffe[nCntGiraffe].mtxWorldGiraffe,					// 2
				&mtxTrans);									// 3

			// カウントモデル
			for (nCntModel = 0; nCntModel < GIRAFFETYPE_MAX; nCntModel++)
			{
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot,
					g_giraffe[nCntGiraffe].model[nCntModel].rot.y,
					g_giraffe[nCntGiraffe].model[nCntModel].rot.x, 
					g_giraffe[nCntGiraffe].model[nCntModel].rot.z);

				// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
				D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
					&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel, &mtxRot);

				// 位置を反映 //
				// 平行移動行列作成(オフセット)
				D3DXMatrixTranslation(&mtxTrans,							// 総合の入れ物
					g_giraffe[nCntGiraffe].model[nCntModel].pos.x,
					g_giraffe[nCntGiraffe].model[nCntModel].pos.y, 
					g_giraffe[nCntGiraffe].model[nCntModel].pos.z);	// それぞれの移動量

				// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
				D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,	// 1
					&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,					// 2
					&mtxTrans);									// 3


				// [すべての親]じゃない場合
				if (g_giraffe[nCntGiraffe].model[nCntModel].nIdxModelParent != -1)
				{
					// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
					D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
						&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
						&g_giraffe[nCntGiraffe].model[g_giraffe[nCntGiraffe].model[nCntModel].nIdxModelParent].mtxWorldModel);
				}

				else
				{
					// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
					D3DXMatrixMultiply(&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
						&g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel,
						&g_giraffe[nCntGiraffe].mtxWorldGiraffe);
				}

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_giraffe[nCntGiraffe].model[nCntModel].mtxWorldModel);

				// 現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);

				// マテリアル情報に対するポインタを取得
				pMat = (D3DXMATERIAL*)g_giraffeModel[nCntModel].paBuffMat->GetBufferPointer();

				// テクスチャー設定
				pDevice->SetTexture(0, g_paTextureGiraffe[0]);

				// カウントマテリアル
				for (int nCntMat = 0; nCntMat < (int)g_giraffeModel[nCntModel].nNumMat; nCntMat++)
				{
					// マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					// 描画
					g_giraffeModel[nCntModel].paMesh->DrawSubset(nCntMat);
				}

				// マテリアルをデフォルトに戻す
				pDevice->SetMaterial(&matDef);
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// 選択しているオブジェクトを設定
// ------------------------------------------------------------------------------------------
bool SeleGiraffe(int nCntGiraffe)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得
	ARRANGEMENT	*pArrangement = GetArrangement();

	// 移動
	if (GetMousePress(0))
	{
		// 配置の位置情報を取得
		g_giraffe[nCntGiraffe].pos = pArrangement->pos;
	}

	// 削除
	if (GetKeyboardSpeacePress(DIK_BACK))
	{
		g_giraffe[nCntGiraffe].bDisp = false;
		g_giraffe[nCntGiraffe].bUse = false;
	}

	return g_giraffe[nCntGiraffe].bUse;
}

// ------------------------------------------------------------------------------------------
// キリン設置処理
// ------------------------------------------------------------------------------------------
void SetGiraffe(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// カウント素材
	for (int nCntGiraffe = 0; nCntGiraffe < ALL_MAX; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse == false)
		{
			// 設定を代入
			g_giraffe[nCntGiraffe].pos = pos;
			g_giraffe[nCntGiraffe].rot = rot;

			g_giraffe[nCntGiraffe].bUse = true;
			g_giraffe[nCntGiraffe].bDisp = true;
			break;
		}
	}
}


// ------------------------------------------------------------------------------------------
// 配置場所がわかるために描画していたものを非表示に処理
// ------------------------------------------------------------------------------------------
void NotDispGiraffe(void)
{
	int nCntGiraffe;

	for (nCntGiraffe = 0; nCntGiraffe < ALL_MAX; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse == false &&
			g_giraffe[nCntGiraffe].bDisp == true)
		{
			g_giraffe[nCntGiraffe].bDisp = false;

			break;
		}
	}
}

// ------------------------------------------------------------------------------------------
// キリンパーツ情報処理
// ------------------------------------------------------------------------------------------
void Init_PartsGiraffe(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = GetDevice();	// デバイスの取得
	int		nCntModel;							// モデルのカウント

	// モデルファイル名定義
	char *paGiraffeFile[GIRAFFETYPE_MAX] =
	{
		{ GIRAFFE_TEX1"/giraffe/00_Giraffe_body.x" },			// [0]体
		{ GIRAFFE_TEX1"/giraffe/01_Giraffe_JointRoot_Neck.x" },	// [1]首の根元の関節
		{ GIRAFFE_TEX1"/giraffe/02_Giraffe_Neck.x" },			// [2]首
		{ GIRAFFE_TEX1"/giraffe/03_Giraffe_JointTip_Neck.x" },	// [3]首の先端の関節
		{ GIRAFFE_TEX1"/giraffe/04_Giraffe_Head.x" },			// [4]頭
		{ GIRAFFE_TEX1"/giraffe/05_Giraffe_RightHand.x" },		// [5]右手
		{ GIRAFFE_TEX1"/giraffe/06_Giraffe_LeftHand.x" },		// [6]左手
		{ GIRAFFE_TEX1"/giraffe/07_Giraffe_RightLeg.x" },		// [7]右足
		{ GIRAFFE_TEX1"/giraffe/08_Giraffe_LeftLeg.x" },		// [8]左足
		{ GIRAFFE_TEX1"/giraffe/09_Giraffe_Joint_Tail.x" },		// [9]しっぽの関節
		{ GIRAFFE_TEX1"/giraffe/10_Giraffe_JTail.x" }			// [10]しっぽ
	};

	// テクスチャーファイル名定義
	char *paTextureFile[TEXTURE_GIRAFFE] =
	{
		{ "data/TEXTURE/giraffe/00_giraffe_Texture.png" },
	};

	// パーツの読み込み //
	for (nCntModel = 0; nCntModel < GIRAFFETYPE_MAX; nCntModel++)
	{
		g_giraffeModel[nCntModel].paBuffMat			= NULL;
		g_giraffeModel[nCntModel].paMesh			= NULL;
		g_giraffeModel[nCntModel].nNumMat			= 0;
		g_giraffeModel[nCntModel].pos				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].posLast			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].posKeyBetween		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].rotLast			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].rotKeyBetween		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].size				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].vtxMinMaterials	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].vtxMaxMaterials	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffeModel[nCntModel].nIdxModelParent	= 0;
		g_giraffeModel[nCntModel].nFram				= 0;
		g_giraffeModel[nCntModel].nCntKeySet		= 0;

		// Xファイルの読み込み
		D3DXLoadMeshFromX(
			paGiraffeFile[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_giraffeModel[nCntModel].paBuffMat,
			NULL,
			&g_giraffeModel[nCntModel].nNumMat,
			&g_giraffeModel[nCntModel].paMesh);

		// モデルの頂点座標の最大値・最小値の設定
		int nNumVertex;			// 頂点数
		DWORD sizeFVF;			// 頂点フォーマットのサイズ
		BYTE *pVertexBuffer;	// 頂点バッファへのポインタ

								// 頂点数を取得
		nNumVertex = g_giraffeModel[nCntModel].paMesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		sizeFVF = D3DXGetFVFVertexSize(g_giraffeModel[nCntModel].paMesh->GetFVF());

		// 頂点バッファをロック
		g_giraffeModel[nCntModel].paMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);

		// 頂点数分繰り返す
		for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
		{
			// 番地情報のD3DXVE
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

			// -----------すべての頂点情報を比較して最小値・最大値を抜き出す---------- //
			// x //
			// 最小値
			if (g_giraffeModel[nCntModel].vtxMinMaterials.x > vtx.x)
			{
				g_giraffeModel[nCntModel].vtxMinMaterials.x = vtx.x;
			}

			// 最大値
			if (g_giraffeModel[nCntModel].vtxMaxMaterials.x < vtx.x)
			{
				g_giraffeModel[nCntModel].vtxMaxMaterials.x = vtx.x;
			}

			// x //
			// 最小値
			if (g_gireffe_vtxMin.x > g_giraffeModel[nCntModel].vtxMinMaterials.x)
			{
				g_gireffe_vtxMin.x = g_giraffeModel[nCntModel].vtxMinMaterials.x;
			}

			// 最大値
			if (g_gireffe_vtxMax.x < g_giraffeModel[nCntModel].vtxMaxMaterials.x)
			{
				g_gireffe_vtxMax.x = g_giraffeModel[nCntModel].vtxMaxMaterials.x;
			}

			// y //
			// 最小値
			if (g_giraffeModel[nCntModel].vtxMinMaterials.y > vtx.y)
			{
				g_giraffeModel[nCntModel].vtxMinMaterials.y = vtx.y;
			}

			// 最大値
			if (g_giraffeModel[nCntModel].vtxMaxMaterials.y < vtx.y)
			{
				g_giraffeModel[nCntModel].vtxMaxMaterials.y = vtx.y;
			}

			// y //
			// 最小値
			if (g_gireffe_vtxMin.y > g_giraffeModel[nCntModel].vtxMinMaterials.y)
			{
				g_gireffe_vtxMin.y = g_giraffeModel[nCntModel].vtxMinMaterials.y;
			}

			// 最大値
			if (g_gireffe_vtxMax.y < g_giraffeModel[nCntModel].vtxMaxMaterials.y)
			{
				g_gireffe_vtxMax.y = g_giraffeModel[nCntModel].vtxMaxMaterials.y;
			}

			// z //
			// 最小値
			if (g_giraffeModel[nCntModel].vtxMinMaterials.z > vtx.z)
			{
				g_giraffeModel[nCntModel].vtxMinMaterials.z = vtx.z;
			}

			// 最大値
			if (g_giraffeModel[nCntModel].vtxMaxMaterials.z < vtx.z)
			{
				g_giraffeModel[nCntModel].vtxMaxMaterials.z = vtx.z;
			}

			// z //
			// 最小値
			if (g_gireffe_vtxMin.z > g_giraffeModel[nCntModel].vtxMinMaterials.z)
			{
				g_gireffe_vtxMin.z = g_giraffeModel[nCntModel].vtxMinMaterials.z;
			}

			// 最大値
			if (g_gireffe_vtxMax.z < g_giraffeModel[nCntModel].vtxMaxMaterials.z)
			{
				g_gireffe_vtxMax.z = g_giraffeModel[nCntModel].vtxMaxMaterials.z;
			}

			// サイズポインタを進める
			pVertexBuffer += sizeFVF;
		}

		// アンロック
		g_giraffeModel[nCntModel].paMesh->UnlockVertexBuffer();

		// 最大値と最小値の距離
		g_giraffeModel[nCntModel].size = g_giraffeModel[nCntModel].vtxMaxMaterials - g_giraffeModel[nCntModel].vtxMinMaterials;
		g_gireffe_size = g_gireffe_vtxMax - g_gireffe_vtxMin;
	}

	// パーツ読込
	LoadGiraffeModel();

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, paTextureFile[0], &g_paTextureGiraffe[0]);
}

// ------------------------------------------------------------------------------------------
// キリン情報処理
// ------------------------------------------------------------------------------------------
void Init_GiraffeInfo(void)
{
	// 変数宣言
	int		nCntGiraffe;	// プレイヤーカウント
	int		nCntModel;		// モデルのカウント

	// キリンの初期設定 //
	for (nCntGiraffe = 0; nCntGiraffe < ALL_MAX; nCntGiraffe++)
	{
		g_giraffe[nCntGiraffe].pos				= D3DXVECTOR3(0.0f, 20.0f, 0.0f);
		g_giraffe[nCntGiraffe].posold			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].posLast			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].posKeyBetween	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].posOrigin		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].move				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].rot				= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].rotLast			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].rotbetween		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].rotKeyBetween	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_giraffe[nCntGiraffe].vtxMinMaterials	= g_gireffe_vtxMin;
		g_giraffe[nCntGiraffe].vtxMaxMaterials	= g_gireffe_vtxMax;
		g_giraffe[nCntGiraffe].size				= g_gireffe_size;
		g_giraffe[nCntGiraffe].nMotionType		= 0;
		g_giraffe[nCntGiraffe].nMotionTypeOld	= 0;
		g_giraffe[nCntGiraffe].nParent			= 0;
		g_giraffe[nCntGiraffe].nIndex			= nCntGiraffe;
		g_giraffe[nCntGiraffe].nFram			= 0;
		g_giraffe[nCntGiraffe].nCntKeySet		= 0;
		g_giraffe[nCntGiraffe].nIdxShadow		= nCntGiraffe;
		g_giraffe[nCntGiraffe].fLength			= 0.0f;
		g_giraffe[nCntGiraffe].bUse				= false;
		g_giraffe[nCntGiraffe].bDisp			= false;

		// パーツごとのカウント
		for (nCntModel = 0; nCntModel < GIRAFFETYPE_MAX; nCntModel++)
		{
			g_giraffe[nCntGiraffe].model[nCntModel].pos				= g_giraffeModel[nCntModel].pos;
			g_giraffe[nCntGiraffe].model[nCntModel].posLast			= g_giraffeModel[nCntModel].posLast;
			g_giraffe[nCntGiraffe].model[nCntModel].posKeyBetween	= g_giraffeModel[nCntModel].posKeyBetween;
			g_giraffe[nCntGiraffe].model[nCntModel].rot				= g_giraffeModel[nCntModel].rot;
			g_giraffe[nCntGiraffe].model[nCntModel].rotLast			= g_giraffeModel[nCntModel].rotLast;
			g_giraffe[nCntGiraffe].model[nCntModel].rotKeyBetween	= g_giraffeModel[nCntModel].rotKeyBetween;
			g_giraffe[nCntGiraffe].model[nCntModel].size			= g_giraffeModel[nCntModel].size;
			g_giraffe[nCntGiraffe].model[nCntModel].vtxMinMaterials = g_giraffeModel[nCntModel].vtxMinMaterials;
			g_giraffe[nCntGiraffe].model[nCntModel].vtxMaxMaterials = g_giraffeModel[nCntModel].vtxMaxMaterials;
			g_giraffe[nCntGiraffe].model[nCntModel].nIdxModelParent = g_giraffeModel[nCntModel].nIdxModelParent;
			g_giraffe[nCntGiraffe].model[nCntModel].nFram			= g_giraffeModel[nCntModel].nFram;
			g_giraffe[nCntGiraffe].model[nCntModel].nCntKeySet		= g_giraffeModel[nCntModel].nCntKeySet;
		}
	}
	// ロードキリン
	LoadGiraffe();
}

// ------------------------------------------------------------------------------------------
// キリンモーション処理
// ------------------------------------------------------------------------------------------
void Init_MotionGiraffe(void)
{
	// 変数宣言
	// モーション用変数
	int		nCntMotion;							// モーションカウント
	int		nCntKeySet;							// モーションカウント
	int		nCntKey;							// キーカウント

												// モーションカウント //
	for (nCntMotion = 0; nCntMotion < GIRAFFEMOSIONSET_MAX; nCntMotion++)
	{
		// モーションの初期設定
		g_giraffeKey[nCntMotion].nNumKey	= 0;		// ナンバーキー
		g_giraffeKey[nCntMotion].nLoop		= 0;		// ループの有無
		for (nCntKeySet = 0; nCntKeySet < 5; nCntKeySet++)
		{
			g_giraffeKey[nCntMotion].KeyInfo[nCntKeySet].nFram = 0;

			// キーごとに初期化
			for (nCntKey = 0; nCntKey < GIRAFFETYPE_MAX; nCntKey++)
			{
				g_giraffeKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_giraffeKey[nCntMotion].KeyInfo[nCntKeySet].key[nCntKey].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}

	// プレイヤーモーション読み込み
	LoadGiraffeMosion();
}

// ------------------------------------------------------------------------------------------
// キリンモデルの読み込み処理
// ------------------------------------------------------------------------------------------
void LoadGiraffeModel(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntModel = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	pFile = fopen("data/LOAD/GIRAFFE/giraffe_model.txt", "r");

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
				else if (strcmp(cHeadText, "GIRAFFESET") == 0)
				{

					// エンドモデルセットが来るまでループ
					while (strcmp(cHeadText, "END_GIRAFFESET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 親情報読み込み
						if (strcmp(cHeadText, "PARENT") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_giraffeModel[nCntModel].nIdxModelParent);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_giraffeModel[nCntModel].pos.x,
								&g_giraffeModel[nCntModel].pos.y, 
								&g_giraffeModel[nCntModel].pos.z);

							// 位置情報読み取り
							g_giraffeModel[nCntModel].posOrigin = g_giraffeModel[nCntModel].pos;
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_giraffeModel[nCntModel].rot.x, 
								&g_giraffeModel[nCntModel].rot.y,
								&g_giraffeModel[nCntModel].rot.z);
						}
					}
					// モデルの更新
					nCntModel++;
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
// キリン情報の読み込み処理
// ------------------------------------------------------------------------------------------
void LoadGiraffe(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntGiraffe = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// もし表示しているものがあったら
	for (nCntGiraffe = 0; nCntGiraffe < ALL_MAX; nCntGiraffe++)
	{
		if (g_giraffe[nCntGiraffe].bUse == true)
		{
			g_giraffe[nCntGiraffe].bUse = false;
			g_giraffe[nCntGiraffe].bDisp = false;
		}
	}

	// カウント初期化
	nCntGiraffe = 0;

	// ファイル開
	pFile = fopen(g_cGiraffeFIleName_3D[GetGameStage()], "r");

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
				else if (strcmp(cHeadText, "GIRAFFESET") == 0)
				{

					// エンドモデルセットが来るまでループ
					while (strcmp(cHeadText, "END_GIRAFFESET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 位置情報読み込み
						if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&g_giraffe[nCntGiraffe].pos.x,
								&g_giraffe[nCntGiraffe].pos.y,
								&g_giraffe[nCntGiraffe].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&g_giraffe[nCntGiraffe].rot.x,
								&g_giraffe[nCntGiraffe].rot.y,
								&g_giraffe[nCntGiraffe].rot.z);

							// 使用状態
							g_giraffe[nCntGiraffe].bUse = true;

							// 表示状態
							g_giraffe[nCntGiraffe].bDisp = true;

							// 影の作成
							g_giraffe[nCntGiraffe].nIdxShadow = SetShadow(
								g_giraffe[nCntGiraffe].pos,
								g_giraffe[nCntGiraffe].rot,
								D3DXVECTOR3(20.0f, 0.0f, 20.0f));
						}
					}
					// モデルの更新
					nCntGiraffe++;
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
// キリンモーションの読み込み処理
// ------------------------------------------------------------------------------------------
void LoadGiraffeMosion(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntMotion = 0;							// プレイヤーカウント
	int	nCntModel = 0;							// モデルカウント
	int	nCntKeySet = 0;							// モーションカウント
	int	nCntKey = 0;							// キーカウント
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	pFile = fopen(g_cGiraffeFIleName_3D[GetGameStage()], "r");

	// 開けた
	if (pFile != NULL)
	{
		// スクリプトが来るまでループ
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);	// 一文を読み込む
			sscanf(cRaedText, "%s", &cHeadText);		// 比較用テキストに文字を代入
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

				// モーションセットが来たら
				else if (strcmp(cHeadText, "MOTIONSET") == 0)
				{
					// モデルの初期化
					nCntModel = 0;

					// エンドモデルセットが来るまでループ
					while (strcmp(cHeadText, "END_MOTIONSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// ループするかどうか情報読み込み
						if (strcmp(cHeadText, "LOOP") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_giraffeKey[nCntMotion].nLoop);
						}

						// キー数情報読み込み
						else if (strcmp(cHeadText, "NUM_KEY") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &g_giraffeKey[nCntMotion].nNumKey);
						}

						// キー情報読み込み
						else if (strcmp(cHeadText, "KEYSET") == 0)
						{
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// フレーム情報読み込み
							if (strcmp(cHeadText, "FRAME") == 0)
							{
								// フレーム数
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie, &g_giraffeKey[nCntMotion].KeyInfo[nCntModel].nFram);
							}

							// キーの初期化
							nCntKey = 0;

							// エンドキーセットが来るまでループ
							while (strcmp(cHeadText, "END_KEYSET") != 0)
							{
								fgets(cRaedText, sizeof(cRaedText), pFile);
								sscanf(cRaedText, "%s", &cHeadText);

								// キー情報読み込み
								if (strcmp(cHeadText, "KEY") == 0)
								{

									// エンドキーが来るまでループ
									while (strcmp(cHeadText, "END_KEY") != 0)
									{
										fgets(cRaedText, sizeof(cRaedText), pFile);
										sscanf(cRaedText, "%s", &cHeadText);

										// 位置情報読み込み
										if (strcmp(cHeadText, "POS") == 0)
										{
											// フレーム数
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.x,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.y,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos.z);

											// 
											g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].pos += g_giraffe[0].model[nCntKey].pos;
										}

										// 回転情報読み込み
										else if (strcmp(cHeadText, "ROT") == 0)
										{
											// フレーム数
											sscanf(cRaedText, "%s %s %f %f %f",
												&cDie, &cDie,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.x,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.y,
												&g_giraffeKey[nCntMotion].KeyInfo[nCntModel].key[nCntKey].rot.z);
										}
									}

									// カウントキー
									nCntKey++;
								}
							}

							// カウントモデル
							nCntModel++;
						}
					}

					// カウントプレイヤー
					nCntMotion++;
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
// セーブ情報処理
// ------------------------------------------------------------------------------------------
void SaveGiraffe(void)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int		nCntGiraffe = 0;
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

	// キリンデータの種類
	char cGiraffeData[3][128] =
	{
		{ "POS" },				// 位置
		{ "ROT" },				// 回転
		{ "END_GIRAFFESET" }	// エンドフロアー
	};

	// セーブ用 //
	// ファイル開
	pFile = fopen(g_cGiraffeFIleName_3D[GetGameStage()], "w");

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
				if (nCntGiraffe < ALL_MAX - 1)
				{
					if (g_giraffe[nCntGiraffe].bUse == true)
					{
						fputs(cEnter, pFile);

						// エンドスクリプトではない
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "	GIRAFFESET\n");
							fputs(cWriteText, pFile);

							// エンドモデルセットが来るまでループ
							while (strcmp(cWriteText, "END_GIRAFFESET") != 0)
							{
								// 文字列をコピーする
								strcpy(cWriteText, cGiraffeData[nCntDataLoad]);

								// 位置情報保存
								if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n",
										&cWriteText, 
										&cEqual,
										g_giraffe[nCntGiraffe].pos.x,
										g_giraffe[nCntGiraffe].pos.y,
										g_giraffe[nCntGiraffe].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 回転情報保存
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_giraffe[nCntGiraffe].rot.x,
										g_giraffe[nCntGiraffe].rot.y,
										g_giraffe[nCntGiraffe].rot.z);

									fputs(cHeadText, pFile);
									// カウントデータアップ
									nCntDataLoad++;
								}

								// エンドが入ったら
								else if (strcmp(cWriteText, "END_GIRAFFESET") == 0)
								{
									sprintf(cHeadText, "    %s", &cWriteText);

									fputs(cHeadText, pFile);

									// カウントデータアップ
									nCntDataLoad = 0;
								}
							}
						}
					}
					// 素材カウント変更
					nCntGiraffe++;
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
		MessageBox(NULL, "セーブ用キリンデータの書き込みに失敗しました", "警告！", MB_ICONWARNING);

		// ファイル閉
		fclose(pFile);
	}

	// カウントの初期化
	nCntGiraffe = 0;
	nCntDataLoad = 0;
	nCntBug = 0;

	// ロード用 //
	// ファイル開
	pFile = fopen("data/LOAD/GIRAFFE/giraffe.txt", "w");

	// 開けた
	if (pFile != NULL)
	{
		//------------------ コメント ------------------//
		strcpy(cWriteText, cComment);

		strcat(cWriteText, cSharpEnter);

		strcat(cWriteText, "# ロード素材\n");

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
				if (nCntGiraffe < ALL_MAX - 1)
				{
					if (g_giraffe[nCntGiraffe].bUse == true)
					{
						fputs(cEnter, pFile);

						// エンドスクリプトではない
						if (strcmp(cWriteText, "END_SCRIPT") != 0)
						{
							strcpy(cWriteText, "    GIRAFFESET\n");
							fputs(cWriteText, pFile);

							// エンドモデルセットが来るまでループ
							while (strcmp(cWriteText, "END_GIRAFFESET") != 0)
							{
								// 文字列をコピーする
								strcpy(cWriteText, cGiraffeData[nCntDataLoad]);

								// 位置情報保存
								if (strcmp(cWriteText, "POS") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_giraffe[nCntGiraffe].pos.x,
										g_giraffe[nCntGiraffe].pos.y,
										g_giraffe[nCntGiraffe].pos.z);

									fputs(cHeadText, pFile);
									nCntDataLoad++;
								}

								// 回転情報保存
								else if (strcmp(cWriteText, "ROT") == 0)
								{
									sprintf(cHeadText, "        %s %s %0.3f %0.3f %0.3f\n", &cWriteText, &cEqual,
										g_giraffe[nCntGiraffe].rot.x,
										g_giraffe[nCntGiraffe].rot.y,
										g_giraffe[nCntGiraffe].rot.z);

									fputs(cHeadText, pFile);

									// カウントデータ初期化
									nCntDataLoad++;
								}

								// 回転情報保存
								else if (strcmp(cWriteText, "END_GIRAFFESET") == 0)
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
					nCntGiraffe++;
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
		MessageBox(NULL, "ロード用素材データの書き込みに失敗しました", "警告！", MB_ICONWARNING);

		// ファイル閉
		fclose(pFile);
	}
}

// ------------------------------------------------------------------------------------------
// モデル情報処理
// ------------------------------------------------------------------------------------------
GIRAFFE *GetGiraffe(void)
{
	return &g_giraffe[0];
}