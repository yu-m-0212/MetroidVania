#include "DI2014_Ver3_4.h"

namespace DI_
{
	namespace  KS {
		enum Type {	//�L�[���	
			Err,
			On,		//�����Ă���Ԓ�
			Off,	//�����Ă���Ԓ�
			Down,	//����������i�P�t���[���̂ݔ����j
			Up,		//����������i�P�t���[���̂ݔ����j
		};
	}
	struct  StickInfo {
		float                angle;		//�A�i���O�X�e�B�b�N�̊p�x�iRT0�x�@DW90�x�̃��W�A���j
		float                volume;	//�A�i���O�X�e�B�b�N�̋���0.0f�`1.0f
		ML::Vec2             axis;		//�����̗�-1.0f�`+1.0f
		KS::Type       xm;		//�{�^���Ɍ����Ă���ԁ@��
		KS::Type       xp;		//�{�^���Ɍ����Ă���ԁ@�E
		KS::Type       ym;		//�{�^���Ɍ����Ă���ԁ@��
		KS::Type       yp;		//�{�^���Ɍ����Ă���ԁ@��
	};
	namespace  But {
		enum Type {	//�v���O������Ŏg�p�ł���L�[�̎��
					//��,
			B1,
			//�~
			B2,
			//��
			B3,
			//��
			B4,
			L1, R1, L2, R2,
			ST, SE, L3, R3,
			//��
			L,		//�n�b�g�X�C�b�`
					//��
			R,
			//��
			U,
			//��
			D,
			S0, S1, S2, S3, S4, S5, S6, S7, S8, S9,
			KMax		//�@�{�^������
		};
	}
	namespace  Axis {
		enum Type {	//�A�i���O�X�e�B�b�N�Ƀ{�^���Ɠ��l�ɃA�N�Z�X�邷��ꍇ�Ɏg�p
					//��
			L,		//�n�b�g�X�C�b�`
					//��
			R,
			//��
			U,
			//��
			D,
			KMax		//�@�{�^������
		};
	}

	namespace GPBut	//�Q�[���p�b�h�̃L�[��\��
	{
		enum Type {
			B01, B02, B03, B04, B05, B06, B07, B08,
			B09, B10, B11, B12, B13, B14, B15, B16,
			HSL, HSR, HSU, HSD,		//�n�b�g�X�C�b�` 
			KMax				//�@�{�^������
		};
	}
	namespace MB	//�}�E�X�̃{�^����\��
	{
		enum Type {  //�}�E�X�{�^���̎�� 8�܂ŔF���\
			LB, RB, CB, RESERVED1,
			RESERVED2, RESERVED3, RESERVED4, RESERVED5,
			kMax
		};
	}
	//�L�[�ݒ�L�[�{�[�h�p
	struct  KeyDataKB {
		DWORD           vKey;	//�L�[�{�[�h�̃o�[�`�����L�[
		But::Type       pKey;	//�{�^���̎�ށi�o�f��ł̖��́j
	};
	struct  AnalogAxisKB {
		DWORD           vKeyL;	//�L�[�{�[�h�̃o�[�`�����L�[
		DWORD           vKeyR;	//�L�[�{�[�h�̃o�[�`�����L�[
		DWORD           vKeyU;	//�L�[�{�[�h�̃o�[�`�����L�[
		DWORD           vKeyD;	//�L�[�{�[�h�̃o�[�`�����L�[
	};
	//�L�[�ݒ�Q�[���p�b�h�p
	struct  KeyDataGP {
		GPBut::Type      gKey;	//�Q�[���p�b�h�̃{�^��
		But::Type        pKey;	//�{�^���̎�ށi�o�f��ł̖��́j
	};

	//��O�Ƃ��ē�������G���[���
	namespace Err{
		enum Type
		{
			NOTHING,		//�G���[�Ȃ�
			CREATE,			//�������s
			NOTDEVICE,		//�f�o�C�X���擾�I�Ȃ�
			UNNONTYPE,		//�s���ȃf�o�C�X���
			ENUMDEVICE,		//�f�o�C�X�̗񋓂Ɏ��s
			CREATEDEVICE,	//�f�o�C�X�̐������s
			DATAFORMAT,		//�f�[�^�t�H�[�}�b�g�̐ݒ莸�s
			COOPLEVEL,		//�������x���ݒ莸�s
			GPAXIS,			//
			KBPROPERTY,
		};
	}

	class  Obj
	{
		IDirectInput8*  di;
		HWND  wnd;

		typedef  weak_ptr<Obj>  WP;
		static  Obj::WP  winst;		//	�P�������݂ł��Ȃ��C���X�^���X�̃E�B�[�N�|�C���^
		Obj();
		bool  Initialize(HWND wnd_);
	public:
		typedef  shared_ptr<Obj>  SP;
		~Obj();
		//	DI_���C�u�����𐶐�����
		static  Obj::SP  Create(HWND  wnd_);
		static  Obj::SP  GetInst();
		HWND  Wnd(){ return  wnd; }
		IDirectInput8&  DInput(){ return  *di; }
	};
	class  Input
	{
	public:
	private:
		enum  DeviceKind{ KeyBoard, GamePad, UnNon };	//�f�o�C�X�̎��
		struct sKeyParam{
			DWORD             dKey;		//���ۂ̃{�^���܂��̓L�[�{�[�h�̃o�[�`�����L�[
			But::Type     pKey;		//�{�^���̎�ށi�o�f��ł̖��́j
			KS::Type      state;	//�L�[�̏��
		};
		IDirectInputDevice8*  device;
		DeviceKind            dk;
		vector<sKeyParam>     buttons;
		vector<DWORD>         kbAxisL;	//�L�[�{�[�h��p�����L�[
		vector<DWORD>         kbAxisR;	//�L�[�{�[�h��p�����L�[
		vector<DWORD>         kbPov4;	//�L�[�{�[�h��pPOV�L�[
		StickInfo         ls, rs;

