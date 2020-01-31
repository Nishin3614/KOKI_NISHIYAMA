// ----------------------------------------
//
// �o�b�O�O���E���h�h�[�������̐���[bgdome.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _BGDOME_H_
#define _BGDOME_H_	 // �t�@�C������������߂�

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "scene.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------


// ----------------------------------------
//
// �O���錾
//
// ----------------------------------------
class CMeshdome;

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CBgdome : public CScene
{
public:
	/* �񋓌^ */
	typedef enum
	{
		TYPE_BG = 0,
		TYPE_STAR,
		TYPE_MAX
	} TYPE;
	/* �֐� */
	CBgdome();
	~CBgdome();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	static HRESULT Load(void);
	static void UnLoad(void);
	static CBgdome * Create(void);		// �쐬
	static void SetUse(bool bUse);			// �g�p���
	static void SetPos(D3DXVECTOR3 const &pos);	// �ʒu
	static void SetRot(D3DXVECTOR3 const &rot);	// ��]
	static void Set(
		D3DXVECTOR3 const &pos,
		D3DXVECTOR3 const &rot,
		int const &nFrame
	);
protected:

private:
	/* �ϐ� */
	static CMeshdome	*m_aMeshdome[TYPE_MAX];	// ���b�V���h�[��
	static bool			m_bUse;					// �g�p���
	static int			m_CntFrame;				// �J�E���g�t���[��
	static int			m_nMaxFrame;			// �ő�t���[��
};

#endif