#pragma once
#include "GameEngine_Ver3_7.h"
#include "Dm2008_Ver1_3.h"
#include <fstream>
#include <sstream>

//ゲーム内全域で使用する構造体などを定義する
//-----------------------------------------------------------------------------------------------
#define SIZE_CHIP 32						//マップチップサイズ
#define SIZE_MAP_W 300						//サイズマップ横
#define SIZE_MAP_H 300						//サイズマップ縦
#define VOLUME_ALL_GAME 1000				//ゲーム全体の基礎音量
//音量管理マネージャーを要検討
//BGM
#define VOLUME_BGM_BUBBLE_TITLE 850
#define VOLUME_BGM_MAIN_GAME 850			//本編BGM音量
#define VOLUME_BGM_BOSS 800					//ボスBGM音量
//SE
//プレイヤ
#define VOLUME_SE_HIT_SHOT_PLAYER 850		//プレイヤショットヒット
#define VOLUME_SE_DAMAGE_PLAYER 900			//被弾
#define VOLUME_SE_SHOT 900					//ショット
#define VOLUME_SE_LADING_PLAYER 900			//着地
#define VOLUME_SE_REFLECT_SHOT 900			//ショット反射
#define VOLUME_SE_BARRIER 950				//バリアアビリティ
//エネミー
#define VOLUME_SE_SHOT_ENEMY 900			//エネミーショット
//ボス
#define VOLUME_SE_HIT_REFLECTION 900		//ボスが反射弾を受けた際の音量
#define VOLUME_SE_APPEAR_UNDER 900			//ボス登場
//-----------------------------------------------------------------------------------------------
namespace  MyPG
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//----------------------------------------------
	//カメラ基本形
	class Camera
	{
		Camera( ){ }
		Camera(
				const ML::Vec3&		tg_,	//	被写体の位置
				const ML::Vec3&		pos_,	//	カメラの位置
				const ML::Vec3&		up_,	//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
				float				fov_,	//	視野角
				float				np_,	//	前クリップ平面（これより前は映らない）
				float				fp_,	//	後クリップ平面（これより後ろは映らない）
				float				asp_);	//	アスペクト比（画面の比率に合わせる　横÷縦）			
	public:
	//	ビュー情報（方向関連）
		ML::Vec3 target;			//	被写体の位置
		ML::Vec3 pos;				//	カメラの位置
		ML::Vec3 up;				//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
	//	射影情報（視野範囲関連）
		float fov;					//	視野角
		float nearPlane;			//	前クリップ平面（これより前は映らない）
		float forePlane;			//	後クリップ平面（これより後ろは映らない）
		float aspect;				//	アスペクト比（画面の比率に合わせる　横÷縦）
	//	行列情報
		ML::Mat4x4  matView, matProj;
		~Camera( );
		typedef shared_ptr<Camera> SP;
	//	カメラを生成する
		static SP Create(
				const ML::Vec3&		tg_,	//	被写体の位置
				const ML::Vec3&		pos_,	//	カメラの位置
				const ML::Vec3&		up_,	//	カメラの上方向を示すベクトル（大体Ｙ＋固定）
				float				fov_,	//	視野角
				float				np_,	//	前クリップ平面（これより前は映らない）
				float				fp_,	//	後クリップ平面（これより後ろは映らない）
				float				asp_);	//	アスペクト比（画面の比率に合わせる　横÷縦）	
	//	カメラの設定
		void UpDate( );
	};
	//----------------------------------------------
	class MyGameEngine : public GameEngine
	{
	public:
		MyGameEngine( );
		//ゲームエンジンに追加したものの初期化と開放
		bool Initialize(HWND wnd_);
		~MyGameEngine( );
		//ゲームエンジンに追加したもののステップ処理
		void UpDate( );

		//3DPG1対応により追加
		//2D描画環境のデフォルトパラメータ設定
		void Set2DRenderState(DWORD l_);
		//3D描画環境のデフォルトパラメータ設定
		void Set3DRenderState(DWORD l_);

		//カメラの傾きをＹ軸に対する角度で取得する
		//X軸＋方向を0度としたカメラの向きを返す。
		float GetCameraDirectionY_XPlus0(UINT cn_);


	//ゲームエンジンに追加したいものは下に加える
	//----------------------------------------------
		MyPG::Camera::SP	camera[4];	//カメラ
		ML::Box2D	camera2D;			// ２Ｄスクロール制御用
		bool clear;						//クリアフラグ
		bool failure;					//ミスフラグ
		bool pause;						//ポーズフラグ
		ML::Vec2 center;				//画面中心
	//----------------------------------------------
	};
}
extern MyPG::MyGameEngine* ge;


