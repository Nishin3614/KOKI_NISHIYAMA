// ------------------------------------------------------------------------------------------
//
// �O�Տ��� [trajectory.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#include "trajectory.h"

#include "player.h"

#include "input.h"

#include "giraffe.h"

#include "king_rion.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define TRAJECTORY_PHONT "data/TEXTURE/soil.jpg"	// �O�Ղ̎ʐ^
#define TRAJECTORY_MAX (256)						// �O�Ղ̍ő吔
#define TRAJECTORY_TEXMAX (1)					// �e�N�X�`���[��

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// �O���[�o���ϐ�
// ------------------------------------------------------------------------------------------
TRAJECTORY g_trajectory[TRAJECTORY_MAX];								// �O��
LPDIRECT3DTEXTURE9		g_apTextureTrajectory[TRAJECTORY_TEXMAX] = {};	// ���_�e�N�X�`���[�̃|�C���^

// ------------------------------------------------------------------------------------------
// ����������
// ------------------------------------------------------------------------------------------
void InitTrajectory(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	int nCntTrajectory;							// �O�Ղ̃J�E���g
	int nCntMeshObit;							// �O�Ճ|�C���g�̃J�E���g

												// �O�ՃJ�E���g�̏�����
	for (nCntTrajectory = 0; nCntTrajectory < TRAJECTORY_MAX; nCntTrajectory++)
	{
		// �ʒu�E�I�t�Z�b�g�E��]�̏����ݒ�
		g_trajectory[nCntTrajectory].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_trajectory[nCntTrajectory].ofset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_trajectory[nCntTrajectory].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �t���[���֌W�̏����ݒ�
		g_trajectory[nCntTrajectory].nCntFram = 0;
		g_trajectory[nCntTrajectory].nStartFram = 0;
		g_trajectory[nCntTrajectory].nEndFram = 0;

		// �^�C�v�̏����ݒ�
		g_trajectory[nCntTrajectory].nType = 0;

		// �o�b�t�@�̏����ݒ�
		g_trajectory[nCntTrajectory].pVtxBuffTrajectory = NULL;

		// �O�Ճ|�C���g�̏�����
		for (nCntMeshObit = 0;
			nCntMeshObit < MAX_MESHOBIT;
			nCntMeshObit++)
		{
			g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0] = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
			g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1] = D3DXVECTOR3(0.0f, -10.0f, 0.0f);
		}

		// �ő�O�Ր��̏�����
		g_trajectory[nCntTrajectory].nMaxMeshObit = 0;

		// �����_���E�C���f�b�N�X�E�O�Ղ̏����ݒ�(�v�Z)
		g_trajectory[nCntTrajectory].nNumberVertexMeshObit =
			g_trajectory[nCntTrajectory].nMaxMeshObit * 2;

		// �r���{�[�h��
		g_trajectory[nCntTrajectory].nNumPolygon =
			g_trajectory[nCntTrajectory].nMaxMeshObit * 2 - 2;

		// �g�p��ԁE�\����Ԃ̏����ݒ�
		g_trajectory[nCntTrajectory].bUse = false;
		g_trajectory[nCntTrajectory].bDisp = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TRAJECTORY_PHONT, &g_apTextureTrajectory[0]);
}

// ------------------------------------------------------------------------------------------
// �I������
// ------------------------------------------------------------------------------------------
void UninitTrajectory(void)
{
	for (int nCntTrajectory = 0; nCntTrajectory < TRAJECTORY_MAX; nCntTrajectory++)
	{
		// �e�N�X�`���̊J��
		if (g_apTextureTrajectory[0] != NULL)
		{
			g_apTextureTrajectory[0]->Release();
			g_apTextureTrajectory[0] = NULL;
		}

		// ���_�o�b�t�@�̊J��
		if (g_trajectory[nCntTrajectory].pVtxBuffTrajectory != NULL)
		{
			g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Release();
			g_trajectory[nCntTrajectory].pVtxBuffTrajectory = NULL;
		}
	}
}

