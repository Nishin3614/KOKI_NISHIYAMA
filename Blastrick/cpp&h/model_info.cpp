// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// ���f����񏈗� [model_info.cpp]
// Author : KOKI NISHIYAMA
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "model_info.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ÓI�ϐ��錾
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �R���X�g���N�^����
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
// �f�X�g���N�^����
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CModel_info::~CModel_info()
{
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���f���̏��̓ǂݍ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CModel_info::Load(
	LPDIRECT3DDEVICE9 pDevice,
	const char * charModel
)
{
	// �ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X
	D3DXMATERIAL *pMat;				// ���݂̃}�e���A���ۑ�
	int nNumVertex;					// ���_��
	DWORD sizeFVF;					// ���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVertexBuffer;			// ���_�o�b�t�@�ւ̃|�C���^

	// X�t�@�C���̓ǂݍ���
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

	// �}�e���A���̒��_�ʒu�̍ő�l�E�ŏ��l
	m_Xmodel.vtxMinMaterials = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);
	m_Xmodel.vtxMaxMaterials = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);
	// ���_�����擾
	nNumVertex = m_Xmodel.pMesh->GetNumVertices();
	// ���_�t�H�[�}�b�g�̃T�C�Y���擾
	sizeFVF = D3DXGetFVFVertexSize(m_Xmodel.pMesh->GetFVF());
	// �}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)m_Xmodel.pBuffMat->GetBufferPointer();
	m_Xmodel.ppTexture = new LPDIRECT3DTEXTURE9[m_Xmodel.nNumMat];
	// �}�e���A�������擾
	for (int nCntMat = 0; nCntMat < (int)m_Xmodel.nNumMat; nCntMat++, pMat++)
	{
		m_Xmodel.ppTexture[nCntMat] = NULL;
		D3DXCreateTextureFromFile(pDevice, pMat->pTextureFilename, &m_Xmodel.ppTexture[nCntMat]);
	}

	// ���_�o�b�t�@�����b�N
	m_Xmodel.pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVertexBuffer);
	// ���_�����J��Ԃ�
	for (int nCntVtx = 0; nCntVtx < nNumVertex; nCntVtx++)
	{
		// �Ԓn����D3DXVE
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVertexBuffer;

		// -----------���ׂĂ̒��_�����r���čŏ��l�E�ő�l�𔲂��o��---------- //
		// x //
		// �ŏ��l
		if (m_Xmodel.vtxMinMaterials.x > vtx.x)
		{
			m_Xmodel.vtxMinMaterials.x = vtx.x;
		}
		// �ő�l
		if (m_Xmodel.vtxMaxMaterials.x < vtx.x)
		{
			m_Xmodel.vtxMaxMaterials.x = vtx.x;
		}
		// y //
		// �ŏ��l
		if (m_Xmodel.vtxMinMaterials.y > vtx.y)
		{
			m_Xmodel.vtxMinMaterials.y = vtx.y;
		}
		// �ő�l
		if (m_Xmodel.vtxMaxMaterials.y < vtx.y)
		{
			m_Xmodel.vtxMaxMaterials.y = vtx.y;
		}
		// z //
		// �ŏ��l
		if (m_Xmodel.vtxMinMaterials.z > vtx.z)
		{
			m_Xmodel.vtxMinMaterials.z = vtx.z;
		}
		// �ő�l
		if (m_Xmodel.vtxMaxMaterials.z < vtx.z)
		{
			m_Xmodel.vtxMaxMaterials.z = vtx.z;
		}
		// �T�C�Y�|�C���^��i�߂�
		pVertexBuffer += sizeFVF;
	}
	// �A�����b�N
	m_Xmodel.pMesh->UnlockVertexBuffer();
	// �T�C�Y
	m_Xmodel.size = m_Xmodel.vtxMaxMaterials - m_Xmodel.vtxMinMaterials;
	return S_OK;
}


// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���f���̏��̓ǂݍ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CModel_info::Unload(void)
{
	// ���b�V���̊J��
	if (m_Xmodel.pMesh != NULL)
	{
		m_Xmodel.pMesh->Release();
		m_Xmodel.pMesh = NULL;
	}

	// �}�e���A���̊J��
	if (m_Xmodel.pBuffMat != NULL)
	{
		m_Xmodel.pBuffMat->Release();
		m_Xmodel.pBuffMat = NULL;
	}

	// �e�N�X�`���[�̊J��
	for (int nCntTex = 0; nCntTex < (int)m_Xmodel.nNumMat; nCntTex++)
	{
		if (m_Xmodel.ppTexture[nCntTex] != NULL)
		{
			m_Xmodel.ppTexture[nCntTex]->Release();
			m_Xmodel.ppTexture[nCntTex] = NULL;
		}
	}
	// �e�N�X�`���[�̔j��
	if (m_Xmodel.ppTexture != NULL)
	{
		delete[] m_Xmodel.ppTexture;
		m_Xmodel.ppTexture = NULL;
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ���f���̃e�L�X�g�f�[�^�̓ǂݍ���
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void  CModel_info::TextLoad(
	MODEL_ALL *pmodelAll,				
	int const &nMaxMotion,
	int &nMaxkey,
	const char * file_name
)
{
	// �t�@�C���|�C���g
	FILE *pFile;

	// �ϐ��錾
	int	nCntModel = 0;		// ���f���J�E���g
	int nCntFile = 0;		// �t�@�C���J�E���g
	int	nCntMotion = 0;		// ���[�V�����J�E���g
	int	nCntKeySet = 0;		// �t���[����
	int	nCntKey = 0;		// ���f����
	char cRaedText[128];	// �����Ƃ��ēǂݎ��p
	char cHeadText[128];	// ��r����悤
	char cDie[128];			// �s�K�v�ȕ���
	int nCameraType = 0;	// �J�����^�C�v

	// ���[�V�����̐���
	pmodelAll->pMotion = new MOTION[nMaxMotion];

	// �t�@�C���J
	pFile = fopen(file_name, "r");

	// �J����
	if (pFile != NULL)
	{
		// �X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "SCRIPT") != 0)
		{
			fgets(cRaedText, sizeof(cRaedText), pFile);	// �ꕶ��ǂݍ���
			sscanf(cRaedText, "%s", &cHeadText);		// ��r�p�e�N�X�g�ɕ�������
		}

		// �G���h�X�N���v�g������܂Ń��[�v
		while (strcmp(cHeadText, "END_SCRIPT") != 0)
		{
			// ������
			cHeadText[0] = '\0';
			fgets(cRaedText, sizeof(cRaedText), pFile);
			sscanf(cRaedText, "%s", &cHeadText);
			// ���f������������
			if (strcmp(cHeadText, "NUM_MODEL") == 0)
			{
				sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
					&nMaxkey);
				pmodelAll->pModel_offset = new MODEL_OFFSET[nMaxkey];
			}
			// ���f���t�@�C������������
			else if (strcmp(cHeadText, "MODEL_FILENAME") == 0)
			{
				sscanf(cRaedText, "%s %s %s", &cDie, &cDie,
					&pmodelAll->pModel_offset[nCntFile].cXfile);
				nCntFile++;
			}
			// �p�[�c�Z�b�g��������
			else if (strcmp(cHeadText, "PARTSSET") == 0)
			{
				// �G���h�p�[�c�Z�b�g������܂Ń��[�v
				while (strcmp(cHeadText, "END_PARTSSET") != 0)
				{
					// ������
					cHeadText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);
					// �e���ǂݍ���
					if (strcmp(cHeadText, "PARENT") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].nParent);
					}
					// �ʒu���ǂݍ���
					else if (strcmp(cHeadText, "POS") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f",
							&cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].pos.x,
							&pmodelAll->pModel_offset[nCntModel].pos.y,
							&pmodelAll->pModel_offset[nCntModel].pos.z);
					}
					// ��]���ǂݍ���
					else if (strcmp(cHeadText, "ROT") == 0)
					{
						sscanf(cRaedText, "%s %s %f %f %f", &cDie, &cDie,
							&pmodelAll->pModel_offset[nCntModel].rot.x,
							&pmodelAll->pModel_offset[nCntModel].rot.y,
							&pmodelAll->pModel_offset[nCntModel].rot.z);
					}
				}
				// ���f���̍X�V
				nCntModel++;
			}
			// ���[�V�����Z�b�g��������
			else if (strcmp(cHeadText, "MOTIONSET") == 0)
			{
				// �J�����^�C�v�̏�����
				nCameraType = 0;	// �J�����^�C�v
				// ���f���̏�����
				nCntKeySet = 0;
				// �G���h���[�V�����Z�b�g������܂Ń��[�v
				while (strcmp(cHeadText, "END_MOTIONSET") != 0)
				{
					// ������
					cHeadText[0] = '\0';
					fgets(cRaedText, sizeof(cRaedText), pFile);
					sscanf(cRaedText, "%s", &cHeadText);

					// ���[�v���邩�ǂ������ǂݍ���
					if (strcmp(cHeadText, "LOOP") == 0)
					{
						sscanf(cRaedText, "%s %s %d",
							&cDie, &cDie,
							&pmodelAll->pMotion[nCntMotion].nLoop);
					}
					// �L�[�����ǂݍ���
					else if (strcmp(cHeadText, "NUM_KEY") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &pmodelAll->pMotion[nCntMotion].nNumKey);
						// ���[�V����������
						pmodelAll->pMotion[nCntMotion].KeyInfo = new KEY_INFO[pmodelAll->pMotion[nCntMotion].nNumKey];
					}
					// �L�[�����ǂݍ���
					else if (strcmp(cHeadText, "CAMERAINFO") == 0)
					{
						sscanf(cRaedText, "%s %s %d", &cDie, &cDie, &nCameraType);
					}
					// �����蔻����ǂݍ���
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

					// �_���[�W���ǂݍ���
					else if (strcmp(cHeadText, "COLLISION_DAMAGE") == 0)
					{
						sscanf(cRaedText, "%s %s %d",
							&cDie, &cDie,
							&pmodelAll->pMotion[nCntMotion].Collision_Damage);
					}
					// �L�[���ǂݍ���
					else if (strcmp(cHeadText, "KEYSET") == 0)
					{
						// �L�[�̏�����
						nCntKey = 0;
						// ���f��������
						pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key = new KEY[nMaxkey];
						// �J�����^�C�v�����ݒ�
						pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nCameraType = nCameraType;
						// �G���h�L�[�Z�b�g������܂Ń��[�v
						while (strcmp(cHeadText, "END_KEYSET") != 0)
						{
							// ������
							cHeadText[0] = '\0';
							fgets(cRaedText, sizeof(cRaedText), pFile);
							sscanf(cRaedText, "%s", &cHeadText);

							// �U�������蔻��̕p�x���ǂݍ���
							if (strcmp(cHeadText, "NUMCOLLISION") == 0)
							{
								// �t���[����
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nNumCollision);
							}

							// �J�����^�C�v���ǂݍ���
							else if (strcmp(cHeadText, "CAMERATYPE") == 0)
							{
								// �t���[����
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nCameraType);
								// �J�����^�C�v��ۑ��p�ɕۑ�
								nCameraType = pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nCameraType;
							}

							// ������ѕ����ǂݍ���
							else if (strcmp(cHeadText, "BLUST") == 0)
							{
								// �t���[����
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nBlust);
							}

							// �t���[�����ǂݍ���
							else if (strcmp(cHeadText, "FRAME") == 0)
							{
								// �t���[����
								sscanf(cRaedText, "%s %s %d",
									&cDie, &cDie,
									&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nFrame);
								// ���[�V�����S�̂̃t���[�������Z
								pmodelAll->pMotion[nCntMotion].nAllFrame +=
									pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nFrame;
								// �����蔻��̉񐔂��Ȃ��ꍇ�֐��𔲂���
								if (pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nNumCollision > 0)
								{
									// �U������p�x
									pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nMaxCollisiontime =
										pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nFrame /
										pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].nNumCollision;
								}

							}

							// �L�[���ǂݍ���
							else if (strcmp(cHeadText, "KEY") == 0)
							{
								// �G���h�L�[������܂Ń��[�v
								while (strcmp(cHeadText, "END_KEY") != 0)
								{
									fgets(cRaedText, sizeof(cRaedText), pFile);
									sscanf(cRaedText, "%s", &cHeadText);

									// �ʒu���ǂݍ���
									if (strcmp(cHeadText, "POS") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].pos.x,
											&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].pos.y,
											&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].pos.z);

										// �p�[�c�̈ʒu����������
										pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].pos += pmodelAll->pModel_offset[nCntKey].pos;
									}

									// ��]���ǂݍ���
									else if (strcmp(cHeadText, "ROT") == 0)
									{
										sscanf(cRaedText, "%s %s %f %f %f",
											&cDie, &cDie,
											&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].rot.x,
											&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].rot.y,
											&pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].rot.z);
										// �p�[�c�̈ʒu����������
										pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeySet].Key[nCntKey].rot += pmodelAll->pModel_offset[nCntKey].rot;
									}
								}
								// �J�E���g�L�[
								nCntKey++;
							}
						}
						// �J�E���g���f��
						nCntKeySet++;
					}
				}
				// �J�E���g�v���C���[
				nCntMotion++;
			}
		}
		// �t�@�C����
		fclose(pFile);
	}

	// �J���Ȃ�
	else
	{
		MessageBox(NULL, "�v���C���[�f�[�^�̓ǂݍ��݂Ɏ��s���܂���", "�x���I", MB_ICONWARNING);
	}
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// �ǂݍ��񂾃e�L�X�g�f�[�^�̔j��
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CModel_info::TextUnload(
	MODEL_ALL * pmodelAll,	// ���f���E���[�V�����̏��
	int const & nMaxMotion	// ���[�V������
)
{
	// �I�t�Z�b�g���̔j��
	delete[] pmodelAll->pModel_offset;
	pmodelAll->pModel_offset = NULL;
	// ���[�V�������̔j��
	for (int nCntMotion = 0; nCntMotion < nMaxMotion; nCntMotion++)
	{
		for (int nCntKeyInfo = 0; nCntKeyInfo < pmodelAll->pMotion[nCntMotion].nNumKey; nCntKeyInfo++)
		{
			// �L�[�̔j��
			delete[] pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeyInfo].Key;
			pmodelAll->pMotion[nCntMotion].KeyInfo[nCntKeyInfo].Key = NULL;
		}
		//�L�[���̔j��
		delete[] pmodelAll->pMotion[nCntMotion].KeyInfo;
		pmodelAll->pMotion[nCntMotion].KeyInfo = NULL;
	}
	// ���[�V�����̔j��
	delete[] pmodelAll->pMotion;
	pmodelAll->pMotion = NULL;
	// ���f�����S�̂̔j��
	delete pmodelAll;
	pmodelAll = NULL;
}

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// X���f�����擾
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
XMODEL & CModel_info::GetXModel(void)
{
	return m_Xmodel;
}
