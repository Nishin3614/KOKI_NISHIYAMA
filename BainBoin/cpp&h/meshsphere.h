//-----------------------------------------------------------------------------
//
// メッシュドーム処理 [meshsphere.h]
// Author : Yamaya Takumu
//
//-----------------------------------------------------------------------------
#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_

#define _CRT_SECURE_NO_WARNINGS

//-----------------------------------------------------------------------------
// インクルードファイル
//-----------------------------------------------------------------------------
#include "Scene.h"


//-----------------------------------------------------------------------------
// マクロ定義
//-----------------------------------------------------------------------------
#define MAX_MESHSPHERE	(20)
#define RADIUS_SIZE		(45.0f)
#define SEPARATE		(15)


//-----------------------------------------------------------------------------
// クラス定義
//-----------------------------------------------------------------------------
class CMeshsphere :public CScene
{
public:
	typedef struct
	{
		D3DXVECTOR3		pos;								//座標
		D3DXVECTOR3		rot;								//回転
		D3DXCOLOR		col;								//色
		D3DXMATRIX		mtxWorldMeshSphere;					//ワールドマトリックス
		float			fRadius;							//半径
		float			fRot;								//各頂点の角度
		float			fRot2;								//各頂点の角度
		int				nHeightBlock;						//高さの分割数
		int				nSeparate;							//円の頂点の個数
		int				nMaxVtx;							//頂点の個数
		int				nMaxIndex;							//インデックスの個数
		int				nMaxPolygon;						//ポリゴンの枚数
		int				nIdxCollisionSphere;				//円判定のID
		bool			bUse;								//視点と注視点の距離
		bool			bDisp;								//写すかどうか
		LPDIRECT3DVERTEXBUFFER9	pVtxBuffMeshSphere = NULL;	//頂点バッファへのポインタ
		LPDIRECT3DINDEXBUFFER9	pIdxBuffMeshSphere = NULL;	//インデックスバッファへのポインタ
	}	MESHSPHERE;
	CMeshsphere();
	~CMeshsphere();

	static CMeshsphere *create(void);
	void Init(void);	// 初期化処理
	void Uninit(void);	// 終了処理
	void Update(void);	// 更新処理
	void Draw(void);	// 描画処理
	static void Load(void);	// 読み込み
	static void Unload(void);	// 破棄
	static int Set(CONST D3DXVECTOR3 pos, const float fRadius);

	static void SetPosition(int nIdxCollisionSphere, D3DXVECTOR3 pos);
	static void DeleteCollision(int nIdxCollisionSphere);
	static void SetRadius(int nIdxCollisionSphere, float fRadius);
private:
	static MESHSPHERE	m_aMeshSphere[MAX_MESHSPHERE];
	static LPDIRECT3DTEXTURE9 m_pTex;
};

#endif
