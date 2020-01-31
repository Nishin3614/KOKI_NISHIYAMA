//=============================================================================
//
// �V���h�E�}�b�s���O���� [player.cpp]
// Author : KOKI NISHIYAMA
//
//=============================================================================
#include "shadow_mapping.h"
#include "light.h"

//=============================================================================
// �}�N����`
//=============================================================================

// ----------------------------------------
// �e�̕`��(���f��)����
// ----------------------------------------
void CShadowmapping::Draw(
	LPDIRECT3DDEVICE9	&pDevice,	// �f�o�C�X���
	XMODEL				&Xmodel,	// ���f�����
	D3DXMATRIX			&mtx		// �}�g���b�N�X���
)
{
	// �ϐ��錾
	D3DMATERIAL9		matDef;					// �}�e���A���f�[�^�̃|�C���^
	D3DXMATRIX			mtxShadow;				// �V���h�E�}�g���b�N�X
	D3DXPLANE			planeField;				// ����
	D3DXVECTOR4			vecLight;				// ���C�g�̕���
	D3DXVECTOR3			pos, normal;			// �ʒu�E�@��
	CLight				*pLight;				// ���C�g�̏��
	// ���擾
	pLight = CManager::GetRenderer()->GetLight();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);
	// ���C�g�̕����ݒ�
	vecLight = D3DXVECTOR4(
		-pLight->GetVec(CLight::TYPE_MAIN).x,
		-pLight->GetVec(CLight::TYPE_MAIN).y,
		-pLight->GetVec(CLight::TYPE_MAIN).z,
		0.0f
	);
	// �ʒu
	pos = D3DXVECTOR3(0, 1.0f, 0);
	// �@��
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// �`�悷�镽�ʂ̈ʒu�����߂�
	D3DXPlaneFromPointNormal(
		&planeField,			// ����
		&pos,					// �ʒu
		&normal					// �@��
	);
	// �e�𕽖ʂɂ���
	D3DXMatrixShadow(
		&mtxShadow,				// �V���h�E�}�g���b�N�X
		&vecLight,				// ���C�g����
		&planeField				// ����
	);

	// �s��̐�(1:�e�s�� = 2:�V���h�E�}�b�s���O�s�� * 3:�e�s��)
	D3DXMatrixMultiply(&mtxShadow,	// 1
		&mtx,						// 2
		&mtxShadow					// 3
	);
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);
	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);
	// �J�E���g�}�e���A��
	for (int nCntMat = 0; nCntMat < (int)Xmodel.nNumMat; nCntMat++)
	{
		// �e�̐F
		matDef.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		// �}�e���A���̐ݒ�
		pDevice->SetMaterial(&matDef);
		// �`��
		Xmodel.pMesh->DrawSubset(nCntMat);
	}
	// �}�e���A�����f�t�H���g�ɖ߂�
	pDevice->SetMaterial(&matDef);
}

// ----------------------------------------
// �e�̕`��(���b�V��)����
// ----------------------------------------
void CShadowmapping::Draw(
	LPDIRECT3DDEVICE9	&pDevice,		// �f�o�C�X���
	D3DXMATRIX			&mtx,			// �}�g���b�N�X���
	LPDIRECT3DINDEXBUFFER9	&pIndex,	// �C���f�b�N�X���
	int	&nNumberVertex,					// �C���f�b�N�X���
	int	&nNumPolygon					// �C���f�b�N�X���
)
{
	// �ϐ��錾
	D3DMATERIAL9		matDef;					// �}�e���A���f�[�^�̃|�C���^
	D3DXMATRIX			mtxShadow;				// �V���h�E�}�g���b�N�X
	D3DXPLANE			planeField;				// ����
	D3DXVECTOR4			vecLight;				// ���C�g�̕���
	D3DXVECTOR3			pos, normal;			// �ʒu�E�@��
	CLight				*pLight;				// ���C�g�̏��
												// ���擾
	pLight = CManager::GetRenderer()->GetLight();

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxShadow);
	// ���C�g�̕����ݒ�
	vecLight = D3DXVECTOR4(
		-pLight->GetVec(CLight::TYPE_MAIN).x,
		-pLight->GetVec(CLight::TYPE_MAIN).y,
		-pLight->GetVec(CLight::TYPE_MAIN).z,
		0.0f
	);
	// �ʒu
	pos = D3DXVECTOR3(0, 1.0f, 0);
	// �@��
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// �`�悷�镽�ʂ̈ʒu�����߂�
	D3DXPlaneFromPointNormal(
		&planeField,			// ����
		&pos,					// �ʒu
		&normal					// �@��
	);
	// �e�𕽖ʂɂ���
	D3DXMatrixShadow(
		&mtxShadow,				// �V���h�E�}�g���b�N�X
		&vecLight,				// ���C�g����
		&planeField				// ����
	);

	// �s��̐�(1:�e�s�� = 2:�V���h�E�}�b�s���O�s�� * 3:�e�s��)
	D3DXMatrixMultiply(&mtxShadow,	// 1
		&mtx,						// 2
		&mtxShadow					// 3
	);
	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);
	// ���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);
	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[����ݒ�
	pDevice->SetIndices(
		pIndex);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);
	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		nNumberVertex,
		0,
		nNumPolygon);
}
