//*************************************************************************************************************
//
// 3Dパーティクルの処理[particle.h]
// Author : Sekine Ikuto
//
//*************************************************************************************************************
#ifndef _3DPARTICLE_H_
#define _3DPARTICLE_H_

//-------------------------------------------------------------------------------------------------------------
// インクルードファイル
//-------------------------------------------------------------------------------------------------------------
#include "main.h"
#include "Scene.h"

//-------------------------------------------------------------------------------------------------------------
// マクロ定義
//-------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------
// クラス定義
//-------------------------------------------------------------------------------------------------------------
class C3DParticle :public CScene
{
public:
	typedef enum
	{
		TYPE_NORMAL = 0,
		TYPE_CHARGE,
		TYPE_MAX,
	} TYPE;

	typedef enum
	{
		OFFSET_ID_NONE = -1,
		OFFSET_ID_EXPLOSION,
		OFFSET_ID_EXPLOSIONWAVE,
		OFFSET_ID_GET,
		OFFSET_ID_MAX,
	} PARTICLE_OFFSET_ID;

	typedef struct
	{
		int					nFrame;				// フレーム数
		int					nNumber;			// 個数
		int					nEffeTexType;		// エフェクトのテクスチャの種類
		int					nEffeType;			// エフェクトの種類
		C3DParticle::TYPE	type;				// 種類(主に移動の)

		D3DXVECTOR3			*pSpeedRate;			// 速度レート

		D3DXVECTOR3			*pPos;				// 位置のポインタ
		PINTEGER2			pPosXRand;			// 位置X座標ランダム用のポインタ
		PINTEGER2			pPosYRand;			// 位置Y座標ランダム用のポインタ
		PINTEGER2			pPosZRand;			// 位置Y座標ランダム用のポインタ

		float				*pfLength;			// 原点からの距離のポインタ
		PINTEGER2			pnLengthRand;		// 原点からの距離ランダム用のポインタ

		D3DXCOLOR			*pCol;				// 色のポインタ

		float				*pfRadius;			// 半径のポインタ
		PINTEGER2			pnRadiusRand;		// 半径のランダム用のポインタ

		float				*pfRadiusValue;	// 半径の変化量のポインタ

		float				*pfAngle;			// 角度のポインタ
		PINTEGER2			pnAngleRand;		// 角度のランダム用のポインタ

		float				*pfSpeed;			// 速度のポインタ
		PINTEGER2			pnSpeedRand;		// 速度のランダム用のポインタ

		int					*pnLife;			// ライフのポインタ
		PINTEGER2			pnLifeRand;		// ライフのランダム用のポインタ
	} PARTICLE_OFFSET;



	C3DParticle();
	~C3DParticle();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static C3DParticle * Create(PARTICLE_OFFSET_ID OffsetID, CONST D3DXVECTOR3 origin);
	void SetParticle(PARTICLE_OFFSET_ID OffsetID);
	static void Load(void);
	static void Unload(void);

	void SetOrigin(CONST D3DXVECTOR3 &Origin);

private:
	static PARTICLE_OFFSET	m_ParticleOffset[OFFSET_ID_MAX];	// パーティクルのオフセット
	PARTICLE_OFFSET_ID		m_offsetID;							// オフセットID
	int						m_nFlameCount;						// フレームのカウント
	D3DXVECTOR3				m_Origin;							// 原点
};

#endif
