//------------------------------------------------------
//
//	�L���萔�Ƌ^������
//
//--------------------------------------------------------
#ifndef _PAUSE_H_
#define _PAUSE_H_
//------------------------------------------------------
//
//	�C���N���[�h�t�@�C��
//
//--------------------------------------------------------
#include "scene_two.h"

//------------------------------------------------------
//
//	�}�N����`
//
//--------------------------------------------------------
#define PAU_SCR_SIZE_X (500.0f)//�|�[�Y�̔w�i�̉��̃T�C�Y
#define PAU_SCR_SIZE_Y (250.0f)//�|�[�Y�̔w�i�̏c�̃T�C�Y

#define PAU_UI_SIZE_X (250.0f)//����[PAUAE]�̉��̃T�C�Y
#define PAU_UI_SIZE_Y (90.0f)//����[PAUAE]�̏c�̃T�C�Y

#define PAU_BUT_SIZE_X (120.0f)//��ڂ̃{�^���̉��̃T�C�Y
#define PAU_BUT_SIZE_Y (50.0f)//��ڂ̃{�^���̉��̃T�C�Y

#define PAUSE_TEXTURE (4)//�g���摜�̐�
//------------------------------------------------------
//
//	�O���錾
//
//--------------------------------------------------------
class CScene_TWO;

//===========================================================
//
//	�N���X�^�̒�`
//
//===========================================================
class CPause :public CScene_TWO
{
public:
	enum PAUSETYPE
	{
		PAUSETYPE_NORMAL,//�w�i�摜
		PAUSETYPE_UI,//PAUSE����
		PAUSETYPE_BUTTAN1,//�|�[�Y�{�^��1
		PAUSETYPE_BUTTAN2,//�|�[�Y�{�^��2
		PAUSETYPE_MAX
	};

public:
	enum PAUSESTATE
	{
		PAUSESTATE_NORMAL,//���I��
		PAUSESTATE_1,//1�߂̏ꏊ��I��
		PAUSESTATE_2,//2�߂̏ꏊ��I��
	};

public:
	//�֐��錾
	CPause();//�R���X�g���N�^ (�C���X�^���X�����������Ǝ����I�ɌĂяo�����)
	~CPause();//�f�X�g���N�^ (�C���X�^���X���j�������Ǝ����I�ɌĂяo�����)
	static HRESULT Load(void);//�摜�̓ǂݍ���
	static void Unload(void);//�摜�̔j��
	void Init(void);//������
	void Uninit(void);//�I��
	void Update(void);//�X�V
	void Draw(void);//�`��

	static CPause * Create(PAUSETYPE type);//����
	static void SetPause(bool bPause);	// �|�[�Y�̏�Ԑݒ�
	static bool GetPause(void);			// �|�[�Y�̏�Ԏ擾
private:
	//�ÓI�ϐ��錾
	static LPDIRECT3DTEXTURE9 m_pTexture[PAUSE_TEXTURE];//�摜�𕡐��Ǘ�����ϐ�
	static bool m_bPause;
	//�ϐ��錾
	PAUSETYPE m_type;//UI�̎��
	PAUSESTATE m_state;//�I���ł���{�^���̏��
	D3DXCOLOR m_col;//�F�̕ϐ�
};
#endif