		Input();
		bool  Initialize(
			DeviceKind    dk_,
			DWORD         id_,
			AnalogAxisKB* axisL_,
			AnalogAxisKB* axisR_,
			DWORD         numButton_,
			KeyDataKB     buttonKB_[],
			KeyDataGP     buttonGP_[]);
		Err::Type  CreateDevice(DWORD  id_);

		void  CheckKB();
		void  CheckGP();
		void  CheckUnNon();
		bool  CheckBT(But::Type  kk_, KS::Type  ks_);	//���o����{�^���̏��
		bool  CheckStickL(Axis::Type  kk_, KS::Type  ks_);
		bool  CheckStickR(Axis::Type  kk_, KS::Type  ks_);


	public:
		~Input();
		typedef  shared_ptr<Input>  SP;
		typedef  weak_ptr<Input>  WP;
		//	�f�o�C�X�𐶐�����
		static  SP  CreateKB(
			AnalogAxisKB	axisL_,
			AnalogAxisKB	axisR_,
			DWORD			numButton_,
			KeyDataKB	button_[]);
		static  SP  CreateGP(
			DWORD			id_,
			DWORD			numButton_,
			KeyDataGP	button_[]);
		//	�X�V
		void  UpDate();
		//	�{�^���̏�Ԃ��m�F
		bool  Button_On(But::Type  kk_);
		bool  Button_Off(But::Type  kk_);
		bool  Button_Down(But::Type  kk_);
		bool  Button_Up(But::Type  kk_);
		KS::Type  CheckBT(But::Type  kk_);
		//	�A�i���O�X�e�B�b�N�̓��͂𓾂�
		StickInfo  StickL();
		StickInfo  StickR();

		//	�A�i���O�X�e�B�b�N�̂S�������{�^���Ɍ����ĂĎg�p����
		//	�{�^���̂n�m�@�n�e�e�ł����Ȃ��̂ŃA�i���O�̏��ɂ͂Ȃ�Ȃ�
		bool  StickL_On(Axis::Type  kk_);
		bool  StickL_Off(Axis::Type  kk_);
		bool  StickL_Down(Axis::Type  kk_);
		bool  StickL_Up(Axis::Type  kk_);
		bool  StickR_On(Axis::Type  kk_);
		bool  StickR_Off(Axis::Type  kk_);
		bool  StickR_Down(Axis::Type  kk_);
		bool  StickR_Up(Axis::Type  kk_);


	private:
		static  SP  Create(
			DeviceKind		dk_,
			DWORD			id_,
			AnalogAxisKB*	axisL_,
			AnalogAxisKB*	axisR_,
			DWORD			numButton_,
			KeyDataKB		buttonKB_[],
			KeyDataGP		buttonGP_[]);
		SP  link;

	public:
		//�Q�̃f�o�C�X�𓝍�����
		//���S�����l����Ȃ�A�N���X���\�b�h��Link���g����
		SP  Link(SP  l){
			if (this == l.get()){ return 0; }
			else{ SP w = link; link = l; return link; }
		}
		//�Q�̃f�o�C�X�𓝍�����
		//�Е��̃f�o�C�X�������ł��Ă��Ȃ��ꍇ�̑Ή�����
		static  Input::SP  Link(SP  a, SP  b);
	};
	//--------------------------------
	//add koyanagi
	class Mouse{
		//    IDirectInputDevice8*	device;
		//--------------------------------
		//�}�E�X
		LPDIRECTINPUTDEVICE8 mouse;
		DIMOUSESTATE2 mouseState;
		POINT         mouseCPos;
		POINT         mouseWPos;
		long          mouseWheel;
		KS::Type      mouseButton[MB::kMax];
		Err::Type     init(float, float);

		float         scaleX, scaleY;
		//--------------------------------
	public:
		POINT& Distance();
		POINT& CursorPos();
		int    Wheel();
		KS::Type     CheckBT(MB::Type mb_);
		//	�{�^���̏�Ԃ��m�F
		bool  Button_On(MB::Type  mb_);
		bool  Button_Off(MB::Type  mb_);
		bool  Button_Down(MB::Type  mb_);
		bool  Button_Up(MB::Type  mb_);
		bool  CheckBT(MB::Type		mb_, KS::Type		ks_);	//���o����{�^���̏��
		//--------------------------------
		Mouse();
		~Mouse();
		typedef shared_ptr<Mouse> SP;
		typedef weak_ptr<Mouse> WP;
		//����
		static SP   Create(float scaleX_ = 1.f, float scaleY_ = 1.f);
		//	�X�V
		void UpDate();

	};
}
namespace DI_
{
	const  int  analogMax  =  1000;		//�A�i���O���͂̃{�����[����-1000�`1000�Ƃ���
	const  float  digToAnalog  =  1.0f / analogMax;	//

	Obj::WP Obj::winst;	//	�P�������݂ł��Ȃ��C���X�^���X

	Obj::Obj( )
	{
		wnd = 0;
		di = nullptr;
	}
	bool Obj::Initialize(HWND wnd_)
	{
		wnd = wnd_;
		if(FAILED(DirectInput8Create(	GetModuleHandle(nullptr),
										DIRECTINPUT_VERSION,
										IID_IDirectInput8,
										(VOID**)&di,
										nullptr)))
		{
			//	�f�o�C�X�������s
			return false;
		}
		return true;
	}

