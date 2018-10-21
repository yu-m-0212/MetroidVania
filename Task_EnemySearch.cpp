//-------------------------------------------------------------------
//�G�l�~�[�̒T�m��`�\���p
//-------------------------------------------------------------------
#include	"MyPG.h"
#include	"Task_EnemySearch.h"
#include	"Task_Enemy01.h"
namespace  EnemySearch
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "EnemySearch";
		DG::Image_Create(this->imageName, "./data/image/effect.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->imageName);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->recieveBase = this->hitBase;
		
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//�Ǐ]�Ώۂ�����΃A�N�Z�X����
		if (auto tg = this->target.lock())
		{
			this->pos = tg->pos;
			this->state = tg->state;
			this->angle_LR = tg->angle_LR;
			//�G�l�~�[�̌����ō��W���X�V
			if (this->angle_LR == Left)
			{
				this->pos.x -= this->hitBase.w / 2;
			}
			else
			{
				this->pos.x += this->hitBase.w / 2;
			}
		}
		else
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase;
		ML::Box2D src(960, 0, 384, 192);
		//��Ԃɂ���ĐF���ς��
		switch (this->state)
		{
		default:
			src.y = 0;
			break;
		case TargetLost:
			src.y = 192;
			break;
		case Caution:
			src.y = 384;
			break;
		case Bound:
			src.y = 384;
			break;
		}
		//���E���]
		if (this->angle_LR==Left)
		{
			draw.x = -draw.x;
			draw.w = -draw.w;
		}
		draw.Offset(this->pos);
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Rotation(this->res->imageName, 0.0f, ML::Vec2(float(draw.w / 2), float(draw.h / 2)));
		DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.5f, 1.0f, 1.0f, 1.0f));
	}
	//�Ǐ]�Ώۂ�������
	void Object::Set_Target(const weak_ptr<BChara> target_)
	{
		this->target = target_;
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
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
	//���\�[�X�N���X�̐���
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