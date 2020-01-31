// ------------------------------------------------------------------------------------------
//
// �^�C������ [time.cpp]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------

//--------------------------------------------------------
//
// �C���N���[�h�t�@�C��
//
//--------------------------------------------------------
#include "main.h"
#include "time.h"
#include "manager.h"
#include "scene.h"
#include "number.h"

//--------------------------------------------------------
//
// �}�N����`
//
//--------------------------------------------------------
#define TIME_SPACE	(40.0f)//�Ԋu

//--------------------------------------------------------
//
// �ÓI�ϐ�������
//
//--------------------------------------------------------

//------------------------------------------------------
// �R���X�g���N�^
//--------------------------------------------------------
CTime::CTime(void) :CScene(ACTOR_TIME,LAYER_UI)
{
	m_nTime = DERAY_TIME(99);
	m_bTimeOver = false;
}
//------------------------------------------------------
// �f�X�g���N�^
//--------------------------------------------------------
CTime::~CTime()
{
}
//------------------------------------------------------
// ����������
//------------------------------------------------------
void CTime::Init(void)
{
	// �ϐ��錾
	int nNumber;

	for (int nCount = 0; nCount < TIME_DIGIT; nCount++)
	{
		// ����
		m_apNumber[nCount] = CNumber::Create();
		// �ʒu�̐ݒ�
		m_apNumber[nCount]->SetPos(D3DXVECTOR3(m_pos.x + nCount * TIME_SPACE, m_pos.y, 0.0f), TIME_SPACE);
		// �^�C����1�����擾
		nNumber = (int)(m_nTime / 60) % (int)powf(10.0f, (float)(TIME_DIGIT - nCount)) / (int)powf(10.0f, TIME_DIGIT - 1.0f - nCount);
		// �i���o�[�̐ݒ�
		m_apNumber[nCount]->SetNum(nNumber);
	}
}
//------------------------------------------------------
// �I������
//------------------------------------------------------
void CTime::Uninit(void)
{
	for (int nCount = 0; nCount < TIME_DIGIT; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Uninit();
			delete m_apNumber[nCount];
			m_apNumber[nCount] = NULL;
		}
	}
}
//------------------------------------------------------
// �X�V����
//------------------------------------------------------
void CTime::Update(void)
{
	// �ϐ��錾
	int nNumber;
	// �^�C���_�E��
	DecreasesTime();
	for (int nCount = 0; nCount < TIME_DIGIT; nCount++)
	{
		// �k���`�F�b�N
		if (m_apNumber[nCount] != NULL)
		{
			// �i���o�[�̍X�V
			m_apNumber[nCount]->Update();
			// �^�C����1�����擾
			nNumber = int((m_nTime / 60) % (int)powf(10.0f, (float)(TIME_DIGIT - nCount)) / (int)powf(10.0f, TIME_DIGIT - 1.0f - nCount));
			// �i���o�[�̐ݒ�
			m_apNumber[nCount]->SetNum(nNumber);
		}
	}
}
//------------------------------------------------------
// �`�揈��
//------------------------------------------------------
void CTime::Draw(void)
{
	for (int nCount = 0; nCount < TIME_DIGIT; nCount++)
	{
		if (m_apNumber[nCount] != NULL)
		{
			m_apNumber[nCount]->Draw();
		}
	}
}

#ifdef _DEBUG
// ------------------------------------------
// �f�o�b�O�\��
// ------------------------------------------
void CTime::Debug(void)
{
}
#endif // _DEBUG

//------------------------------------------------------
// �|�W�V�����̐ݒ�
//------------------------------------------------------
void CTime::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//------------------------------------------------------
// ���Ԍ�������
//------------------------------------------------------
void CTime::DecreasesTime()
{
	m_nTime--;

	if (m_nTime <= 0)
	{
		m_nTime = 0;
		m_bTimeOver = true;
	}
}
//------------------------------------------------------
// �^�C���I�[�o�[�t���O�̎擾
//------------------------------------------------------
bool CTime::GetTimeOverFlag(void)
{
	return m_bTimeOver;
}
//------------------------------------------------------
// ��������
//------------------------------------------------------
CTime *CTime::Create(D3DXVECTOR3 const &pos)
{
	// �ϐ��錾
	CTime *pTime;
	pTime = new CTime;
	pTime->m_pos = pos;
	pTime->Init();
	return pTime;
}
