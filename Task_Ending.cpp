//-------------------------------------------------------------------
//�G���f�B���O
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ending.h"
#include  "Task_Title.h"
#include  "Task_Display_Effect.h"

namespace  Ending
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->name_image = "name_image";
		DG::Image_Create(this->name_image, "./data/image/Staff_Roll.png");
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
		this->render2D_Priority[1] = 0.1f;												//�`�揇
		this->transition = false;														//�t���O��ʑJ��
		this->cnt_transition = 0;														//�J�E���^��ʑJ��
		this->time_kill_this = 150;														//���Ԏ��g�����ł�����
		this->size_image_w = int(DG::Image_Size(this->res->name_image).x);				//�T�C�Y���\�[�X��
		this->size_image_h = int(DG::Image_Size(this->res->name_image).y);				//�T�C�Y���\�[�X����
		this->pos_staff_roll = 0.0f;													//�\���ʒu�X�^�b�t���[��
		this->add_pos_staff_roll = 3.0f;												//���Z�ʕ\���ʒu�X�^�b�t���[��
		//���^�X�N�̐���
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

		//�������p���^�X�N�̐���
		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			auto  nextTask = Title::Object::Create(true);
		}
		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		//��ʂ���~����܂ł̏���
		if (this->pos_staff_roll > int(-this->size_image_h + ge->screenHeight))
		{
			this->pos_staff_roll -= this->add_pos_staff_roll;
		}
		//��ʑJ��
		if (in.ST.down)
		{
			this->transition = true;
		}
		if (this->transition)
		{
			if (this->cnt_transition == 0)
			{
				auto display_effect =
					ge->GetTask_One_G<Display_Effect::Object>(Display_Effect::defGroupName);
				display_effect->Create_Display_Effect(0);
			}
			this->cnt_transition++;
		}
		if (this->cnt_transition >= this->time_kill_this)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D draw(0, 0,int(this->size_image_w),int(this->size_image_h));
		draw.Offset(ML::Vec2(0, this->pos_staff_roll));
		ML::Box2D  src(0, 0, int(this->size_image_w), int(this->size_image_h));
		DG::Image_Draw(this->res->name_image, draw, src);
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