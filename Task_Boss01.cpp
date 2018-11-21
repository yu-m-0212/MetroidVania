//-------------------------------------------------------------------
//ボス01
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Boss01.h"

namespace  Task_Boss01
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->name_image = "image_boss01";
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
		this->render2D_Priority[1] = 0.6f;				//描画順
		this->state = Boss_Floating;					//ボス用状態管理
		this->hitBase = ML::Box2D(-92, -46, 184, 92);	//マップとの判定矩形
		this->recieveBase = this->hitBase;				//矩形身体
		this->state = Stand;							//状態管理
		this->interval_shake = 20;						//間隔左右揺れ
		this->speed_shake = 15.0f;						//速度揺れ
		this->std_pos_x = 700.0f;						//基準値横軸座標
		this->gravity = ML::Gravity(SIZE_CHIP);			//重力加速度
		this->max_speed_fall = 2.0f;					//最大落下速度

		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放

		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		this->moveCnt++;
		this->animCnt++;
		this->Think();
		this->Move();
		//移動処理
		this->pos += this->moveVec;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo  di = this->Anim();
		di.draw.Offset(this->pos);
		//スクロール対応
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		//他タスクの回転が干渉しないように修正
		DG::Image_Rotation(this->res->name_image, 0.0f, ML::Vec2(float(this->hitBase.w / 2), float(this->hitBase.h / 2)));
		DG::Image_Draw(this->res->name_image, di.draw, di.src);
	}
	//接触時の応答処理（必ず受け身の処理として実装する）
	//引数	：	(攻撃側,攻撃情報,与無敵時間)
	void Object::Received(BChara* from_, AttackInfo at_, const int&)
	{
		//ダメージ処理
		this->hp -= at_.power;
		this->UpdateMotion(Damage);
	}
	//思考＆状況判断(ステータス決定）
	void  Object::Think()
	{
		BChara::State  nm = this->state;	//とりあえず今の状態を指定

		//思考（入力）や状況に応じてモーションを変更する事を目的としている。
		//モーションの変更以外の処理は行わない
		switch (nm)
		{
		default:
			break;
		case Boss_Floating:
		{
			auto boss = ge->GetTask_One_G<Task_Boss01::Object>(Task_Boss01::defGroupName);
			if (nullptr == boss) { return; }
			ML::Box2D  me = this->recieveBase.OffsetCopy(this->pos);
			//頭部との接触を判定する
			ML::Box2D you = ML::Box2D(boss->recieveBase.x, boss->recieveBase.y - 1,
				boss->recieveBase.w, 1);
			you.Offset(boss->pos);
			if (you.Hit(me))
			{
				nm = Boss_Common;
			}
			if (this->CheckFoot()) { nm = Boss_Base; }
			break;
		}
		case Boss_Common:
			if (!this->CheckFoot()) { nm = Boss_Floating; }
			break;
		case Boss_Base:
			break;
		}
		//モーション更新
		this->UpdateMotion(nm);
	}
	//モーションに対応した処理
	void  Object::Move()
	{
		switch (this->state)
		{
		default:
			break;
		case Boss_Floating:
			//重力加速
			this->moveVec.y = min(this->moveVec.y + this->gravity, this->max_speed_fall);
			break;
		case Boss_Common:
			this->moveVec.y = 0.0f;
			break;
		case Boss_Base:
			this->moveVec.y = 0.0f;
			//制作用仮処理
			this->pos.x = this->std_pos_x + float(sin(this->moveCnt / this->interval_shake) * this->speed_shake);
			break;
		}
	}
	//アニメーション制御
	BChara::DrawInfo  Object::Anim()
	{
		//デフォルトの値を用意
		int x = 184, y = 92;
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] =
		{
			{this->recieveBase,ML::Box2D(x * 0,y * 0,184,92),dc}
		};
		BChara::DrawInfo  rtv;
		//アニメーションカウンタの宣言

		switch (this->state) 
		{
		default:
		case Boss_Floating:
		case Boss_Common:
		case Boss_Base:
			rtv = imageTable[0];
			break;
		}
		return rtv;
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