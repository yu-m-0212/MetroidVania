//-------------------------------------------------------------------
//�{�X�i�^�X�N�j
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Boss.h"

namespace  Boss
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
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
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}
	//�v�l
	void Object::Think()
	{
		BChara::State nm = this->state; //�Ƃ肠�������̏�Ԃ��w��
										//�v�l�i���́j��󋵂ɉ����ă��[�V������ύX���邱�Ƃ�ړI�Ƃ��Ă���B
										//���[�V�����̕ύX�ȊO�̏����͍s��Ȃ�
										//���[�V�����X�V
		switch (nm)
		{
		default:
			break;
		case Floating:
			break;
		}
		this->UpdateMotion(nm);
	}
	//�s��
	void Object::Move()
	{
		//-----------------------------------------------------------------
		//���[�V�������ɌŗL�̏���
		switch (this->state)
		{
		default:
			break;
		case Floating:
			break;
		}
	}
	//�U�����󂯂��ہA�Ăяo��
	//����	�F	�i�Ăяo����,�U�����j
	void Object::Recieved(const BChara*, const BChara::AttackInfo&)
	{
		this->UpdateMotion(Damage);
	}
	//�A�j���[�V����
	BChara::DrawInfo Object::Anim()
	{
		//�f�t�H���g�̒l��p��
		ML::Color dc(1, 1, 1, 1);
		BChara::DrawInfo imageTable[] = {
			//draw			src
			{ ML::Box2D(0,0,0,0),ML::Box2D(0,0,0,0),dc }	//[0]
		};
		BChara::DrawInfo  rtv;
		//�A�j���J�E���^�̐錾

		switch (this->state)
		{
		default:
		case Floating:
			rtv = imageTable[0];
			break;
		}
		return rtv;
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