	Obj::~Obj( )
	{
	//	DI_�I�u�W�F�N�g�̉��
		ML::SafeRelease(&di);
	}
	//	DI_���C�u�����𐶐�����
	Obj::SP Obj::Create(	HWND		hw_)	//	�E�B���h�E�n���h��
	{
		if(winst.expired( )){
			Obj::SP sp = Obj::SP(new Obj( ));
			if( sp->Initialize( hw_ ) ){
				winst = sp;
				return sp;
			}
		}
		return 0;
	}
	Obj::SP Obj::GetInst( )
	{
		if(!winst.expired( )){
			return winst.lock( );
		}
		return 0;
	}
	Input::Input(	)
	{
		device = nullptr;
	}
	bool Input::Initialize(	
			DeviceKind			dk_,
			DWORD				id_,
			AnalogAxisKB*	axisL_,
			AnalogAxisKB*	axisR_,
			DWORD				numButton_,
			KeyDataKB		buttonKB_[],
			KeyDataGP		buttonGP_[])
	{
		dk = dk_;
		StickInfo  w = { 0.0f, 0.0f, ML::Vec2(0, 0), KS::Off, KS::Off, KS::Off, KS::Off };
		ls = w;
		rs = w;

		Obj::SP dii = Obj::GetInst( );
		if(dii == 0){	return false; }
		Err::Type  e = this->CreateDevice(id_);
		//���̓f�o�C�X�̐����ɐ������Ȃ�����
		if(e != Err::NOTHING){
			dk = UnNon;
			return true;	//�d�l�ύX�ɂ��true�Ƃ���
		}	
		//�L�[�o�^
		if(axisL_){
			kbAxisL.push_back(axisL_->vKeyL);
			kbAxisL.push_back(axisL_->vKeyR);
			kbAxisL.push_back(axisL_->vKeyU);
			kbAxisL.push_back(axisL_->vKeyD);
		}
		if(axisR_){
			kbAxisR.push_back(axisR_->vKeyL);
			kbAxisR.push_back(axisR_->vKeyR);
			kbAxisR.push_back(axisR_->vKeyU);
			kbAxisR.push_back(axisR_->vKeyD);
		}

		if(buttonKB_){
			for(DWORD c = 0;  c < numButton_; c++)
			{
				sKeyParam w = { buttonKB_[c].vKey, buttonKB_[c].pKey, KS::Off };
				buttons.push_back(w);
			}
		}
		if(buttonGP_){
			for(DWORD c = 0;  c < numButton_; c++)
			{
				sKeyParam w = { (DWORD)buttonGP_[c].gKey, buttonGP_[c].pKey, KS::Off };
				buttons.push_back(w);
			}
		}

		return true;
	}
	Input::~Input( )
	{
		ML::SafeRelease(&device);
	}
	//	�f�o�C�X�𐶐�����
	Input::SP Input::Create(	
			DeviceKind			dk_,
			DWORD				id_,
			AnalogAxisKB*		axisL_,
			AnalogAxisKB*		axisR_,
			DWORD				numButton_,
			KeyDataKB			buttonKB_[],
			KeyDataGP			buttonGP_[])
	{
		auto sp = SP(new Input( ));
		if(sp){
			if( sp->Initialize(dk_,  id_,  axisL_,  axisR_,  numButton_,  buttonKB_,  buttonGP_) )
			{
				return sp;
			}
		}
		return 0;
	}

	Input::SP Input::CreateKB(
				AnalogAxisKB		axisL_,
				AnalogAxisKB		axisR_,
				DWORD				numButton_,
				KeyDataKB			button_[])
	{
		AnalogAxisKB*  aL = nullptr;
		AnalogAxisKB*  aR = nullptr;
		if(axisL_.vKeyL && axisL_.vKeyR && axisL_.vKeyU && axisL_.vKeyD){	aL = &axisL_;} 
		if(axisR_.vKeyL && axisR_.vKeyR && axisR_.vKeyU && axisR_.vKeyD){	aR = &axisR_;} 
		return  Create(KeyBoard,  0,  aL,  aR,  numButton_,  button_,  nullptr);
	}
	Input::SP Input::CreateGP(
				DWORD				id_,
				DWORD				numButton_,
				KeyDataGP		button_[])
	{
		return Create(GamePad,  id_,  nullptr,  nullptr,  numButton_,  nullptr,  button_);
	}
	Input::SP Input::Link(Input::SP a, Input::SP b)
	{
	//�Q�̃f�o�C�X�𓝍����Ďg����悤�ɂ���
		if(a != 0 && b != 0){	a->Link(b);	}	//�L�[�{�[�h�Z�@�Q�[���p�b�h�Z
		else if(a != 0){					}	//�L�[�{�[�h�Z�@�Q�[���p�b�hB2
		else if(b != 0){		a = b;		}	//�L�[�{�[�hB2�@�Q�[���p�b�h�Z
		else{								}	//�L�[�{�[�hB2�@�Q�[���p�b�hB2
		return a;
	}
	//	�X�V
	void Input::UpDate( )
	{
		switch(dk){		case KeyBoard:		CheckKB( );		break;
						case GamePad:		CheckGP();		break;
						case UnNon:		CheckUnNon();	break;	}
		//�֘A�t�����ꂽ�҂�����ꍇ�A������X�V����
		if(link != 0){	link->UpDate( );}
	}
	//�{�^���̏�Ԃ��X�V����
	KS::Type  UpdateButtonState(bool  nbs_, KS::Type  old_)
	{
		if(nbs_){
			if (old_ == KS::Err){ old_ = KS::Off; }
			//�L�[���(�����n)
			switch(old_){
				case KS::Off:
				case KS::Up:
					old_ = KS::Down;	break;	//	�����Ă���&���������ゾ����
				case KS::On:
				case KS::Down:
					old_ = KS::On;	break;	//	�����Ă���&���������ゾ����
			}
		}else{
			//�L�[���(���n)
			switch(old_){
				case KS::Off:
				case KS::Up:
					old_ = KS::Off;	break;	//	�����Ă���&���������ゾ����
				case KS::On:
				case KS::Down:
					old_ = KS::Up;	break;	//	�����Ă���&���������ゾ����
			}
		}
		return old_;
	}
	void  SIUpdate(StickInfo&  si_, float  ax_, float  ay_)
	{
		si_.axis.x  =  ax_;
		si_.axis.y  =  ay_;

		//���͗ʂ����Ȃ��ꍇ�A�j���[�g�����Ƃ���
		if( -0.1f < si_.axis.x  &&  si_.axis.x < 0.1f &&
			-0.1f < si_.axis.y  &&  si_.axis.y < 0.1f ){
				si_.axis.x = 0;
				si_.axis.y = 0;
		}

		//�p�x�ƒ����ɕϊ������A�i���O�X�e�B�b�N����
		if(si_.axis.x  !=  0 || si_.axis.y  !=  0){//�X�e�B�b�N����X�������o�ł���
			//�E��0�x�A����90�x�Ƃ����p�x�����߂�
			si_.angle  =  atan2(si_.axis.y, si_.axis.x);//�X������p�x���Z�o
			//���o�[�̌X���ʂ́}0�`1�ɒu��������
			si_.volume  =  sqrt(si_.axis.x * si_.axis.x  +  si_.axis.y * si_.axis.y);
			si_.volume  =  min(si_.volume, 1.0f);
		}
		else{
			si_.volume  =  0.0f;
		}
		//�S�����{�^���̃C���[�W�Ń}�b�s���O
		si_.xm  =  UpdateButtonState(ax_ < -0.1f,  si_.xm);
		si_.xp  =  UpdateButtonState(ax_ > +0.1f,  si_.xp);
		si_.ym  =  UpdateButtonState(ay_ < -0.1f,  si_.ym);
		si_.yp  =  UpdateButtonState(ay_ > +0.1f,  si_.yp);
	}
	void Input::CheckKB( )
	{
		BYTE keyStatus[256];
		ZeroMemory(keyStatus, 256);
		//�f�o�C�X���g�p�\���m�F����
		int acquireCnt = 0;
		BOOL brFlag = FALSE;
		if(FAILED(device->Poll( )))
		{
			do
			{
				if(SUCCEEDED(device->Acquire( ))){		brFlag = TRUE;	}
				if(++acquireCnt > 30){					return;		}
			}while(brFlag == FALSE);
		}
		//	�L�[�{�[�h���擾
		if(FAILED(device->GetDeviceState(sizeof(BYTE) * 256, &keyStatus))){	return;	}
		//	�������͂ɓo�^���ꂽ�L�[�ւ̑Ή�
		float  ax_L=0,  ay_L=0;
		if(kbAxisL.size( )==4){
			if(keyStatus[kbAxisL[0]] != 0){	ax_L -= 1;}
			if(keyStatus[kbAxisL[1]] != 0){	ax_L += 1;}
			if(keyStatus[kbAxisL[2]] != 0){	ay_L -= 1;}
			if(keyStatus[kbAxisL[3]] != 0){	ay_L += 1;}
		}
		float  ax_R=0,  ay_R=0;
		if(kbAxisR.size( )==4){
			if(keyStatus[kbAxisR[0]] != 0){	ax_R -= 1;}
			if(keyStatus[kbAxisR[1]] != 0){	ax_R += 1;}
			if(keyStatus[kbAxisR[2]] != 0){	ay_R -= 1;}
			if(keyStatus[kbAxisR[3]] != 0){	ay_R += 1;}
		}

		//LR�X�e�B�b�N���X�V
		SIUpdate(ls,  ax_L,  ay_L);
		SIUpdate(rs,  ax_R,  ay_R);
		//�@�{�^���Ƃ��ēo�^���ꂽ�L�[�ւ̑Ή�
		for(auto it = begin(buttons); it != end(buttons); ++it){
			it->state  =  UpdateButtonState(keyStatus[it->dKey] != 0,  it->state);
		}
	}
	
