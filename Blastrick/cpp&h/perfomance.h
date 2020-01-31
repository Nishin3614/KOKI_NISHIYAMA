// ------------------------------------------
//
// ���o�̏���[perfomance.h]
// Author : Nishiyama koki
//
// ------------------------------------------
#ifndef _PERFOMANCE_H_
#define _PERFOMANCE_H_

// ------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------
#include "manager.h"

// ------------------------------------------
// �}�N����`
// ------------------------------------------

// ------------------------------------------
// �N���X��`
// ------------------------------------------
class CPerfomance
{
public:
	/* �񋓌^ */
	typedef enum
	{
		TYPE_MESHOBIT1 = 0,
		TYPE_MESHOBIT2,
		TYPE_MESHOBIT3,
		TYPE_,
		TYPE_1,
		TYPE_MAX
	} TYPE;
	/* �֐� */
	CPerfomance();
	~CPerfomance();
	// ------------------------------------------
	// �ݒ�
	// 1�s��A2�^�C�v
	// ------------------------------------------
	static void Set(
		D3DXMATRIX *pmtx,	// �s��
		TYPE type,			// �^�C�v
		D3DXVECTOR3 &pos,	// �ʒu
		D3DXVECTOR3 &rot,	// ��]
		int Frame			// �t���[����
		);

#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

private:
};

#endif
