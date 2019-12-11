// ----------------------------------------
//
// �G�t�F�N�g�����̐���[effect.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _EFFECT_H_
#define _EFFECT_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "scene_two.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define MAX_EFFECT (1028)

// ------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------
class CEffect : public CScene_TWO
{
public:
	/* �񋓌^ */
	// �^�C�v
	typedef enum
	{
		TYPE_CERCLE,
		TYPE_FOUNTAIN,
	} TYPE;
	/* �֐� */
	CEffect();
	~CEffect();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Fountain(					/* �����ړ� */
		float fGoalrot,				// �ړI�p�x
		int nSplit					// ������
		);		
	void Circle(					/* �~�ړ� */
		int nSplit					// ������
		);
	static HRESULT Load(void);
	static void UnLoad(void);
	static CEffect * Create(		/* �쐬 */
		D3DXVECTOR3 pos,			// �ʒu
		D3DXCOLOR col,				// �J���[
		D3DXCOLOR goalcol,			// �ڕW�̃J���[
		float fRot,					// �����p�x
		float fSpeed,				// ����
		float fSize,				// �T�C�Y
		float fGoalsize,			// �ڕW�T�C�Y
		int nLife					// ���C�t(������)
		);		
protected:

private:
	/* �ϐ��錾 */
	static LPDIRECT3DTEXTURE9 m_pTex;					// �e�N�X�`���[
	D3DXVECTOR3 m_pos = {};								// ���S���W
	D3DXCOLOR m_col = {};								// �J���[
	D3DXCOLOR m_Goalcol = {};							// �ڕW�J���[
	D3DXCOLOR m_Diffcol = {};							// �J���[����
	float m_fRot;										// �p�x
	float m_fGoalrot;									// �ڕW�p�x
	float m_fDiffrot;									// �p�x����
	float m_Speed;										// ����
	float m_fSize;										// �T�C�Y
	float m_fGoalsize;									// �ڕW�T�C�Y
	float m_fDiffsize;									// �T�C�Y����
	int m_nLife;										// ���C�t
	int m_nSplit;										// ������
	int m_nCntSplit;									// �����J�E���g
};

#endif