//*************************************************************************************************************
//
// 3Dパーティクルの処理[particle.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "3Dparticle.h"
#include "3Deffect.h"

//-------------------------------------------------------------------------------------------------------------
// 静的メンバ変数の初期化
//-------------------------------------------------------------------------------------------------------------
C3DParticle::PARTICLE_OFFSET C3DParticle::m_ParticleOffset[C3DParticle::OFFSET_ID_MAX] = {};

//-------------------------------------------------------------------------------------------------------------
// コンストラクタ
//-------------------------------------------------------------------------------------------------------------
C3DParticle::C3DParticle():CScene(ACTOR_3DPARTICLE,LAYER_3DPARTICLE)
{
}

//-------------------------------------------------------------------------------------------------------------
// デストラクタ
//-------------------------------------------------------------------------------------------------------------
C3DParticle::~C3DParticle()
{
}

//-------------------------------------------------------------------------------------------------------------
// 初期化
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Init(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 終了
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Uninit(void)
{

}

//-------------------------------------------------------------------------------------------------------------
// 更新
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Update(void)
{
	m_nFlameCount++;

	if (m_nFlameCount > m_ParticleOffset[m_offsetID].nFrame)
	{
		Release();
	}
	else
	{
		// 変数宣言
		D3DXVECTOR3	move;		// 移動量
		D3DXVECTOR3	pos;		// 位置
		D3DXCOLOR	col;		// 色
		float		fRadius;	// 半径
		int			nLife;		// ライフ
		float		fSpeed;		// 速度
		float		fAngle[3];		// 角度

		// 設定数ループ
		for (int nCntEffect = 0; nCntEffect < m_ParticleOffset[m_offsetID].nNumber; nCntEffect++)
		{
			if (m_ParticleOffset[m_offsetID].pCol != NULL)
			{
				col = *m_ParticleOffset[m_offsetID].pCol;
			}
			else
			{
				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			// 半径の設定
			if (m_ParticleOffset[m_offsetID].pnRadiusRand == NULL &&
				m_ParticleOffset[m_offsetID].pfRadius == NULL)
			{// どちらも情報がないとき
				fRadius = 0.0f;
			}
			else if(m_ParticleOffset[m_offsetID].pnRadiusRand != NULL)
			{// ランダムに情報があるとき
				fRadius = (rand() % m_ParticleOffset[m_offsetID].pnRadiusRand->nMax + m_ParticleOffset[m_offsetID].pnRadiusRand->nMin) * 0.1f;
			}
			else
			{// それ以外
				fRadius = *m_ParticleOffset[m_offsetID].pfRadius;
			}

			// ライフの設定
			if (m_ParticleOffset[m_offsetID].pnLifeRand == NULL &&
				m_ParticleOffset[m_offsetID].pnLife == NULL)
			{// どちらも情報がないとき
				nLife = 0;
			}
			else if (m_ParticleOffset[m_offsetID].pnLifeRand != NULL)
			{// ランダムに情報があるとき
				nLife = (rand() % m_ParticleOffset[m_offsetID].pnLifeRand->nMax + m_ParticleOffset[m_offsetID].pnLifeRand->nMin);
			}
			else
			{// それ以外
				nLife = *m_ParticleOffset[m_offsetID].pnLife;
			}

			// 速度の設定
			if (m_ParticleOffset[m_offsetID].pnSpeedRand == NULL &&
				m_ParticleOffset[m_offsetID].pfSpeed == NULL)
			{// どちらも情報がないとき
				fSpeed = 0.0f;
			}
			else if (m_ParticleOffset[m_offsetID].pnSpeedRand != NULL)
			{// ランダムに情報があるとき
				fSpeed = (float)(rand() % m_ParticleOffset[m_offsetID].pnSpeedRand->nMax + m_ParticleOffset[m_offsetID].pnSpeedRand->nMin);
			}
			else
			{// それ以外
				fSpeed = *m_ParticleOffset[m_offsetID].pfSpeed;
			}

			// 角度の設定
			if (m_ParticleOffset[m_offsetID].pnAngleRand == NULL &&
				m_ParticleOffset[m_offsetID].pfAngle == NULL)
			{// どちらも情報がないとき
				fAngle[0] = 0.0f;
				fAngle[1] = 0.0f;
				fAngle[2] = 0.0f;
			}
			else if (m_ParticleOffset[m_offsetID].pnAngleRand != NULL)
			{// ランダムに情報があるとき
				fAngle[0] = (rand() % m_ParticleOffset[m_offsetID].pnAngleRand->nMax + m_ParticleOffset[m_offsetID].pnAngleRand->nMin)*0.01f;
				fAngle[1] = (rand() % m_ParticleOffset[m_offsetID].pnAngleRand->nMax + m_ParticleOffset[m_offsetID].pnAngleRand->nMin)*0.01f;
				fAngle[2] = (rand() % m_ParticleOffset[m_offsetID].pnAngleRand->nMax + m_ParticleOffset[m_offsetID].pnAngleRand->nMin)*0.01f;
			}
			else
			{// それ以外
				fAngle[0] = *m_ParticleOffset[m_offsetID].pfAngle;
				fAngle[1] = *m_ParticleOffset[m_offsetID].pfAngle;
				fAngle[2] = *m_ParticleOffset[m_offsetID].pfAngle;
			}

			if (m_ParticleOffset[m_offsetID].type == TYPE_CHARGE)
			{
				// 原点からの距離の設定
				float fLength;

				//(m_nLengthRand != NULL) ?
				//(rand() % m_nLengthRand->nMax + m_nLengthRand->nMin)*0.1f :
				//m_fLength;
				if (m_ParticleOffset[m_offsetID].pnLengthRand == NULL &&
					m_ParticleOffset[m_offsetID].pfLength == NULL)
				{// どちらも情報がないとき
					fLength = 0.0f;
				}
				else if (m_ParticleOffset[m_offsetID].pnLengthRand != NULL)
				{// ランダムに情報があるとき
					fLength = (rand() % m_ParticleOffset[m_offsetID].pnLengthRand->nMax + m_ParticleOffset[m_offsetID].pnLengthRand->nMin)*0.1f;
				}
				else
				{// それ以外
					fLength = *m_ParticleOffset[m_offsetID].pfLength;
				}


				// 位置の設定
				if (m_ParticleOffset[m_offsetID].pPos != NULL)
				{
					pos =
					{
						m_ParticleOffset[m_offsetID].pPos->x + sinf(fAngle[0] + *m_ParticleOffset[m_offsetID].pfAngle)*sinf(fAngle[1] + *m_ParticleOffset[m_offsetID].pfAngle)*fLength,
						m_ParticleOffset[m_offsetID].pPos->y + cosf(fAngle[1] + *m_ParticleOffset[m_offsetID].pfAngle)*sinf(fAngle[2] + *m_ParticleOffset[m_offsetID].pfAngle)*fLength,
						m_ParticleOffset[m_offsetID].pPos->z + cosf(fAngle[0] + *m_ParticleOffset[m_offsetID].pfAngle)*cosf(fAngle[2] + *m_ParticleOffset[m_offsetID].pfAngle)*fLength
					};
				}

				// ベクトルの作成
				D3DXVECTOR3 vec = *m_ParticleOffset[m_offsetID].pPos - pos;
				// ベクトルの正規化
				D3DXVec3Normalize(&vec, &vec);
				// 移動量に代入
				move.x = (vec.x*fSpeed)*m_ParticleOffset[m_offsetID].pSpeedRate->x;
				move.y = (vec.y*fSpeed)*m_ParticleOffset[m_offsetID].pSpeedRate->y;
				move.z = (vec.z*fSpeed)*m_ParticleOffset[m_offsetID].pSpeedRate->z;
			}
			else
			{
				// 位置の設定
				pos = *m_ParticleOffset[m_offsetID].pPos;

				// 移動量の設定
				move =
				{
					sinf(fAngle[0])*sinf(fAngle[1])*fSpeed*m_ParticleOffset[m_offsetID].pSpeedRate->x,
					cosf(fAngle[1])*sinf(fAngle[2])*fSpeed*m_ParticleOffset[m_offsetID].pSpeedRate->y,
					cosf(fAngle[0])*cosf(fAngle[2])*fSpeed*m_ParticleOffset[m_offsetID].pSpeedRate->z
				};
			}
			C3DEffect::Set3DEffect(
				(C3DEffect::EFFECT_TYPE)m_ParticleOffset[m_offsetID].nEffeType, m_ParticleOffset[m_offsetID].nEffeTexType,
				pos + m_Origin, move, col, fRadius, nLife);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 描画
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Draw(void)
{
}

//-------------------------------------------------------------------------------------------------------------
// 生成
//-------------------------------------------------------------------------------------------------------------
C3DParticle * C3DParticle::Create(PARTICLE_OFFSET_ID OffsetID, CONST D3DXVECTOR3 origin)
{
	C3DParticle *p3DParticle = new C3DParticle;
	p3DParticle->SetParticle(OffsetID);
	p3DParticle->SetOrigin(origin);
	return p3DParticle;
}

//-------------------------------------------------------------------------------------------------------------
// パーティクルの設定
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::SetParticle(PARTICLE_OFFSET_ID OffsetID)
{
	m_offsetID = OffsetID;
}

//-------------------------------------------------------------------------------------------------------------
// 読み込み
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Load(void)
{
	// 変数宣言
	FILE *pFile = NULL;				// ファイルのポインタ
	char cRead[128];				// 読み込み用
	char cComp[128];				// 比較用
	char cEmpty[128];				// 要らないもの用
	int nCntError = 0;				// エラー用
	int nCntOffset = 0;				// オフセットのカウント
	
	// 一時保存用
	INTEGER2 Save;
	// ファイルが開かれていなかったら
	if ((pFile = fopen("data/LOAD/ParticleInfo.txt", "r")) == NULL)
	{
		return;
	}

	while (strcmp(cComp, "SCRIPT") != 0)
	{
		// 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > 1048576)
		{// エラー
			nCntError = 0;
			return;
		}
	}

	while (strcmp(cComp, "END_SCRIPT") != 0)
	{// END_SCRIPTまでループ
	 // 1行読み込む
		fgets(cRead, sizeof(cRead), pFile);
		// 読み込んど文字列代入
		sscanf(cRead, "%s", &cComp);

		// エラーカウントをインクリメント
		nCntError++;
		if (nCntError > 1048576)
		{// エラー
			nCntError = 0;
			fclose(pFile);
			return;
		}
		else if (strcmp(cComp, "OFFSET") == 0)
		{
			while (strcmp(cComp, "END_OFFSET") != 0)
			{
				// 1行読み込む
				fgets(cRead, sizeof(cRead), pFile);
				// 読み込んど文字列代入
				sscanf(cRead, "%s", &cComp);
				if (strcmp(cComp, "IF") == 0)
				{
					// 読み込んど文字列代入
					sscanf(cRead, "%s %s", &cEmpty, &cComp);
					if (strcmp(cComp, "POSRAND") == 0)
					{
						// 読み込んど文字列代入
						sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty , &cComp);
						if (strcmp(cComp, "NULL") != 0)
						{
							while (strcmp(cComp, "}") != 0)
							{
								// 1行読み込む
								fgets(cRead, sizeof(cRead), pFile);
								// 読み込んど文字列代入
								sscanf(cRead, "%s", &cComp);

								if (strcmp(cComp, "POSX") == 0)
								{
									sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty, &Save.nMax, &Save.nMin);
									if (Save.nMax != -1 && Save.nMin != -1)
									{// メモリの確保
										m_ParticleOffset[nCntOffset].pPosXRand = new INTEGER2;
										// 最大
										m_ParticleOffset[nCntOffset].pPosXRand->nMax = Save.nMax;
										// 最小
										m_ParticleOffset[nCntOffset].pPosXRand->nMin = Save.nMin;
										Save.nMax = -1;
										Save.nMin = -1;
									}
									// 文字列の初期化
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "POSY") == 0)
								{
									sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty, &Save.nMax, &Save.nMin);
									if (Save.nMax != -1 && Save.nMin != -1)
									{// メモリの確保
										m_ParticleOffset[nCntOffset].pPosYRand = new INTEGER2;
										// 最大
										m_ParticleOffset[nCntOffset].pPosYRand->nMax = Save.nMax;
										// 最小
										m_ParticleOffset[nCntOffset].pPosYRand->nMin = Save.nMin;
										Save.nMax = -1;
										Save.nMin = -1;
									}

									// 文字列の初期化
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "POSZ") == 0)
								{
									sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty, &Save.nMax, &Save.nMin);
									if (Save.nMax != -1 && Save.nMin != -1)
									{// メモリの確保
										m_ParticleOffset[nCntOffset].pPosZRand = new INTEGER2;
										// 最大
										m_ParticleOffset[nCntOffset].pPosZRand->nMax = Save.nMax;
										// 最小
										m_ParticleOffset[nCntOffset].pPosZRand->nMin = Save.nMin;
										Save.nMax = -1;
										Save.nMin = -1;
									}

									// 文字列の初期化
									cComp[0] = '\0';
								}
							}
							// 文字列の初期化
							cComp[0] = '\0';
						}
						// 文字列の初期化
						cComp[0] = '\0';
					}
					else if (strcmp(cComp, "LENGTHRAND") == 0)
					{
						// 読み込んど文字列代入
						sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);
						if (strcmp(cComp, "NULL") != 0)
						{
							while (strcmp(cComp, "}") != 0)
							{
								// 1行読み込む
								fgets(cRead, sizeof(cRead), pFile);
								// 読み込んど文字列代入
								sscanf(cRead, "%s", &cComp);
								if (strcmp(cComp, "MAX") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMax);
									// 文字列の初期化
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "MIN") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMin);
									// 文字列の初期化
									cComp[0] = '\0';
								}

								if (Save.nMax != -1 && Save.nMin != -1)
								{
									// メモリの確保
									m_ParticleOffset[nCntOffset].pnLengthRand = new INTEGER2;
									// 最大
									m_ParticleOffset[nCntOffset].pnLengthRand->nMax = Save.nMax;
									// 最小
									m_ParticleOffset[nCntOffset].pnLengthRand->nMin = Save.nMin;
									Save.nMax = -1;
									Save.nMin = -1;
								}
							}
							// 文字列の初期化
							cComp[0] = '\0';
						}
						// 文字列の初期化
						cComp[0] = '\0';
					}
					else if (strcmp(cComp, "RADIUSRAND") == 0)
					{
						// 読み込んど文字列代入
						sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);
						if (strcmp(cComp, "NULL") != 0)
						{
							while (strcmp(cComp, "}") != 0)
							{
								// 1行読み込む
								fgets(cRead, sizeof(cRead), pFile);
								// 読み込んど文字列代入
								sscanf(cRead, "%s", &cComp);
								if (strcmp(cComp, "MAX") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMax);
									// 文字列の初期化
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "MIN") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMin);
									// 文字列の初期化
									cComp[0] = '\0';
								}
								if (Save.nMax != -1 && Save.nMin != -1)
								{
									// メモリの確保
									m_ParticleOffset[nCntOffset].pnRadiusRand = new INTEGER2;
									// 最大
									m_ParticleOffset[nCntOffset].pnRadiusRand->nMax = Save.nMax;
									// 最小
									m_ParticleOffset[nCntOffset].pnRadiusRand->nMin = Save.nMin;
									Save.nMax = -1;
									Save.nMin = -1;
								}
							}
							// 文字列の初期化
							cComp[0] = '\0';
						}
						// 文字列の初期化
						cComp[0] = '\0';
					}
					else if (strcmp(cComp, "ANGLERAND") == 0)
					{
						// 読み込んど文字列代入
						sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);
						if (strcmp(cComp, "NULL") != 0)
						{
							while (strcmp(cComp, "}") != 0)
							{
								// 1行読み込む
								fgets(cRead, sizeof(cRead), pFile);
								// 読み込んど文字列代入
								sscanf(cRead, "%s", &cComp);
								if (strcmp(cComp, "MAX") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMax);
									// 文字列の初期化
									cComp[0] = '\0';

								}
								else if (strcmp(cComp, "MIN") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMin);
									// 文字列の初期化
									cComp[0] = '\0';

								}
								if (Save.nMax != -1 && Save.nMin != -1)
								{
									// メモリの確保
									m_ParticleOffset[nCntOffset].pnAngleRand = new INTEGER2;
									// 最大
									m_ParticleOffset[nCntOffset].pnAngleRand->nMax = Save.nMax;
									// 最小
									m_ParticleOffset[nCntOffset].pnAngleRand->nMin = Save.nMin;
									Save.nMax = -1;
									Save.nMin = -1;
								}
							}
							// 文字列の初期化
							cComp[0] = '\0';
						}
						// 文字列の初期化
						cComp[0] = '\0';
					}
					else if (strcmp(cComp, "SPEEDRAND") == 0)
					{
						// 読み込んど文字列代入
						sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);
						if (strcmp(cComp, "NULL") != 0)
						{
							while (strcmp(cComp, "}") != 0)
							{
								// 1行読み込む
								fgets(cRead, sizeof(cRead), pFile);
								// 読み込んど文字列代入
								sscanf(cRead, "%s", &cComp);
								if (strcmp(cComp, "MAX") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMax);
									// 文字列の初期化
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "MIN") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMin);
									// 文字列の初期化
									cComp[0] = '\0';
								}
								if (Save.nMax != -1 && Save.nMin != -1)
								{
									// メモリの確保
									m_ParticleOffset[nCntOffset].pnSpeedRand = new INTEGER2;
									// 最大
									m_ParticleOffset[nCntOffset].pnSpeedRand->nMax = Save.nMax;
									// 最小
									m_ParticleOffset[nCntOffset].pnSpeedRand->nMin = Save.nMin;
									Save.nMax = -1;
									Save.nMin = -1;
								}
							}
							// 文字列の初期化
							cComp[0] = '\0';
						}
						// 文字列の初期化
						cComp[0] = '\0';
					}
					else if (strcmp(cComp, "LIFERAND") == 0)
					{
						// 読み込んど文字列代入
						sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);
						if (strcmp(cComp, "NULL") != 0)
						{
							while (strcmp(cComp, "}") != 0)
							{
								// 1行読み込む
								fgets(cRead, sizeof(cRead), pFile);
								// 読み込んど文字列代入
								sscanf(cRead, "%s", &cComp);
								if (strcmp(cComp, "MAX") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMax);
									// 文字列の初期化
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "MIN") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMin);
									// 文字列の初期化
									cComp[0] = '\0';
								}
								if (Save.nMax != -1 && Save.nMin != -1)
								{
									// メモリの確保
									m_ParticleOffset[nCntOffset].pnLifeRand = new INTEGER2;
									// 最大
									m_ParticleOffset[nCntOffset].pnLifeRand->nMax = Save.nMax;
									// 最小
									m_ParticleOffset[nCntOffset].pnLifeRand->nMin = Save.nMin;
									Save.nMax = -1;
									Save.nMin = -1;
								}
							}
							// 文字列の初期化
							cComp[0] = '\0';
						}
						// 文字列の初期化
						cComp[0] = '\0';
					}
				}
				else if (strcmp(cComp, "POS") == 0)
				{
					// 文字列の初期化
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						m_ParticleOffset[nCntOffset].pPos = new D3DXVECTOR3;
						sscanf(cRead, "%s %s %f %f %f", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].pPos->x,
							&m_ParticleOffset[nCntOffset].pPos->y,
							&m_ParticleOffset[nCntOffset].pPos->z);
					}

					// 文字列の初期化
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "LENGTH") == 0)
				{
					// 文字列の初期化
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						m_ParticleOffset[nCntOffset].pfLength = new float;
						sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty,
							m_ParticleOffset[nCntOffset].pfLength);
					}
					// 文字列の初期化
					cComp[0] = '\0';

				}
				else if (strcmp(cComp, "COL") == 0)
				{
					// 文字列の初期化
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						m_ParticleOffset[nCntOffset].pCol = new D3DXCOLOR;
						sscanf(cRead, "%s %s %f %f %f %f", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].pCol->r,
							&m_ParticleOffset[nCntOffset].pCol->g,
							&m_ParticleOffset[nCntOffset].pCol->b,
							&m_ParticleOffset[nCntOffset].pCol->a);
					}

					// 文字列の初期化
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "RADIUS") == 0)
				{
					// 文字列の初期化
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						m_ParticleOffset[nCntOffset].pfRadius = new float;
						sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty,
							m_ParticleOffset[nCntOffset].pfRadius);
					}
					// 文字列の初期化
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "SPEED") == 0)
				{
					// 文字列の初期化
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						m_ParticleOffset[nCntOffset].pfSpeed = new float;
						sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty,
							m_ParticleOffset[nCntOffset].pfSpeed);
					}
					// 文字列の初期化
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "LIFE") == 0)
				{
					// 文字列の初期化
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						m_ParticleOffset[nCntOffset].pnLife = new int;
						sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
							m_ParticleOffset[nCntOffset].pnLife);
					}
					// 文字列の初期化
					cComp[0] = '\0';
				}

				else if (strcmp(cComp, "FRAME") == 0)
				{
					// 文字列の初期化
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].nFrame);
					}
					// 文字列の初期化
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "NUMBER") == 0)
				{
					// 文字列の初期化
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].nNumber);
					}
					// 文字列の初期化
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "TEXTYPE") == 0)
				{
					// 文字列の初期化
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].nEffeTexType);
					}
					// 文字列の初期化
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "EFFETYPE") == 0)
				{
					// 文字列の初期化
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].nEffeType);
					}
					// 文字列の初期化
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "PARTICLETYPE") == 0)
				{
					// 文字列の初期化
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].type);
					}
					// 文字列の初期化
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "SPEEDRATE") == 0)
				{
					// 文字列の初期化
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						m_ParticleOffset[nCntOffset].pSpeedRate = new D3DXVECTOR3;
						sscanf(cRead, "%s %s %f %f %f", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].pSpeedRate->x,
							&m_ParticleOffset[nCntOffset].pSpeedRate->y,
							&m_ParticleOffset[nCntOffset].pSpeedRate->z);
					}
					// 文字列の初期化
					cComp[0] = '\0';
				}
			}
			// オフセットカウントを進める
			nCntOffset++;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 開放
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::Unload(void)
{
	for (int nCntOffset = 0; nCntOffset < C3DParticle::OFFSET_ID_MAX; nCntOffset++)
	{
		if (m_ParticleOffset[nCntOffset].pPosXRand != NULL)
		{// X座標のランダムの開放
			delete m_ParticleOffset[nCntOffset].pPosXRand;
			m_ParticleOffset[nCntOffset].pPosXRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pPosYRand != NULL)
		{// Y座標のランダムの開放
			delete m_ParticleOffset[nCntOffset].pPosYRand;
			m_ParticleOffset[nCntOffset].pPosYRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pPosZRand != NULL)
		{// Z座標のランダムの開放
			delete m_ParticleOffset[nCntOffset].pPosZRand;
			m_ParticleOffset[nCntOffset].pPosZRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pnLengthRand != NULL)
		{// 原点距離ランダムの開放
			delete m_ParticleOffset[nCntOffset].pnLengthRand;
			m_ParticleOffset[nCntOffset].pnLengthRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pnAngleRand != NULL)
		{// ランダム角度の開放
			delete m_ParticleOffset[nCntOffset].pnAngleRand;
			m_ParticleOffset[nCntOffset].pnAngleRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pnLifeRand != NULL)
		{// ライフのランダムの開放
			delete m_ParticleOffset[nCntOffset].pnLifeRand;
			m_ParticleOffset[nCntOffset].pnLifeRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pnRadiusRand != NULL)
		{// ランダム半径の開放
			delete m_ParticleOffset[nCntOffset].pnRadiusRand;
			m_ParticleOffset[nCntOffset].pnRadiusRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pnSpeedRand != NULL)
		{// ランダム速度の開放
			delete m_ParticleOffset[nCntOffset].pnSpeedRand;
			m_ParticleOffset[nCntOffset].pnSpeedRand = NULL;
		}

		if (m_ParticleOffset[nCntOffset].pPos != NULL)
		{// 原点からの位置の開放
			delete m_ParticleOffset[nCntOffset].pPos;
			m_ParticleOffset[nCntOffset].pPos = NULL;
		}

		if (m_ParticleOffset[nCntOffset].pfLength != NULL)
		{// 原点からの距離の開放
			delete m_ParticleOffset[nCntOffset].pfLength;
			m_ParticleOffset[nCntOffset].pfLength = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pCol != NULL)
		{// 色の開放
			delete m_ParticleOffset[nCntOffset].pCol;
			m_ParticleOffset[nCntOffset].pCol = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pfRadius != NULL)
		{// 半径の開放
			delete m_ParticleOffset[nCntOffset].pfRadius;
			m_ParticleOffset[nCntOffset].pfRadius = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pfSpeed != NULL)
		{// 速度の開放
			delete m_ParticleOffset[nCntOffset].pfSpeed;
			m_ParticleOffset[nCntOffset].pfSpeed = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pnLife != NULL)
		{// ライフの開放
			delete m_ParticleOffset[nCntOffset].pnLife;
			m_ParticleOffset[nCntOffset].pnLife = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pSpeedRate != NULL)
		{// 速度の変化量の開放
			delete m_ParticleOffset[nCntOffset].pSpeedRate;
			m_ParticleOffset[nCntOffset].pSpeedRate = NULL;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
// 原点位置の設定
//-------------------------------------------------------------------------------------------------------------
void C3DParticle::SetOrigin(CONST D3DXVECTOR3 & Origin)
{
	m_Origin = Origin;
}

