// ----------------------------------------
//
// ���Ԓn�_�����̐���[waypoint.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _WAYPOINT_H_
#define _WAYPOINT_H_	 // �t�@�C������������߂�

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
class CWaypoint : public CScene
{
public:
	/* �֐� */
	CWaypoint();
	~CWaypoint();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);
	static void UnLoad(void);
	static CWaypoint * Create(		// �쐬
		D3DXVECTOR3 pos
	);
	bool GetWaypoint(void);	// ���Ԓn�_
	D3DXVECTOR3 GetPos(void);	// ���Ԓn�_�̈ʒu
protected:

private:
	/* �֐� */
	/* �ϐ� */
	static LPDIRECT3DTEXTURE9 m_pTex;
	CMeshdome * m_dome;					// �h�[�����
	D3DXVECTOR3 m_pos;					// �ʒu
	bool		m_bWaypoint;			// ���Ԓn�_
};

#endif