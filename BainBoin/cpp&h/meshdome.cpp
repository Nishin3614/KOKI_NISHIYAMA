// ----------------------------------------
//
// メッシュドーム処理の説明[meshdome.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// インクルードファイル
//
// ----------------------------------------
/* 描画 */
#include "meshdome.h"
#include "game.h"
#include "scene.h"
#include "debugproc.h"

// ----------------------------------------
//
// マクロ定義
//
// ----------------------------------------
#define MESHDOME_ROTMOVE (0.1f)				// 角度の移動量
#define MESHDOME_SIZE (100.0f)					// サイズ

// ----------------------------------------
//
// グローバル変数
//
// ----------------------------------------

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------
LPDIRECT3DTEXTURE9 CMeshdome::m_pTex[TYPE_MAX] = {};

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CMeshdome::CMeshdome() : CScene(ACTOR_MESHDOME, LAYER_3DOBJECT)
{
	/* 変数の初期化 */
	// 回転量
	m_pVtxBuff = NULL;
	m_pIndex = NULL;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nNumberVertex = 4;
	m_nNumIndex = 4;
	m_nNumPolygon = 2;
	m_nBlock_Width = 1;
	m_nBlock_Depth = 1;
	m_type = TYPE_NORMAL;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CMeshdome::~CMeshdome()
{
}

// ----------------------------------------
// 初期化処理
// ----------------------------------------
void CMeshdome::Init(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();
	D3DXVECTOR3 VecA, VecB;					// ベクトル
	D3DXVECTOR3 Cross[2000];				// 外積
	// ブロック描画の原点の初期設定
	m_OriginBlock = D3DXVECTOR3(
		m_size.x * -0.5f * m_nBlock_Width,
		0.0f,
		m_size.z * 0.5f * m_nBlock_Depth);
	// 総頂点数・インデックス・ポリゴンの初期設定(計算)
	m_nNumberVertex =
		(m_nBlock_Depth + 1) * (m_nBlock_Width + 1);
	m_nNumIndex =
		(m_nBlock_Width + 1) * 2 * m_nBlock_Depth +
		2 * (m_nBlock_Depth - 1);
	m_nNumPolygon =
		m_nBlock_Depth * m_nBlock_Width * 2 +
		4 * (m_nBlock_Depth - 1);

	// 頂点座標の生成
	MakeVertex(pDevice);
}

// ----------------------------------------
// 終了処理
// ----------------------------------------
void CMeshdome::Uninit(void)
{
	// 頂点バッファの開放
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

// ----------------------------------------
// 更新処理
// ----------------------------------------
void CMeshdome::Update(void)
{
	// 変数宣言
	VERTEX_3D *pVtx;	// 頂点情報へのポイント
	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);
	// 変数宣言
	int nCountDirect;
	int nCountWidth;

			//頂点設定 //
			//行ループ
	for (nCountDirect = 0; nCountDirect < m_nBlock_Depth + 1; nCountDirect++)
	{
		// 列ループ
		for (nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{
			// テクスチャーの設定
			pVtx[0].tex = pVtx[0].tex +
				D3DXVECTOR2(
					1.0f / m_nBlock_Width * 0.1f,
					0.0f);
			// 色設定
			pVtx[0].col = m_col;

			// ポイント合わせ
			pVtx++;
		}
	}
	// アンロック
	m_pVtxBuff->Unlock();
}

// ----------------------------------------
// 描画処理
// ----------------------------------------
void CMeshdome::Draw(void)
{
	// 変数宣言
	LPDIRECT3DDEVICE9 pDevice =		// デバイスの取得
		CManager::GetRenderer()->GetDevice();

	// 回転・位置の計算用変数
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化(単位行列にするため)
	D3DXMatrixIdentity(&m_mtxWorld);

	// 回転を反映 //
	// ヨーピッチロールによる回転行列作成(y,x,z)
	D3DXMatrixRotationYawPitchRoll(&mtxRot,	// 総合の入れ物
		m_rot.y, m_rot.x, m_rot.z);			// それぞれの回転量
	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxRot);					// 3
	// 平行移動行列作成(オフセット)
	D3DXMatrixTranslation(&mtxTrans,// 総合の入れ物
		m_pos.x, m_pos.y, m_pos.z);	// それぞれの移動量
	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームを設定
	pDevice->SetIndices(
		m_pIndex);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(
		0,
		m_pTex[m_type]);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		m_nNumberVertex,
		0,
		m_nNumPolygon);
}

