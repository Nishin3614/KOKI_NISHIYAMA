// ------------------------------------------------------------------------------------------
//
// �^�C������ [time.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _TIME_H_
#define _TIME_H_
//------------------------------------------------------
//
//	�C���N���[�h�t�@�C��
//
//--------------------------------------------------------
#include "scene.h"
#include "renderer.h"
#include "number.h"

//------------------------------------------------------
//
//	�}�N����`
//
//--------------------------------------------------------
#define TIME_DIGIT (2)//����

//------------------------------------------------------
//
//	�O���錾
//
//--------------------------------------------------------
class CScene;

//--------------------------------------------------------
//
//	�N���X�^�̒�`
//
//--------------------------------------------------------
class CTime :public CScene
{
public:
	/* �֐� */
	CTime();	//�R���X�g���N�^ (�C���X�^���X�����������Ǝ����I�ɌĂяo�����)
	~CTime();	//�f�X�g���N�^ (�C���X�^���X���j�������Ǝ����I�ɌĂяo�����)

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

	static CTime * Create(D3DXVECTOR3 const &pos);	//����
	void SetPos(D3DXVECTOR3 pos);					//���W�̐ݒ�
	void DecreasesTime(void);						//���Ԃ̌���
	bool GetTimeOverFlag(void);						// �^�C���I�[�o�[�t���O

private:

	//�ÓI�ϐ��錾

	//�ϐ��錾
	CNumber *m_apNumber[TIME_DIGIT];				//�������̕ϐ�
	D3DXVECTOR3 m_pos;								//���W
	int m_nTime;									//����
	bool m_bTimeOver;								
};
#endif