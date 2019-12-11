//=============================================================================
//
// カメラ処理 [camera.h]
// Author : KOKI_NISHIYAMA
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#define _CRT_SECURE_NO_WARNINGS

//=============================================================================
// インクルードファイル
//=============================================================================
#include "main.h"

// ------------------------------------------
//
// クラス
//
// ------------------------------------------
class CCamera
{
public:
	/* 関数 */
	CCamera();
	~CCamera();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCamera * Create(void);	// 作成
	// カメラ情報
	CCamera *Get(void);
	// カメラ設定
	void Set(void);
	D3DXVECTOR3 GetRot(void);
protected:

private:
	/* 関数 */
	void Rot(void);
	void PlayerFoll(void);

	/* 変数 */
	D3DXVECTOR3 m_posV;			// 視点
	D3DXVECTOR3 m_posVDest;		// 目的視点
	D3DXVECTOR3 m_posVDiff;		// 目的視点から視点
	D3DXVECTOR3 m_posR;			// 注視点
	D3DXVECTOR3 m_posRDest;		// 目標注視点
	D3DXVECTOR3 m_posRDiff;		// 目的注視点から注視点
	D3DXVECTOR3 m_posU;			// 上方向ベクトル
	D3DXVECTOR3 m_between;		// プレイヤーとの距離 
	D3DXVECTOR3 m_last;			// 
	D3DXVECTOR3 m_move;			// 移動量
	D3DXVECTOR3 m_rot;			// 視点の回転量
	D3DXVECTOR3 m_rotDest;		// 視点の目的回転地点
	D3DXVECTOR3 m_rotDiff;		// 目的回転地点から現在回転地点
	D3DXVECTOR3 m_rotBetween;		// 目的回転地点と現在回転地点の差分
	D3DXMATRIX	m_mtxProjection;	// プロジェクションマトリックス(テレビ視点)
	D3DXMATRIX	m_mtxView;		// ビューマトリックス(カメラマン視点)
	float		m_fLength;		// 視点と注視点の距離
	int			m_nCntTime;		// 正面に向くのにかかる時間
	bool		m_bfront;			// 正面に向くか向かないか

	float		m_fY;			// yの移動量
	float		m_fx;			// xの移動量
	float		m_fz;			// zの移動量
};

#endif