	bool  KeyCheck(KS::Type  state_, KS::Type  m_)
	{
		namespace  KS = KS;
		//�L�[�ƍ�
		if(	(m_ == KS::Down  &&  state_ == KS::Down)  ||
			(m_ == KS::Up    &&  state_ == KS::Up)    ||
			(m_ == KS::Off   &&  (state_ == KS::Up   || state_ == KS::Off))  ||
			(m_ == KS::On    &&  (state_ == KS::Down || state_ == KS::On))   ){
			return true;
		}
		return false;
	}


	bool  Input::StickL_On(Axis::Type  kk_){   return  this->CheckStickL(kk_, KS::On); }
	bool  Input::StickL_Off(Axis::Type  kk_){  return  this->CheckStickL(kk_, KS::Off); }
	bool  Input::StickL_Down(Axis::Type  kk_){ return  this->CheckStickL(kk_, KS::Down); }
	bool  Input::StickL_Up(Axis::Type  kk_){   return  this->CheckStickL(kk_, KS::Up); }
	bool  Input::StickR_On(Axis::Type  kk_){   return  this->CheckStickR(kk_, KS::On); }
	bool  Input::StickR_Off(Axis::Type  kk_){  return  this->CheckStickR(kk_, KS::Off); }
	bool  Input::StickR_Down(Axis::Type  kk_){ return  this->CheckStickR(kk_, KS::Down); }
	bool  Input::StickR_Up(Axis::Type  kk_){   return  this->CheckStickR(kk_, KS::Up); }

	bool Input::CheckStickL(Axis::Type  kk_, KS::Type  ks_)
	{
		KS::Type   w;
		if(     kk_  ==  Axis::L){	w  =  StickL( ).xm;}
		else if(kk_  ==  Axis::R){	w  =  StickL( ).xp;}
		else if(kk_  ==  Axis::U){	w  =  StickL( ).ym;}
		else if(kk_  ==  Axis::D){	w  =  StickL( ).yp;}
		else{						return  false;}
		return  KeyCheck(w,  ks_);
	}
	bool Input::CheckStickR(Axis::Type  kk_, KS::Type  ks_)
	{
		KS::Type   w;
		if(     kk_  ==  Axis::L){	w  =  StickR( ).xm;}
		else if(kk_  ==  Axis::R){	w  =  StickR( ).xp;}
		else if(kk_  ==  Axis::U){	w  =  StickR( ).ym;}
		else if(kk_  ==  Axis::D){	w  =  StickR( ).yp;}
		else{						return  false;}
		return  KeyCheck(w,  ks_);
	}

