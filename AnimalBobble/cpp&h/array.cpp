// ----------------------------------------
//
// �z�񏈗��̐���[array.cpp]
// Author : Koki Nishiyama
//
// ----------------------------------------

// ----------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ----------------------------------------
/* �`�� */
#include "array.h"
#include "line.h"
#include "bullet.h"

// ----------------------------------------
//
// �}�N����`
//
// ----------------------------------------

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
int CArray::m_nDownCnt = 0;

// ----------------------------------------
// �R���X�g���N�^����
// ----------------------------------------
CArray::CArray() : CScene(ACTOR_ARRAY, LAYER_UI)
{
	m_BiginPos;							// �������ꂽ�Ƃ��̏����ʒu
	m_OriginPos;						// �����ʒu
	m_PlayerId = CManager::PLAYER_1;	// �v���C���[�ԍ�
	m_pline = NULL;						// ��
}

// ----------------------------------------
// �f�X�g���N�^����
// ----------------------------------------
CArray::~CArray()
{
}

// ----------------------------------------
// ����������
// ----------------------------------------
void CArray::Init(void)
{

	//----- �z��̐ݒ� -----//
	// �s
	for (int nCntLine = 0; nCntLine < MAX_ARRAY_LINE; nCntLine++)
	{
		// �����̏ꍇ
		if (nCntLine % 2 == 0)
		{
			// ��
			for (int nCntColumn = 0; nCntColumn < MAX_ARRAY_COLUMN; nCntColumn++)
			{
				// �ʒu�ݒ�
				m_VariousPos[nCntLine][nCntColumn] = 
					m_OriginPos + 
					D3DXVECTOR3(BULLETSIZE * nCntColumn, BULLETSIZE * nCntLine, 0.0f);
				// �z�u���
				m_bState[nCntLine][nCntColumn] = false;
			}
		}
		// ��̏ꍇ
		else
		{
			// ��
			for (int nCntColumn = 0; nCntColumn < MAX_ARRAY_COLUMN; nCntColumn++)
			{
				// �ʒu�ݒ�
				m_VariousPos[nCntLine][nCntColumn] =
					m_OriginPos +
					D3DXVECTOR3(BULLETSIZE / 2 + BULLETSIZE * nCntColumn, BULLETSIZE * nCntLine,0.0f);
				// �z�u���
				m_bState[nCntLine][nCntColumn] = false;
			}
		}
		// �����
		if (nCntLine == MAX_ARRAY_LINE - 1)
		{
			// ����̐�����
			m_pline	= CLine::Create(
				m_VariousPos[MAX_ARRAY_LINE - 1][0] + D3DXVECTOR3(-BULLETSIZE, -BULLETSIZE / 2, 0.0f),
				m_VariousPos[MAX_ARRAY_LINE - 1][MAX_ARRAY_COLUMN - 1] + D3DXVECTOR3(0.0f, -BULLETSIZE / 2, 0.0f));
		}
	}
}

// ----------------------------------------
// �I������
// ----------------------------------------
void CArray::Uninit(void)
{
	// ���̔j��
	if (m_pline != NULL)
	{
		m_pline->Uninit();
		delete m_pline;
		m_pline = NULL;
	}
}

// ----------------------------------------
// �X�V����
// ----------------------------------------
void CArray::Update(void)
{
	// �k���`�F�b�N
	if (m_pline != NULL)
	{
		m_pline->Update();
	}
}

// ----------------------------------------
// �`�揈��
// ----------------------------------------
void CArray::Draw(void)
{
	// �k���`�F�b�N
	if (m_pline != NULL)
	{
		m_pline->Draw();
	}
}

// ----------------------------------------
// �ʒu���擾����
// ----------------------------------------
D3DXVECTOR3 CArray::GetPos(int nCntLine, int nCntColumn)
{
	return m_VariousPos[nCntLine][nCntColumn];
}

