//-----------------------------------------------------------------------------
//
// メッシュスフィア処理 [meshsphere.cpp]
// Author : Nishiyama Koki
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "meshsphere.h"
#include "game.h"

//-----------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-----------------------------------------------------------------------------
CMeshsphere::MESHSPHERE CMeshsphere::m_aMeshSphere[TEXTYPE_MAX] = {};
LPDIRECT3DTEXTURE9 CMeshsphere::m_pTex[TEXTYPE_MAX] = {};

//-----------------------------------------------------------------------------
// コンストラクタ
//-----------------------------------------------------------------------------
CMeshsphere::CMeshsphere() :CScene(ACTOR_MESHSPHERE,LAYER_3DOBJECT)
{
}

//-----------------------------------------------------------------------------
// デストラクタ
//-----------------------------------------------------------------------------
CMeshsphere::~CMeshsphere()
{
}

//-----------------------------------------------------------------------------
// 生成
//-----------------------------------------------------------------------------
CMeshsphere * CMeshsphere::Create(void)
{
	CMeshsphere *pMeshsphere = new CMeshsphere;
	pMeshsphere->Init();
	return pMeshsphere;
}

//-----------------------------------------------------------------------------
// 初期化処理
//-----------------------------------------------------------------------------
void CMeshsphere::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得

	for (int nCntSphere = 0; nCntSphere < TEXTYPE_MAX; nCntSphere++)
	{
		// 位置・回転の初期設定
		m_aMeshSphere[nCntSphere].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aMeshSphere[nCntSphere].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_aMeshSphere[nCntSphere].col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 0.5f);		//色
		m_aMeshSphere[nCntSphere].fRadius = 0.0f;
		m_aMeshSphere[nCntSphere].nHeightBlock = SEPARATE;
		m_aMeshSphere[nCntSphere].nSeparate = SEPARATE;
		m_aMeshSphere[nCntSphere].nMaxFrame = 0;
		m_aMeshSphere[nCntSphere].nCntFrame = 0;
		m_aMeshSphere[nCntSphere].bUse = false;
		m_aMeshSphere[nCntSphere].bDisp = false;

		m_aMeshSphere[nCntSphere].nMaxVtx = (m_aMeshSphere[nCntSphere].nSeparate + 1) * (m_aMeshSphere[nCntSphere].nHeightBlock + 1);
		m_aMeshSphere[nCntSphere].nMaxIndex = (m_aMeshSphere[nCntSphere].nSeparate * 2 + 2) * m_aMeshSphere[nCntSphere].nHeightBlock + ((m_aMeshSphere[nCntSphere].nHeightBlock - 1) * 4);
		m_aMeshSphere[nCntSphere].nMaxPolygon = m_aMeshSphere[nCntSphere].nSeparate * 2 * m_aMeshSphere[nCntSphere].nHeightBlock + ((m_aMeshSphere[nCntSphere].nHeightBlock - 1) * 6);

		m_aMeshSphere[nCntSphere].fRot = (D3DX_PI * 2) / m_aMeshSphere[nCntSphere].nSeparate;
		m_aMeshSphere[nCntSphere].fRot2 = (D3DX_PI * 2) / m_aMeshSphere[nCntSphere].nHeightBlock;

		// 頂点情報の作成
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_aMeshSphere[nCntSphere].nMaxVtx,	//確保するバッファサイズ
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,			//頂点フォーマット
			D3DPOOL_MANAGED,
			&m_aMeshSphere[nCntSphere].pVtxBuffMeshSphere,
			NULL);

		// インデックス情報の作成
		pDevice->CreateIndexBuffer(sizeof(WORD) * m_aMeshSphere[nCntSphere].nMaxIndex,	//確保するバッファサイズ
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,				//頂点フォーマット
			D3DPOOL_MANAGED,
			&m_aMeshSphere[nCntSphere].pIdxBuffMeshSphere,
			NULL);

		VERTEX_3D *pVtx;

		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_aMeshSphere[nCntSphere].pVtxBuffMeshSphere->Lock(0, 0, (void**)&pVtx, 0);

		//頂点の情報
		//縦の個数分カウント
		for (int nCntV = 0; nCntV < m_aMeshSphere[nCntSphere].nHeightBlock + 1; nCntV++)
		{
			//円の頂点分
			for (int nCntH = 0; nCntH < m_aMeshSphere[nCntSphere].nSeparate + 1; nCntH++)
			{
				//座標
				pVtx[0].pos.x = 0.0f;
				pVtx[0].pos.y = 0.0f;
				pVtx[0].pos.z = 0.0f;

				pVtx[0].nor = pVtx[0].pos;							//法線は必ず正規化
				D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);		//正規化

				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//色
				pVtx[0].tex = D3DXVECTOR2((float)nCntV, (float)nCntH);	//(テクスチャのUV座標)

				pVtx++;
			}
		}

		//頂点データをアンロック
		m_aMeshSphere[nCntSphere].pVtxBuffMeshSphere->Unlock();

		WORD *pIdx;
		//インデックスバッファの範囲をロックし、インデックスデータへのポインタを取得
		m_aMeshSphere[nCntSphere].pIdxBuffMeshSphere->Lock(0, 0, (void**)&pIdx, 0);

		//インデックスの設定
		for (int nCntV = 0; nCntV < m_aMeshSphere[nCntSphere].nHeightBlock; nCntV++)
		{
			for (int nCntH = 0; nCntH < m_aMeshSphere[nCntSphere].nSeparate + 1; nCntH++)
			{
				pIdx[0] = (m_aMeshSphere[nCntSphere].nSeparate + 1) + nCntH + nCntV * (m_aMeshSphere[nCntSphere].nSeparate + 1);			//一列分下 + 何個進んだか + (縦*一列分)
				pIdx[1] = nCntH + nCntV * (m_aMeshSphere[nCntSphere].nSeparate + 1);											//何個進んだか + (縦*一列分)

				pIdx += 2;
				if ((nCntH + 1) % (m_aMeshSphere[nCntSphere].nSeparate + 1) == 0 && nCntV < m_aMeshSphere[nCntSphere].nHeightBlock - 1)
				{
					//縮退ポリゴン分の計算
					pIdx[0] = nCntH + nCntV * (m_aMeshSphere[nCntSphere].nSeparate + 1);
					pIdx[1] = nCntH + nCntV * (m_aMeshSphere[nCntSphere].nSeparate + 1) + 1;
					pIdx[2] = nCntH + nCntV * (m_aMeshSphere[nCntSphere].nSeparate + 1) + 1;
					pIdx[3] = (m_aMeshSphere[nCntSphere].nSeparate + 1) + nCntH + nCntV * (m_aMeshSphere[nCntSphere].nSeparate + 1) + 1;
					pIdx += 4;

				}
			}
		}

		//インデックスバッファのデータをアンロック
		m_aMeshSphere[nCntSphere].pIdxBuffMeshSphere->Unlock();
	}
}

