#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�{�X�i�^�X�N�j
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Boss
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�{�X");	//�O���[�v��
	const  string  defName("�{�X");		//�^�X�N��
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
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	public:
	//�ύX������������������������������������������������������
		enum State_Boss
		{
			Floating,	//�ǂ��ɂ��ڑ����Ă��Ȃ�
			Common,		//�y��ȊO�Ɛڑ����
			Base		//�}�b�v�Ɛڑ����Ă���
		};
	private:
		State_Boss state_boss;	//��ԊǗ�
		float speed_fall;		//���x����
		float max_speed_fall;	//�ő呬�x����
	public:
		//�R���X�g���N�^
		Object();
		//�v�l
		void Think();
		//�s��
		void Move();
		//�U�����󂯂��ہA�Ăяo��
		//����	�F	�i�Ăяo����,�U�����j
		void Recieved(const BChara*, const BChara::AttackInfo&);
		//�A�j���[�V����
		BChara::DrawInfo Anim();
	};
}