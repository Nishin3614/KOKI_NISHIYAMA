// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3Dパーティクルの処理[particle.h]
// Author : Nishiyama koki
//
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _3DPARTICLE_H_
#define _3DPARTICLE_H_

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"
#include "Scene.h"

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// マクロ定義
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// クラス定義
// --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class C3DParticle :public CScene
{
public:
	/* 列挙型 */
	// エフェクトタイプ
	typedef enum
	{
		TYPE_NORMAL = 0,
		TYPE_CHARGE,
		TYPE_MAX,
	} TYPE;
	// 初期設定
	typedef enum
	{
		OFFSET_ID_NONE = -1,
		OFFSET_ID_DIRT,				// 土煙
		OFFSET_ID_AWAYSMOKE,		// 吹っ飛びけむり
		OFFSET_ID_EXPLOSIONSMOKE,	// 爆発けむり
		OFFSET_ID_SHOCKWAVE,		// 衝撃波
		OFFSET_ID_GET,				// 
		OFFSET_ID_SPARKS,			// 火花
		OFFSET_ID_STAR,				// 星空
		OFFSET_ID_CROSSLINE,		// クロスライン
		OFFSET_ID_WEAVELINE,		// 編み物ライン
		OFFSET_ID_RANDAMLINE,		// ランダムライン
		OFFSET_ID_CENTERRANDAMLINE,	// 中心ランダムライン
		OFFSET_ID_ONELINE,			// 一つライン
		OFFSET_ID_CONCENTLINE,		// コネクトライン
		OFFSET_ID_MAX,
	} PARTICLE_OFFSET_ID;
	/* 構造体 */
	typedef struct
	{
		int					nFrame;			// フレーム数
		int					nNumber;		// 個数
		int					nEffeTexType;	// エフェクトのテクスチャの種類
		int					nEffeType;		// エフェクトの種類
		C3DParticle::TYPE	type;			// 種類(主に移動の)

		D3DXVECTOR3			*pSpeedRate;	// 速度レート

		D3DXVECTOR3			*pPos;			// 位置のポインタ
		PINTEGER2			pPosXRand;		// 位置X座標ランダム用のポインタ
		PINTEGER2			pPosYRand;		// 位置Y座標ランダム用のポインタ
		PINTEGER2			pPosZRand;		// 位置Y座標ランダム用のポインタ


		float				*pfLength;		// 原点からの距離のポインタ
		PINTEGER2			pnLengthRand;	// 原点からの距離ランダム用のポインタ

		D3DXCOLOR			*pCol;			// 色のポインタ
		bool				bRedRand;		// 赤ランダム
		bool				bGreenRand;		// 緑ランダム
		bool				bBlueRand;		// 青ランダム
		bool				bAlphaRand;		// 透明度ランダム

		D3DXVECTOR2			*pSize;			// サイズ
		PINTEGER2			pSizeXRand;		// サイズxランダム用のポインター
		PINTEGER2			pSizeYRand;		// サイズyランダム用のポインター

		D3DXVECTOR3			*pRot;			// 回転のポインター
		PINTEGER2			pnAngleRand;	// 角度のランダム用のポインタ

		float				*pfSpeed;		// 速度のポインタ
		PINTEGER2			pnSpeedRand;	// 速度のランダム用のポインタ

		int					*pnLife;		// ライフのポインタ
		PINTEGER2			pnLifeRand;		// ライフのランダム用のポインタ
	} PARTICLE_OFFSET;

	/* 関数 */
	C3DParticle();
	~C3DParticle();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	static C3DParticle * Create(PARTICLE_OFFSET_ID OffsetID, CONST D3DXVECTOR3 origin);
	static HRESULT Load(void);
	static void Unload(void);
	void SetParticle(PARTICLE_OFFSET_ID OffsetID);
	void SetOrigin(CONST D3DXVECTOR3 &Origin);

private:
	/* 関数 */
	void Update_Col(void);		// カラー
	void Update_Size(void);		// サイズ
	void Update_Life(void);		// ライフ
	void Update_Speed(void);	// スピード
	void Update_Angle(void);	// 角度
	void Update_Length(void);	// 長さ
	void Update_Pos(void);		// 位置
	void Update_Move(void);		// 移動量

	/* 変数 */
	static PARTICLE_OFFSET	m_ParticleOffset[OFFSET_ID_MAX];	// パーティクルのオフセット
	PARTICLE_OFFSET_ID		m_offsetID;							// オフセットID
	int						m_nFlameCount;						// フレームのカウント
	D3DXVECTOR3				m_Origin;							// 原点
};

#endif