// ----------------------------------------
// ��̈ʒu���␳����
// ----------------------------------------
D3DXVECTOR3 CArray::TopPosCorrection(
	D3DXVECTOR3 &pos,	// �ʒu�A�h���X
	int nLine, 			// �s
	int *nIdColumn		// ��|�C���^
)
{
	// �ϐ��錾
	D3DXVECTOR3 posDiffe = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Lengh[MAX_ARRAY_COLUMN];

	// �ʒu���̍���
	posDiffe = CCalculation::Difference_Between(pos, m_VariousPos[nLine][*nIdColumn]);	// �e-�z��
	// �����̌v�Z
	Lengh[*nIdColumn] = sqrtf(
		posDiffe.x * posDiffe.x +
		posDiffe.y * posDiffe.y);

	/* �s��̐ݒ� */
	// ��
	for (int nCntColumn = 0; nCntColumn < MAX_ARRAY_COLUMN; nCntColumn++)
	{
		// �z�u��Ԃ�OFF�̏ꍇ
		if (m_bState[nLine][nCntColumn] == false)
		{
			// �ʒu���̍���
			posDiffe = CCalculation::Difference_Between(pos, m_VariousPos[nLine][nCntColumn]);	// �e-�z��
			// �����̌v�Z
			Lengh[nCntColumn] = sqrtf(
				posDiffe.x * posDiffe.x +
				posDiffe.y * posDiffe.y);
			// �����̔�r
			if (Lengh[*nIdColumn] > Lengh[nCntColumn])
			{
				// �s��ݒ�
				*nIdColumn = nCntColumn;
			}
		}
	}
	// �z�u���ON
	m_bState[nLine][*nIdColumn] = true;
	return m_VariousPos[nLine][*nIdColumn];
}

// ----------------------------------------
// ���͂̈ʒu���␳����
// ----------------------------------------
D3DXVECTOR3 CArray::AroundPosCorrection(
	D3DXVECTOR3 &pos,	// �ʒu�A�h���X
	int nLine,			// �s
	int nColumn,		// ��
	int *nIdLine,		// �s���|�C���^
	int *nIdColumn		// ����|�C���^
)
{
	// �ϐ��錾
	D3DXVECTOR3 posDiffe = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float Lengh[MAX_ARRAY_LINE][MAX_ARRAY_COLUMN] = {};

	// ���߂̍s�ԍ��L��
	for (int nCntLine = nLine + 1; nCntLine >= nLine - 1; nCntLine--)
	{
		// �s���ȍs�ւ̃A�N�Z�X����
		if (nCntLine >= 0)
		{
			*nIdLine = nCntLine;
		}
	}
	// ���߂̗�ԍ��L��
	for (int nCntColumn = nColumn + 1; nCntColumn >= nColumn - 1; nCntColumn--)
	{
		// �s���ȍs�ւ̃A�N�Z�X����
		if (nCntColumn >= 0)
		{
			*nIdColumn = nCntColumn;
		}
	}

	// �ʒu���̍���
	posDiffe = CCalculation::Difference_Between(pos, m_VariousPos[*nIdLine][*nIdColumn]);	// �e-�z��
	// �����̌v�Z
	Lengh[*nIdLine][*nIdColumn] = sqrtf(
		posDiffe.x * posDiffe.x +
		posDiffe.y * posDiffe.y);

	/* �s��̐ݒ� */
	// �s
	for (int nCntLine = nLine - 1; nCntLine <= nLine + 1; nCntLine++)
	{
		// ��
		for (int nCntColumn = nColumn - 1; nCntColumn <= nColumn + 1; nCntColumn++)
		{
			// �z����̏ꍇ
			if (nCntLine >= 0 && nCntLine <= MAX_ARRAY_LINE - 1 &&		// �s�͈̔�
				nCntColumn >= 0 && nCntColumn <= MAX_ARRAY_COLUMN - 1)	// ��͈̔�
			{
				// �z�u���
				if (m_bState[nCntLine][nCntColumn] == false &&		// �z��ɂȂɂ��z�u����Ă��Ȃ����
					!(nCntLine == nLine && nCntColumn == nColumn))	// �����s��������ł͂Ȃ��ꍇ
				{
					// �ʒu���̍���
					posDiffe = CCalculation::Difference_Between(pos, m_VariousPos[nCntLine][nCntColumn]);	// �e-�z��
																														// �����̌v�Z
					Lengh[nCntLine][nCntColumn] = sqrtf(
						posDiffe.x * posDiffe.x +
						posDiffe.y * posDiffe.y);
					// �����̔�r
					if (fabsf(Lengh[*nIdLine][*nIdColumn]) > fabsf(Lengh[nCntLine][nCntColumn]))
					{
						// �s��ݒ�
						*nIdLine = nCntLine;
						*nIdColumn = nCntColumn;
					}
				}
			}
		}
	}
	// �z�u���ON
	m_bState[*nIdLine][*nIdColumn] = true;
 	return m_VariousPos[*nIdLine][*nIdColumn];
}

