//-------------------------------------------------------------------
//�G�t�F�N�g
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect.h"
using namespace ML;
namespace  Effect
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->effectImage = "effectImage";
		DG::Image_Create(this->effectImage,"./data/image/effect.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->effectImage);
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
		this->cntLimit = 0;
		this->animCnt = 0;
		this->moveCnt = 0;
		this->hitBase = Box2D(-96, -64, 192, 128);//�\����`�͂����ő��삷��
		
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
		this->moveCnt++;
		this->animCnt++;
		//���f�[�^���^�X�N���
		//���E�̎��Ԃ��}���������
		if (this->moveCnt >= this->cntLimit) {
			//���Ő\��
			this->Kill();
			return;
		}
		//�ړ�
		this->pos += this->moveVec;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		BChara::DrawInfo  di = this->Anim();
		di.draw.Offset(this->pos);
		//�X�N���[���Ή�
		di.draw.Offset(-ge->camera2D.x, -ge->camera2D.y);
		DG::Image_Draw(this->res->effectImage, di.draw, di.src);
	}
	//-------------------------------------------------------------------
	BChara::DrawInfo Object::Anim()
	{
		//�f�t�H���g�J���[��錾
		Color dc(1, 1, 1, 1);
		//�e�G�t�F�N�g���e�[�u���ŗp�ӂ���
		BChara::DrawInfo imageTable[]
		{
			{this->hitBase,Box2D(0,  0,this->hitBase.w,this->hitBase.h),dc},//�X�g���v���n�̏Ռ�1[0]
			{this->hitBase,Box2D(0,128,this->hitBase.w,this->hitBase.h),dc},//�X�g���v���n�̏Ռ�2[1]
			{this->hitBase,Box2D(0,256,this->hitBase.w,this->hitBase.h),dc} //�X�g���v���n�̏Ռ�3[2]
		};
		//�Ԃ��ϐ���p��
		BChara::DrawInfo  rtv;
		//�e�A�j���[�V�����̍ہA���̕ϐ���animCnt�����v�Z���s��
		int animCnt = 0;
		//��Ԃ��Ƃ̕`�揈��
		switch (this->state)
		{
		case Unnon:
			break;
		case StompLanding:
			//�X�g���v���n�̏Ռ��A�j���[�V����
			//�؂�ւ��t���[����
			animCnt	= this->animCnt / 6;
			//�p�^�[����
			animCnt %= 3;
			//animCnt�ɑΏۂ̗v�f�ԍ��𑫂��ĕԂ�
			rtv = imageTable[animCnt+0];
			break;
		}
		return rtv;
	}
	//�Ăяo���ۂɏ��ł܂ł̎��Ԃ��w�肷��
	//����	�F	�i���ł܂ł̎��ԁj
	void Object::Set_Limit(const int& limit_)
	{
		this->cntLimit = limit_;
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