// ------------------------------------------
//
// �O�Ղ̏���[trajectory.h]
// Author : Nishiyama koki
//
// ------------------------------------------
#ifndef _TRAJECTORY_H_
#define _TRAJECTORY_H_

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
class CTrajectory
{
public:
	/* �񋓌^ */
	typedef enum
	{
		TYPE_NORMAL = 0,	// �ʏ�O��
		TYPE_TRICK1,		// �鉜�`1�̋O��
		TYPE_TRICK2,		// �鉜�`2�̋O��
		TYPE_MAX
	} TYPE;
	/* �֐� */
	CTrajectory();
	~CTrajectory();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	// ------------------------------------------
	// �ݒ�
	// 1�s��A2�^�C�v
	// ------------------------------------------
	static void Set(
		D3DXMATRIX *pmtx,	// �s��
		TYPE type,			// �^�C�v
		int &nFrame			// �t���[����
	);

#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	static CTrajectory * Create(void);
	static HRESULT LoadCreate(void);	// ���炩���ߋO�Ղ̐���������
	static HRESULT Load(void);
	static void Unload(void);

private:
};

#endif
