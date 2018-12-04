//-------------------------------------------------------------------
//ボス（センター）
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Boss_Center.h"
#include  "Task_Boss_Upper_Middle.h"

namespace  Boss_Center
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->name_image = "center";
		DG::Image_Create(this->name_image, "./data/image/Boss01.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->name_image);
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
		this->render2D_Priority[1] = 0.6f;					//描画順
		this->hitBase = ML::Box2D(-92, -46, 184, 92);		//マップとの判定矩形
		this->recieveBase = this->hitBase;					//キャラクタとの判定矩形
		this->speed_chase = 0.06f;							//速度追従
		this->shot = new Shot01::Object();					//メソッド呼び出し
		this->boss = new Boss();							//メソッド呼び出し
		this->vec_shot = ML::Vec2(SPEED_SHOT, 0.0f);		//移動量ショット
		this->hit_shot = ML::Box2D(-8, -8, 16, 16);			//矩形ショット
		this->cnt_move = 0;									//カウンタ行動
		this->interval_shot = -1;							//生成時間ショット
		
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放
		delete this->shot;
		delete this->boss;
		//★引き継ぎタスクの生成
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		//ショット生成用カウンタを進める
		this->cnt_move++;
		//基準となるタスクに追従する
		auto upper_middle = ge->GetTask_One_G<Boss_Upper_Middle::Object>(Boss_Upper_Middle::defGroupName);
		//存在するか確認
		if (nullptr == upper_middle) { return; }
		//目標に向かって移動する
		this->pos.x += this->boss->Chase_Target(this->pos.x, upper_middle->pos.x, this->speed_chase);
		//ショットの生成時間が初期値なら値を入れる
		if (this->interval_shot == -1)
		{
			this->interval_shot = rand() % MAX_INTERVAL_SHOT;
		}
		//生成時間になったらショットを生成する
		if (this->cnt_move == this->interval_shot)
		{
			this->shot->Create_Shot(this->pos, this->vec_shot, this->hit_shot, LIMIT_SHOT, POWER_SHOT, true);
			//カウンタと生成時間をリセットする
			this->cnt_move = 0;
			this->interval_shot = -1;
		}
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		ML::Box2D  src(0, 0, 184, 92);
		DG::Image_Draw(this->res->name_image, draw, src);
	}
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