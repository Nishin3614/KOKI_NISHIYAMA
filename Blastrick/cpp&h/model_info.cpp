// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// モデル情報処理 [model_info.cpp]
// Author : KOKI NISHIYAMA
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "model_info.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 静的変数宣言
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// コンストラクタ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CModel_info::CModel_info()
{
	m_Xmodel.pMesh = NULL;
	m_Xmodel.pBuffMat = NULL;
	m_Xmodel.ppTexture = NULL;
	m_Xmodel.nNumMat = 0;
	m_Xmodel.vtxMinMaterials = D3DXVECTOR3(0.0f,0.0f,0.0f);
	m_Xmodel.vtxMaxMaterials = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Xmodel.nParent = 0;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// デストラクタ処理
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CModel_info::~CModel_info()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モデルの情報の読み込み
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CModel_info::Load(
	LPDIRECT3DDEVICE9 pDevice,
	const char * charModel
)
{
	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATERIAL *pMat;				// 現在のマテリアル保存
	int nNumVertex;					// 頂点数
	DWORD sizeFVF;					// 頂点フォーマットのサイズ
	BYTE *pVertexBuffer;			// 頂点バッファへのポインタ

	// Xファイルの読み込み
	if(!D3DXLoadMeshFromX(
		charModel,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_Xmodel.pBuffMat,
		NULL,
		&m_Xmodel.nNumMat,
		&m_Xmodel.pMesh) == S_OK
		) return E_FAIL;

	// マテリアルの頂点位置の最大値・最小値
	m_Xmodel.vtxMinMaterials = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	m_Xmodel.vtxMaxMaterials = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	// 頂点数を取得
	nNumVertex = m_Xmodel.pMesh->GetNumVertices();
	// 頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_Xmodel.pMesh->GetFVF());
	// マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_Xmodel.pBuffMat->GetBufferPointer();
	m_Xmodel.ppTexture = new LPDIRECT3DTEXTURE9[m_Xmodel.nNumMat];
	// マテリアル情報を取得
	for (int nCntMat = 0; nCntMat < (int)m_Xmodel.nNumMat; nCntMat++, pMat++)
	{
		m_Xmodel.ppTexture[nCntMat] = NULL;
		D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_Xmodel.ppTexture[nCntMat]);
	}

	// 頂点バッファをロック
	m_Xmodel.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);
	// 頂点数分繰り返す
	for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
	{
		// 番地情報のD3DXVE
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

		// -----------すべての頂点情報を比較して最小値・最大値を抜き出す---------- //
		// x //
		// 最小値
		if (m_Xmodel.vtxMinMaterials.x > vtx.x)
		{
			m_Xmodel.vtxMinMaterials.x = vtx.x;
		}
		// 最大値
		if (m_Xmodel.vtxMaxMaterials.x < vtx.x)
		{
			m_Xmodel.vtxMaxMaterials.x = vtx.x;
		}
		// y //
		// 最小値
		if (m_Xmodel.vtxMinMaterials.y > vtx.y)
		{
			m_Xmodel.vtxMinMaterials.y = vtx.y;
		}
		// 最大値
		if (m_Xmodel.vtxMaxMaterials.y < vtx.y)
		{
			m_Xmodel.vtxMaxMaterials.y = vtx.y;
		}
		// z //
		// 最小値
		if (m_Xmodel.vtxMinMaterials.z > vtx.z)
		{
			m_Xmodel.vtxMinMaterials.z = vtx.z;
		}
		// 最大値
		if (m_Xmodel.vtxMaxMaterials.z < vtx.z)
		{
			m_Xmodel.vtxMaxMaterials.z = vtx.z;
		}
		// サイズポインタを進める
		pVertexBuffer += sizeFVF;
	}
	// アンロック
	m_Xmodel.pMesh->UnlockVertexBuffer();
	// サイズ
	m_Xmodel.size = m_Xmodel.vtxMaxMaterials - m_Xmodel.vtxMinMaterials;
	return S_OK;
}


// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モデルの情報の読み込み
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CModel_info::Unload(void)
{
	// メッシュの開放
	if (m_Xmodel.pMesh != NULL)
	{
		m_Xmodel.pMesh->Release();
		m_Xmodel.pMesh = NULL;
	}

	// マテリアルの開放
	if (m_Xmodel.pBuffMat != NULL)
	{
		m_Xmodel.pBuffMat->Release();
		m_Xmodel.pBuffMat = NULL;
	}

	// テクスチャーの開放
	for (int nCntTex = 0; nCntTex < (int)m_Xmodel.nNumMat; nCntTex++)
	{
		if (m_Xmodel.ppTexture[nCntTex] != NULL)
		{
			m_Xmodel.ppTexture[nCntTex]->Release();
			m_Xmodel.ppTexture[nCntTex] = NULL;
		}
	}
	// テクスチャーの破棄
	if (m_Xmodel.ppTexture != NULL)
	{
		delete[] m_Xmodel.ppTexture;
		m_Xmodel.ppTexture = NULL;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// モデルのテキストデータの読み込み
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  CModel_info::TextLoad(
	MODEL_ALL *pmodelAll,				
	int const &nMaxMotion,
	int &nMaxkey,
	const char * file_name
)
{
	// ファイルポイント
	FILE *pFile;

	// 変数宣言
	int	nCntModel = 0;		// モデルカウント
	int nCntFile = 0;		// ファイルカウント
	int	nCntMotion = 0;		// モーションカウント
	int	nCntKeySet = 0;		// フレーム数
	int	nCntKey = 0;		// モデル数
	char cRaedText[128];	// 文字として読み取り用
	char cHeadText[128];	// 比較するよう
	char cDie[128];			// 不必要な文字
	int nCameraType = 0;	// カメラタイプ

	// モーションの生成
	pmodelAll->pMotion = new MOTION[nMaxMotion];

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

		// エンドスクリプトが来るまでループ
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			// 初期化
			cHeadText[0] = '\0';
			fgets(cRaedText, sizeof(cRaedText), pFile);
			sscanf(cRaedText, "%s", &cHeadText);
			// モデル数が来たら
			if (strcmp(cHeadText, "NUM_MODEL") == 0)
			{
				sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
					&nMaxkey);
				pmodelAll->pModel_offset = new MODEL_OFFSET[nMaxkey];
			}
			// モデルファイル名が来たら
			else if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
			{
				sscanf(cRaedText, "%s %s %s", &cDie, &cDie,
					&pmodelAll->pModel_offset[nCntFile].cXfile);
				nCntFile++;
			}
			// パーツセットが来たら
			else if (strcmp(cHeadText, "PARTSSET") == 0)
			{
				// エンドパーツセットが来るまでループ
				while (strcmp(cHeadText, "END_PARTSSET") != 0)
				{
					// 初期化
					cHeadText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);
					// 親情報読み込み
					if (strcmp(cHeadText, "PARENT") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].nParent);
					}
					// 位置情報読み込み
					else if (strcmp(cHeadText, "POS") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].pos.x,
							&pmodelAll->pModel_offset[nCntModel].pos.y,
							&pmodelAll->pModel_offset[nCntModel].pos.z);
					}
					// 回転情報読み込み
					else if (strcmp(cHeadText, "ROT") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].rot.x,
							&pmodelAll->pModel_offset[nCntModel].rot.y,
							&pmodelAll->pModel_offset[nCntModel].rot.z);
					}
				}
				// モデルの更新
				nCntModel++;
			}
			// モーションセットが来たら
			else if (strcmp(cHeadText, "MOTIONSET") == 0)
			{
				// カメラタイプの初期化
				nCameraType = 0;	// カメラタイプ
				// モデルの初期化
				nCntKeySet = 0;
				// エンドモーションセットが来るまでループ
				while (strcmp(cHeadText, "END_MOTIONSET") != 0)
				{
					// 初期化
					cHeadText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);

					// ループするかどうか情報読み込み
					if (strcmp(cHeadText, "LOOP") == 0)
					{
						sscanf(cRaedText, "%s %s %d",
							&cDie, &cDie,
							&pmodelAll->pMotion[nCntMotion].nLoop);
					}
					// キー数情報読み込み
					else if (strcmp(cHeadText, "NUM_KEY") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &pmodelAll->pMotion[nCntMotion].nNumKey);
						// モーション数生成
						pmodelAll->pMotion[nCntMotion].KeyInfo = new KEY_INFO[pmodelAll->pMotion[nCntMotion].nNumKey];
					}
					// キー数情報読み込み
					else if (strcmp(cHeadText, "CAMERAINFO") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &nCameraType);
					}
					// 当たり判定情報読み込み
					else if (strcmp(cHeadText, "COLLISION") == 0)
					{
						sscanf(cRaedText, "%s %s %d %f %f %f %f %d %d",
							&cDie, &cDie,
							&pmodelAll->pMotion[nCntMotion].Collision_nParts,
							&pmodelAll->pMotion[nCntMotion].Collision_Ofset.x,
							&pmodelAll->pMotion[nCntMotion].Collision_Ofset.y,
							&pmodelAll->pMotion[nCntMotion].Collision_Ofset.z,
							&pmodelAll->pMotion[nCntMotion].Collision_Radius,
							&pmodelAll->pMotion[nCntMotion].Collision_StartFram,
							&pmodelAll->pMotion[nCntMotion].Collision_EndFram);
					}

					// ダメージ情報読み込み
					else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
					{
						sscanf(cRaedText, "%s %s %d",
							&cDie, &cDie,
							&pmodelAll->pMotion[nCntMotion].Collision_Damage);
					}
					// キー情報読み込み
					else if (strcmp(cHeadText, "KEYSET") == 0)
					{
						// キーの初期化
						nCntKey = 0;
						// モデル数生成
						pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key = new KEY[nMaxkey];
						// カメラタイプ初期設定
						pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nCameraType = nCameraType;
						// エンドキーセットが来るまでループ
						while (strcmp(cHeadText, "END_KEYSET") != 0)
						{
							// 初期化
							cHeadText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// 攻撃当たり判定の頻度情報読み込み
							if (strcmp(cHeadText, "NUMCOLLISION") == 0)
							{
								// フレーム数
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nNumCollision);
							}

							// カメラタイプ情報読み込み
							else if (strcmp(cHeadText, "CAMERATYPE") == 0)
							{
								// フレーム数
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nCameraType);
								// カメラタイプを保存用に保存
								nCameraType = pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nCameraType;
							}

							// 吹っ飛び方情報読み込み
							else if (strcmp(cHeadText, "BLUST") == 0)
							{
								// フレーム数
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nBlust);
							}

							// フレーム情報読み込み
							else if (strcmp(cHeadText, "FRAME") == 0)
							{
								// フレーム数
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nFrame);
								// モーション全体のフレーム数加算
								pmodelAll->pMotion[nCntMotion].nAllFrame +=
									pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nFrame;
								// 当たり判定の回数がない場合関数を抜ける
								if (pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nNumCollision > 0)
								{
									// 攻撃判定頻度
									pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nMaxCollisiontime =
										pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nFrame /
										pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nNumCollision;
								}

							}

							// キー情報読み込み
							else if (strcmp(cHeadText, "KEY") == 0)
							{
								// エンドキーが来るまでループ
								while (strcmp(cHeadText, "END_KEY") != 0)
								{
									fgets(cRaedText, sizeof(cRaedText), pFile);
									sscanf(cRaedText, "%s", &cHeadText);

									// 位置情報読み込み
									if (strcmp(cHeadText, "POS") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].pos.x,
											&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].pos.y,
											&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].pos.z);

										// パーツの位置情報を加える
										pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].pos += pmodelAll->pModel_offset[nCntKey].pos;
									}

									// 回転情報読み込み
									else if (strcmp(cHeadText, "ROT") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].rot.x,
											&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].rot.y,
											&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].rot.z);
										// パーツの位置情報を加える
										pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].rot += pmodelAll->pModel_offset[nCntKey].rot;
									}
								}
								// カウントキー
								nCntKey++;
							}
						}
						// カウントモデル
						nCntKeySet++;
					}
				}
				// カウントプレイヤー
				nCntMotion++;
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

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 読み込んだテキストデータの破棄
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CModel_info::TextUnload(
	MODEL_ALL * pmodelAll,	// モデル・モーションの情報
	int const & nMaxMotion	// モーション数
)
{
	// オフセット情報の破棄
	delete[] pmodelAll->pModel_offset;
	pmodelAll->pModel_offset = NULL;
	// モーション情報の破棄
	for (int nCntMotion = 0; nCntMotion < nMaxMotion; nCntMotion++)
	{
		for (int nCntKeyInfo = 0; nCntKeyInfo < pmodelAll->pMotion[nCntMotion].nNumKey; nCntKeyInfo++)
		{
			// キーの破棄
			delete[] pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeyInfo].Key;
			pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeyInfo].Key = NULL;
		}
		//キー情報の破棄
		delete[] pmodelAll->pMotion[nCntMotion].KeyInfo;
		pmodelAll->pMotion[nCntMotion].KeyInfo = NULL;
	}
	// モーションの破棄
	delete[] pmodelAll->pMotion;
	pmodelAll->pMotion = NULL;
	// モデル情報全体の破棄
	delete pmodelAll;
	pmodelAll = NULL;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Xモデル情報取得
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
XMODEL & CModel_info::GetXModel(void)
{
	return m_Xmodel;
}
