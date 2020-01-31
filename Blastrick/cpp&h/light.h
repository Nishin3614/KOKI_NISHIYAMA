//=============================================================================
//
// ���C�g���� [light.h]
// Author : KOKI_NISHIYAMA
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// �C���N���[�h�t�@�C��
//=============================================================================
#include "main.h"

//=============================================================================
// �}�N����`
//=============================================================================

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CLight
{
public:
	/* �񋓌^ */
	typedef enum
	{
		TYPE_MAIN = 0,
		TYPE_SUB1,
		TYPE_SUB2,
		TYPE_MAX
	} TYPE;

	/* �֐� */
	CLight();
	~CLight();
	void Init(void);
	void Uninit(void);
	void Update(void);
	static CLight * Create(void);	// �쐬
	D3DLIGHT9 &GetLight(TYPE type);		// ���C�g���
	D3DXCOLOR &GetCol(TYPE type);		// �F���
	D3DVECTOR &GetVec(TYPE type);		// �������
protected:

private:
	D3DLIGHT9 m_light[TYPE_MAX];	// ���C�g���
	// �g�U���̐F
	D3DXCOLOR m_col[TYPE_MAX] =		// �J���[
	{
		D3DXCOLOR(1.0f,1.0f,1.0f,1.0f),
		D3DXCOLOR(0.5f,0.5f,0.5f,1.0f),
		D3DXCOLOR(0.2f,0.2f,0.2f,1.0f),
	};
	// ���̕���
	D3DXVECTOR3 m_VecDir[TYPE_MAX] =// ����
	{
		D3DXVECTOR3(-1.0f,-0.9f,-1.0f),	// ���C�����C�g
		D3DXVECTOR3(-0.5f, -0.5, 0.0f),	// �T�u�P
		D3DXVECTOR3(0.5f, -1.0f, 0.5f)	// �T�u�Q
	};
};

#endif
