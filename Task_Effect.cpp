//-------------------------------------------------------------------
//エフェクト
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect.h"
#include  "Task_Player.h"
using namespace ML;
namespace  Effect
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->effectImage = "effectImage";
		DG::Image_Create(this->effectImage,"./data/image/effect.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->effectImage);
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
		this->cntLimit = 0;
		this->animCnt = 0;
		this->moveCnt = 0;
		this->dist = 0.0f;
		this->angle = 0.0f;
		this->center = Vec2(0, 0);
		this->render2D_Priority[1] = 0.3f;
		
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
		//ポーズ
		if (ge->pause) { return; }
		this->moveCnt++;
		this->animCnt++;
		//状態毎の行動
		this->Move();
		//★データ＆タスク解放
		//限界の時間を迎えたら消滅
		if (this->moveCnt >= this->cntLimit) {
			//消滅申請
			this->Kill();
			return;
		}
		//移動
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
		DG::Image_Draw(this->res->effectImage, di.draw, di.src);
	}
	//-------------------------------------------------------------------
	BChara::DrawInfo Object::Anim()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		//デフォルトカラーを宣言
		Color dc(1, 1, 1, 1);
		//各エフェクトをテーブルで用意する
		BChara::DrawInfo imageTable[]
		{
			{Box2D(-96, -64, 192, 128),Box2D(  0,   0, 192, 128),dc},//ストンプ着地の衝撃1	[ 0]
			{Box2D(-96, -64, 192, 128),Box2D(  0, 128, 192, 128),dc},//ストンプ着地の衝撃2	[ 1]
			{Box2D(-96, -64, 192, 128),Box2D(  0, 256, 192, 128),dc},//ストンプ着地の衝撃3	[ 2]
			{Box2D(-96, -64, 192, 128),Box2D(192,   0, 192, 128),dc},//パンチ風切り1			[ 3]
			{Box2D(-96, -64, 192, 128),Box2D(192, 128, 192, 128),dc},//パンチ風切り2			[ 4]
			{Box2D(-96, -64, 192, 128),Box2D(192, 256, 192, 128),dc},//パンチ風切り3			[ 5]
			{Box2D(-96, -64, 192, 128),Box2D(384,   0, 192, 128),dc},//パンチの衝撃1			[ 6]
			{Box2D(-96, -64, 192, 128),Box2D(384, 128, 192, 128),dc},//パンチの衝撃2			[ 7]
			{Box2D(-96, -64, 192, 128),Box2D(384, 256, 192, 128),dc},//パンチの衝撃3			[ 8]
			{Box2D(-96, -64, 192, 128),Box2D(576,   0, 192, 128),dc},//遺体から回復1			[ 9]
			{Box2D(-96, -64, 192, 128),Box2D(576, 128, 192, 128),dc},//遺体から回復2			[10]
			{Box2D(-96, -64, 192, 128),Box2D(576, 256, 192, 128),dc},//遺体から回復3			[11]
			{Box2D(-96, -96, 192, 192),Box2D(768,   0, 192, 192),dc},//エネミー爆散1			[12]
			{Box2D(-96, -96, 192, 192),Box2D(768, 192, 192, 192),dc},//エネミー爆散2			[13]
			{Box2D(-96, -96, 192, 192),Box2D(768, 384, 192, 192),dc} //エネミー爆散3			[14]

		};
		//返す変数を用意
		BChara::DrawInfo  rtv;
		//各アニメーションの際、この変数にanimCntを入れ計算を行う
		int effectCnt = 0;
		//状態ごとの描画処理
		switch (this->state)
		{
		//アニメーションを付ける場合
		//切り替わるフレーム数
		//パターン数
		//effectCntに対象の要素番号を足して返す
		case Unnon:
			break;
		case ImpactPunch:
			effectCnt = this->animCnt / 6;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 6];
			break;
		case StompLanding:
			effectCnt	= this->animCnt / 6;
			effectCnt %= 3;
			rtv = imageTable[effectCnt+0];
			break;
		case Heal:
			effectCnt	= this->animCnt / 8;
			effectCnt %= 3;
			rtv = imageTable[effectCnt+9];
			break;
		case Lose:
			effectCnt = this->animCnt / 8;
			effectCnt %= 3;
			rtv = imageTable[effectCnt + 12];
			break;
		}
		//	向きに応じて画像を左右反転する
		if (this->angle_LR == Left)
		{
			rtv.draw.x = -rtv.draw.x;
			rtv.draw.w = -rtv.draw.w;
		}
		return rtv;
	}
	//呼び出す際に消滅までの時間を指定する
	//引数	：	（消滅までの時間）
	void Object::Set_Limit(const int& limit_)
	{
		this->cntLimit = limit_;
	}
	//状態ごとに行動を指定する
	void Object::Move()
	{
		auto pl = ge->GetTask_One_G<Player::Object>("プレイヤ");
		if (nullptr == pl) { return; }
		switch (this->state)
		{
		default:
			break;
		case StompLanding:
			break;
		case Shoot:
			break;
		case Jumpshoot:
		case Fallshoot:
			break;
		case Heal:
			this->moveVec = pl->moveVec;
			if (pl->CheckFront_LR())
			{
				this->moveVec.x = 0.0f;
			}
			break;
		case Lose:
			break;
		}
	}
	//中心点から広がるエフェクトを呼び出す際、中心からの初期位置を指定する
	//引数	：	（中心点からの初期位置)
	void Object::Set_Dist(const float& dist_)
	{
		this->dist = dist_;
	}
	//中心点から広がるエフェクトの中心からの距離を取得する
	float Object::Get_Dist()
	{
		return this->dist;
	}
	//表示する角度を指定する
	//引数	：	 (ML::ToRadian(角度))
	void Object::Set_Angle(const float& angle_)
	{
		this->angle = angle_;
	}
	//回転の中心を外部から指定する
	//引数	：	（中心座標）
	void Object::Set_Center(const Vec2& center_)
	{
		this->center = center_;
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