//-----------------------------------------------------------------------------
// 終了処理
//-----------------------------------------------------------------------------
void CMeshsphere::Uninit(void)
{
	for (int nCntSphere = 0; nCntSphere < TEXTYPE_MAX; nCntSphere++)
	{
		// 頂点バッファの開放
		if (m_aMeshSphere[nCntSphere].pVtxBuffMeshSphere != NULL)
		{
			m_aMeshSphere[nCntSphere].pVtxBuffMeshSphere->Release();
			m_aMeshSphere[nCntSphere].pVtxBuffMeshSphere = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// 更新処理
//-----------------------------------------------------------------------------
void CMeshsphere::Update(void)
{
	for (int nCntSphere = 0; nCntSphere < TEXTYPE_MAX; nCntSphere++)
	{
		if (!m_aMeshSphere[nCntSphere].bUse)
		{
			continue;
		}
		switch (nCntSphere)
		{
		case TEXTYPE_WORLD:
			World(m_aMeshSphere[nCntSphere]);
			break;
		case TEXTYPE_STAR:
			if(!Star(m_aMeshSphere[nCntSphere])) continue;
			break;
		default:
			break;
		}
		// 変数宣言
		VERTEX_3D *pVtx;	// 頂点情報
		//頂点データの範囲をロックし、頂点バッファへのポインタを取得
		m_aMeshSphere[nCntSphere].pVtxBuffMeshSphere->Lock(0, 0, (void**)&pVtx, 0);

		//頂点の情報
		//縦の個数分カウント
		for (int nCntV = 0; nCntV < m_aMeshSphere[nCntSphere].nHeightBlock + 1; nCntV++)
		{
			//円の頂点分
			for (int nCntH = 0; nCntH < m_aMeshSphere[nCntSphere].nSeparate + 1; nCntH++)
			{
				pVtx[0].col = m_aMeshSphere[nCntSphere].col;	//色

				pVtx++;
			}
		}
		//頂点データをアンロック
		m_aMeshSphere[nCntSphere].pVtxBuffMeshSphere->Unlock();
	}
}

//-----------------------------------------------------------------------------
// 描画処理
//-----------------------------------------------------------------------------
void CMeshsphere::Draw(void)
{
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();	// デバイスの取得
	D3DXMATRIX			mtxRot, mtxTrans;								// 計算用

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// 表面(右回り)をカリングする
	pDevice->SetRenderState(D3DRS_LIGHTING, false);				// ライティングoff

	for (int nCntSphere = 0; nCntSphere < TEXTYPE_MAX; nCntSphere++)
	{
		if (m_aMeshSphere[nCntSphere].bUse)
		{
			if (m_aMeshSphere[nCntSphere].bDisp)
			{
				// テクスチャの設定
				pDevice->SetTexture(
					0,
					m_pTex[m_aMeshSphere[nCntSphere].Textype]);
				// ワールドマトリックスの初期化
				D3DXMatrixIdentity(&m_aMeshSphere[nCntSphere].mtxWorldMeshSphere);

				// 回転を反映
				D3DXMatrixRotationYawPitchRoll(&mtxRot, m_aMeshSphere[nCntSphere].rot.y, m_aMeshSphere[nCntSphere].rot.x, m_aMeshSphere[nCntSphere].rot.z);
				D3DXMatrixMultiply(&m_aMeshSphere[nCntSphere].mtxWorldMeshSphere, &m_aMeshSphere[nCntSphere].mtxWorldMeshSphere, &mtxRot);


				// 位置を反映
				D3DXMatrixTranslation(&mtxTrans, m_aMeshSphere[nCntSphere].pos.x, m_aMeshSphere[nCntSphere].pos.y, m_aMeshSphere[nCntSphere].pos.z);
				D3DXMatrixMultiply(&m_aMeshSphere[nCntSphere].mtxWorldMeshSphere, &m_aMeshSphere[nCntSphere].mtxWorldMeshSphere, &mtxTrans);

				// ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &m_aMeshSphere[nCntSphere].mtxWorldMeshSphere);

				//頂点バッファをデバイスのデータストリームにバインド
				pDevice->SetStreamSource(0, m_aMeshSphere[nCntSphere].pVtxBuffMeshSphere, 0, sizeof(VERTEX_3D));

				//インデックスバッファをデバイスのデータストリームにバインド
				pDevice->SetIndices(m_aMeshSphere[nCntSphere].pIdxBuffMeshSphere);

				//頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				// ポリゴンの描画	
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
					0,
					0,
					m_aMeshSphere[nCntSphere].nMaxIndex,		//頂点数
					0,
					m_aMeshSphere[nCntSphere].nMaxPolygon);	//ポリゴンの枚数
			}
		}
	}

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	// 描画
	pDevice->SetRenderState(D3DRS_LIGHTING, true);				// ライティングon

	// レンダーステート(Zテスト)
	pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);					// Zテスト有効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);			// Zテスト無効
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);				// 条件
}
#ifdef _DEBUG
//-----------------------------------------------------------------------------
// デバッグ表示
//-----------------------------------------------------------------------------
void CMeshsphere::Debug(void)
{
}
#endif // _DEBUG
//-----------------------------------------------------------------------------
// 読み込み
//-----------------------------------------------------------------------------
HRESULT CMeshsphere::Load(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice =
		CManager::GetRenderer()->GetDevice();
	// テクスチャー情報代入
	char TexData[TEXTYPE_MAX][72] =
	{
		"data/TEXTURE/cloud.jpg",
		"data/TEXTURE/stars.jpg",
	};

	// テクスチャの設定
	for (int nCntTex = 0; nCntTex < TEXTYPE_MAX; nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice, TexData[nCntTex], &m_pTex[nCntTex]);
	}

	return S_OK;
}

