//-------------------------------------------------------------------
//�O�񎀖S�����n�_�ɒu�������
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Corpse.h"
using namespace ML;
namespace  Corpse
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->corpseImage = "corpseImage";
		DG::Image_Create(this->corpseImage,"./data/image/Player.png");
		this->uiImage = "uiImage";
		DG::Image_Create(this->uiImage, "./data/image/ui.png");
		DG::Font_Create("fontCorpse", "�l�r �S�V�b�N", 16, 32);
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->corpseImage);
		DG::Image_Erase(this->uiImage);
		DG::Font_Erase("fontCorpse");
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
		this->render2D_Priority[1] = 0.6f;
		this->hitBase = Box2D(-48, -24, 96, 48);
		this->recieveBase = this->hitBase;
		this->gravity = ML::Gravity(CHIP_SIZE);		//�d�͉����x�����ԑ��x�ɂ����Z��
		this->max_speed_fall = 15.0f;				//�ő嗎�����x
		this->transparency = 1.0f;					//�`�擧���x
		
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
		//�㏸���������͑����ɒn�ʂ�����
		if (this->moveVec.y < 0 ||
			this->CheckFoot() == false) {
			this->moveVec.y = min(this->moveVec.y + this->gravity, this->max_speed_fall);
		}
		//�n�ʂɐڐG���Ă���
		else
		{
			this->moveVec.y = 0.0f;
		}
		//�ړ�����
		this->pos += this->moveVec;
		//���ł��J�n����
		if (this->Get_Flag_Erase())
		{
			this->transparency -= 0.01f;
		}
		//�����x�����ȉ��ɂȂ����Ƃ����ł���
		if (this->transparency < 0.0f)
		{
			this->Kill();
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		if(ge->debugMode)
		{
			//���\��
			Box2D textBox(1450, 0, 500, 75);
			string text =
				"corpse->pos.x = " + to_string(this->pos.x) + "\n" +
				"corpse->pos.y = " + to_string(this->pos.y);
			DG::Image_Draw(this->res->uiImage, textBox, ML::Box2D(32, 0, 32, 32), ML::Color(0.7f, 0.0f, 0.0f, 0.0f));
			DG::Font_Draw("fontCorpse", textBox, text, Color(1.0f, 1.0f, 1.0f, 1.0f));
		}
		Box2D draw(-69, -69, 138, 138);
		//�\�������̋t�]
		if (false == this->angle_LR)
		{
			draw.x = -draw.x;
			draw.w = -draw.w;
		}
		draw.Offset(this->pos);
		//�X�N���[���Ή�
		draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		Box2D  src(2484, 0, 138, 138);
		DG::Image_Rotation(this->res->corpseImage, 0.0f,
			ML::Vec2(float(this->hitBase.w / 2), float(this->hitBase.h / 2)));
		DG::Image_Draw(this->res->corpseImage, draw, src,ML::Color(this->transparency,1.0f,1.0f,1.0f));
	}
	//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
	void Object::Received(BChara* from_, AttackInfo at_)
	{
		//��x�����v���C�����񕜂���
		if (!this->Get_Flag_Erase())
		{
			from_->hp = from_->max_Hp;
		}
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