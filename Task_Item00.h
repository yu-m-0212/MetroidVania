#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�A�C�e��00(�̗͏���A�b�v)
//-------------------------------------------------------------------
#include "BChara.h"
#include "Task_Tutorials.h"
#include "Task_Effect.h"

namespace  Item00
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�A�C�e��");		//�O���[�v��
	const  string  defName("NoName");			//�^�X�N��
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
		//���L����ϐ��͂����ɒǉ�����
		string imageName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	//�ύX�s����������������������������������������������������
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
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	//�ύX������������������������������������������������������
	private:
		int add_Hp;						//�v���C����HP������
		int limit_message;				//�������ԃ��b�Z�[�W
		ML::Vec2 center;				//��ʒ���
		Tutorials::Object* tutorials;	//�|�C���^�`���[�g���A��
	public:
		//�R���X�g���N�^
		Object();
		//�A�j���[�V��������
		BChara::DrawInfo Anim();
		//�ڐG���̉��������i�K���󂯐g�̏����Ƃ��Ď�������j
		//����	�F	�i�U�����̃|�C���^,�U�����,�^���G���ԁj
		void Recieved(BChara* from_, AttackInfo at_, const int&);
		//�A�C�e��00�̐���
		//����	�F	�i�������W�j
		void Create_Item00(const ML::Vec2&);
	};
}