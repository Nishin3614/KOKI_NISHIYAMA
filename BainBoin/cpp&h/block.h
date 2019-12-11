// ----------------------------------------
//
// �u���b�N�����̐���[block.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _BLOCK_H_
#define _BLOCK_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "scene_X.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------

//=============================================================================
// �O���錾
//=============================================================================

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CBlock : public CScene_X
{
public:
	/* �񋓌^ */
	// �^�C�v
	typedef enum
	{
		TYPE_1 = 0,	// ��]���A�b�v
		TYPE_MAX
	} TYPE;

	/* �֐� */
	CBlock();
	~CBlock();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos);			// �ʒu�ݒ�
	void SetRot(D3DXVECTOR3 rot);			// ��]�ݒ�
	D3DXVECTOR3 GetPos(void);				// �ʒu�擾
	static CBlock * Create(void);			// �쐬
	static void CBlock::LoadCreate(void);	// �ǂݍ��񂾂��̂𐶐�
	static HRESULT Load(void);				// �ǂݍ���
	static void UnLoad(void);				// �j��
protected:
private:
	static LPDIRECT3DTEXTURE9 m_pTexture[TYPE_MAX];		// �e�N�X�`���ւ̃|�C���^
	static CLoad	*m_pload;							// ���[�h
	static CModel_info	*m_pmodel_info;					// ���f�����
	static int		m_nAll;								// ����
	TYPE			m_Type;								// �^�C�v
};

#endif