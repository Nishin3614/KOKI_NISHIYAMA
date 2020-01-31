// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// キャラクター処理 [character.h]
// Author : koki_nishiyama
//
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#define _CRT_SECURE_NO_WARNINGS

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// インクルードファイル
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#include "scene.h"
#include "model.h"

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 前方宣言
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCollision;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// 構造体
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// クラス
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class CCharacter : public CScene
{
public:
	/* 列挙型 */
	// ステータス
	typedef enum
	{
		STATE_NORMAL = 0,
		STATE_BEFORE_AT,
		STATE_ATTACK,
		STATE_AFTER_AT,
		STATE_DAMAGE,
		STATE_DIE
	} STATE;
	// キャラクター
	typedef enum
	{
		CHARACTER_PLAYER = 0,
		CHARACTER_ROTHUMAN,
		CHARACTER_ENEMY,
		CHARACTER_MAX
	} CHARACTER;
	// 当たった時の反応
	typedef enum
	{
		BLUST_STOP = 0,
		BLUST_SLASH,
		BLUST_UP,
		BLUST_NEAR,
		BLUST_MAX
	} BLUST;
	/* 構造体 */
	//-------------------------------------------------------------------------------------------------------------
	// ステータス情報
	// 1ステータス、2オフセット始点、3オフセット終点、4色、5タイプ、6サイズ、7線の個数、8フレーム数
	//-------------------------------------------------------------------------------------------------------------
	typedef struct STATUS
	{
		/* 関数 */
		// 初期化
		STATUS()
		{
			nMaxLife = 0;				// 最大HP
			nMaxMP = 0;					// 最大MP
			nAttack = 0;				// 攻撃力
			nInviTime = 0;				// 無敵時間
			fLength = 0;				// 攻撃の当たり範囲
			State = STATE_NORMAL;		// 現状のステータス
			nCntState = 0;				// カウントステータス
			nLife = 0;					// HP
			nLastLife = 0;				// 目的のHP
			nMP = 0;					// MP
			nLastMP = 0;				// 目的のMP
			fAlpha = 1.0f;				// アルファ値
			bAttack = false;			// 攻撃状態
			bDefense = false;			// 防御状態
			bInvincible = false;		// 無敵状態
		}
		/* 変数 */
		STATE		State;			// 現状のステータス
		int			nCntState;		// カウントステータス
		int			nMaxLife;		// 最大HP
		int			nLife;			// HP
		int			nLastLife;		// 目的のHP
		int			nMaxMP;			// 最大MP
		int			nMP;			// MP
		int			nLastMP;		// 目的のMP
		int			nAttack;		// 攻撃力
		float		fLength;		// 攻撃の当たり範囲
		float		fAlpha;			// アルファ値
		bool		bAttack;		// 攻撃状態
		bool		bDefense;		// 防御状態
		bool		bInvincible;	// 無敵状態
		int			nInviTime;		// 無敵時間
	} STATUS,*PSTATUS;
	/* 関数 */
	CCharacter();
	CCharacter(ACTOR actor);
	~CCharacter();
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	// 必要に応じた動作
	void AplayDamage(
		D3DXVECTOR3 const &pos,
		BLUST		const &bBlust,
		int			const &nDamage
	);			// ダメージ
	
	// 設定
	void SetCharacter(CHARACTER const character);	// キャラクター
	void SetMotion(int const nMotiontype);			// モーション
	void SetPos(D3DXVECTOR3 const &pos);			// 位置
	void SetMove(D3DXVECTOR3 const &move);			// 移動量
	void SetRot(D3DXVECTOR3 const &rot);			// 回転
	void SetAttack(bool bAttack);					// 攻撃状態
	void SetState(STATE const &state);				// 状態
	void SetInvi(bool const bInvi);					// 無敵状態
	// 取得
	CHARACTER	GetCharacter(void);					// キャラクター
	D3DXVECTOR3 &GetPos(void);						// 位置
	D3DXVECTOR3 &GetMove(void);						// 移動情報
	D3DXVECTOR3 &GetRot(void);						// 回転
	D3DXVECTOR3 *GetPartsRot(int const nModelID);	// 親子関係の全回転
	D3DXVECTOR3 * GetPartsPos(int const nModelID);	// 親子関係の全位置
	D3DXMATRIX *GetMatrix(int const nModelID);		// マトリックス
	int GetMotion(void);							// モーション番号
	int GetKeyInfoCnt(void);						// キーカウント
	STATUS &GetStatus(void);						// ステータス
	STATE GetState(void);							// 状態
	bool GetAttack(void);							// 攻撃状態	
	bool GetFloorHeight(void);						// 床の高さ
	// モーションのフレーム情報取得処理
	int GetMaxFrame(
		CHARACTER character,					// キャラクター
		int nMotionID,							// モーションID
		int nNowKeyCnt = -1						// 現在のキーカウント
	);												
	// カメラ情報取得
	int GetCameraType(void);					// カメラ情報取得
	// 作成
	static CCharacter * Create(				
		int const nMaxMotion,
		CHARACTER const character
		);
	// ロード・アンロード
	static void Load(
		CHARACTER const character,		// キャラクター
		int const nMaxMotion,			// 最大モーション
		const char * file_name			// ファイル名
		);			
	static HRESULT LoadStatus(void);	// ステータス情報
	static void UnLoad(
		CHARACTER const character,		// キャラクター
		int const nMaxkey				// 最大キー
		);	
#ifdef _DEBUG
	void Debug(void);
#endif // _DEBUG
protected:
	void FagGravity(void);							// 重力
	void SetRotDest(D3DXVECTOR3 const &rotDest);		// 目標回転量設定
	D3DXVECTOR3 GetRotDest(void);						// 目標回転量取得
	// 仮
	bool				m_bDamage;						// ダメージが当たった判定
private:
	/* 関数 */
	void NextKeyMotion(void);							// 次のモーション
	void Move(void);									// 移動
	void Motion(void);									// モーション
	void AttackCollision(void);							// 攻撃の判定
	void InviTime(void);								// 無敵時間
	void ModelUpdate(void);								// モデルの更新
	// 状態効果
	void State(void);									// 状態
	void NormalState(void);								// 普通の状態
	void Before_ATState(void);							// 攻撃前の状態
	void AttackState(void);								// 攻撃状態
	void After_ATState(void);							// 攻撃後の状態
	void DamageState(void);								// ダメージ状態
	void DieState(void);								// 死の状態

	void NockBack(void);								// ノックバック
	void Dead(void);									// 死
	/* 変数 */
	/* 構造体のスタティックにする */
	static MODEL_ALL	*m_modelAll[CHARACTER_MAX];		// モデル全体の情報
	static CModel_info	*m_model_info[CHARACTER_MAX];	// モデル情報
	static int			m_NumModel[CHARACTER_MAX];		// 最大モデル数
	static STATUS		m_sStatus[CHARACTER_MAX];		// キャラクターすべてのスタータス情報
	CModel 				*m_pModel;						// モデル
	CHARACTER			m_character;					// キャラクター
	D3DXMATRIX			m_mtxWorld;						// 行列
	D3DXVECTOR3			m_pos;							// 位置
	D3DXVECTOR3			m_posold;						// 前の位置
	D3DXVECTOR3			m_move;							// 移動量
	D3DXVECTOR3			m_BlustMove;					// 吹き飛ばし
	D3DXVECTOR3			m_rot;							// 現在回転量
	D3DXVECTOR3			m_rotLast;						// 向きたい方向
	D3DXVECTOR3			m_rotbetween;					// 回転の差分
	D3DXVECTOR3			m_size;							// キャラクターのサイズ
	int					m_nMotiontype;					// モーションタイプ
	int					m_nMotiontypeOld;				// 前回のモーションタイプ
	int					m_nMaxMotion;					// 最大モーション数
	int					m_keyinfoCnt;					// キー情報のカウント
	int					m_nFrame;						// フレームカウント
	STATUS				m_Status = {};					// ステータス情報
	int					m_nIdAttackKey;					// 攻撃用のキーID
	int					m_nSlow;						// スロー
	int					m_nIDWho;						// 敵か味方か
	BLUST				m_blust;						// 吹っ飛び方
	CCollision			*m_pCollision;					// 当たり判定
};

#endif