// ----------------------------------------
// �z�u��Ԃ�������鏈��
// ----------------------------------------
D3DXVECTOR3 CArray::TargetPosCorrection(
	int nLine,	// �s
	int nColumn	// ��
)
{
	return m_VariousPos[nLine][nColumn];
}

// ----------------------------------------
// �����l��ω�����
// ----------------------------------------
void CArray::OriginPosChange(void)
{
	// �����ʒuy������ɉ�����
	m_OriginPos.y += BULLETSIZE;
	// �J�E���g�A�b�v
	m_nDownCnt++;
	// �z��ʒu���Đݒ�\\
	// �s
	for (int nCntLine = 0; nCntLine < MAX_ARRAY_LINE; nCntLine++)
	{
		// �����̏ꍇ
		if (nCntLine % 2 == 0)
		{
			// ��
			for (int nCntColumn = 0; nCntColumn < MAX_ARRAY_COLUMN; nCntColumn++)
			{
				m_VariousPos[nCntLine][nCntColumn] = m_OriginPos + D3DXVECTOR3(BULLETSIZE * nCntColumn, BULLETSIZE * nCntLine, 0.0f);
			}
		}
		// ��̏ꍇ
		else
		{
			// ��
			for (int nCntColumn = 0; nCntColumn < MAX_ARRAY_COLUMN; nCntColumn++)
			{
				m_VariousPos[nCntLine][nCntColumn] = m_OriginPos + D3DXVECTOR3(BULLETSIZE / 2 + BULLETSIZE * nCntColumn, BULLETSIZE * nCntLine, 0.0f);
			}
		}
	}
}

// ----------------------------------------
// �������ꂽ�Ƃ��̏����l����
// ----------------------------------------
void CArray::BiginPos(void)
{
	m_OriginPos = m_BiginPos;
	m_nDownCnt = 0;				// �J�E���g�_�E����������
	// �z��ʒu���Đݒ�\\
		// �s
	for (int nCntLine = 0; nCntLine < MAX_ARRAY_LINE; nCntLine++)
	{
		// �����̏ꍇ
		if (nCntLine % 2 == 0)
		{
			// ��
			for (int nCntColumn = 0; nCntColumn < MAX_ARRAY_COLUMN; nCntColumn++)
			{
				m_VariousPos[nCntLine][nCntColumn] = m_OriginPos + D3DXVECTOR3(BULLETSIZE * nCntColumn, BULLETSIZE * nCntLine, 0.0f);
			}
		}
		// ��̏ꍇ
		else
		{
			// ��
			for (int nCntColumn = 0; nCntColumn < MAX_ARRAY_COLUMN; nCntColumn++)
			{
				m_VariousPos[nCntLine][nCntColumn] = m_OriginPos + D3DXVECTOR3(BULLETSIZE / 2 + BULLETSIZE * nCntColumn, BULLETSIZE * nCntLine, 0.0f);
			}
		}
	}

}

// ----------------------------------------
// �z�u��ԏ���
// ----------------------------------------
void CArray::DeleteState(int nCntLine, int nCntColumn)
{
	m_bState[nCntLine][nCntColumn] = false;
}

// ----------------------------------------
// �ǂݍ��ݏ���
// ----------------------------------------
void CArray::SetState(int nCntline, int nCntColumn,bool bState)
{
	m_bState[nCntline][nCntColumn] = bState;
}

// ----------------------------------------
// �쐬����
// ----------------------------------------
CArray * CArray::Create(CManager::PLAYER playerId, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	// �ϐ��錾
	CArray * pArray;
	// �������̐���(����->��{�N���X,��->�h���N���X)
	pArray = new CArray();
	// �����ʒu�̐ݒ�
	pArray->m_OriginPos = D3DXVECTOR3(pos.x - size.x / 2 + BULLETSIZE / 2, pos.y - size.y / 2 + BULLETSIZE / 2, 0.0f);
	// �������ꂽ�Ƃ��̏����z�u
	pArray->m_BiginPos = pArray->m_OriginPos;
	// ����������
	pArray->Init();
	// �v���C���[�ԍ�
	pArray->m_PlayerId = playerId;
	// ���������I�u�W�F�N�g��Ԃ�
	return pArray;
}

// ----------------------------------------
// �_�E���J�E���g���擾����
// ----------------------------------------
int CArray::GetDownCnt(void)
{
	return m_nDownCnt;
}