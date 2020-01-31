// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// 3Dエフェクトの処理[effect.h]
// Author : Nishiyama koki
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _3DEFFECT_H_
#define _3DEFFECT_H_

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// マクロ定義
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define EFFECT_MAX			(10000)						// 最大数
#define EFFECT_NUM_VERTEX	(4)							// 頂点数

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// クラス定義
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CRenderer;
class C3DEffect :public CScene
{
public:
	/* 列挙型 */
	typedef enum
	{
		EFFECT_TYPE_NONE = -1,	// なし
		EFFECT_TYPE_EXPLOSION,	// 爆発
		EFFECT_TYPE_SPARK,		// 火花
		EFFECT_TYPE_SMOKE,		// 煙
		EFFECT_TYPE_LINE,		// 線
		EFFECT_TYPE_MAX			// 最大数
	} EFFECT_TYPE;

	/* 構造体 */
	// エフェクトプリミティブ
	typedef struct
	{
		D3DXVECTOR3		pos;			// 位置
		D3DXVECTOR3		rot;			// 回転量
		D3DXVECTOR3		move;			// 移動量
		D3DXCOLOR		col;			// 色
		D3DXMATRIX		mtxWorld;		// ワールドマトリックス
		D3DXMATRIX		mtxView;		// マトリックスビュー
		//float			fRadius;		// 半径
		D3DXVECTOR2		size;			// サイズ
		D3DXVECTOR2		sizeValue;		// サイズの変化値
		float			fAngle;			// 角度
		float			fAlphaValue;	// アルファ値の変化値
		int				nLife;			// 持ち時間
		int				nTexType;		// テクスチャの種類
		bool			bUse;			// 使用しているかどうか
		EFFECT_TYPE		EffectType;		// エフェクトの種類
	} EFFECT;
	// ファイルタグ
	typedef struct
	{
		char string[64];
	} FILETAG, *PFILETAG;
	// テクスチャー情報
	typedef struct
	{
		LPDIRECT3DTEXTURE9 pTexture;	// テクスチャ情報
	} TEXTURE_INFO;

	/* 関数 */
	C3DEffect();
	~C3DEffect();
	static HRESULT Load(void);
	static void Unload(void);
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static C3DEffect *Create(void);
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
	// 初期化
	void InitValues(void);
	// 頂点作成
	HRESULT MakeVertex(LPDIRECT3DDEVICE9 pDevice);
	// 頂点サイズ設定
	static void SetVartexSize(
		VERTEX_3D *pVtx,
		EFFECT *pEffect = NULL,
		const int *nIndex = NULL
	);
	// 頂点カラー設定
	static void SetVetexColor(
		VERTEX_3D *pVtx,
		EFFECT *pEffect = NULL, 
		const int *nIndex = NULL
	);
	// 消去
	void Delete(int nCntIndex);
	// 全体の設定
	static void Set3DEffect(
		EFFECT_TYPE EffectType,
		int nTexType,
		D3DXVECTOR3 pos,
		D3DXVECTOR3 rot,
		D3DXVECTOR3 move,
		D3DXCOLOR col,
		D3DXVECTOR2 size,
		int nLife,
		D3DXVECTOR2 sizeValue = D3DVECTOR2_ZERO
	);
private:
	/* 関数 */
	// テクスチャーの読み込み
	static HRESULT LoadTextureFile(void);
	bool UpdateLife(VERTEX_3D *pVtx, EFFECT *pEffect);
	bool UpdateVetex(VERTEX_3D *pVtx, EFFECT *pEffect);
	void UpdateMove(VERTEX_3D *pVtx, EFFECT *pEffect);

	/* メンバ変数 */
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;					// バッファ情報
	static TEXTURE_INFO				*m_pTexInfo;				// テクスチャ情報
	static EFFECT					m_aEffect[EFFECT_MAX];		// エフェクト情報
	static const float				m_cfBaseAngle;				// 基本角度
	static int						m_nNumTextureMax;			// テクスチャの最大数
};

#endif