// ------------------------------------------------------------------------------------------
// �X�V����
// ------------------------------------------------------------------------------------------
void UpdateTrajectory(void)
{
	// �ϐ��錾
	// ���_���̐ݒ�
	VERTEX_3D *pVtx;									// ���_���ւ̃|�C���g
	int nCntTrajectory;
	D3DXVECTOR3 obit_ofset;								// �O�Ղ̃I�t�Z�b�g

	int nCntMeshObit = 0;								// �O�Ճ|�C���g�̃J�E���g1
	int nCntMeshObit2 = 0;								// �O�Ճ|�C���g�̃J�E���g2

														// �v���C���[���
	MODEL *pPlayerModel;								// �v���C���[�̃��f�����
	D3DXVECTOR3 pPlayer_Pos = GetPlayer_Pos();			// �v���C���[�̈ʒu���擾
	D3DXVECTOR3 pPlayer_PosOld = GetPlayer_PosOld();	// �v���C���[�̑O��̈ʒu���擾
	D3DXVECTOR3 pPlayer_Rot = GetPlayer_Rot();			// �v���C���[�̉�]���擾

														// �L�������
	GIRAFFE *pGiraffe;									// �L�������

	KING_RION *pKing_rion;								// �S�b�̉����

	// �O�ՃJ�E���g
	for (nCntTrajectory = 0; nCntTrajectory < TRAJECTORY_MAX; nCntTrajectory++)
	{
		// �\�����
		if (g_trajectory[nCntTrajectory].bDisp == true)
		{
			// �t���[������UP
			g_trajectory[nCntTrajectory].nCntFram++;

			// �t���[�������J�n�t���[�����ȏ�ɂȂ�����
			if (g_trajectory[nCntTrajectory].nCntFram >=
				g_trajectory[nCntTrajectory].nStartFram)
			{
				// �O�Ղ̐��̐�
				nCntMeshObit2 = g_trajectory[nCntTrajectory].nMaxMeshObit - 1;

				// �L�����N�^�[�^�C�v
				switch (g_trajectory[nCntTrajectory].nType)
				{
				case TRAJECTORYTYPE_PLAYER:
					// �O��̈��p���̂��߂̃��[�v
					for (nCntMeshObit = nCntMeshObit2 - 1;
						nCntMeshObit >= 0;
						nCntMeshObit--, nCntMeshObit2--)
					{
						// 1�ڂ̋O�Ղ̃|�C���g
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit2].pos[0] =
							g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0];

						// 2�ڂ̋O�Ղ̃|�C���g
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit2].pos[1] =
							g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1];
					}

					// �v���C���[�̃��f�����̎擾
					pPlayerModel = GetPlayerModel();

					// �v���C���[�̃��f�����X�V
					pPlayerModel += g_trajectory[nCntTrajectory].nParts;

					// �O�Ղ̃I�t�Z�b�g���p�x�Őݒ�
					obit_ofset.x = sinf(pPlayer_Rot.y) * g_trajectory[nCntTrajectory].ofset.z;
					obit_ofset.z = cosf(pPlayer_Rot.y) * g_trajectory[nCntTrajectory].ofset.z;

					// �|�W�V�����̊l��
					g_trajectory[nCntTrajectory].MeshObit[0].pos[0] =
						D3DXVECTOR3(pPlayerModel->mtxWorldModel._41,
							pPlayerModel->mtxWorldModel._42,
							pPlayerModel->mtxWorldModel._43);

					// �|�W�V�����̊l��
					g_trajectory[nCntTrajectory].MeshObit[0].pos[1] =
						D3DXVECTOR3(
							pPlayerModel->mtxWorldModel._41 +
							obit_ofset.x,
							pPlayerModel->mtxWorldModel._42,
							pPlayerModel->mtxWorldModel._43 +
							obit_ofset.z);


					// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
					g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Lock(0, 0, (void **)&pVtx, 0);

					// �O�Ճ|�C���g�J�E���g
					for (nCntMeshObit = 0;
						nCntMeshObit < g_trajectory[nCntTrajectory].nMaxMeshObit;
						nCntMeshObit++)
					{
						// ���_���W�̐ݒ�
						// 1
						pVtx[0].pos =
							D3DXVECTOR3(
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].x,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].y,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].z);

						// 2
						pVtx[1].pos =
							D3DXVECTOR3(
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].x,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].y,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].z);

						// ���_���̃|�C���g���킹
						pVtx += 2;
					}

					// �A�����b�N
					g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Unlock();

					// �v���C���[�̃��f�����X�V�O�ɖ߂�
					pPlayerModel -= g_trajectory[nCntTrajectory].nParts;


					break;

				case TRAJECTORYTYPE_GIRAFFE:
					// �O��̈��p���̂��߂̃��[�v
					for (nCntMeshObit = nCntMeshObit2 - 1;
						nCntMeshObit >= 0;
						nCntMeshObit--, nCntMeshObit2--)
					{
						// 1�ڂ̋O�Ղ̃|�C���g
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit2].pos[0] =
							g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0];

						// 2�ڂ̋O�Ղ̃|�C���g
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit2].pos[1] =
							g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1];
					}

					// �v���C���[�̃��f�����̎擾
					pGiraffe = GetGiraffe();

					// �L�����̔ԍ��X�V
					pGiraffe += g_trajectory[nCntTrajectory].nCntObject;

					// �O�Ղ̃I�t�Z�b�g���p�x�Őݒ�
					obit_ofset.x = sinf(pGiraffe->rot.y) * g_trajectory[nCntTrajectory].ofset.z;
					obit_ofset.z = cosf(pGiraffe->rot.y) * g_trajectory[nCntTrajectory].ofset.z;

					// �|�W�V�����̊l��
					g_trajectory[nCntTrajectory].MeshObit[0].pos[0] =
						D3DXVECTOR3(pGiraffe->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._41,
							pGiraffe->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._42,
							pGiraffe->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._43);

					// �|�W�V�����̊l��
					g_trajectory[nCntTrajectory].MeshObit[0].pos[1] =
						D3DXVECTOR3(
							pGiraffe->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._41 +
							obit_ofset.x,
							pGiraffe->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._42,
							pGiraffe->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._43 +
							obit_ofset.z);


					// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
					g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Lock(0, 0, (void **)&pVtx, 0);

					// �O�Ճ|�C���g�J�E���g
					for (nCntMeshObit = 0;
						nCntMeshObit < g_trajectory[nCntTrajectory].nMaxMeshObit;
						nCntMeshObit++)
					{
						// ���_���W�̐ݒ�
						// 1
						pVtx[0].pos =
							D3DXVECTOR3(
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].x,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].y,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].z);

						// 2
						pVtx[1].pos =
							D3DXVECTOR3(
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].x,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].y,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].z) +
							g_trajectory[nCntTrajectory].ofset;

						// ���_���̃|�C���g���킹
						pVtx += 2;
					}

					// �A�����b�N
					g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Unlock();

					// �L�������X�V�O�ɖ߂�
					pGiraffe -= g_trajectory[nCntTrajectory].nCntObject;

					break;

				case TRAJECTORYTYPE_KING_RION:
					// �O��̈��p���̂��߂̃��[�v
					for (nCntMeshObit = nCntMeshObit2 - 1;
						nCntMeshObit >= 0;
						nCntMeshObit--, nCntMeshObit2--)
					{
						// 1�ڂ̋O�Ղ̃|�C���g
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit2].pos[0] =
							g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0];

						// 2�ڂ̋O�Ղ̃|�C���g
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit2].pos[1] =
							g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1];
					}

					// �v���C���[�̃��f�����̎擾
					pKing_rion = GetKing_Rion();

					// �L�����̔ԍ��X�V
					pKing_rion += g_trajectory[nCntTrajectory].nCntObject;

					// �O�Ղ̃I�t�Z�b�g���p�x�Őݒ�
					obit_ofset.x = sinf(pKing_rion->rot.y) * g_trajectory[nCntTrajectory].ofset.z;
					obit_ofset.z = cosf(pKing_rion->rot.y) * g_trajectory[nCntTrajectory].ofset.z;

					// �|�W�V�����̊l��
					g_trajectory[nCntTrajectory].MeshObit[0].pos[0] =
						D3DXVECTOR3(pKing_rion->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._41,
							pKing_rion->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._42,
							pKing_rion->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._43);

					// �|�W�V�����̊l��
					g_trajectory[nCntTrajectory].MeshObit[0].pos[1] =
						D3DXVECTOR3(
							pKing_rion->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._41 +
							obit_ofset.x,
							pKing_rion->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._42,
							pKing_rion->model[g_trajectory[nCntTrajectory].nParts].mtxWorldModel._43 +
							obit_ofset.z);


					// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
					g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Lock(0, 0, (void **)&pVtx, 0);

					// �O�Ճ|�C���g�J�E���g
					for (nCntMeshObit = 0;
						nCntMeshObit < g_trajectory[nCntTrajectory].nMaxMeshObit;
						nCntMeshObit++)
					{
						// ���_���W�̐ݒ�
						// 1
						pVtx[0].pos =
							D3DXVECTOR3(
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].x,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].y,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].z);

						// 2
						pVtx[1].pos =
							D3DXVECTOR3(
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].x,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].y,
								g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].z) +
							g_trajectory[nCntTrajectory].ofset;

						// ���_���̃|�C���g���킹
						pVtx += 2;
					}

					// �A�����b�N
					g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Unlock();

					// �L�������X�V�O�ɖ߂�
					pKing_rion -= g_trajectory[nCntTrajectory].nCntObject;

					break;

				default:
					break;
				}

				// �t���[�������I���J�E���g�ȏ�ɂȂ�����
				if (g_trajectory[nCntTrajectory].nCntFram >=
					g_trajectory[nCntTrajectory].nEndFram)
				{
					// ������
					g_trajectory[nCntTrajectory].nCntFram = 0;
					g_trajectory[nCntTrajectory].bDisp = false;
				}
			}
		}
	}
}

