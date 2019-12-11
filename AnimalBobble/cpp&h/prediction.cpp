// ----------------------------------------
//
// �\���������̐���[prediction.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
#include "prediction.h"
#include "frame.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------
#define PREDICTION_TEXTURE ("data/TEXTURE/Player/Line.png")

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
LPDIRECT3DTEXTURE9 CPrediction::m_pTex = NULL;

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CPrediction::CPrediction() : CScene_TWO::CScene_TWO(ACTOR_PREDICTION, LAYER_OBJ)
{
	/* �ϐ��̏����� */
	// �ʒu
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// �^�C�v
	m_type = CPrediction::TYPE_BEE;
	// �v���C���[�ԍ�
	m_PlayerId = CManager::PLAYER_1;
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CPrediction::~CPrediction()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CPrediction::Init(void)
{
	// �T�C�Y�ݒ�
	CScene_TWO::SetSize(D3DXVECTOR2(50.0f, 300.0f));
	// ������
	CScene_TWO::Init();
	// ����
	CScene_TWO::SetLengh(D3DXVECTOR2(CScene_TWO::GetSize().x / 2, CScene_TWO::GetSize().y));
	// ���݂̈ʒu�ݒ�
	CScene_TWO::SetPosition(m_pos);
	// �F�ݒ�
	CScene_TWO::SetCol(m_col);
	// �e�N�X�`���[�ݒ�
	CScene_TWO::BindTexture(m_pTex);
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CPrediction::Uninit(void)
{
	CScene_TWO::Uninit();
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CPrediction::Update(void)
{
	// ���݂̈ʒu�ݒ�
	CScene_TWO::SetPosition(m_pos);
	// �X�V
	CScene_TWO::Update();
	// ����
	CScene_TWO::SetLengh(D3DXVECTOR2(CScene_TWO::GetSize().x / 2, CScene_TWO::GetSize().y));
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CPrediction::Draw(void)
{
	// �`�揈��
	CScene_TWO::Draw();
}

// ----------------------------------------
// �ʒu�ݒ菈��
// ----------------------------------------
void CPrediction::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
	CScene_TWO::SetPosition(pos);
}

// ----------------------------------------
// ��]�ݒ菈��
// ----------------------------------------
void CPrediction::SetRot(float fRot)
{
	Limit(fRot);					// ���E�_
	CScene_TWO::SetRot(fRot);	// ��]�ݒ�
}

// ----------------------------------------
// ���E�_����
// ----------------------------------------
void CPrediction::Limit(float fRot)
{
	// �ϐ��錾
	CFrame *pFrame = NULL;
	float fCons = 0;
	float fSection = 0;
	float fLengh;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posdiff;
	D3DXVECTOR2 Linear;

	// ���擾
	for (int nCntLayer = 0; nCntLayer < GetMaxLayer(LAYER_OBJ); nCntLayer++)
	{
		if (pFrame == NULL)	pFrame = (CFrame*)CScene::GetActor(ACTOR_FRAME, LAYER_OBJ, nCntLayer);	// �t���[��
	}

	// x�̈ʒu����������Ȃ��ꍇ
	if (m_pos.x != m_pos.x + sinf(fRot))
	{
		// �ꎟ�֐�
		Linear = CCalculation::Linear_function(m_pos, D3DXVECTOR3(m_pos.x + sinf(fRot), m_pos.y + cosf(fRot), 0.0f));

		/* ����� */
		if (Linear.x * (pFrame->GetPos().x - pFrame->GetSize().x / 2) + Linear.y < (pFrame->GetPos().y + pFrame->GetSize().y / 2) &&
			Linear.x * (pFrame->GetPos().x - pFrame->GetSize().x / 2) + Linear.y >(pFrame->GetPos().y - pFrame->GetSize().y / 2))
		{
			pos = D3DXVECTOR3(
				pFrame->GetPos().x - pFrame->GetSize().x / 2,
				Linear.x * (pFrame->GetPos().x - pFrame->GetSize().x / 2) + Linear.y,
				0.0f);
		}
		/* �E���*/
		else if (Linear.x * (pFrame->GetPos().x + pFrame->GetSize().x / 2) + Linear.y < (pFrame->GetPos().y + pFrame->GetSize().y / 2) &&
			Linear.x * (pFrame->GetPos().x + pFrame->GetSize().x / 2) + Linear.y >(pFrame->GetPos().y - pFrame->GetSize().y / 2))
		{
			pos = D3DXVECTOR3(
				pFrame->GetPos().x + pFrame->GetSize().x / 2,
				Linear.x * (pFrame->GetPos().x + pFrame->GetSize().x / 2) + Linear.y,
				0.0f);
		}
		/* ���� */
		else if ((Linear.y - pFrame->GetPos().y - pFrame->GetSize().y / 2) / -Linear.x > (pFrame->GetPos().x - pFrame->GetSize().x / 2) &&
			(Linear.y - pFrame->GetPos().y - pFrame->GetSize().y / 2) / -Linear.x < (pFrame->GetPos().x + pFrame->GetSize().x / 2))
		{
			pos = D3DXVECTOR3(
				(Linear.y - (pFrame->GetPos().y - pFrame->GetSize().y / 2)) / -Linear.x,
				pFrame->GetPos().y - pFrame->GetSize().y / 2,
				0.0f);
		}
	}
	// ����ȊO
	else
	{
		pos.x = m_pos.x;
		pos.y = pFrame->GetPos().y - pFrame->GetSize().y / 2;
	}
	posdiff = pos - m_pos;

	fLengh = sqrtf(posdiff.x * posdiff.x + posdiff.y * posdiff.y);
	CScene_TWO::SetSize(D3DXVECTOR2(50.0f, fLengh));
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
HRESULT CPrediction::Load(void)
{
	// �ϐ��錾
	LPDIRECT3DDEVICE9 pDevice =					// �f�o�C�X�̎擾
		CManager::GetRenderer()->GetDevice();
	// �e�N�X�`���[�ݒ�
	D3DXCreateTextureFromFile(pDevice, PREDICTION_TEXTURE, &m_pTex);

	return S_OK;
}

// ----------------------------------------
// �ǂݍ��񂾏���j��
// ----------------------------------------
void CPrediction::UnLoad(void)
{
	// �e�N�X�`���̊J��
	if (m_pTex != NULL)
	{
		m_pTex->Release();
		m_pTex = NULL;
	}
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CPrediction * CPrediction::Create(CManager::PLAYER playerId,D3DXVECTOR3 pos, TYPE type)
{
	// �ϐ��錾
	CPrediction * pPrediction;		// �\����2D�N���X
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pPrediction = new CPrediction();
	// ���݂̈ʒu
	pPrediction->m_pos = pos;
	// �^�C�v
	pPrediction->m_type = type;
	// �F
	switch (pPrediction->m_type)
	{
		// �n�`
	case TYPE_BEE:
		pPrediction->m_col = D3DXCOLOR(1.0f, 0.7f, 0.2f, 1.0f);
		break;
		// �L
	case TYPE_CAT:
		pPrediction->m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
		// ��
	case TYPE_FISH:
		pPrediction->m_col = D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f);
		break;
		// ��
	case TYPE_HAWK:
		pPrediction->m_col = D3DXCOLOR(0.4f, 0.2f, 0.2f, 1.0f);
		break;
		// ��
	case TYPE_SNAKE:
		pPrediction->m_col = D3DXCOLOR(0.1f, 1.0f, 0.1f, 1.0f);
		break;
	default:
		break;
	}
	// ����������
	pPrediction->Init();
	// �v���C���[�ԍ�
	pPrediction->m_PlayerId = playerId;
	// ���������I�u�W�F�N�g��Ԃ�
	return pPrediction;
}