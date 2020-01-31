// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// カメラ処理 [camera.h]
// Author : KOKI_NISHIYAMA
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _CAMERA_H_
#define _CAMERA_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"

// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// クラス
//
// ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCamera
{
public:
	/* 列挙型 */
	typedef enum
	{
		TYPE_FOLLOW = 0,
		TYPE_FRONT,
		TYPE_BACK,
		TYPE_RIGHT,
		TYPE_LEFT,
		TYPE_SLASH,
		TYPE_SLASH_NEAR,
		TYPE_SLASH_FAR,
		TYPE_MAX,
	} TYPE;
	/* 構造体 */
	// 読み込み用
	typedef struct
	{
		D3DXVECTOR3 rot;	// 回転量
		D3DXVECTOR2 offset;	// 注視点のオフセット
		float fLengh;		// 注視点と視点の長さ
		float fHeight;		// 注視点と視点の高さ
		int nType;			// タイプ
	} LOAD;
	/* 関数 */
	CCamera();
	~CCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCamera * Create(void);	// 作成
	// 読み込み
	static HRESULT Load(void);
	// 破棄
	static void Unload(void);
	// カメラの情報初期化
	void InfoInit(
		D3DXVECTOR3 const &pos,	// 位置情報
		D3DXVECTOR3 const &rot	// 回転情報
		);
	// カメラ情報取得
	CCamera *Get(void);
	// カメラ設定
	void Set(void);
	void SetPosR(
		D3DXVECTOR3 const & pos,	// 位置情報 
		D3DXVECTOR3 const & rot		// 回転情報 
	);
	void SetType(TYPE type);
	D3DXVECTOR3 GetRot(void);
protected:

private:
	/* 関数 */
	void Rot(void);						// 回転処理
	void Type(void);					// タイプ処理
	/* 変数 */
	D3DXVECTOR3		m_posV;				// 視点
	D3DXVECTOR3		m_posVDest;			// 目的視点
	D3DXVECTOR3		m_posVDiff;			// 目的視点から視点
	D3DXVECTOR3		m_posR;				// 注視点
	D3DXVECTOR3		m_posRDest;			// 目標注視点
	D3DXVECTOR3		m_posRDiff;			// 目的注視点から注視点
	D3DXVECTOR3		m_posU;				// 上方向ベクトル
	D3DXVECTOR3		m_between;			// プレイヤーとの距離 
	D3DXVECTOR3		m_last;				// 
	D3DXVECTOR3		m_move;				// 移動量
	D3DXVECTOR3		m_rot;				// 視点の回転量
	D3DXVECTOR3		m_rotDest;			// 視点の目的回転地点
	D3DXVECTOR3		m_rotDiff;			// 目的回転地点から現在回転地点
	D3DXVECTOR3		m_rotBetween;		// 目的回転地点と現在回転地点の差分
	D3DXVECTOR3		m_rotOrigin;		// 元の回転量
	D3DXMATRIX		m_mtxProjection;	// プロジェクションマトリックス(テレビ視点)
	D3DXMATRIX		m_mtxView;			// ビューマトリックス(カメラマン視点)
	TYPE			m_type;				// タイプ[
	float			m_fHeight;			// 注視点からの視点の高さ
	float			m_fLength;			// 視点と注視点の距離
	static	LOAD	m_load[TYPE_MAX];	// 情報保存
};

#endif
