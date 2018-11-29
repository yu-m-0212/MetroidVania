//-------------------------------------------------------------------
//ボス（タスク）
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Boss.h"

namespace  Boss
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
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
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
	}
	//思考
	void Object::Think()
	{
		BChara::State nm = this->state; //とりあえず今の状態を指定
										//思考（入力）や状況に応じてモーションを変更することを目的としている。
										//モーションの変更以外の処理は行わない
										//モーション更新
		switch (nm)
		{
		default:
			break;
		case Floating:
			break;
		}
		this->UpdateMotion(nm);
	}
	//行動
	void Object::Move()
	{
		//-----------------------------------------------------------------
		//モーション毎に固有の処理
		switch (this->state)
		{
		default:
			break;
		case Floating:
			break;
		}
	}
	//攻撃を受けた際、呼び出す
	//引数	：	（呼び出し元,攻撃情報）
	void Object::Recieved(const BChara*, const BChara::AttackInfo&)
	{
		this->UpdateMotion(Damage);
	}
	//アニメーション
	BChara::DrawInfo Object::Anim()
	{
		//デフォルトの値を用意
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] = {
			//draw			src
			{ ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),dc }	//[0]
		};
		BChara::DrawInfo  rtv;
		//アニメカウンタの宣言

		switch (this->state)
		{
		default:
		case Floating:
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