	StickInfo  Input::StickL()
	{
		StickInfo  me = this->ls;
		StickInfo  linkRtv;
		if(link != nullptr){	
			linkRtv = link->StickL( );
			if(me.volume  <  linkRtv.volume){
				return linkRtv;
			}
		}
		return me;
	}
	StickInfo  Input::StickR()
	{
		StickInfo  me = this->rs;
		StickInfo  linkRtv;
		if(link != nullptr){	
			linkRtv = link->StickR( );
			if(me.volume  <  linkRtv.volume){	return linkRtv;}
		}
		return me;
	}
	void  Input::CheckUnNon()
	{
		//�A�i���O�X�e�B�b�N�Ή�
		SIUpdate(ls, 0, 0);
		SIUpdate(rs, 0, 0);

		//�{�^���ւ̑Ή�
		for (auto it = begin(buttons); it != end(buttons); ++it)
		{
			it->state = UpdateButtonState(false, it->state);
		}
	}


	void Input::CheckGP( )
	{
		DIJOYSTATE2 js;	//	�L�[�����i�[����
		//�f�o�C�X���g�p�\���m�F����
		int acquireCnt = 0;
		BOOL brFlag = FALSE;
		if(FAILED(device->Poll( )))
		{
			do
			{
				if(SUCCEEDED(device->Acquire( ))){		brFlag = TRUE;		}
				if(++acquireCnt > 30){					return;				}
			}while(brFlag == FALSE);
		}

		//	�p�b�h���擾
		if(FAILED(device->GetDeviceState(sizeof(DIJOYSTATE2), &js)))
		{
			return;
		}
		//�A�i���O�X�e�B�b�N�Ή�
		SIUpdate(ls,  js.lX * digToAnalog,  js.lY  * digToAnalog);
		SIUpdate(rs,  js.lZ * digToAnalog,  js.lRz * digToAnalog);


		//POV�Ή�
		int m = 8;
		if(js.rgdwPOV[0] != 0xFFFFFFFF){	m = js.rgdwPOV[0] / 4500;}//0-7
		bool xym[9][4] = {
		//   L R U D
			{0,0,1,0},		//0  UP
			{0,1,1,0},		//1  UPRT
			{0,1,0,0},		//2  RT
			{0,1,0,1},		//3  DWRT
			{0,0,0,1},		//4  DW
			{1,0,0,1},		//5  DWLE
			{1,0,0,0},		//6  LE
			{1,1,0,0},		//7  UPLE
			{0,0,0,0},		//8  NT
		};
		//�{�^���ւ̑Ή�
		bool keyFlag;
		for(auto it = begin(buttons); it != end(buttons); ++it)
		{
			//�L�[�R�[�h�ɑΉ������f�[�^�ύX�������s��
			keyFlag = false;
			//POV�Ή�
			if(		it->dKey == GPBut::HSL){		keyFlag = xym[m][0];}
			else if(it->dKey == GPBut::HSR){		keyFlag = xym[m][1];}
			else if(it->dKey == GPBut::HSU){		keyFlag = xym[m][2];}
			else if(it->dKey == GPBut::HSD){		keyFlag = xym[m][3];}
			//�{�^���O�O�`�P�S
			else if(it->dKey >= GPBut::B01 &&	
					it->dKey <= GPBut::B16){		keyFlag = js.rgbButtons[it->dKey] != 0;}
			//���Ή��̃L�[���ݒ肳��Ă���
			else{	continue;	}
			it->state  =  UpdateButtonState(keyFlag,  it->state);
		}
	}

	//�Q�̃L�[�𓝍��������A�œ|�ȏ�Ԃ�Ԃ�
	KS::Type  IntegrationKey(KS::Type  a_, KS::Type  b_)
	{
		namespace KS = KS;
		KS::Type ret = KS::Off;

		KS::Type  mat[5][5] = {
			//Err  On  Off   Down  Up
			{KS::Err,  KS::On, KS::Off,  KS::Down, KS::Up}, //Err
			{KS::On,   KS::On, KS::On,   KS::On,   KS::On}, //On
			{KS::Off,  KS::On, KS::Off,  KS::Down, KS::Up}, //Off
			{KS::Down, KS::On, KS::Down, KS::Down, KS::On}, //Down
			{KS::Up,   KS::On, KS::Up,   KS::On,   KS::Up}, //Up
		};
		return  mat[a_][b_];
	}
	KS::Type	 Input::CheckBT(But::Type		kk_)
	{
		KS::Type ret = KS::Off;

		//�L�[�z��̒�����ΏۂɂȂ�L�[�����o����
		for(auto it = begin(buttons); it != end(buttons); ++it)
		{
			if(kk_ == it->pKey)		//�o�^�ς݂̃L�[�ꗗ����Y������L�[�����o����
			{
				ret = (KS::Type)it->state;
			}
			if(link != 0){
				ret = IntegrationKey(ret, link->CheckBT(kk_));
			}
		}
		return ret;
	}



	bool  Input::Button_On(But::Type  kk_){   return  this->CheckBT(kk_, KS::On); }
	bool  Input::Button_Off(But::Type  kk_){  return  this->CheckBT(kk_, KS::Off); }
	bool  Input::Button_Down(But::Type  kk_){ return  this->CheckBT(kk_, KS::Down); }
	bool  Input::Button_Up(But::Type  kk_){   return  this->CheckBT(kk_, KS::Up); }
	//	�{�^���̏�Ԃ��m�F
	bool Input::CheckBT(But::Type  kk_, KS::Type  ks_)
	{
		KS::Type  ks = CheckBT(kk_);
		//�L�[�ƍ�
		if ((ks_ == KS::Down  &&  ks == KS::Down) ||
			(ks_ == KS::Up    &&  ks == KS::Up)   ||
			(ks_ == KS::Off   && (ks == KS::Up    || ks == KS::Off)) ||
			(ks_ == KS::On    && (ks == KS::Down  || ks == KS::On))){
			return true;
		}
		return false;
	}

