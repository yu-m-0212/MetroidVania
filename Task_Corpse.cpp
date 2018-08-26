//-------------------------------------------------------------------
//前回死亡した地点に置かれる遺体
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Corpse.h"
using namespace ML;
namespace  Corpse
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->imageName = "corpseImage";
		DG::Image_Create(this->imageName,"./data/image/chara(仮)01.png");
		DG::Font_Create("fontCorpse", "ＭＳ ゴシック", 16, 32);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		DG::Font_Erase("fontCorpse");
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
		this->render2D_Priority[1] = 0.6f;
		this->hitBase = Box2D(-64, -64, 128, 128);
		this->recieveBase = this->hitBase;
		this->gravity = ML::Gravity(64);			//重力加速度＆時間速度による加算量
		this->maxFallSpeed = 15.0f;					//最大落下速度
		
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
		//上昇中もしくは足元に地面が無い
		if (this->moveVec.y < 0 ||
			this->CheckFoot() == false) {
			this->moveVec.y = min(this->moveVec.y + this->gravity, this->maxFallSpeed);
		}
		//地面に接触している
		else
		{
			this->moveVec.y = 0.0f;
		}
		//移動処理
		this->pos += this->moveVec;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		if(ge->debugMode)
		{
			//情報表示
			Box2D textBox(1450, 50, 500, 500);
			string text =
				"corpse->pos.x = " + to_string(this->pos.x) + "\n" +
				"corpse->pos.y = " + to_string(this->pos.y);
			DG::Font_Draw("fontCorpse", textBox, text, Color(1.0f, 1.0f, 1.0f, 1.0f));
		}
		Box2D draw(-64, -64, 128, 128);
		draw.Offset(this->pos);
		//スクロール対応
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		Box2D  src(1088, 0, 128, 128);
		DG::Image_Draw(this->res->imageName, draw, src);
	}
	//接触時の応答処理（必ず受け身の処理として実装する）
	void Object::Received(BChara* from_, AttackInfo at_)
	{
		//一度だけプレイヤを回復する
		from_->hp = from_->max_Hp;
		//自身を消滅させる
		this->Kill();
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