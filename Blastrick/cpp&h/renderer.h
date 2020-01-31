// ------------------------------------------
//
// �����_�����O�����̐���[renderer.h]
// Author : Koki Nishiyama
//
// ------------------------------------------

#ifndef _RENDERER_H_
#define _RENDERER_H_	 // �t�@�C������������߂�

// ------------------------------------------
//
// �C���N���[�h�t�@�C��
//
// ------------------------------------------
#include "main.h"

// ------------------------------------------
//
// �}�N���֐�
//
// ------------------------------------------

// ------------------------------------------
//
// �O���錾
//
// ------------------------------------------
class CDebugproc;
class CCamera;
class CLight;	

// ------------------------------------------
//
// �N���X
//
// ------------------------------------------
class CRenderer
{
public:
	/* �񋓌^ */
	typedef enum
	{
		TYPE_NORMAL = 0,
		// �J�����O�m�[�}��
		TYPE_CULLNORMAL,
		// �J�����O(��)
		TYPE_CULLBACK,
		// �J�����O�Ȃ�
		TYPE_CULLNULL,
		// �ʏ퍇��
		TYPE_NORMALMIX,
		// ���Z����
		TYPE_ADDMIX,
		// ���Z����
		TYPE_DECMIX,
		// ���C�e�B���O�I��
		TYPE_RIGHTINGON,
		// ���C�e�B���O�I�t
		TYPE_RIGHTINGOFF,
		// Z�o�b�t�@�I��
		TYPE_ZBUFFON,
		// Z�o�b�t�@�I�t
		TYPE_ZBUFFOFF,
		// 3D�G�t�F�N�g�p�I��
		TYPE_3DEFFECT_ON,
		// 3D�G�t�F�N�g�p�I�t
		TYPE_3DEFFECT_OFF,
		TYPE_MAX
	} TYPE;

	/* �֐� */
	CRenderer();
	~CRenderer();
	HRESULT  Init(HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �`����
	void SetType(TYPE type = TYPE_NORMAL);
	
	// ���擾
	LPDIRECT3DDEVICE9 GetDevice(void);	// �f�o�C�X
	CCamera * GetCamera(void);			// �J����
	CLight * GetLight(void);			// ���C�g
protected:

private:
	/* �ϐ� */
	LPDIRECT3D9 m_pD3D;					// Direct3D�I�u�W�F�N�g�̃|�C���^
	LPDIRECT3DDEVICE9 m_pDevice;		// Direct3D�f�o�C�X�̃|�C���^
	CCamera * m_pCamera;				// �J�������
	CLight * m_pLight;					// ���C�g���
	static bool m_bDebug;				// �f�o�b�O���[�h
#ifdef _DEBUG
	static CDebugproc * m_debugproc;	// �f�o�b�O�\��
	void DrawFPS(void);					// FPS�֐�
#endif	
};

#endif // !_RENDERER_H_