	static GUID					a_guid;
	static int					a_id;
	BOOL CALLBACK diEnumDevice(		const DIDEVICEINSTANCE* inDevice,
									void* inContext)
	{
		//�f�o�C�X�ԍ�
		DWORD* num = (DWORD*)inContext;
		//�w�肳�ꂽ�ԍ��̃f�o�C�X�ɓ��B
		if((*num) == a_id)
		{
			a_guid = inDevice->guidInstance;	//�f�t�h�c��ۑ�
			(*num)++;
			return DIENUM_STOP;					//�f�o�C�X�̗񋓂��I������
		}
		else
		{
			(*num)++;
			return DIENUM_CONTINUE;			//���̃f�o�C�X�̃`�F�b�N�Ɉڂ�
		}
	}
	BOOL CALLBACK enumObjectsGamePad(	const DIDEVICEOBJECTINSTANCE* inDevice,
										void* inContext)
	{
		IDirectInputDevice8* did = (IDirectInputDevice8*)inContext;
		DIPROPRANGE diprg; 
		//�f�o�C�X�̓��͂͐�Ύ������Ύ��ł���
		if(inDevice->dwType&DIDFT_AXIS)
		{
			diprg.diph.dwSize = sizeof(DIPROPRANGE);
			diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
			diprg.diph.dwHow = DIPH_BYID;
			diprg.diph.dwObj = inDevice->dwType;
			diprg.lMin = -analogMax;
			diprg.lMax =  analogMax;
			//	�f�o�C�X�̃^�C�v��ݒ肷��
			did->SetProperty(DIPROP_RANGE, &diprg.diph);
		}
		else
		{
			MessageBox(nullptr, "���̃^�C�v�̃R���g���[���͑Ή����Ă��܂���", nullptr, MB_OK);
		}
		return DIENUM_CONTINUE;
	}
	Err::Type Input::CreateDevice(DWORD id_)
	{
		Obj::SP dii = Obj::GetInst( );
		if(dii == 0){	return Err::NOTDEVICE; }
		if(!(	dk == KeyBoard ||
				dk == GamePad ) ){
			return Err::UNNONTYPE;
		}

		struct {
			DWORD				dType;
			LPCDIDATAFORMAT		dFormat;
			DWORD				cLevel;

		}kind[] = {
			//Input::KeyBoard
			{DI8DEVCLASS_KEYBOARD, &c_dfDIKeyboard,  DISCL_NONEXCLUSIVE | DISCL_FOREGROUND},
			//Input::GamePad
			{DI8DEVCLASS_GAMECTRL, &c_dfDIJoystick2, DISCL_EXCLUSIVE	| DISCL_FOREGROUND},
		};

	//�f�o�C�X�̌��o���s��----------------------------------------------------------
		//�w��f�o�C�X���o�i�O���[�o���ϐ��@a_guid�@����с@a_id�@�ɒ��Ӂj
		DWORD deviceCount = 0;
		a_id = id_;
		if(FAILED(  dii->DInput( ).EnumDevices(
							kind[dk].dType,  diEnumDevice,
							&deviceCount, DIEDFL_ATTACHEDONLY))){		return Err::ENUMDEVICE;	}
		if(dk == GamePad  &&  deviceCount <= id_){						return Err::ENUMDEVICE;	}

	//���̓f�o�C�X�𐶐�����
		if(FAILED(dii->DInput( ).CreateDevice( a_guid, &device, nullptr))){		return Err::CREATEDEVICE;	}
	//���̓f�o�C�X�̃t�H�[�}�b�g��ݒ肷��
		if(FAILED(device->SetDataFormat(kind[dk].dFormat))){					return Err::DATAFORMAT;		}
	//���̓f�o�C�X�̋������x����ݒ肷��
		if(FAILED(device->SetCooperativeLevel(dii->Wnd( ), kind[dk].cLevel))){	return Err::COOPLEVEL;		}

	//���̓Q�[���p�b�h�̏ꍇ�A�����L�[�ɐ�Ύ��������͑��Ύ����g�p�\�Ȃ��Ǝ����m�F����B
		switch(dk)
		{
			case GamePad:
				//��Ύ�����ё��Ύ����g�p�\�ȏꍇ�A���̐ݒ���s��
				if(FAILED(device->EnumObjects(	enumObjectsGamePad,
												device,
												DIDFT_AXIS)))
				{
					return Err::GPAXIS;
				}
				break;
			case KeyBoard:
				{
					DIPROPDWORD dipdw;
					dipdw.diph.dwSize		= sizeof(DIPROPDWORD);
					dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
					dipdw.diph.dwObj		= 0;
					dipdw.diph.dwHow		= DIPH_DEVICE;
					dipdw.dwData			= 8;//�����ɔ���ł���L�[�̐��ɂȂ�̂��ȁH
					if(FAILED(device->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph)))
					{
						return Err::KBPROPERTY;
					}
				}			
				break;
		}
		return Err::NOTHING;
	}
	Err::Type Mouse::init(float scaleX_, float scaleY_)
	{
		Obj::SP dii = Obj::GetInst();
		if (dii == 0){ return Err::NOTDEVICE; }

		//--------------------------------
		//DirectMouse
		if (FAILED(dii->DInput().CreateDevice(GUID_SysMouse, &mouse, nullptr))){ return Err::CREATEDEVICE; }
		if (FAILED(mouse->SetDataFormat(&c_dfDIMouse2))){ return Err::DATAFORMAT; } // �}�E�X�p�̃f�[�^�E�t�H�[�}�b�g��ݒ�
		mouse->SetCooperativeLevel(dii->Wnd(), DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);//���x���ݒ�
		// �f�o�C�X�̐ݒ�    
		DIPROPDWORD diprop;
		diprop.diph.dwSize = sizeof(diprop);
		diprop.diph.dwHeaderSize = sizeof(diprop.diph);
		diprop.diph.dwObj = 0;
		diprop.diph.dwHow = DIPH_DEVICE;
		diprop.dwData = DIPROPAXISMODE_REL; // ���Βl���[�hDIPROPAXISMODE_REL�Őݒ�i��Βl��DIPROPAXISMODE_ABS�j    
		mouse->SetProperty(DIPROP_AXISMODE, &diprop.diph);

		// ���͐���J�n    
		mouse->Acquire();
		//--------------------------------
		scaleX = scaleX_;
		scaleY = scaleY_;

		return Err::NOTHING;
	}

	Mouse::SP Mouse::Create(float scaleX_, float scaleY_)
	{
		auto sp = SP(new Mouse());
		if (sp){
			if (sp->init(scaleX_, scaleY_) != Err::NOTHING){
				MessageBox(nullptr, _T("�}�E�X������܂����I"), nullptr, MB_OK);
				return nullptr;
			}
		}
		return sp;
	}
	Mouse::Mouse()
	{
	}

	Mouse::~Mouse()
	{
		if (mouse != NULL)
		{
			mouse->Unacquire();
			ML::SafeRelease(&mouse);
		}
	}

	void Mouse::UpDate()
	{
		//--------------------------------
		//add koyanagi
		// �l�̍X�V
		ZeroMemory(&mouseState, sizeof(DIMOUSESTATE2));
		if (FAILED(mouse->GetDeviceState(sizeof(DIMOUSESTATE2), &mouseState))){
			mouse->Acquire();
		}
		GetCursorPos(&mouseWPos);
		Obj::SP dii = Obj::GetInst();
		ScreenToClient(dii->Wnd(), &mouseWPos);
		mouseWPos.x = (LONG)(mouseWPos.x / scaleX);
		mouseWPos.y = (LONG)(mouseWPos.y / scaleY);

		mouseCPos.x = mouseState.lX;
		mouseCPos.y = mouseState.lY;
		mouseWheel = mouseState.lZ;
		for (int i = 0; i < MB::kMax; i++)
		{
			if ((mouseState.rgbButtons[i] & 0x80) >> 7){
				if (mouseButton[i] == KS::Off || mouseButton[i] == KS::Up)  mouseButton[i] = KS::Down;
				else  mouseButton[i] = KS::On;
			}
			else{
				if (mouseButton[i] == KS::On || mouseButton[i] == KS::Down)  mouseButton[i] = KS::Up;
				else  mouseButton[i] = KS::Off;
			}
		}
		//--------------------------------
	}
	//==================================
	//Mouse�̃��[�J�����W���擾
	//==================================
	POINT& Mouse::Distance()
	{
		return mouseCPos;
	}
	int Mouse::Wheel()
	{
		return mouseWheel;
	}
	//==================================
	//Mouse�̐�΍��W���擾
	//==================================
	POINT& Mouse::CursorPos()
	{
		return mouseWPos;
	}

	KS::Type Mouse::CheckBT(MB::Type mb)
	{
		return mouseButton[mb];
	}
	bool Mouse::CheckBT(MB::Type mb, KS::Type ks)
	{
		if (CheckBT(mb) == ks)
			return true;
		return false;
	}
	bool  Mouse::Button_On(MB::Type  mb_){ return	 this->CheckBT(mb_, KS::On); }
	bool  Mouse::Button_Off(MB::Type  mb_){ return	 this->CheckBT(mb_, KS::Off); }
	bool  Mouse::Button_Down(MB::Type  mb_){ return  this->CheckBT(mb_, KS::Down); }
	bool  Mouse::Button_Up(MB::Type  mb_){ return	 this->CheckBT(mb_, KS::Up); }
}

