//-------------------------------------------------------------------
//�J����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Sprite.h"
#include  "Task_Map2D.h"
#include  "Task_Player.h"

namespace  Sprite
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName = "SpriteImg";
		DG::Image_Create(this->imageName, "./data/image/�d��.png");
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
		this->render2D_Priority[1] = 0.5f;	//�`�揇
		this->controllerName = "P1";		//�R���g���[���[��������
		this->speed_to_Vec = 0.05f;			//�ڕW�ւ̈ړ���
		this->dist_to_Vec = 200.0f;			//�v���C������̋���

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
		//�|�[�Y
		if (ge->pause) { return; }
		//�v���C���Ƃ̑��΋������擾
		//�Ώۂ����݂��邩�m�F���Ă���A�N�Z�X
		if (auto tg = this->target.lock())
		{
			ML::Vec2  toVec = tg->pos - this->pos;
			//�E�X�e�B�b�N�̌����ɍ��킹�Ď����̈ړ����ύX
			auto in = DI::GPad_GetState(this->controllerName);
			float angle = atan2(in.RStick.axis.y, in.RStick.axis.x);
			//�X�e�B�b�N�����͂���Ă���ꍇ�̂�
			if (!angle == 0.0f)
			{
				toVec += ML::Vec2(cos(angle)*this->dist_to_Vec, sin(angle)*this->dist_to_Vec);
			}
			//�^�[�Q�b�g�ɋ߂Â�
			this->pos += toVec * this->speed_to_Vec;
		}
		//�J�����̈ʒu���Ē���
		{
			//��������ʂ̉����ɒu�����i����͉�ʒ����j
			int  px = ge->camera2D.w / 2;
			int  py = ge->camera2D.h / 2;
			//��������ʒ����ɒu�������̃J�����̍�����W�����߂�
			int  cpx = int(this->pos.x) - px;
			int  cpy = int(this->pos.y) - py;
			//�J�����̍��W���X�V
			ge->camera2D.x = cpx;
			ge->camera2D.y = cpy;
		}
		//�}�b�v�O���ʂ��Ȃ��悤�ɂ��钲������
		auto   map = ge->GetTask_One_GN<Map2D::Object>("�t�B�[���h", "�}�b�v");
		if (nullptr != map) {
			map->AjastCameraPos();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//�f�o�b�O���[�h�̂ݕ\��
		if (!ge->debugMode) { return; }
		ML::Box2D  draw(-16, -16, 32, 32);
		draw.Offset(this->pos);
		ML::Box2D  src(0, 0, 32, 32);

		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->imageName, draw, src, ML::Color(0.5f, 1, 1, 1));
	}
	//�Ǐ]�Ώۂ��w��
	void Object::Set_Target(const weak_ptr<BChara> pl_)
	{
		this->target = pl_;
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