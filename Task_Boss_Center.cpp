//-------------------------------------------------------------------
//�{�X�i�Z���^�[�j
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Boss_Center.h"
#include  "Task_Boss_Upper_Middle.h"

namespace  Boss_Center
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->name_image = "center";
		DG::Image_Create(this->name_image, "./data/image/Boss01.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->name_image);
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
		this->render2D_Priority[1] = 0.6f;					//�`�揇
		this->hitBase = ML::Box2D(-92, -46, 184, 92);		//�}�b�v�Ƃ̔����`
		this->recieveBase = this->hitBase;					//�L�����N�^�Ƃ̔����`
		this->speed_chase = 0.06f;							//���x�Ǐ]
		this->shot = new Shot01::Object();					//���\�b�h�Ăяo��
		this->boss = new Boss();							//���\�b�h�Ăяo��
		this->vec_shot = ML::Vec2(SPEED_SHOT, 0.0f);		//�ړ��ʃV���b�g
		this->hit_shot = ML::Box2D(-8, -8, 16, 16);			//��`�V���b�g
		this->cnt_move = 0;									//�J�E���^�s��
		this->interval_shot = -1;							//�������ԃV���b�g
		
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		delete this->shot;
		delete this->boss;
		//�������p���^�X�N�̐���
		if (!ge->QuitFlag() && this->nextTaskCreate) 
		{
			
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//�V���b�g�����p�J�E���^��i�߂�
		this->cnt_move++;
		//��ƂȂ�^�X�N�ɒǏ]����
		auto upper_middle = ge->GetTask_One_G<Boss_Upper_Middle::Object>(Boss_Upper_Middle::defGroupName);
		//���݂��邩�m�F
		if (nullptr == upper_middle) { return; }
		//�ڕW�Ɍ������Ĉړ�����
		this->pos.x += this->boss->Chase_Target(this->pos.x, upper_middle->pos.x, this->speed_chase);
		//�V���b�g�̐������Ԃ������l�Ȃ�l������
		if (this->interval_shot == -1)
		{
			this->interval_shot = rand() % MAX_INTERVAL_SHOT;
		}
		//�������ԂɂȂ�����V���b�g�𐶐�����
		if (this->cnt_move == this->interval_shot)
		{
			this->shot->Create_Shot(this->pos, this->vec_shot, this->hit_shot, LIMIT_SHOT, POWER_SHOT, true);
			//�J�E���^�Ɛ������Ԃ����Z�b�g����
			this->cnt_move = 0;
			this->interval_shot = -1;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw = this->hitBase.OffsetCopy(this->pos);
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		ML::Box2D  src(0, 0, 184, 92);
		DG::Image_Draw(this->res->name_image, draw, src);
	}
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