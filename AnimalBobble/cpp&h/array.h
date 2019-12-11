// ----------------------------------------
//
// �z�񏈗��̐���[array.h]
// Author : Koki Nishiyama
//
// ----------------------------------------
#ifndef _ARRAY_H_
#define _ARRAY_H_	 // �t�@�C������������߂�

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
#define MAX_ARRAY_LINE (10)		// �s�̍ő吔
#define MAX_ARRAY_COLUMN (10)	// ��̍ő吔

// ------------------------------------------------------------------------------------------
//
// �O���錾
//
// ------------------------------------------------------------------------------------------
class CLine;	// ��

// ------------------------------------------------------------------------------------------
//
// �N���X
//
// ------------------------------------------------------------------------------------------
class CArray : public CScene
{
public:
	/* �֐� */
	CArray();
	~CArray();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPos(int nCntLine, int nCntColumn);
	// ����̍s�����n������
	D3DXVECTOR3 TopPosCorrection(
		D3DXVECTOR3 &pos,	// �ʒu�A�h���X
		int nLine, 			// �s
		int *nIdColumn		// ��|�C���^
	);						
	// ����̍s�����n������
	D3DXVECTOR3 AroundPosCorrection(
		D3DXVECTOR3 &pos,	// �ʒu�A�h���X
		int nLine,			// �s
		int nColumn,		// ��
		int *nIdLine,		// �s���|�C���^
		int *nIdColumn		// ����|�C���^
	);	
	// �I�������s���������ɉ�����
	D3DXVECTOR3 TargetPosCorrection(
		int nLine,	// �s
		int nColumn	// ��
	);												
	void OriginPosChange(void);																				// �����l��ω�
	void BiginPos(void);																					// �������ꂽ�Ƃ̏����l
	void DeleteState(int nCntLine, int nCntColumn);															// �z�u��Ԃ�false��
	void SetState(int nCntline, int nCntColumn,bool bState);												// �z���Ԑݒ�
	static CArray * Create(CManager::PLAYER playerId, D3DXVECTOR3 pos, D3DXVECTOR2 size);					// �쐬
	static int GetDownCnt(void);																			// �J�E���g�_�E�����擾
protected:

private:
	/* �ϐ� */
	D3DXVECTOR3 m_BiginPos = {};										// �������ꂽ�Ƃ��̏����ʒu
	D3DXVECTOR3 m_OriginPos = {};										// �����ʒu
	D3DXVECTOR3 m_VariousPos[MAX_ARRAY_LINE][MAX_ARRAY_COLUMN] = {};	// �l�X�Ȉʒu
	CManager::PLAYER m_PlayerId;										// �v���C���[�ԍ�
	bool m_bState[MAX_ARRAY_LINE][MAX_ARRAY_COLUMN] = {};				// �z�u����Ă��邩����Ă��Ȃ���
	CLine * m_pline;													// ��
	static int m_nDownCnt;												// ������J�E���g
};

#endif