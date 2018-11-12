#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�G00(�X���C��
//-------------------------------------------------------------------
#include "BChara.h"
#include "Task_Effect.h"

namespace  Enemy01
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�G");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
	//�ύX������������������������������������������������������
		//���L����ϐ��͂����ɒǉ�����
		string imageName;				//���O�摜
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();		//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();			//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
		int interval_Caution;		//�v���C�������E����O�ꂽ��A�ēx�x���ɓ���܂ł̎���
		int interval_Attack;		//�e�𐶐�����Ԋu
		int interval_Flash;			//�_�ŊԊu
		int size_h_resource;		//��e���A�z���C�g�A�E�g����ۂ̊�l
		int add_un_hit;				//�v���C���ɗ^���閳�G����
		float decSpeed;				//���x������
		float max_speed_fall;		//�ő呬�x�~��
		ML::Box2D searchBase;		//���F��`
		ML::Vec2 shot_Init;			//�e�̐������A���g�̍��W�ɑ����l
		Task_Effect::Object* eff;	//���\�b�h�Ăяo��
	public:
		//�R���X�g���N�^
		Object();
		//�v�l&�󋵔��f(�X�e�[�^�X����)
		void Think();	
		//���[�V�����ɑΉ���������
		void Move();		
		//�v���C���𔭌�������x�����[�h�ɓ���
		bool Search_Player();
		//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
		//����	�F	�i�U�����̃|�C���^,�U�����,�^���G���ԁj
		void Received(BChara* from_, AttackInfo at_,const int&);
		//�A�j���[�V��������
		BChara::DrawInfo Anim();	
		//�G�l�~�[�̐���
		//����	�F	�i�������W�j
		void Create_Enemy01(const ML::Vec2&);

		//�A�N�Z�T
		//�T�m��`���擾����
		ML::Box2D Get_Search();
	};
}