//-----------------------------------------------------------------------------
// 読み込んだものを破棄
//-----------------------------------------------------------------------------
void CMeshsphere::Unload(void)
{
	for (int nCntTex = 0; nCntTex < TEXTYPE_MAX; nCntTex++)
	{
		if (m_pTex[nCntTex] != NULL)
		{
			m_pTex[nCntTex]->Release();
			m_pTex[nCntTex] = NULL;
		}
	}
}

//-----------------------------------------------------------------------------
// 配置
//-----------------------------------------------------------------------------
int CMeshsphere::Set(
	const D3DXVECTOR3 pos,
	const float fRadius,
	const int nMaxFrame
)
{
	int nCntSphere;

	for (nCntSphere = 0; nCntSphere < TEXTYPE_MAX; nCntSphere++)
	{
		if (!m_aMeshSphere[nCntSphere].bUse)
		{
			// 位置・回転の初期設定
			m_aMeshSphere[nCntSphere].pos = pos;
			m_aMeshSphere[nCntSphere].fRadius = fRadius;
			m_aMeshSphere[nCntSphere].nMaxFrame = nMaxFrame;
			m_aMeshSphere[nCntSphere].Textype = (TEXTYPE)nCntSphere;
			m_aMeshSphere[nCntSphere].bUse = true;
			m_aMeshSphere[nCntSphere].bDisp = true;

			m_aMeshSphere[nCntSphere].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			VERTEX_3D *pVtx;

			//頂点データの範囲をロックし、頂点バッファへのポインタを取得
			m_aMeshSphere[nCntSphere].pVtxBuffMeshSphere->Lock(0, 0, (void**)&pVtx, 0);

			//頂点の情報
			//縦の個数分カウント
			for (int nCntV = 0; nCntV < m_aMeshSphere[nCntSphere].nHeightBlock + 1; nCntV++)
			{
				//円の頂点分
				for (int nCntH = 0; nCntH < m_aMeshSphere[nCntSphere].nSeparate + 1; nCntH++)
				{
					//座標
					pVtx[0].pos.x = sinf(m_aMeshSphere[nCntSphere].fRot * nCntH) * sinf(m_aMeshSphere[nCntSphere].fRot2 * nCntV * 0.5f) * m_aMeshSphere[nCntSphere].fRadius;		//カメラみたいな感じで	
					pVtx[0].pos.y = cosf(m_aMeshSphere[nCntSphere].fRot2 * nCntV * 0.5f) * m_aMeshSphere[nCntSphere].fRadius;							//高さ
					pVtx[0].pos.z = cosf(m_aMeshSphere[nCntSphere].fRot * nCntH) * sinf(m_aMeshSphere[nCntSphere].fRot2 * nCntV * 0.5f) * m_aMeshSphere[nCntSphere].fRadius;

					pVtx[0].nor = pVtx[0].pos;							//法線は必ず正規化
					D3DXVec3Normalize(&pVtx[0].nor, &pVtx[0].nor);		//正規化

					pVtx[0].tex = D3DXVECTOR2((float)nCntV, (float)nCntH);	//(テクスチャのUV座標)

					pVtx[0].col = m_aMeshSphere[nCntSphere].col;		//色

					pVtx++;
				}
			}
			//頂点データをアンロック
			m_aMeshSphere[nCntSphere].pVtxBuffMeshSphere->Unlock();
			break;
		}
	}
	return nCntSphere;

}

