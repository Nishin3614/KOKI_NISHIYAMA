// ----------------------------------------
//
// �f�o�b�O�p�̓����蔻��\�������̐���[debugcollision.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _DEBUGCOLLISION_H_
#define _DEBUGCOLLISION_H_	 // �t�@�C������������߂�

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
#define MAX_DEBUGCOLLISION (10)

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CDebugcollision : public CScene
{
public:
	/* �񋓌^ */
	typedef enum
	{
		COLLISIONTYPE_BOX = 0,
		COLLISIONTYPE_CICLE,
		COLLISIONTYPE_MAX
	} COLLISIONTYPE;

	/* �֐� */
	CDebugcollision();
	~CDebugcollision();

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	static CDebugcollision * Create(
		D3DXVECTOR3 * pos,
		D3DXVECTOR3 * size,
		COLLISIONTYPE type
	);
protected:

private:
	void BoxCollision(void);


	D3DXVECTOR3 * m_pos;
	D3DXVECTOR3 * m_size;
	COLLISIONTYPE m_type;
};

#endif