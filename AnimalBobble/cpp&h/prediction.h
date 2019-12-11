// ----------------------------------------
//
// �\���������̐���[prediction.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _PREDICTION_H_
#define _PREDICTION_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "main.h"
#include "scene_two.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------

// ------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------
class CPrediction : public CScene_TWO
{
public:
	/* �񋓌^ */
	// �^�C�v
	typedef enum
	{
		TYPE_BEE = 0,	// �n�`
		TYPE_CAT,		// �L
		TYPE_FISH,		// ��
		TYPE_HAWK,		// ��
		TYPE_SNAKE,		// ��
		TYPE_MAX
	} TYPE;

	/* �֐� */
	CPrediction();
	~CPrediction();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetPos(D3DXVECTOR3 pos);
	void SetRot(float fRot);
	static CPrediction * Create(CManager::PLAYER playerId,D3DXVECTOR3 pos, TYPE nType);	// �쐬
	static HRESULT Load(void);									// �ǂݍ���
	static void UnLoad(void);									// �ǂݍ��񂾏���j��
protected:

private:
	/* �֐� */
	void Limit(float fRot);				// ���E�_
	/* �ϐ� */
	static LPDIRECT3DTEXTURE9 m_pTex;	// �e�N�X�`���[���
	D3DXVECTOR3 m_pos;					// �ʒu
	D3DXCOLOR m_col;					// �J���[
	TYPE m_type;						// �^�C�v
	CManager::PLAYER m_PlayerId;		// �v���C���[�ԍ�
};
#endif