//-----------------------------------------------------------------------------
// 位置の設定
//-----------------------------------------------------------------------------
void CMeshsphere::SetPosition(int nIdxCollisionSphere, D3DXVECTOR3 pos)
{
	m_aMeshSphere[nIdxCollisionSphere].pos = pos;
}

//-----------------------------------------------------------------------------
// 消去処理
//-----------------------------------------------------------------------------
void CMeshsphere::DeleteCollision(int nIdxCollisionSphere)
{
	if (!m_aMeshSphere[nIdxCollisionSphere].bUse &&
		!m_aMeshSphere[nIdxCollisionSphere].bDisp) return;
	m_aMeshSphere[nIdxCollisionSphere].fRadius = 0.0f;
	m_aMeshSphere[nIdxCollisionSphere].bUse = false;
	m_aMeshSphere[nIdxCollisionSphere].bDisp = false;
}

//-----------------------------------------------------------------------------
// スフィアの半径の変更
//-----------------------------------------------------------------------------
void CMeshsphere::SetRadius(int nIdxCollisionSphere, float fRadius)
{
	m_aMeshSphere[nIdxCollisionSphere].fRadius = fRadius;
	VERTEX_3D *pVtx;

	//頂点データの範囲をロックし、頂点バッファへのポインタを取得
	m_aMeshSphere[nIdxCollisionSphere].pVtxBuffMeshSphere->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の情報
	//縦の個数分カウント
	for (int nCntV = 0; nCntV < m_aMeshSphere[nIdxCollisionSphere].nHeightBlock + 1; nCntV++)
	{
		//円の頂点分
		for (int nCntH = 0; nCntH < m_aMeshSphere[nIdxCollisionSphere].nSeparate + 1; nCntH++)
		{
			//座標
			pVtx[0].pos.x = sinf(m_aMeshSphere[nIdxCollisionSphere].fRot * nCntH) * sinf(m_aMeshSphere[nIdxCollisionSphere].fRot2 * nCntV * 0.5f) * m_aMeshSphere[nIdxCollisionSphere].fRadius;		//カメラみたいな感じで	
			pVtx[0].pos.y = cosf(m_aMeshSphere[nIdxCollisionSphere].fRot2 * nCntV * 0.5f) * m_aMeshSphere[nIdxCollisionSphere].fRadius;							//高さ
			pVtx[0].pos.z = cosf(m_aMeshSphere[nIdxCollisionSphere].fRot * nCntH) * sinf(m_aMeshSphere[nIdxCollisionSphere].fRot2 * nCntV * 0.5f) * m_aMeshSphere[nIdxCollisionSphere].fRadius;

			pVtx++;

		}
	}
	//インデックスバッファのデータをアンロック
	m_aMeshSphere[nIdxCollisionSphere].pIdxBuffMeshSphere->Unlock();

}

//-----------------------------------------------------------------------------
// スフィアの半径の変更
//-----------------------------------------------------------------------------
void CMeshsphere::Change(void)
{
}

//-----------------------------------------------------------------------------
// 世界
//-----------------------------------------------------------------------------
void CMeshsphere::World(MESHSPHERE & meshsphere)
{
}

//-----------------------------------------------------------------------------
// 星
//-----------------------------------------------------------------------------
bool CMeshsphere::Star(MESHSPHERE & meshsphere)
{
	meshsphere.nCntFrame++;
	if (meshsphere.nCntFrame >= meshsphere.nMaxFrame)
	{
		meshsphere.bUse = false;
		meshsphere.bDisp = false;
		meshsphere.nCntFrame = 0;
		return false;
	}

	return true;
}
