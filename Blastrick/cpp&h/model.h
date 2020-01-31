// ----------------------------------------------------------------------------------------------------
//
// ���f������ [model.h]
// Author : KOKI NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------
#ifndef _MODEL_H_
#define _MODEL_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ----------------------------------------------------------------------------------------------------
#include "manager.h"
#include "model_info.h"

// ----------------------------------------------------------------------------------------------------
//
// �N���X
//
// ----------------------------------------------------------------------------------------------------
class CModel
{
public:
	/* �֐� */
	CModel();
	~CModel();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(
		D3DXMATRIX & mtx,
		float & fAlpha
	);
	// �쐬
	static CModel * Create(void);											
	// ���f���̏��n������
	void BindModel(								
		XMODEL const &xModel,				// ���f���̏��
		MODEL_OFFSET const &offset			// ���f���̃I�t�Z�b�g
	);
	// �ݒ�
	void SetParent(CModel * pModel);		// �e���
	void SetPos(D3DXVECTOR3 &pos);			// �ʒu���
	void SetMotion(KEY &key,int nMaxFrame);	// ���[�V�����ݒ�
	// �擾
	XMODEL &GetXModel(void);				// ���f�����
	D3DXMATRIX	&GetMtx(void);				// �s����
	D3DXVECTOR3 &GetPos(void);				// �ʒu���
	D3DXVECTOR3	&GetRot(void);				// ��]���
protected:
private:
	XMODEL				m_Xmodel;			// x���f�����
	D3DXMATRIX			m_mtxWorld;			// ���[���h�}�g���b�N�X
	D3DXVECTOR3			m_size;				// �T�C�Y
	int					m_nFrame;			// �t���[��
	CModel				*m_Parent;			// �e�p�[�c
};
#endif