// ------------------------------------------------------------------------------------------
// �`�揈��
// ------------------------------------------------------------------------------------------
void DrawTrajectory(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

												// ��]�E�ʒu�̌v�Z�p�ϐ�
	D3DXMATRIX mtxRot, mtxTrans;

	int nCntTrajectory;

	// ���ʂ�`��
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �����_�[�X�e�[�g(���Z��������)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (nCntTrajectory = 0; nCntTrajectory < TRAJECTORY_MAX; nCntTrajectory++)
	{
		if (g_trajectory[nCntTrajectory].bDisp == true)
		{
			// ���[���h�}�g���b�N�X�̏�����(�P�ʍs��ɂ��邽��)
			D3DXMatrixIdentity(&g_trajectory[nCntTrajectory].mtxWorld);

			// ��]�𔽉f //
			// ���[�s�b�`���[���ɂ���]�s��쐬(y,x,z)
			D3DXMatrixRotationYawPitchRoll(&mtxRot,					// �����̓��ꕨ
				g_trajectory[nCntTrajectory].rot.y, g_trajectory[nCntTrajectory].rot.x, g_trajectory[nCntTrajectory].rot.z);	// ���ꂼ��̉�]��

																																// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:��]�s��)
			D3DXMatrixMultiply(&g_trajectory[nCntTrajectory].mtxWorld,	// 1
				&g_trajectory[nCntTrajectory].mtxWorld,					// 2
				&mtxRot);							// 3


													// �ʒu�𔽉f //
													// ���s�ړ��s��쐬(�I�t�Z�b�g)
			D3DXMatrixTranslation(&mtxTrans,						// �����̓��ꕨ
				g_trajectory[nCntTrajectory].pos.x,
				g_trajectory[nCntTrajectory].pos.y,
				g_trajectory[nCntTrajectory].pos.z);	// ���ꂼ��̈ړ���

														// �s��̐�(1:���[���h�s�� = 2:���[���h�s�� * 3:�ړ��s��)
			D3DXMatrixMultiply(&g_trajectory[nCntTrajectory].mtxWorld,	// 1
				&g_trajectory[nCntTrajectory].mtxWorld,					// 2
				&mtxTrans);							// 3

													// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_trajectory[nCntTrajectory].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_trajectory[nCntTrajectory].pVtxBuffTrajectory, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureTrajectory[0]);

			// �O�Ղ̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, g_trajectory[nCntTrajectory].nNumPolygon);
		}
	}

	// �ʏ�u�����h
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ����(�����)�ŃJ�����O����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

// ------------------------------------------------------------------------------------------
// �O�Ղ̍쐬
// ------------------------------------------------------------------------------------------
int SetTrajectory(
	D3DXVECTOR3 ofset,
	int nParts,
	int MaxMeshObit,
	int StartFram,
	int EndFram,
	int	Type)
{
	// �J�E���g�O��
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	VERTEX_3D *pVtx;
	int	nCntTrajectory = 0;
	int	nCntMeshObit;

	// �O�ՃJ�E���g
	for (nCntTrajectory = 0; nCntTrajectory < TRAJECTORY_MAX; nCntTrajectory++)
	{
		if (g_trajectory[nCntTrajectory].bUse == false)
		{
			// �I�t�Z�b�g
			g_trajectory[nCntTrajectory].ofset = ofset;

			// �p�[�c���
			g_trajectory[nCntTrajectory].nParts = nParts;

			// �ő�O�Ր�
			g_trajectory[nCntTrajectory].nMaxMeshObit = MaxMeshObit;

			// �J�n�t���[��
			g_trajectory[nCntTrajectory].nStartFram = StartFram;

			// �I���t���[��
			g_trajectory[nCntTrajectory].nEndFram = EndFram;

			// �^�C�v
			g_trajectory[nCntTrajectory].nType = Type;

			// �����_���E�C���f�b�N�X�E�O�Ղ̏����ݒ�(�v�Z)
			g_trajectory[nCntTrajectory].nNumberVertexMeshObit =
				g_trajectory[nCntTrajectory].nMaxMeshObit * 2;

			// �r���{�[�h��
			g_trajectory[nCntTrajectory].nNumPolygon =
				g_trajectory[nCntTrajectory].nMaxMeshObit * 2 - 2;

			// ���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(
				sizeof(VERTEX_3D) * g_trajectory[nCntTrajectory].nNumberVertexMeshObit,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_trajectory[nCntTrajectory].pVtxBuffTrajectory,
				NULL);

			// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
			g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Lock(0, 0, (void **)&pVtx, 0);

			// �O�Ճ|�C���g�J�E���g
			for (nCntMeshObit = 0;
				nCntMeshObit < g_trajectory[nCntTrajectory].nMaxMeshObit;
				nCntMeshObit++)
			{
				// ���_���W�̐ݒ�
				// 1
				pVtx[0].pos =
					D3DXVECTOR3(
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].x,
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].y,
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[0].z);

				// �@���x�N�g���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �F�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				// �e�N�X�`���[�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

				// 2
				pVtx[1].pos =
					D3DXVECTOR3(
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].x,
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].y,
						g_trajectory[nCntTrajectory].MeshObit[nCntMeshObit].pos[1].z) +
					g_trajectory[nCntTrajectory].ofset;

				// �@���x�N�g���̐ݒ�
				pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				// �F�̐ݒ�
				pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

				// �e�N�X�`���[�̐ݒ�
				pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);

				pVtx += 2;
			}

			// �A�����b�N
			g_trajectory[nCntTrajectory].pVtxBuffTrajectory->Unlock();

			// �g�p���
			g_trajectory[nCntTrajectory].bUse = true;

			// ���[�v���甲���o��
			break;
		}
	}

	return nCntTrajectory;
}