namespace DI
{
	DI_::Obj::SP		obj;
	unordered_map<string, DI_::Input::SP>  inputs;
	bool inputs_NotObject = false;

	DI_::Mouse::SP  mouse;

	//������
	bool  Initialize(HWND		hw_, float scaleX_, float scaleY_)
	{
		if (!obj)
		{
			obj = DI_::Obj::Create(hw_);
			mouse = DI_::Mouse::Create(scaleX_, scaleY_);
		}
		return  obj != nullptr;
	}
	//���
	void  Finalize()
	{
		GPad_Clear();
		obj.reset();
	}
	//�X�V
	void  UpDate()
	{
		for (auto it = inputs.begin(); it != inputs.end(); it++)
		{
			if ((*it).second) {
				(*it).second->UpDate();
			}
		}
		mouse->UpDate();
	}
	//���ݓo�^���Ă��邷�ׂẴf�o�C�X��j��
	void  GPad_Clear()
	{
		inputs.clear();
	}
	//�w�肵���f�o�C�X��j��
	void  GPad_Erase(const string name_)
	{
		inputs.erase(name_);
	}

	//�L�[�{�[�h�Ńf�o�C�X����
	void  GPad_CreateKB(
		const string		name_,
		const AnalogAxisKB	axisL_,
		const AnalogAxisKB	axisR_,
		const KeyDatas_KB	button_)
	{
		DI_::AnalogAxisKB  aL;
		memcpy(&aL, &axisL_, sizeof(aL));
		DI_::AnalogAxisKB  aR;
		memcpy(&aR, &axisR_, sizeof(aR));
		auto  bt = new DI_::KeyDataKB[button_.size( )];
		memcpy(bt, &button_[0], sizeof(DI_::KeyDataKB) * button_.size());


		auto  w = DI_::Input::CreateKB(aL, aR, button_.size(), bt);
		delete[] bt;
		if (!w) { return; }
		inputs[name_] = w;
	}
	//�Q�[���p�b�h�Ńf�o�C�X����
	void  GPad_CreateGP(
		const string			name_,
		const DWORD				id_,
		const KeyDatas_GP		button_)
	{
		auto  bt = new DI_::KeyDataGP[button_.size()];
		memcpy(bt, &button_[0], sizeof(DI_::KeyDataGP) * button_.size());

		auto  w = DI_::Input::CreateGP(id_, button_.size(), bt);
		delete[] bt;
		if (!w) { return; }
		inputs[name_] = w;
	}
	//�L�[�{�[�h���Q�[���p�b�h�Ńf�o�C�X����
	void  GPad_CreateKBGP(
		const string	name_,
		const AnalogAxisKB	axisL_,
		const AnalogAxisKB	axisR_,
		const KeyDatas_KB	buttonK_,
		const DWORD			id_,
		const KeyDatas_GP	buttonG_)
	{
		DI_::AnalogAxisKB  aL;
		memcpy(&aL, &axisL_, sizeof(aL));
		DI_::AnalogAxisKB  aR;
		memcpy(&aR, &axisR_, sizeof(aR));
		auto  btK = new DI_::KeyDataKB[buttonK_.size()];
		memcpy(btK, &buttonK_[0], sizeof(DI_::KeyDataKB) * buttonK_.size());

		auto  btG = new DI_::KeyDataGP[buttonG_.size()];
		memcpy(btG, &buttonG_[0], sizeof(DI_::KeyDataGP) * buttonG_.size());

		auto  w1 = DI_::Input::CreateKB(aL, aR, buttonK_.size(), btK);
		delete[] btK;
		auto  w2 = DI_::Input::CreateGP(id_, buttonG_.size(), btG);
		delete[] btG;
		w1 = DI_::Input::Link(w1, w2);
		inputs[name_] = w1;
	}

