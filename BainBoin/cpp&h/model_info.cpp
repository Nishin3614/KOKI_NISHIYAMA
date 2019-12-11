// ----------------------------------------
//
// モデル情報処理 [model_info.cpp]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------
#include "model_info.h"

// ----------------------------------------
//
// 静的変数宣言
//
// ----------------------------------------

// ----------------------------------------
// コンストラクタ処理
// ----------------------------------------
CModel_info::CModel_info()
{
	m_model_info.pMesh = NULL;
	m_model_info.pBuffMat = NULL;
	m_model_info.pTexture = NULL;
	m_model_info.nNumMat = 0;
	m_model_info.vtxMinMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_model_info.vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_model_info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_model_info.posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_model_info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_model_info.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_model_info.nParent = 0;
}

// ----------------------------------------
// デストラクタ処理
// ----------------------------------------
CModel_info::~CModel_info()
{
}

// ----------------------------------------
// 読み込み
// ----------------------------------------
void CModel_info::Load(
	LPDIRECT3DDEVICE9 pDevice,
	const char * charModel
)
{
	// Xファイルの読み込み
	D3DXLoadMeshFromX(
		charModel,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_model_info.pBuffMat,
		NULL,
		&m_model_info.nNumMat,
		&m_model_info.pMesh);

	D3DXCreateTextureFromFile(pDevice, "data/TEXTURE/UV_TEX2.png", &m_model_info.pTexture);

	// モデルの頂点座標の最大値・最小値の設定
	int nNumVertex;			// 頂点数
	DWORD sizeFVF;			// 頂点フォーマットのサイズ
	BYTE *pVertexBuffer;	// 頂点バッファへのポインタ
							// マテリアルの頂点位置の最大値・最小値
	m_model_info.vtxMinMaterials = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	m_model_info.vtxMaxMaterials = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	// 頂点数を取得
	nNumVertex = m_model_info.pMesh->GetNumVertices();
	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_model_info.pMesh->GetFVF());
	// 頂点バッファをロック
	m_model_info.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);
	// 頂点数分繰り返す
	for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
	{
		// 番地情報のD3DXVE
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

		// -----------すべての頂点情報を比較して最小値・最大値を抜き出す---------- //
		// x //
		// 最小値
		if (m_model_info.vtxMinMaterials.x > vtx.x)
		{
			m_model_info.vtxMinMaterials.x = vtx.x;
		}
		// 最大値
		if (m_model_info.vtxMaxMaterials.x < vtx.x)
		{
			m_model_info.vtxMaxMaterials.x = vtx.x;
		}
		// y //
		// 最小値
		if (m_model_info.vtxMinMaterials.y > vtx.y)
		{
			m_model_info.vtxMinMaterials.y = vtx.y;
		}
		// 最大値
		if (m_model_info.vtxMaxMaterials.y < vtx.y)
		{
			m_model_info.vtxMaxMaterials.y = vtx.y;
		}
		// z //
		// 最小値
		if (m_model_info.vtxMinMaterials.z > vtx.z)
		{
			m_model_info.vtxMinMaterials.z = vtx.z;
		}
		// 最大値
		if (m_model_info.vtxMaxMaterials.z < vtx.z)
		{
			m_model_info.vtxMaxMaterials.z = vtx.z;
		}
		// サイズポインタを進める
		pVertexBuffer += sizeFVF;
	}
	// アンロック
	m_model_info.pMesh->UnlockVertexBuffer();

	m_model_info.scal = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
}

// ----------------------------------------
// Unload
// ----------------------------------------
void CModel_info::Unload(void)
{
	// メッシュの開放
	if (m_model_info.pMesh != NULL)
	{
		m_model_info.pMesh->Release();
		m_model_info.pMesh = NULL;
	}

	// マテリアルの開放
	if (m_model_info.pBuffMat != NULL)
	{
		m_model_info.pBuffMat->Release();
		m_model_info.pBuffMat = NULL;
	}

	// テクスチャーの開放
	if (m_model_info.pTexture != NULL)
	{
		m_model_info.pTexture->Release();
		m_model_info.pTexture = NULL;
	}
}

// ----------------------------------------
// モデル情報取得
// ----------------------------------------
MODEL_INFO & CModel_info::GetModel_info(void)
{
	return m_model_info;
}

// ----------------------------------------
// メッシュ情報取得
// ----------------------------------------
LPD3DXMESH & CModel_info::GetMesh(void)
{
	return m_model_info.pMesh;
}

// ----------------------------------------
// マテリアル情報取得
// ----------------------------------------
LPD3DXBUFFER & CModel_info::GetBuffmat(void)
{
	return m_model_info.pBuffMat;
}

// ----------------------------------------
// テクスチャー情報取得
// ----------------------------------------
LPDIRECT3DTEXTURE9 & CModel_info::GetTex(void)
{
	return m_model_info.pTexture;
}

// ----------------------------------------
// マテリアル数情報取得
// ----------------------------------------
DWORD & CModel_info::GetNumMat(void)
{
	return m_model_info.nNumMat;
}

// ----------------------------------------
// 頂点座標の最小数情報取得
// ----------------------------------------
D3DXVECTOR3 & CModel_info::GetvtxMinMat(void)
{
	return m_model_info.vtxMinMaterials;
}

// ----------------------------------------
// 頂点座標の最大数情報取得
// ----------------------------------------
D3DXVECTOR3 & CModel_info::GetvtxMaxMat(void)
{
	return m_model_info.vtxMaxMaterials;
}

// ----------------------------------------
// 位置情報取得
// ----------------------------------------
D3DXVECTOR3 & CModel_info::GetPos(void)
{
	return m_model_info.pos;
}

// ----------------------------------------
// 回転情報取得
// ----------------------------------------
D3DXVECTOR3 & CModel_info::GetRot(void)
{
	return m_model_info.rot;
}

// ----------------------------------------
// 親情報取得
// ----------------------------------------
int & CModel_info::GetParent(void)
{
	return m_model_info.nParent;
}

// ----------------------------------------
// モデルのオフセットの設定
// ----------------------------------------
void  CModel_info::ModelOffset(
	MODEL_OFFSET * pModel_offset,
	const char * file_name
	)
{
	// やること
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int  nCntLoad = 0;
	int nCntPlayer = 0;
	int	nCntModel = 0;
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字

	// ファイル開
	pFile = fopen(file_name, "r");

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
				else if (strcmp(cHeadText, "PARTSSET") == 0)
				{

					// エンドモデルセットが来るまでループ
					while (strcmp(cHeadText, "END_PARTSSET") != 0)
					{
						fgets(cRaedText, sizeof(cRaedText), pFile);
						sscanf(cRaedText, "%s", &cHeadText);

						// 親情報読み込み
						if (strcmp(cHeadText, "PARENT") == 0)
						{
							sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
								&pModel_offset[nCntModel].nParent);
						}

						// 位置情報読み込み
						else if (strcmp(cHeadText, "POS") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f",
								&cDie, &cDie,
								&pModel_offset[nCntModel].pos.x,
								&pModel_offset[nCntModel].pos.y,
								&pModel_offset[nCntModel].pos.z);
						}

						// 回転情報読み込み
						else if (strcmp(cHeadText, "ROT") == 0)
						{
							sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
								&pModel_offset[nCntModel].rot.x,
								&pModel_offset[nCntModel].rot.y,
								&pModel_offset[nCntModel].rot.z);
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
		MessageBox(NULL, "プレイヤーデータの読み込みに失敗しました", "警告！", MB_ICONWARNING);
	}
}
