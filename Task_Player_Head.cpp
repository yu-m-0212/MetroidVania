//-------------------------------------------------------------------
//プレイヤ頭部表示用
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Player_Head.h"

namespace  Player_Head
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "PlayerHeadImg";
		DG::Image_Create(this->imageName, "./data/image/player.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->render2D_Priority[1] = 0.5f;				//描画順
		this->interval_anim_def = 16;					//アニメーション間隔デフォルト
		this->interval_anim_shot = 4;					//アニメーション間隔ショット
		this->angle = 0.0f;								//角度
		this->hitBase= ML::Box2D(-69, -46, 138, 92);	//判定矩形
		this->center_Rotate = ML::Vec2(69, 36);			//回転軸
		this->controllerName = "P1";					//コントローラー宣言
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		this->animCnt++;
		if (ge->pause) { return; }
		this->moveCnt++;
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		if (nullptr == pl) { return; }
		auto in = DI::GPad_GetState(this->controllerName);
		this->angle_LR = pl->angle_LR;
		this->pos = pl->pos;
		//共通の処理
		switch (pl->state)
		{
		default:
			this->angle = atan2(in.RStick.axis.y, in.RStick.axis.x);
			//角度に上下限を設ける場合
			if (this->angle_LR == Right)
			{
				if (ML::ToDegree(this->angle) > 45.0)
				{
					this->angle = ML::ToRadian(45.0f);
				}
				else if (ML::ToDegree(this->angle) < -45.0f)
				{
					this->angle = ML::ToRadian(-45.0f);
				}
			}
			else
			{
				if (in.RStick.axis != ML::Vec2(0, 0))
				{
					this->angle += ML::ToRadian(180.0f);
					if (ML::ToDegree(this->angle) > 45.0f && ML::ToDegree(this->angle) < 180.0f)
					{
						this->angle = ML::ToRadian(45.0f);
					}
					else if (ML::ToDegree(this->angle) < 315.0f &&
						!(ML::ToDegree(this->angle) > 0.0f && ML::ToDegree(this->angle) < 45.0f))
					{
						this->angle = ML::ToRadian(315.0f);
					}
				}
			}
			break;
			//例外処理（頭を動かさない）
		case SlowDown:
		case Landing:
		case PreStomp:
		case LandStomp:
		case AirStomp:
		case StompLanding:
		case Damage:
			break;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		if (nullptr == pl) { return; }
		//無敵中は8フレーム中4フレーム画像を表示しない（点滅する）
		if (pl->time_un_hit > 0) {
			if ((pl->time_un_hit / 4) % 2 == 0) {
				return;
			}
		}
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		//デフォルトの値を用意
		int wide = 138, height = 92;
		//アニメーション
		int anim = 0;
		switch (pl->state)
		{
		default:
			anim = this->animCnt / this->interval_anim_def;
			break;
		case Shoot:
		case Jumpshoot:
		case Fallshoot:
			anim = this->animCnt / this->interval_anim_shot;
		}
		anim %= 4;
		ML::Box2D  src(wide * 16, anim*height, wide, height);
		//	向きに応じて画像を左右反転する
		if (false == this->angle_LR) {
			src.y = src.y + height * 4;
		}
		//ダメージ
		if (pl->time_un_hit>0)
		{
			src.x += wide;
		}
		DG::Image_Rotation(this->res->imageName, this->angle,
			this->center_Rotate);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//リソースクラスの生成
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}