	ButtonInfo ConvertKS(DI_::KS::Type k_)
	{
		ButtonInfo  rtv;
		rtv.down = k_ == DI_::KS::Down;
		rtv.up   = k_ == DI_::KS::Up;
		rtv.off  = k_ == DI_::KS::Off;
		rtv.on   = k_ == DI_::KS::On;
		return  rtv;
	}
	//	�{�^���̏�Ԃ��m�F
	VGamePad  GPad_GetState(const string  name_)
	{
		VGamePad  rtv;
		auto it = inputs.find(name_);
		if (it == inputs.end()) {
			if (inputs_NotObject == false) {
				MessageBox(nullptr, name_.c_str(), _T("���̖��O�̉��z�R���g���[���[�͑��݂��܂���"), MB_OK);
			}
			inputs_NotObject = true;
			
		}
		else {
			auto ls = it->second->StickL();
			rtv.LStick.angle = ls.angle;
			rtv.LStick.volume = ls.volume;
			rtv.LStick.axis = ls.axis;
			rtv.LStick.L = ConvertKS(ls.xm);
			rtv.LStick.R = ConvertKS(ls.xp);
			rtv.LStick.U = ConvertKS(ls.ym);
			rtv.LStick.D = ConvertKS(ls.yp);

			auto rs = it->second->StickR();
			rtv.RStick.angle = rs.angle;
			rtv.RStick.volume = rs.volume;
			rtv.RStick.axis = rs.axis;
			rtv.RStick.L = ConvertKS(rs.xm);
			rtv.RStick.R = ConvertKS(rs.xp);
			rtv.RStick.U = ConvertKS(rs.ym);
			rtv.RStick.D = ConvertKS(rs.yp);

			rtv.B1 = ConvertKS(it->second->CheckBT(DI_::But::B1));
			rtv.B2 = ConvertKS(it->second->CheckBT(DI_::But::B2));
			rtv.B3 = ConvertKS(it->second->CheckBT(DI_::But::B3));
			rtv.B4 = ConvertKS(it->second->CheckBT(DI_::But::B4));

			rtv.L1 = ConvertKS(it->second->CheckBT(DI_::But::L1));
			rtv.L2 = ConvertKS(it->second->CheckBT(DI_::But::L2));
			rtv.L3 = ConvertKS(it->second->CheckBT(DI_::But::L3));

			rtv.R1 = ConvertKS(it->second->CheckBT(DI_::But::R1));
			rtv.R2 = ConvertKS(it->second->CheckBT(DI_::But::R2));
			rtv.R3 = ConvertKS(it->second->CheckBT(DI_::But::R3));

			rtv.ST = ConvertKS(it->second->CheckBT(DI_::But::ST));
			rtv.SE = ConvertKS(it->second->CheckBT(DI_::But::SE));

			rtv.HL = ConvertKS(it->second->CheckBT(DI_::But::L));
			rtv.HR = ConvertKS(it->second->CheckBT(DI_::But::R));
			rtv.HU = ConvertKS(it->second->CheckBT(DI_::But::U));
			rtv.HD = ConvertKS(it->second->CheckBT(DI_::But::D));

			rtv.S0 = ConvertKS(it->second->CheckBT(DI_::But::S0));
			rtv.S1 = ConvertKS(it->second->CheckBT(DI_::But::S1));
			rtv.S2 = ConvertKS(it->second->CheckBT(DI_::But::S2));
			rtv.S3 = ConvertKS(it->second->CheckBT(DI_::But::S3));
			rtv.S4 = ConvertKS(it->second->CheckBT(DI_::But::S4));
			rtv.S5 = ConvertKS(it->second->CheckBT(DI_::But::S5));
			rtv.S6 = ConvertKS(it->second->CheckBT(DI_::But::S6));
			rtv.S7 = ConvertKS(it->second->CheckBT(DI_::But::S7));
			rtv.S8 = ConvertKS(it->second->CheckBT(DI_::But::S8));
			rtv.S9 = ConvertKS(it->second->CheckBT(DI_::But::S9));
		}
		return rtv;
	}
	//	�}�E�X�̏�Ԃ��m�F
	Mouse  Mouse_GetState( )
	{
		Mouse  rtv;
		rtv.distance = mouse->Distance();
		rtv.cursorPos = mouse->CursorPos();
		rtv.wheel = mouse->Wheel();
		rtv.LB = ConvertKS(mouse->CheckBT(DI_::MB::LB));
		rtv.RB = ConvertKS(mouse->CheckBT(DI_::MB::RB));
		rtv.CB = ConvertKS(mouse->CheckBT(DI_::MB::CB));

		rtv.B1 = ConvertKS(mouse->CheckBT(DI_::MB::RESERVED1));
		rtv.B2 = ConvertKS(mouse->CheckBT(DI_::MB::RESERVED2));
		rtv.B3 = ConvertKS(mouse->CheckBT(DI_::MB::RESERVED3));
		rtv.B4 = ConvertKS(mouse->CheckBT(DI_::MB::RESERVED4));
		rtv.B5 = ConvertKS(mouse->CheckBT(DI_::MB::RESERVED5));
		return rtv;
	}
}