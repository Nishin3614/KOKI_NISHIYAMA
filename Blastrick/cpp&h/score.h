// ----------------------------------------
//
// �����w�i�����̐���[score.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _SCORE_H_
#define _SCORE_H_	 // �t�@�C������������߂�

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
#define MAX_SCORE (7)

class CNumber;
// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CScore : public CScene
{
public:
	/* �֐� */
	CScore();
	~CScore();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

	void SetScore(int nPoint);
	int GetScore(void);
	D3DXVECTOR3 GetPos(void);
	static CScore * Create(D3DXVECTOR3 pos);	// �쐬

protected:

private:
	/* �֐� */
	/* �ϐ� */
	CNumber *m_pNumber[MAX_SCORE];	// �X�R�A�\��
	D3DXVECTOR3 m_pos;	// �ʒu���
	int m_nScore;	// ���_
};

#endif