// ------------------------------------------------------------------------------------------
// �O�Ղ̔���
// ------------------------------------------------------------------------------------------
void OccurrenceTrajectory(int IdTrajectory, int nCnt)
{
	// �v���C���[�̃��f�����
	MODEL *pPlayerModel;

	// �L�����̏��
	GIRAFFE *pGiraffe;

	// �S�b�̉��̏��
	MODEL *pKing_Rion_Model;

	// ���_���
	VERTEX_3D *pVtx;

	// �L�����N�^�[�ԍ�
	g_trajectory[IdTrajectory].nCntObject = nCnt;

	// ������
	g_trajectory[IdTrajectory].nCntFram = 0;
	g_trajectory[IdTrajectory].bDisp = false;

	switch (g_trajectory[IdTrajectory].nType)
	{
	case TRAJECTORYTYPE_PLAYER:
		// �v���C���[�̃��f�����̎擾
		pPlayerModel = GetPlayerModel();

		// �v���C���[�̃��f�����X�V
		pPlayerModel += g_trajectory[IdTrajectory].nParts;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
		g_trajectory[IdTrajectory].pVtxBuffTrajectory->Lock(0, 0, (void **)&pVtx, 0);

		// �O�Ճ|�C���g�J�E���g
		for (int nCntMeshObit = 0;
			nCntMeshObit < g_trajectory[IdTrajectory].nMaxMeshObit;
			nCntMeshObit++)
		{
			// �|�W�V�����̊l��
			g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0] =
				D3DXVECTOR3(pPlayerModel->mtxWorldModel._41,
					pPlayerModel->mtxWorldModel._42,
					pPlayerModel->mtxWorldModel._43);


			// �|�W�V�����̊l��
			g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1] =
				D3DXVECTOR3(pPlayerModel->mtxWorldModel._41,
					pPlayerModel->mtxWorldModel._42,
					pPlayerModel->mtxWorldModel._43);


			// ���_���W�̐ݒ�
			// 1
			pVtx[0].pos =
				D3DXVECTOR3(
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].x,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].y,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].z);

			// 2
			pVtx[1].pos =
				D3DXVECTOR3(
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].x,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].y,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].z) +
				g_trajectory[IdTrajectory].ofset;

			// ���_���̃|�C���g���킹
			pVtx += 2;
		}
		break;
	case TRAJECTORYTYPE_GIRAFFE:
		// �L�������̎擾
		pGiraffe = GetGiraffe();

		// �L�����̏��X�V
		pGiraffe += g_trajectory[IdTrajectory].nCntObject;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
		g_trajectory[IdTrajectory].pVtxBuffTrajectory->Lock(0, 0, (void **)&pVtx, 0);

		// �O�Ճ|�C���g�J�E���g
		for (int nCntMeshObit = 0;
			nCntMeshObit < g_trajectory[IdTrajectory].nMaxMeshObit;
			nCntMeshObit++)
		{
			// �|�W�V�����̊l��
			g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0] =
				D3DXVECTOR3(pGiraffe->model[g_trajectory[IdTrajectory].nParts].mtxWorldModel._41,
					pGiraffe->model[g_trajectory[IdTrajectory].nParts].mtxWorldModel._42,
					pGiraffe->model[g_trajectory[IdTrajectory].nParts].mtxWorldModel._43);


			// �|�W�V�����̊l��
			g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1] =
				D3DXVECTOR3(pGiraffe->model[g_trajectory[IdTrajectory].nParts].mtxWorldModel._41,
					pGiraffe->model[g_trajectory[IdTrajectory].nParts].mtxWorldModel._42,
					pGiraffe->model[g_trajectory[IdTrajectory].nParts].mtxWorldModel._43);


			// ���_���W�̐ݒ�
			// 1
			pVtx[0].pos =
				D3DXVECTOR3(
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].x,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].y,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].z);

			// 2
			pVtx[1].pos =
				D3DXVECTOR3(
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].x,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].y,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].z) +
				g_trajectory[IdTrajectory].ofset;

			// ���_���̃|�C���g���킹
			pVtx += 2;
		}
		break;

	case TRAJECTORYTYPE_KING_RION:
		// �S�b�̉��̃��f�����̎擾
		pKing_Rion_Model = GetKing_Rion_Model(nCnt);

		// �S�b�̉��̃��f���X�V
		pKing_Rion_Model += g_trajectory[IdTrajectory].nParts;

		// ���_�f�[�^�͈̔͂����b�N���A���_�o�b�t�@�ւ̃|�C���^
		g_trajectory[IdTrajectory].pVtxBuffTrajectory->Lock(0, 0, (void **)&pVtx, 0);

		// �O�Ճ|�C���g�J�E���g
		for (int nCntMeshObit = 0;
			nCntMeshObit < g_trajectory[IdTrajectory].nMaxMeshObit;
			nCntMeshObit++)
		{
			// �|�W�V�����̊l��
			g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0] =
				D3DXVECTOR3(
					pKing_Rion_Model->mtxWorldModel._41,
					pKing_Rion_Model->mtxWorldModel._42,
					pKing_Rion_Model->mtxWorldModel._43);


			// �|�W�V�����̊l��
			g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1] =
				D3DXVECTOR3(
					pKing_Rion_Model->mtxWorldModel._41,
					pKing_Rion_Model->mtxWorldModel._42,
					pKing_Rion_Model->mtxWorldModel._43);

			// ���_���W�̐ݒ�
			// 1
			pVtx[0].pos =
				D3DXVECTOR3(
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].x,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].y,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[0].z);

			// 2
			pVtx[1].pos =
				D3DXVECTOR3(
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].x,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].y,
					g_trajectory[IdTrajectory].MeshObit[nCntMeshObit].pos[1].z) +
				g_trajectory[IdTrajectory].ofset;

			// ���_���̃|�C���g���킹
			pVtx += 2;
		}
		break;
	}
	g_trajectory[IdTrajectory].bDisp = true;
}

// ------------------------------------------------------------------------------------------
// �O�Ղ̏������
// ------------------------------------------------------------------------------------------
void DeleteTrajectory(int nIdTrajectory)
{
	g_trajectory[nIdTrajectory].bUse = false;
	g_trajectory[nIdTrajectory].bDisp = false;
}

// ------------------------------------------------------------------------------------------
// �O�Տ��
// ------------------------------------------------------------------------------------------
TRAJECTORY *GetTrajectory(void)
{
	return &g_trajectory[0];
}