// ----------------------------------------
// 読み込み処理
// ----------------------------------------
HRESULT CMeshdome::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice =		
		CManager::GetRenderer()->GetDevice();
	// テクスチャー名
	char cTex[TYPE_MAX][128] =
	{
		"png",
		"data/TEXTURE/Line.png",
	};

	// テクスチャー設定
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, cTex[nCntTex], &m_pTex[nCntTex]);
	}

	return S_OK;
}

// ----------------------------------------
// 読み込んだ情報を破棄
// ----------------------------------------
void CMeshdome::UnLoad(void)
{
	// テクスチャの開放
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_pTex[nCntTex] != NULL)
		{
			m_pTex[nCntTex]->Release();
			m_pTex[nCntTex] = NULL;
		}
	}
}

// ----------------------------------------
// 作成処理
// ----------------------------------------
CMeshdome * CMeshdome::Create(
	D3DXVECTOR3 pos,
	D3DXVECTOR3 size,
	D3DXVECTOR3 rot,
	D3DXCOLOR	col,
	int nWidth,
	int nDepth,
	TYPE type
)
{
	// 変数宣言
	CMeshdome * pMeshdome;		// シーン2Dクラス
	// メモリの生成(初め->基本クラス,後->派生クラス)
	pMeshdome = new CMeshdome();
	// 位置情報
	pMeshdome->m_pos = pos;
	// サイズ情報
	pMeshdome->m_size = size;
	// 回転情報
	pMeshdome->m_rot = rot;
	// 色情報
	pMeshdome->m_col = col;
	// 横ブロック
	pMeshdome->m_nBlock_Width = nWidth;
	// 縦ブロック
	pMeshdome->m_nBlock_Depth = nDepth;
	// タイプ
	pMeshdome->m_type = type;
	// 初期化処理
	pMeshdome->Init();
	// 生成したオブジェクトを返す
	return pMeshdome;
}

// ----------------------------------------
// 色情報設定
// ----------------------------------------
void CMeshdome::SetCol(D3DXCOLOR col)
{
	m_col = col;
}

