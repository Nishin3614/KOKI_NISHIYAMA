// ------------------------------------------------------------------------------------------
//
// �I���R�����g�w�b�_�[�����̐���[selectcoment.h]
// Author : Koki Nishiyama
//
// ------------------------------------------------------------------------------------------
#ifndef _SELECTCOMENT_H_
#define _SELECTCOMENT_H_	 // �t�@�C������������߂�

// ------------------------------------------------------------------------------------------
// �C���N���[�h�t�@�C��
// ------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------
// �}�N����`
// ------------------------------------------------------------------------------------------
#define SELECTCOMENT_ON_Y (320)		// ��
#define SELECTCOMENT_MAX (2)			// �摜��

// ------------------------------------------------------------------------------------------
// �񋓌^
// ------------------------------------------------------------------------------------------
// ---------�^�C�v---------- //
typedef enum
{
	SELECTCOMMENTTYPE_YES = 0,
	SELECTCOMMENTTYPE_NO,
	SELECTCOMMENTTYPE_COME,
	SELECTCOMMENTTYPE_MAX
} SELECTCOMMENTTYPE;

// ---------�V�[��---------- //
typedef enum
{
	SELECTCOMMENTSCENE_TUTORIALS = 0,
	SELECTCOMMENTSCENE_CONTINUE,
	SELECTCOMMENTSCENE_AREA,
	SELECTCOMMENTSCENE_MAX
} SELECTCOMMENTSCENE;

// ------------------------------------------------------------------------------------------
// �\���̒�`
// ------------------------------------------------------------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR	col;		// �J���[
	int nType;				// ���
	int nCntCol;			// �J���[�J�E���g
	bool bUse;				// �g�p���Ă��邩�ǂ���
} SELECTCOMENT;

// ------------------------------------------------------------------------------------------
// �v���g�^�C�v�錾
// ------------------------------------------------------------------------------------------
// ��{�v���g�^�C�v	//
void InitSelectComment(void);
void UninitSelectComment(void);
void UpdateSelectComment(void);
void DrawSelectComment(void);

void SceneSelectComment(int nScene);
int GetSelectCommentSelect(void);
#endif