//-----------------------------------------------------------------------------
//
// メッシュスフィア処理 [meshsphere.h]
// Author : Nishiyama Koki
//
//-----------------------------------------------------------------------------
#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_

#define _CRT_SECURE_NO_WARNINGS

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "scene.h"

//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define RADIUS_SIZE		(45.0f)
#define SEPARATE		(15)

//-----------------------------------------------------------------------------
// クラス定義
//-----------------------------------------------------------------------------
class CMeshsphere :public CScene
{
public:
	/* 列挙型 */
	typedef enum
	{
		TEXTYPE_WORLD = 0,
		TEXTYPE_STAR,
		TEXTYPE_MAX
	} TEXTYPE;
	/* 構造体 */
	typedef struct
	{
		D3DXVECTOR3		pos;								// 座標
		D3DXVECTOR3		rot;								// 回転
		D3DXCOLOR		col;								// 色
		D3DXMATRIX		mtxWorldMeshSphere;					// ワールドマトリックス
		float			fRadius;							// 半径
		float			fRot;								// 各頂点の角度
		float			fRot2;								// 各頂点の角度
		int				nHeightBlock;						// 高さの分割数
		int				nSeparate;							// 円の頂点の個数
		int				nMaxVtx;							// 頂点の個数
		int				nMaxIndex;							// インデックスの個数
		int				nMaxPolygon;						// ポリゴンの枚数
		int				nIdxCollisionSphere;				// 円判定のID
		int				nMaxFrame;							// 最大フレーム数
		int				nCntFrame;							// フレーム数
		bool			bUse;								// 視点と注視点の距離
		bool			bDisp;								// 写すかどうか
		TEXTYPE			Textype;							//  テクスチャータイプ
		LPDIRECT3DVERTEXBUFFER9	pVtxBuffMeshSphere = NULL;	// 頂点バッファへのポインタ
		LPDIRECT3DINDEXBUFFER9	pIdxBuffMeshSphere = NULL;	// インデックスバッファへのポインタ
	}	MESHSPHERE;
	/* 関数 */
	CMeshsphere();
	~CMeshsphere();

	static CMeshsphere *Create(void);
	void Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG

	static HRESULT Load(void);	// 読み込み
	static void Unload(void);	// 破棄
	static int Set(
		const D3DXVECTOR3 pos,
		const float fRadius,
		const int nMaxFrame = 0
	);

	static void SetPosition(int nIdxCollisionSphere, D3DXVECTOR3 pos);
	static void DeleteCollision(int nIdxCollisionSphere);
	static void SetRadius(int nIdxCollisionSphere, float fRadius);
private:
	/* 関数 */
	void Change(void);	// 変化処理
	void World(MESHSPHERE &meshsphere);	// 世界
	bool Star(MESHSPHERE &meshsphere);	// 星
	/* 変数 */
	static MESHSPHERE	m_aMeshSphere[TEXTYPE_MAX];
	static LPDIRECT3DTEXTURE9 m_pTex[TEXTYPE_MAX];
};

#endif
