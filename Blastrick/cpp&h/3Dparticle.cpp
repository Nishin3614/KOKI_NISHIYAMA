// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3D�p�[�e�B�N���̏���[particle.h]
// Author : Nishiyama koki
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "3Dparticle.h"
#include "3Deffect.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ÓI�����o�ϐ��̏�����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DParticle::PARTICLE_OFFSET C3DParticle::m_ParticleOffset[C3DParticle::OFFSET_ID_MAX] = {};

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DParticle::C3DParticle():CScene(ACTOR_3DPARTICLE,LAYER_3DPARTICLE)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�X�g���N�^
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DParticle::~C3DParticle()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ������
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Init(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �I��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Uninit(void)
{

}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�V
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Update(void)
{

	m_nFlameCount++;
	if (m_nFlameCount > m_ParticleOffset[m_offsetID].nFrame)
	{
		Release();
	}
	else
	{
		// �ϐ��錾
		D3DXVECTOR3	move;		// �ړ���
		D3DXVECTOR3	pos;		// �ʒu
		D3DXVECTOR3 rot;		// ��]
		D3DXVECTOR2	size;		// �T�C�Y
		D3DXCOLOR	col;		// �F
		D3DXVECTOR3	posRand = {};	// �ʒu�����_��
		int			nLife;		// ���C�t
		int			nColRand;	// �F�̃����_��
		float		fSpeed;		// ���x
		float		fAngle[3];	// �p�x

		// �ݒ萔���[�v
		for (int nCntEffect = 0; nCntEffect < m_ParticleOffset[m_offsetID].nNumber; nCntEffect++)
		{
			if (m_ParticleOffset[m_offsetID].pCol != NULL)
			{
				col = *m_ParticleOffset[m_offsetID].pCol;
				if (m_ParticleOffset[m_offsetID].bRedRand)
				{
					nColRand = rand() % 10;
					col.r = (float)nColRand / 10;
				}
				if (m_ParticleOffset[m_offsetID].bGreenRand)
				{
					nColRand = rand() % 10;
					col.g = (float)nColRand / 10;
				}
				if (m_ParticleOffset[m_offsetID].bBlueRand)
				{
					nColRand = rand() % 10;
					col.b = (float)nColRand / 10;
				}
				if (m_ParticleOffset[m_offsetID].bAlphaRand)
				{
					nColRand = rand() % 10;
					col.a = (float)nColRand / 10;
				}
			}
			else
			{
				col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
			// ���a�̐ݒ�
			if (m_ParticleOffset[m_offsetID].pSizeXRand == NULL &&
				m_ParticleOffset[m_offsetID].pSizeYRand == NULL &&
				m_ParticleOffset[m_offsetID].pSize == NULL)
			{// �ǂ������񂪂Ȃ��Ƃ�
				size = D3DVECTOR2_INI;
			}
			else if (m_ParticleOffset[m_offsetID].pSizeXRand != NULL)
			{// �����_���ɏ�񂪂���Ƃ�
				size.x = (float)(rand() % m_ParticleOffset[m_offsetID].pSizeXRand->nMax + m_ParticleOffset[m_offsetID].pSizeXRand->nMin);
				if (m_ParticleOffset[m_offsetID].pSizeYRand != NULL)
				{// �����_���ɏ�񂪂���Ƃ�
					size.y = (float)(rand() % m_ParticleOffset[m_offsetID].pSizeYRand->nMax + m_ParticleOffset[m_offsetID].pSizeYRand->nMin);
				}
			}
			else
			{// ����ȊO
				size = *m_ParticleOffset[m_offsetID].pSize;
			}

			// ���C�t�̐ݒ�
			if (m_ParticleOffset[m_offsetID].pnLifeRand == NULL &&
				m_ParticleOffset[m_offsetID].pnLife == NULL)
			{// �ǂ������񂪂Ȃ��Ƃ�
				nLife = 0;
			}
			else if (m_ParticleOffset[m_offsetID].pnLifeRand != NULL)
			{// �����_���ɏ�񂪂���Ƃ�
				nLife = (rand() % m_ParticleOffset[m_offsetID].pnLifeRand->nMax + m_ParticleOffset[m_offsetID].pnLifeRand->nMin);
			}
			else
			{// ����ȊO
				nLife = *m_ParticleOffset[m_offsetID].pnLife;
			}

			// ���x�̐ݒ�
			if (m_ParticleOffset[m_offsetID].pnSpeedRand == NULL &&
				m_ParticleOffset[m_offsetID].pfSpeed == NULL)
			{// �ǂ������񂪂Ȃ��Ƃ�
				fSpeed = 0.0f;
			}
			else if (m_ParticleOffset[m_offsetID].pnSpeedRand != NULL)
			{// �����_���ɏ�񂪂���Ƃ�
				fSpeed = (float)(rand() % m_ParticleOffset[m_offsetID].pnSpeedRand->nMax + m_ParticleOffset[m_offsetID].pnSpeedRand->nMin);
			}
			else
			{// ����ȊO
				fSpeed = *m_ParticleOffset[m_offsetID].pfSpeed;
			}

			// �p�x�̐ݒ�
			if (m_ParticleOffset[m_offsetID].pnAngleRand == NULL &&
				m_ParticleOffset[m_offsetID].pRot == NULL)
			{// �ǂ������񂪂Ȃ��Ƃ�
				fAngle[0] = 0.0f;
				fAngle[1] = 0.0f;
				fAngle[2] = 0.0f;
			}
			else if (m_ParticleOffset[m_offsetID].pnAngleRand != NULL)
			{// �����_���ɏ�񂪂���Ƃ�
				fAngle[0] = (rand() % m_ParticleOffset[m_offsetID].pnAngleRand->nMax + m_ParticleOffset[m_offsetID].pnAngleRand->nMin)*0.01f;
				fAngle[1] = (rand() % m_ParticleOffset[m_offsetID].pnAngleRand->nMax + m_ParticleOffset[m_offsetID].pnAngleRand->nMin)*0.01f;
				fAngle[2] = (rand() % m_ParticleOffset[m_offsetID].pnAngleRand->nMax + m_ParticleOffset[m_offsetID].pnAngleRand->nMin)*0.01f;
			}
			else
			{// ����ȊO
				fAngle[0] = m_ParticleOffset[m_offsetID].pRot->x;
				fAngle[1] = m_ParticleOffset[m_offsetID].pRot->y;
				fAngle[2] = m_ParticleOffset[m_offsetID].pRot->z;
			}

			if (m_ParticleOffset[m_offsetID].type == TYPE_CHARGE)
			{
				// ���_����̋����̐ݒ�
				float fLength;

				//(m_nLengthRand != NULL) ?
				//(rand() % m_nLengthRand->nMax + m_nLengthRand->nMin)*0.1f :
				//m_fLength;
				if (m_ParticleOffset[m_offsetID].pnLengthRand == NULL &&
					m_ParticleOffset[m_offsetID].pfLength == NULL)
				{// �ǂ������񂪂Ȃ��Ƃ�
					fLength = 0.0f;
				}
				else if (m_ParticleOffset[m_offsetID].pnLengthRand != NULL)
				{// �����_���ɏ�񂪂���Ƃ�
					fLength = (rand() % m_ParticleOffset[m_offsetID].pnLengthRand->nMax + m_ParticleOffset[m_offsetID].pnLengthRand->nMin)*0.1f;
				}
				else
				{// ����ȊO
					fLength = *m_ParticleOffset[m_offsetID].pfLength;
				}


				// �ʒu�̐ݒ�
				if (m_ParticleOffset[m_offsetID].pPosXRand != NULL ||
					m_ParticleOffset[m_offsetID].pPosYRand != NULL || 
					m_ParticleOffset[m_offsetID].pPosZRand != NULL)
				{
					posRand.x = (float)(rand() % m_ParticleOffset[m_offsetID].pPosXRand->nMax + m_ParticleOffset[m_offsetID].pPosXRand->nMin);
					posRand.y = (float)(rand() % m_ParticleOffset[m_offsetID].pPosYRand->nMax + m_ParticleOffset[m_offsetID].pPosYRand->nMin);
					posRand.z = (float)(rand() % m_ParticleOffset[m_offsetID].pPosZRand->nMax + m_ParticleOffset[m_offsetID].pPosZRand->nMin);

					pos =
					{
						posRand.x + sinf(fAngle[0] + m_ParticleOffset[m_offsetID].pRot->x)*sinf(fAngle[1] + m_ParticleOffset[m_offsetID].pRot->x)*fLength,
						posRand.y + cosf(fAngle[1] + m_ParticleOffset[m_offsetID].pRot->y)*sinf(fAngle[2] + m_ParticleOffset[m_offsetID].pRot->y)*fLength,
						posRand.z + cosf(fAngle[0] + m_ParticleOffset[m_offsetID].pRot->z)*cosf(fAngle[2] + m_ParticleOffset[m_offsetID].pRot->z)*fLength
					};
				}
				else if (m_ParticleOffset[m_offsetID].pPos != NULL)
				{
					pos =
					{
						m_ParticleOffset[m_offsetID].pPos->x + sinf(fAngle[0] + m_ParticleOffset[m_offsetID].pRot->x)*sinf(fAngle[1] + m_ParticleOffset[m_offsetID].pRot->x)*fLength,
						m_ParticleOffset[m_offsetID].pPos->y + cosf(fAngle[1] + m_ParticleOffset[m_offsetID].pRot->y)*sinf(fAngle[2] + m_ParticleOffset[m_offsetID].pRot->y)*fLength,
						m_ParticleOffset[m_offsetID].pPos->z + cosf(fAngle[0] + m_ParticleOffset[m_offsetID].pRot->z)*cosf(fAngle[2] + m_ParticleOffset[m_offsetID].pRot->z)*fLength
					};
				}

				// �x�N�g���̍쐬
				D3DXVECTOR3 vec = *m_ParticleOffset[m_offsetID].pPos - pos;
				// �x�N�g���̐��K��
				D3DXVec3Normalize(&vec, &vec);
				// �ړ��ʂɑ��
				if (m_ParticleOffset[m_offsetID].pSpeedRate != NULL)
				{
					move.x = (vec.x*fSpeed)*m_ParticleOffset[m_offsetID].pSpeedRate->x;
					move.y = (vec.y*fSpeed)*m_ParticleOffset[m_offsetID].pSpeedRate->y;
					move.z = (vec.z*fSpeed)*m_ParticleOffset[m_offsetID].pSpeedRate->z;
				}
				else
				{
					move.x = 0.0f;
					move.y = 0.0f;
					move.z = 0.0f;
				}
			}
			else
			{

				// �ʒu�̐ݒ�
				if (m_ParticleOffset[m_offsetID].pPosXRand != NULL &&
					m_ParticleOffset[m_offsetID].pPosYRand != NULL &&
					m_ParticleOffset[m_offsetID].pPosZRand != NULL)
				{
					posRand.x = (float)(rand() % m_ParticleOffset[m_offsetID].pPosXRand->nMax + m_ParticleOffset[m_offsetID].pPosXRand->nMin);
					posRand.y = (float)(rand() % m_ParticleOffset[m_offsetID].pPosYRand->nMax + m_ParticleOffset[m_offsetID].pPosYRand->nMin);
					posRand.z = (float)(rand() % m_ParticleOffset[m_offsetID].pPosZRand->nMax + m_ParticleOffset[m_offsetID].pPosZRand->nMin);

					pos =
					{
						posRand.x,
						posRand.y,
						posRand.z
					};
				}
				else if (m_ParticleOffset[m_offsetID].pPos != NULL)
				{
					pos = *m_ParticleOffset[m_offsetID].pPos;
				}
				// �ʒu�̐ݒ�

				// �ړ��ʂ̐ݒ�
				if (m_ParticleOffset[m_offsetID].pSpeedRate != NULL)
				{
					move =
					{
						sinf(fAngle[0])*sinf(fAngle[1])*fSpeed*m_ParticleOffset[m_offsetID].pSpeedRate->x,
						cosf(fAngle[1])*sinf(fAngle[2])*fSpeed*m_ParticleOffset[m_offsetID].pSpeedRate->y,
						cosf(fAngle[0])*cosf(fAngle[2])*fSpeed*m_ParticleOffset[m_offsetID].pSpeedRate->z
					};
				}
				else
				{
					move.x = 0.0f;
					move.y = 0.0f;
					move.z = 0.0f;
				}
			}
			C3DEffect::Set3DEffect(
				(C3DEffect::EFFECT_TYPE)m_ParticleOffset[m_offsetID].nEffeType,
				m_ParticleOffset[m_offsetID].nEffeTexType,
				pos + m_Origin, 
				D3DXVECTOR3(
					fAngle[0],
					fAngle[1],
					fAngle[2]
				),
				move, 
				col, 
				size,
				nLife
			);
		}
	}
}
/*
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J���[
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Update_Col(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �T�C�Y
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Update_Size(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���C�t
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Update_Life(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �X�s�[�h
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Update_Speed(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �p�x
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Update_Angle(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ʒu
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Update_Pos(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ړ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Update_Move(void)
{
}
*/
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �`��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Draw(void)
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
C3DParticle * C3DParticle::Create(PARTICLE_OFFSET_ID OffsetID, CONST D3DXVECTOR3 origin)
{
	C3DParticle *p3DParticle = new C3DParticle;
	p3DParticle->SetParticle(OffsetID);
	p3DParticle->SetOrigin(origin);
	return p3DParticle;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �p�[�e�B�N���̐ݒ�
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::SetParticle(PARTICLE_OFFSET_ID OffsetID)
{
	m_offsetID = OffsetID;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT C3DParticle::Load(void)
{
	// �ϐ��錾
	FILE *pFile = NULL;				// �t�@�C���̃|�C���^
	char cRead[128];				// �ǂݍ��ݗp
	char cComp[128];				// ��r�p
	char cEmpty[128];				// �v��Ȃ����̗p
	int nCntError = 0;				// �G���[�p
	int nCntOffset = 0;				// �I�t�Z�b�g�̃J�E���g
	
	// �ꎞ�ۑ��p
	INTEGER2 Save;
	Save.nMax = -1;
	Save.nMin = -1;
	// �t�@�C�����J����Ă��Ȃ�������
	if ((pFile = fopen("data/LOAD/ParticleInfo.txt", "r")) == NULL)
	{
		CCalculation::Messanger("data/LOAD/ParticleInfo.txt�̃e�L�X�g������܂���");
		return E_FAIL;
	}

	while (strcmp(cComp, "SCRIPT") != 0)
	{
		// 1�s�ǂݍ���
		fgets(cRead, sizeof(cRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(cRead, "%s", &cComp);

		// �G���[�J�E���g���C���N�������g
		nCntError++;
		if (nCntError > 1048576)
		{// �G���[
			nCntError = 0;
			CCalculation::Messanger("�X�N���v�g������܂���");
			return E_FAIL;
		}
	}

	while (strcmp(cComp, "END_SCRIPT") != 0)
	{// END_SCRIPT�܂Ń��[�v
	 // 1�s�ǂݍ���
		fgets(cRead, sizeof(cRead), pFile);
		// �ǂݍ���Ǖ�������
		sscanf(cRead, "%s", &cComp);

		// �G���[�J�E���g���C���N�������g
		nCntError++;
		if (nCntError > 1048576)
		{// �G���[
			nCntError = 0;
			fclose(pFile);
			CCalculation::Messanger("�G���h�X�N���v�g������܂���");
			return E_FAIL;
		}
		else if (strcmp(cComp, "OFFSET") == 0)
		{
			while (strcmp(cComp, "END_OFFSET") != 0)
			{
				// 1�s�ǂݍ���
				fgets(cRead, sizeof(cRead), pFile);
				// �ǂݍ���Ǖ�������
				sscanf(cRead, "%s", &cComp);
				if (strcmp(cComp, "IF") == 0)
				{
					// �ǂݍ���Ǖ�������
					sscanf(cRead, "%s %s", &cEmpty, &cComp);
					if (strcmp(cComp, "POSRAND") == 0)
					{
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty , &cComp);
						if (strcmp(cComp, "NULL") != 0)
						{
							while (strcmp(cComp, "}") != 0)
							{
								// 1�s�ǂݍ���
								fgets(cRead, sizeof(cRead), pFile);
								// �ǂݍ���Ǖ�������
								sscanf(cRead, "%s", &cComp);

								if (strcmp(cComp, "POSX") == 0)
								{
									sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty, &Save.nMax, &Save.nMin);
									if (Save.nMax != -1 && Save.nMin != -1)
									{// �������̊m��
										m_ParticleOffset[nCntOffset].pPosXRand = new INTEGER2;
										// �ő�
										m_ParticleOffset[nCntOffset].pPosXRand->nMax = Save.nMax;
										// �ŏ�
										m_ParticleOffset[nCntOffset].pPosXRand->nMin = Save.nMin;
										Save.nMax = -1;
										Save.nMin = -1;
									}
									// ������̏�����
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "POSY") == 0)
								{
									sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty, &Save.nMax, &Save.nMin);
									if (Save.nMax != -1 && Save.nMin != -1)
									{// �������̊m��
										m_ParticleOffset[nCntOffset].pPosYRand = new INTEGER2;
										// �ő�
										m_ParticleOffset[nCntOffset].pPosYRand->nMax = Save.nMax;
										// �ŏ�
										m_ParticleOffset[nCntOffset].pPosYRand->nMin = Save.nMin;
										Save.nMax = -1;
										Save.nMin = -1;
									}

									// ������̏�����
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "POSZ") == 0)
								{
									sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty, &Save.nMax, &Save.nMin);
									if (Save.nMax != -1 && Save.nMin != -1)
									{// �������̊m��
										m_ParticleOffset[nCntOffset].pPosZRand = new INTEGER2;
										// �ő�
										m_ParticleOffset[nCntOffset].pPosZRand->nMax = Save.nMax;
										// �ŏ�
										m_ParticleOffset[nCntOffset].pPosZRand->nMin = Save.nMin;
										Save.nMax = -1;
										Save.nMin = -1;
									}

									// ������̏�����
									cComp[0] = '\0';
								}
							}
							// ������̏�����
							cComp[0] = '\0';
						}
						// ������̏�����
						cComp[0] = '\0';
					}
					else if (strcmp(cComp, "COLRAND") == 0)
					{
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);
						if (strcmp(cComp, "NULL") != 0)
						{
							m_ParticleOffset[nCntOffset].pCol = new D3DXCOLOR;
							// ������
							m_ParticleOffset[nCntOffset].pCol->r = 1.0f;
							m_ParticleOffset[nCntOffset].pCol->g = 1.0f;
							m_ParticleOffset[nCntOffset].pCol->b = 1.0f;
							m_ParticleOffset[nCntOffset].pCol->a = 1.0f;

							while (strcmp(cComp, "}") != 0)
							{
								// 1�s�ǂݍ���
								fgets(cRead, sizeof(cRead), pFile);
								// �ǂݍ���Ǖ�������
								sscanf(cRead, "%s", &cComp);
								if (strcmp(cComp, "RED") == 0)
								{
									// �����_����Ԃ�
									m_ParticleOffset[nCntOffset].bRedRand = true;
									// ������̏�����
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "GREEN") == 0)
								{
									// �����_����Ԃ�
									m_ParticleOffset[nCntOffset].bGreenRand = true;
									// ������̏�����
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "BLUE") == 0)
								{
									// �����_����Ԃ�
									m_ParticleOffset[nCntOffset].bBlueRand = true;
									// ������̏�����
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "ALPHA") == 0)
								{
									// �����_����Ԃ�
									m_ParticleOffset[nCntOffset].bAlphaRand = true;
									// ������̏�����
									cComp[0] = '\0';
								}
							}
							// ������̏�����
							cComp[0] = '\0';
						}
						// ������̏�����
						cComp[0] = '\0';
					}
					else if (strcmp(cComp, "LENGTHRAND") == 0)
					{
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);
						if (strcmp(cComp, "NULL") != 0)
						{
							while (strcmp(cComp, "}") != 0)
							{
								// 1�s�ǂݍ���
								fgets(cRead, sizeof(cRead), pFile);
								// �ǂݍ���Ǖ�������
								sscanf(cRead, "%s", &cComp);
								if (strcmp(cComp, "MAX") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMax);
									// ������̏�����
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "MIN") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMin);
									// ������̏�����
									cComp[0] = '\0';
								}

								if (Save.nMax != -1 && Save.nMin != -1)
								{
									// �������̊m��
									m_ParticleOffset[nCntOffset].pnLengthRand = new INTEGER2;
									// �ő�
									m_ParticleOffset[nCntOffset].pnLengthRand->nMax = Save.nMax;
									// �ŏ�
									m_ParticleOffset[nCntOffset].pnLengthRand->nMin = Save.nMin;
									Save.nMax = -1;
									Save.nMin = -1;
								}
							}
							// ������̏�����
							cComp[0] = '\0';
						}
						// ������̏�����
						cComp[0] = '\0';
					}
					else if (strcmp(cComp, "SIZERAND") == 0)
					{
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);
						if (strcmp(cComp, "NULL") != 0)
						{
							while (strcmp(cComp, "}") != 0)
							{
								// 1�s�ǂݍ���
								fgets(cRead, sizeof(cRead), pFile);
								// �ǂݍ���Ǖ�������
								sscanf(cRead, "%s", &cComp);

								if (strcmp(cComp, "SIZEX") == 0)
								{
									sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty, &Save.nMax, &Save.nMin);
									if (Save.nMax != -1 && Save.nMin != -1)
									{// �������̊m��
										m_ParticleOffset[nCntOffset].pSizeXRand = new INTEGER2;
										// �ő�
										m_ParticleOffset[nCntOffset].pSizeXRand->nMax = Save.nMax;
										// �ŏ�
										m_ParticleOffset[nCntOffset].pSizeXRand->nMin = Save.nMin;
										Save.nMax = -1;
										Save.nMin = -1;
									}
									// ������̏�����
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "SIZEY") == 0)
								{
									sscanf(cRead, "%s %s %d %d", &cEmpty, &cEmpty, &Save.nMax, &Save.nMin);
									if (Save.nMax != -1 && Save.nMin != -1)
									{// �������̊m��
										m_ParticleOffset[nCntOffset].pSizeYRand = new INTEGER2;
										// �ő�
										m_ParticleOffset[nCntOffset].pSizeYRand->nMax = Save.nMax;
										// �ŏ�
										m_ParticleOffset[nCntOffset].pSizeYRand->nMin = Save.nMin;
										Save.nMax = -1;
										Save.nMin = -1;
									}

									// ������̏�����
									cComp[0] = '\0';
								}
							}
							// ������̏�����
							cComp[0] = '\0';
						}
						// ������̏�����
						cComp[0] = '\0';
					}
					else if (strcmp(cComp, "ANGLERAND") == 0)
					{
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);
						if (strcmp(cComp, "NULL") != 0)
						{
							while (strcmp(cComp, "}") != 0)
							{
								// 1�s�ǂݍ���
								fgets(cRead, sizeof(cRead), pFile);
								// �ǂݍ���Ǖ�������
								sscanf(cRead, "%s", &cComp);
								if (strcmp(cComp, "MAX") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMax);
									// ������̏�����
									cComp[0] = '\0';

								}
								else if (strcmp(cComp, "MIN") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMin);
									// ������̏�����
									cComp[0] = '\0';

								}
								if (Save.nMax != -1 && Save.nMin != -1)
								{
									// �������̊m��
									m_ParticleOffset[nCntOffset].pnAngleRand = new INTEGER2;
									// �ő�
									m_ParticleOffset[nCntOffset].pnAngleRand->nMax = Save.nMax;
									// �ŏ�
									m_ParticleOffset[nCntOffset].pnAngleRand->nMin = Save.nMin;
									Save.nMax = -1;
									Save.nMin = -1;
								}
							}
							// ������̏�����
							cComp[0] = '\0';
						}
						// ������̏�����
						cComp[0] = '\0';
					}
					else if (strcmp(cComp, "SPEEDRAND") == 0)
					{
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);
						if (strcmp(cComp, "NULL") != 0)
						{
							while (strcmp(cComp, "}") != 0)
							{
								// 1�s�ǂݍ���
								fgets(cRead, sizeof(cRead), pFile);
								// �ǂݍ���Ǖ�������
								sscanf(cRead, "%s", &cComp);
								if (strcmp(cComp, "MAX") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMax);
									// ������̏�����
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "MIN") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMin);
									// ������̏�����
									cComp[0] = '\0';
								}
								if (Save.nMax != -1 && Save.nMin != -1)
								{
									// �������̊m��
									m_ParticleOffset[nCntOffset].pnSpeedRand = new INTEGER2;
									// �ő�
									m_ParticleOffset[nCntOffset].pnSpeedRand->nMax = Save.nMax;
									// �ŏ�
									m_ParticleOffset[nCntOffset].pnSpeedRand->nMin = Save.nMin;
									Save.nMax = -1;
									Save.nMin = -1;
								}
							}
							// ������̏�����
							cComp[0] = '\0';
						}
						// ������̏�����
						cComp[0] = '\0';
					}
					else if (strcmp(cComp, "LIFERAND") == 0)
					{
						// �ǂݍ���Ǖ�������
						sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);
						if (strcmp(cComp, "NULL") != 0)
						{
							while (strcmp(cComp, "}") != 0)
							{
								// 1�s�ǂݍ���
								fgets(cRead, sizeof(cRead), pFile);
								// �ǂݍ���Ǖ�������
								sscanf(cRead, "%s", &cComp);
								if (strcmp(cComp, "MAX") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMax);
									// ������̏�����
									cComp[0] = '\0';
								}
								else if (strcmp(cComp, "MIN") == 0)
								{
									sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty, &Save.nMin);
									// ������̏�����
									cComp[0] = '\0';
								}
								if (Save.nMax != -1 && Save.nMin != -1)
								{
									// �������̊m��
									m_ParticleOffset[nCntOffset].pnLifeRand = new INTEGER2;
									// �ő�
									m_ParticleOffset[nCntOffset].pnLifeRand->nMax = Save.nMax;
									// �ŏ�
									m_ParticleOffset[nCntOffset].pnLifeRand->nMin = Save.nMin;
									Save.nMax = -1;
									Save.nMin = -1;
								}
							}
							// ������̏�����
							cComp[0] = '\0';
						}
						// ������̏�����
						cComp[0] = '\0';
					}
				}
				else if (strcmp(cComp, "POS") == 0)
				{
					// ������̏�����
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

					// ������̏�����
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "ANGLE") == 0)
				{
					// ������̏�����
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						m_ParticleOffset[nCntOffset].pRot = new D3DXVECTOR3;
						sscanf(cRead, "%s %s %f %f %f", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].pRot->x,
							&m_ParticleOffset[nCntOffset].pRot->y,
							&m_ParticleOffset[nCntOffset].pRot->z);
					}

					// ������̏�����
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "LENGTH") == 0)
				{
					// ������̏�����
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						m_ParticleOffset[nCntOffset].pfLength = new float;
						sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty,
							m_ParticleOffset[nCntOffset].pfLength);
					}
					// ������̏�����
					cComp[0] = '\0';

				}
				else if (strcmp(cComp, "COL") == 0)
				{
					// ������̏�����
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						// �k���`�F�b�N
						if (m_ParticleOffset[nCntOffset].pCol != NULL)
						{
							delete m_ParticleOffset[nCntOffset].pCol;
							m_ParticleOffset[nCntOffset].pCol = NULL;
						}
						m_ParticleOffset[nCntOffset].pCol = new D3DXCOLOR;
						sscanf(cRead, "%s %s %f %f %f %f", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].pCol->r,
							&m_ParticleOffset[nCntOffset].pCol->g,
							&m_ParticleOffset[nCntOffset].pCol->b,
							&m_ParticleOffset[nCntOffset].pCol->a);
					}

					// ������̏�����
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "SIZE") == 0)
				{
					// ������̏�����
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						m_ParticleOffset[nCntOffset].pSize = new D3DXVECTOR2;
						sscanf(cRead, "%s %s %f %f",
							&cEmpty, 
							&cEmpty,
							&m_ParticleOffset[nCntOffset].pSize->x,
							&m_ParticleOffset[nCntOffset].pSize->y
							);

					}
					// ������̏�����
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "SPEED") == 0)
				{
					// ������̏�����
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						m_ParticleOffset[nCntOffset].pfSpeed = new float;
						sscanf(cRead, "%s %s %f", &cEmpty, &cEmpty,
							m_ParticleOffset[nCntOffset].pfSpeed);
					}
					// ������̏�����
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "LIFE") == 0)
				{
					// ������̏�����
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						m_ParticleOffset[nCntOffset].pnLife = new int;
						sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
							m_ParticleOffset[nCntOffset].pnLife);
					}
					// ������̏�����
					cComp[0] = '\0';
				}

				else if (strcmp(cComp, "FRAME") == 0)
				{
					// ������̏�����
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].nFrame);
					}
					// ������̏�����
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "NUMBER") == 0)
				{
					// ������̏�����
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].nNumber);
					}
					// ������̏�����
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "TEXTYPE") == 0)
				{
					// ������̏�����
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].nEffeTexType);
					}
					// ������̏�����
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "EFFETYPE") == 0)
				{
					// ������̏�����
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].nEffeType);
					}
					// ������̏�����
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "PARTICLETYPE") == 0)
				{
					// ������̏�����
					cComp[0] = '\0';
					sscanf(cRead, "%s %s %s", &cEmpty, &cEmpty, &cComp);

					if (strcmp(cComp, "NULL") != 0)
					{
						sscanf(cRead, "%s %s %d", &cEmpty, &cEmpty,
							&m_ParticleOffset[nCntOffset].type);
					}
					// ������̏�����
					cComp[0] = '\0';
				}
				else if (strcmp(cComp, "SPEEDRATE") == 0)
				{
					// ������̏�����
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
					// ������̏�����
					cComp[0] = '\0';
				}
			}
			// �I�t�Z�b�g�J�E���g��i�߂�
			nCntOffset++;
		}
	}
	fclose(pFile);
	return S_OK;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �J��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Unload(void)
{
	for (int nCntOffset = 0; nCntOffset < C3DParticle::OFFSET_ID_MAX; nCntOffset++)
	{
		if (m_ParticleOffset[nCntOffset].pPosXRand != NULL)
		{// X���W�̃����_���̊J��
			delete m_ParticleOffset[nCntOffset].pPosXRand;
			m_ParticleOffset[nCntOffset].pPosXRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pPosYRand != NULL)
		{// Y���W�̃����_���̊J��
			delete m_ParticleOffset[nCntOffset].pPosYRand;
			m_ParticleOffset[nCntOffset].pPosYRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pPosZRand != NULL)
		{// Z���W�̃����_���̊J��
			delete m_ParticleOffset[nCntOffset].pPosZRand;
			m_ParticleOffset[nCntOffset].pPosZRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pnLengthRand != NULL)
		{// ���_���������_���̊J��
			delete m_ParticleOffset[nCntOffset].pnLengthRand;
			m_ParticleOffset[nCntOffset].pnLengthRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pnAngleRand != NULL)
		{// �����_���p�x�̊J��
			delete m_ParticleOffset[nCntOffset].pnAngleRand;
			m_ParticleOffset[nCntOffset].pnAngleRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pnLifeRand != NULL)
		{// ���C�t�̃����_���̊J��
			delete m_ParticleOffset[nCntOffset].pnLifeRand;
			m_ParticleOffset[nCntOffset].pnLifeRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pSizeXRand != NULL)
		{// �����_���T�C�YX�̊J��
			delete m_ParticleOffset[nCntOffset].pSizeXRand;
			m_ParticleOffset[nCntOffset].pSizeXRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pSizeYRand != NULL)
		{// �����_���T�C�YY�̊J��
			delete m_ParticleOffset[nCntOffset].pSizeYRand;
			m_ParticleOffset[nCntOffset].pSizeYRand = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pnSpeedRand != NULL)
		{// �����_�����x�̊J��
			delete m_ParticleOffset[nCntOffset].pnSpeedRand;
			m_ParticleOffset[nCntOffset].pnSpeedRand = NULL;
		}

		if (m_ParticleOffset[nCntOffset].pPos != NULL)
		{// ���_����̈ʒu�̊J��
			delete m_ParticleOffset[nCntOffset].pPos;
			m_ParticleOffset[nCntOffset].pPos = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pRot!= NULL)
		{// ��]�̊J��
			delete m_ParticleOffset[nCntOffset].pRot;
			m_ParticleOffset[nCntOffset].pRot = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pfLength != NULL)
		{// ���_����̋����̊J��
			delete m_ParticleOffset[nCntOffset].pfLength;
			m_ParticleOffset[nCntOffset].pfLength = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pCol != NULL)
		{// �F�̊J��
			delete m_ParticleOffset[nCntOffset].pCol;
			m_ParticleOffset[nCntOffset].pCol = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pSize != NULL)
		{// �T�C�Y�̊J��
			delete m_ParticleOffset[nCntOffset].pSize;
			m_ParticleOffset[nCntOffset].pSize = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pfSpeed != NULL)
		{// ���x�̊J��
			delete m_ParticleOffset[nCntOffset].pfSpeed;
			m_ParticleOffset[nCntOffset].pfSpeed = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pnLife != NULL)
		{// ���C�t�̊J��
			delete m_ParticleOffset[nCntOffset].pnLife;
			m_ParticleOffset[nCntOffset].pnLife = NULL;
		}
		if (m_ParticleOffset[nCntOffset].pSpeedRate != NULL)
		{// ���x�̕ω��ʂ̊J��
			delete m_ParticleOffset[nCntOffset].pSpeedRate;
			m_ParticleOffset[nCntOffset].pSpeedRate = NULL;
		}
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���_�ʒu�̐ݒ�
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::SetOrigin(CONST D3DXVECTOR3 & Origin)
{
	m_Origin = Origin;
}

#ifdef _DEBUG
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void C3DParticle::Debug(void)
{
}
#endif // _DEBUG