// ----------------------------------------
// 頂点座標の生成
// ----------------------------------------
void CMeshdome::MakeVertex(LPDIRECT3DDEVICE9 pDevice)
{
	// 変数宣言
	VERTEX_3D *pVtx;			// 頂点情報へのポイント
	WORD * pIdx;				// インデックスデータへのポインタを取得
	D3DXVECTOR3 VecA, VecB;		// ベクトル
	int nCountDirect;			// 縦のカウント
	int nCountWidth;			// 横のカウント
	int nCntBlock = 0;
	float fAngle;				// yの角度
	float fRadian;				// yのラジアン値
	D3DXVECTOR3 Cross[2000];	// 外積

	// 角度の計算
	fAngle = D3DX_PI * 2 / m_nBlock_Width;

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nNumberVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) *
		m_nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIndex,
		NULL);

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pVtxBuff->Lock(
		0,
		0,
		(void **)&pVtx,
		0);

	//頂点設定 //
	//行ループ
	for (nCountDirect = 0; nCountDirect < m_nBlock_Depth + 1; nCountDirect++)
	{
		// 列ループ
		for (nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{
			// ラジアン値
			fRadian = fAngle * nCountWidth;
			fRadian = CCalculation::Rot_One_Limit(fRadian);
			// 頂点座標の設定
			pVtx[0].pos =
				D3DXVECTOR3(
					(sinf(fRadian) * m_size.x),
					m_size.y * nCountDirect,
					(cosf(fRadian) * m_size.z));
			// 法線ベクトルの設定
			//pVtx[0].nor = pVtx[0].pos;
			//D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);

			// カラーの設定
			pVtx[0].col = m_col;

			// テクスチャーの設定
			pVtx[0].tex = D3DXVECTOR2(
				1.0f / m_nBlock_Width * nCountWidth,
				1.0f / m_nBlock_Depth * (m_nBlock_Depth - nCountDirect)
			);

			// ポイント合わせ
			pVtx++;
		}
	}
	pVtx -= m_nNumberVertex;
	// ポリゴンごとの法線の設定
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth; nCntDepth++, nCntBlock++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width; nCntWidth++, nCntBlock++)
		{
			// ベクトル
			VecA = pVtx[nCntBlock + m_nBlock_Width + 2].pos - pVtx[nCntBlock].pos;
			VecB = pVtx[nCntBlock + m_nBlock_Width + 1].pos - pVtx[nCntBlock].pos;
			// 外積計算
			Cross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] = CCalculation::Cross_product(VecA, VecB);
			// 正規化
			D3DXVec3Normalize(&Cross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2], &Cross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2]);
			// ベクトル
			VecA = pVtx[nCntBlock + 1].pos - pVtx[nCntBlock].pos;
			VecB = pVtx[nCntBlock + m_nBlock_Width + 2].pos - pVtx[nCntBlock].pos;
			// 外積計算
			Cross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1] = CCalculation::Cross_product(VecA, VecB);
			// 正規化
			D3DXVec3Normalize(&Cross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1], &Cross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1]);
			// 左上
			pVtx[nCntBlock].nor += Cross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] + Cross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
			// 右上
			pVtx[nCntBlock + 1].nor += Cross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
			// 左下
			pVtx[nCntBlock + m_nBlock_Width + 1].nor += Cross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2];
			// 右下
			pVtx[nCntBlock + m_nBlock_Width + 2].nor += Cross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2] + Cross[nCntWidth * 2 + nCntDepth * m_nBlock_Width * 2 + 1];
		}
	}
	nCntBlock = 0;
	// 頂点法線の設定
	for (int nCntDepth = 0; nCntDepth < m_nBlock_Depth + 1; nCntDepth++, nCntBlock++)
	{
		for (int nCntWidth = 0; nCntWidth < m_nBlock_Width + 1; nCntWidth++, nCntBlock++)
		{
			// 最初
			if (nCntDepth == 0 && nCntWidth == 0)
			{
				pVtx[0].nor /= 2;
			}
			// 最後
			else if (nCntDepth == m_nBlock_Depth && nCntWidth == m_nBlock_Width)
			{
				pVtx[0].nor /= 2;
			}
			// 1行の列の最後
			else if (nCntDepth == 0 && nCntWidth == m_nBlock_Width)
			{
			}
			// 最後行の列の最初
			else if (nCntDepth == m_nBlock_Depth && nCntWidth == 0)
			{
			}
			// 最初の行または最後の行
			else if (nCntDepth == 0 || nCntDepth == m_nBlock_Depth)
			{
				pVtx[0].nor /= 3;
			}
			// 最初の列または最後の列
			else if (nCntWidth == 0 || nCntWidth == m_nBlock_Width)
			{
				pVtx[0].nor /= 3;
			}
			// それ以外
			else
			{
				pVtx[0].nor /= 6;
			}
			pVtx++;
		}
	}

	// アンロック
	m_pVtxBuff->Unlock();

	// 頂点データの範囲をロックし、頂点バッファへのポインタ
	m_pIndex->Lock(0, 0, (void **)&pIdx, 0);

	// 縦ブロック個数
	for (nCountDirect = 0; nCountDirect < m_nBlock_Depth; nCountDirect++)
	{
		// ２回目のループ以降
		if (nCountDirect >= 1)
		{
			// 縮退ポリゴン分の頂点追加
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + m_nBlock_Width + 1;

			// インデックスのポイント合わせ
			pIdx++;
		}

		// 横ブロックの頂点数
		for (nCountWidth = 0; nCountWidth < m_nBlock_Width + 1; nCountWidth++)
		{
			// 描画順番のインデックス
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + nCountWidth + m_nBlock_Width + 1;
			pIdx[1] = nCountDirect * (m_nBlock_Width + 1) + nCountWidth;

			// インデックスのポイント合わせ
			pIdx += 2;
		}

		// 縮退ポリゴンを作る必要がある場合
		if (nCountDirect < m_nBlock_Depth - 1)
		{
			// 縮退ポリゴン分の頂点追加
			pIdx[0] = nCountDirect * (m_nBlock_Width + 1) + m_nBlock_Width;
			// インデックスのポイント合わせ
			pIdx++;
		}
	}

	// アンロック
	m_pIndex->Unlock();
}
