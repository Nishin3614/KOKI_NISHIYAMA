// ----------------------------------------
//
// �X�R�A�����̐���[score.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "score.h"
#include "number.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define SCORE_SIZE (50.0f)

// ----------------------------------------
//
// �O���[�o���ϐ�
//
// ----------------------------------------

// ----------------------------------------
//
// �ÓI�ϐ��錾
//
// ----------------------------------------

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CScore::CScore() : CScene(ACTOR_SCORE, LAYER_UI)
{
	// �X�R�A
	m_nScore = 0;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CScore::~CScore()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CScore::Init(void)
{	
	// �ϐ��錾
	int nScore;

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		// ���̐���
		nScore = m_nScore % (int)powf(10.0f, (float)nCnt + 1.0f) / (int)powf(10.0f, (float)nCnt);
		m_pNumber[nCnt] = CNumber::Create();
		m_pNumber[nCnt]->SetNum(nScore);
		m_pNumber[nCnt]->SetPos(
			D3DXVECTOR3(m_pos.x + (MAX_SCORE * SCORE_SIZE) / 2 - nCnt * SCORE_SIZE, m_pos.y, 0.0f),
			SCORE_SIZE);
		m_pNumber[nCnt]->SetTex(CNumber::TEX_SCORE);
	}
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CScore::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		if (m_pNumber[nCnt] != NULL)
		{
			m_pNumber[nCnt]->Uninit();
			delete m_pNumber[nCnt];
			m_pNumber[nCnt] = NULL;
		}
	}
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CScore::Update(void)
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_pNumber[nCnt]->Update();
	}
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CScore::Draw(void)
{
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		m_pNumber[nCnt]->Draw();
	}
}

#ifdef _DEBUG
//-------------------------------------------------------------------------------------------------------------
// �f�o�b�O�\��
//-------------------------------------------------------------------------------------------------------------
void CScore::Debug(void)
{
}
#endif // _DEBUG


// ----------------------------------------
// �X�R�A�ݒ菈��
// ----------------------------------------
void CScore::SetScore(int nPoint)
{
	// �ϐ��錾
	int nScore;

	// ���݂̃|�C���g�����Z
	m_nScore += nPoint;
	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		// ���̐���
		nScore = m_nScore % (int)powf(10.0f, (float)nCnt + 1.0f) / (int)powf(10.0f, (float)nCnt);
		m_pNumber[nCnt]->SetNum(nScore);
		m_pNumber[nCnt]->SetPos(
			D3DXVECTOR3(m_pos.x + (MAX_SCORE * SCORE_SIZE) / 2 - nCnt * SCORE_SIZE, m_pos.y, 0.0f),
			SCORE_SIZE);
	}
}

// ----------------------------------------
// �X�R�A�擾����
// ----------------------------------------
int CScore::GetScore(void)
{
	return m_nScore;
}

// ----------------------------------------
// �ʒu���擾����
// ----------------------------------------
D3DXVECTOR3 CScore::GetPos(void)
{
	return m_pos;
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CScore * CScore::Create(D3DXVECTOR3 pos)
{
	// �ϐ��錾
	CScore * pScore;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pScore = new CScore();
	// �ʒu�����
	pScore->m_pos = pos;
	pScore->m_pos.y -= SCORE_SIZE / 2;
	// ����������
	pScore->Init();
	// ���������I�u�W�F�N�g��Ԃ�
	return pScore;
}