//=============================================================================
//
// モデル処理 [player.cpp]
// Author : KOKI NISHIYAMA
//
//=============================================================================
#include "model.h"
#include "light.h"
#include "shadow_mapping.h"

//=============================================================================
// マクロ定義
//=============================================================================

// ----------------------------------------------------------------------------------------------------
// コンストラクタ処理
// ----------------------------------------------------------------------------------------------------
CModel::CModel()
{
	m_Xmodel.pMesh = NULL;
	m_Xmodel.pBuffMat = NULL;
	m_Xmodel.nNumMat = 0;
	m_Xmodel.vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.rotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.nParent = 0;
	m_Parent = NULL;
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nFrame = 0;
}

// ----------------------------------------------------------------------------------------------------
// デストラクタ処理
// ----------------------------------------------------------------------------------------------------
CModel::~CModel()
{
}

// ----------------------------------------------------------------------------------------------------
// 初期化処理
// ----------------------------------------------------------------------------------------------------
void CModel::Init(void)
{
	m_Xmodel.pMesh = NULL;
	m_Xmodel.pBuffMat = NULL;
	m_Xmodel.nNumMat = 0;
	m_Xmodel.vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.nParent = 0;
	m_Parent = NULL;
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nFrame = 0;
}

// ----------------------------------------------------------------------------------------------------
// 終了処理
// ----------------------------------------------------------------------------------------------------
void CModel::Uninit(void)
{
}

// ----------------------------------------------------------------------------------------------------
// 更新処理
// ----------------------------------------------------------------------------------------------------
void CModel::Update(void)
{
	// モデル事のモーション動作
	m_Xmodel.pos += m_Xmodel.posDiff / (float)m_nFrame;
	m_Xmodel.rot += m_Xmodel.rotDiff / (float)m_nFrame;
}

// ----------------------------------------------------------------------------------------------------
// 描画処理
// ----------------------------------------------------------------------------------------------------
void CModel::Draw(
	D3DXMATRIX & mtx,
	float & fAlpha
)
{
	// 変数宣言
	LPDIRECT3DDEVICE9	pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX			mtxRot, mtxTrans;		// 計算用マトリックス
	D3DXMATERIAL		*pMat;					// 現在のマテリアル保存
	D3DMATERIAL9		matDef;					// マテリアルデータのポインタ
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	// 回転を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_Xmodel.rot.y,
		m_Xmodel.rot.x,
		m_Xmodel.rot.z);

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:回転行列)
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld, &mtxRot);


	// 位置を反映 //
	// 平行移動行列作成(オフセット)
	D3DXMatrixTranslation(&mtxTrans,	// 総合の入れ物
		m_Xmodel.pos.x,
		m_Xmodel.pos.y,
		m_Xmodel.pos.z);

	// 行列の積(1:ワールド行列 = 2:ワールド行列 * 3:移動行列)
	D3DXMatrixMultiply(&m_mtxWorld,	// 1
		&m_mtxWorld,				// 2
		&mtxTrans);					// 3

	// 親情報を持っているとき
	if (m_Parent != NULL)
	{
		// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			&m_Parent->m_mtxWorld);
	}
	// すべての親の場合
	else
	{
		// 親と子の行列の積(1:ワールド行列 = 2:ワールド行列 * 3:体[親]行列)
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld,
			&mtx);
	}

	// シャドウマッピング
	CShadowmapping::Draw(
	pDevice,	// デバイス情報
	m_Xmodel,	// モデル情報
	m_mtxWorld	// マトリックス情報
	);
	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	// 現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_Xmodel.pBuffMat->GetBufferPointer();

	// カウントマテリアル
	for (int nCntMat = 0; nCntMat < (int)m_Xmodel.nNumMat; nCntMat++, pMat++)
	{
		pMat->MatD3D.Diffuse.a = fAlpha;
		// マテリアルの設定
		pDevice->SetMaterial(&pMat->MatD3D);
		// テクスチャー設定
		pDevice->SetTexture(0, m_Xmodel.ppTexture[nCntMat]);
		// 描画
		m_Xmodel.pMesh->DrawSubset(nCntMat);
	}
	// マテリアルをデフォルトに戻す
	pDevice->SetMaterial(&matDef);
}

// ----------------------------------------------------------------------------------------------------
// 生成処理
// ----------------------------------------------------------------------------------------------------
CModel * CModel::Create()
{
	// 変数宣言
	CModel * pModel;	// モデル
	// メモリ確保
	pModel = new CModel();
	// 初期化
	pModel->Init();
	// 情報を返す
	return pModel;
}

// ----------------------------------------------------------------------------------------------------
// 親パーツの設定処理
// ----------------------------------------------------------------------------------------------------
void CModel::SetParent(CModel * pModel)
{
	m_Parent = pModel;
}

// ----------------------------------------------------------------------------------------------------
// モデルの情報渡し処理
// ----------------------------------------------------------------------------------------------------
void CModel::BindModel(
	XMODEL const &xModel,		// モデルの情報
	MODEL_OFFSET const &offset	// モデルのオフセット
)
{
	// モデル情報の取得
	m_Xmodel = xModel;
	// モデルのオフセット
	m_Xmodel.pos = offset.pos;
	m_Xmodel.rot = offset.rot;
	m_Xmodel.nParent = offset.nParent;
}

// ----------------------------------------------------------------------------------------------------
// モデルの位置設定処理
// ----------------------------------------------------------------------------------------------------
void CModel::SetPos(D3DXVECTOR3 &pos)
{
	m_Xmodel.pos = pos;
}

// ----------------------------------------------------------------------------------------------------
// モーションの設定処理
// ----------------------------------------------------------------------------------------------------
void CModel::SetMotion(
	KEY &key, 
	int nMaxFrame
)
{
	m_Xmodel.posDest = key.pos;
	m_Xmodel.rotDest = key.rot;
	m_Xmodel.posDiff = (m_Xmodel.posDest - m_Xmodel.pos);
	m_Xmodel.rotDiff = (m_Xmodel.rotDest - m_Xmodel.rot);
	m_nFrame = nMaxFrame;
}

// ----------------------------------------------------------------------------------------------------
// モデル情報取得処理
// ----------------------------------------------------------------------------------------------------
XMODEL & CModel::GetXModel(void)
{
	return m_Xmodel;
}

// ----------------------------------------------------------------------------------------------------
// 行列情報取得処理
// ----------------------------------------------------------------------------------------------------
D3DXMATRIX & CModel::GetMtx(void)
{
	return m_mtxWorld;
}

// ----------------------------------------------------------------------------------------------------
// 位置情報取得処理
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3 & CModel::GetPos(void)
{
	return m_Xmodel.pos;
}

// ----------------------------------------------------------------------------------------------------
// 回転情報取得処理
// ----------------------------------------------------------------------------------------------------
D3DXVECTOR3 & CModel::GetRot(void)
{
	return m_Xmodel.rot;
}
