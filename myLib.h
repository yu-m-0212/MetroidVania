#pragma warning(disable:4996)
#pragma once
#pragma warning( disable : 4005 )
#include <d3dx10.h>
#include <d3dCompiler.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d10.lib")
#pragma comment(lib,"d3dx10.lib")
#pragma comment(lib,"d3dCompiler.lib")
#pragma warning( default : 4005 )


#include <memory>
#include <tchar.h>
#include <windows.h>
#include <string.h>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <map>
#include <unordered_map>
//////////////////////////////#include "atlstr.h"
using namespace std;
using namespace std::tr1;

namespace ML
{
	float Gravity(const  float  byM_);

	static const float  PI = 3.14159265358979323846f;
	float ToRadian(const  float	degree_);
	float ToDegree(const  float	radian_);

	//�x�N�g���^�Q�c�E�R�c�E�S�c
	class Vec2 : public  D3DXVECTOR2
	{
	public:
		Vec2( );
		Vec2(const float  x_,const float  y_);
		Vec2(const D3DXVECTOR2&  v_);
		Vec2  operator  = (const D3DXVECTOR2&  v_);
		//�x�N�g���𐳋K��
		Vec2  Normalize(  )const;
		//�x�N�g���̒��������߂�
		float  Length()const;
	};
	class Vec3 : public  D3DXVECTOR3
	{
	public:
		Vec3( );
		Vec3(const float  x_,const float  y_,const float  z_);
		Vec3(const D3DXVECTOR3&  v_);
		Vec3  operator  = (const D3DXVECTOR3&  v_);
		//�x�N�g���^�ϊ�
		class  Vec4  Vec4(  );
		//�x�N�g���𐳋K��
		Vec3  Normalize()const;
		//�x�N�g���̒��������߂�
		float  Length()const;
	};
	class Vec4 : public  D3DXVECTOR4
	{
	public:
		Vec4( );
		Vec4(const float  x_,const float  y_,const float  z_,const float  w_);
		Vec4(const D3DXVECTOR4&  v_);
		Vec4  operator  = (const D3DXVECTOR4&  v_);
		//�x�N�g���^�ϊ�
		class  Vec3  Vec3(  );
	};
	//�N�H�[�^�j�I��
	//�N�I�[�^�j�I����(0,0,0,0)�͗L���Ȓl�ł͂Ȃ��̂Œ���
	class QT : public  D3DXQUATERNION
	{
	public:
		QT( );
		QT::QT(float  rY_);
		QT(const  ML::Vec3&  v_,  float  r_);
		void RotAx(const  ML::Vec3&  v_,  float  r_);
		void RotX(float  rX_);
		void RotY(float  rY_);
		void RotZ(float  rZ_);
		void RotXYZ(const  ML::Vec3  rXYZ_);
		QT operator * ( const D3DXQUATERNION& ) const;
		QT& operator *= ( const D3DXQUATERNION& );
	};
	//�s��
	class Mat4x4 : public D3DXMATRIX
	{
	public:
		Mat4x4( );
		Mat4x4(const D3DXMATRIX&  v_);
		Mat4x4  operator  = (const D3DXMATRIX&  v_);
		//�P�ʍs��
		void  Identity( );
		//�r���[�s��
		void  LookAtLH(const  Vec3& pos_,  const  Vec3&  target_,  const  Vec3&  up_ );
		//�ˉe�s��
		void  PerspectiveFovLH(float  fov_,  float  aspect_,  float  nearPlane_,  float  forePlane_ );
		//�Œ莲��]�s��
		void  RotationX(float  r_);
		void  RotationY(float  r_);
		void  RotationZ(float  r_);
		//�C�ӎ���]�s��
		void  RotationAxis(const  Vec3&  v_, float  r_);
		//�N�I�[�^�j�I���ŉ�]�s��
		void  RotationQuaternion(const  QT&  r_);
		//���s�ړ��s��
		void  Translation(const  Vec3&  p_);
		//�X�P�[�����O
		void  Scaling(const  Vec3&  s_);
		void  Scaling(float  s_);

		//�t�s�񐶐�
		Mat4x4 Inverse( );
		//�]�u�s�񐶐�
		Mat4x4 Transpose( ) const;
		//���W�ϊ�
		Vec3  TransformCoord(const Vec3&  p_);
		Vec3  TransformNormal(const Vec3&  v_);
	};

	//�l������l�ɋ߂Â���
	void  ValueControl_ToValue(float&  v_,float  to_,  float  aja_);
	//�x�N�g���̐��������̒������K��l�ɋ߂Â���
	void  VectorControl_XZ_ToValue(Vec3&  v_,float  to_,  float  aja_);
	//�x�N�g���̒������K��l�𒴂��Ă���ꍇ�A�K��l�܂ŒZ������
	void  VectorControl_XZ_Cut(Vec3&  v_,float  to_);

	//�F
	class Color
	{
	public:
		float  c[4];
		void Set(float  a_,float  r_,float  g_,float  b_){	c[3]=a_; c[2]=b_; c[1]=g_; c[0]=r_;}
		Color(float  a_,float  r_,float  g_,float  b_){		Set(a_, r_, g_, b_);}
		Color(float  c_[4]){								Set(c_[3], c_[2], c_[1], c_[0]);}
		Color( ){											Set(1, 0, 0, 0);}

		const float*  Ptr( ) const{return c;}
		D3DXCOLOR  D3D( ) const{	return  D3DXCOLOR(c[0], c[1], c[2], c[3]);} 
		bool  operator==(const  Color&  obj){
			return	   (c[0] == obj.c[0] &&
						c[1] == obj.c[1] &&
						c[2] == obj.c[2] &&
						c[3] == obj.c[3] );
		}
		bool  operator!=(const  Color&  obj){
			return	  !((*this) == obj );
		}
	};

	//������������������������������������������������������������������������������������������
	//�b�t�@�C���p�X����A�t�@�C�������폜���ăp�X���������o��							�@�b
	//�b���ӁI�p�X�Ƀt�@�C�������܂�ł��Ȃ��ꍇ�̓���͕ۏႵ�Ȃ�							�@�b
	//�b����		�F2010/01/15	�{��N�V	�쐬										�@�b
	//������������������������������������������������������������������������������������������
	static string GetPath(const string& fp_)
	{
		string s = fp_;
		string::size_type id;
		id = s.find_last_of("/");
		if(string::npos == id){	id = s.find_last_of("\\");}
		if(string::npos != id){	s = s.substr(0, id);}
		return s;
	}
	//base�Ɠ����p�X��fn���쐬
	static string CreateFilePath(const string&  base_, const string&  fn_)
	{
		string s = GetPath(base_);
		s += "/";
		s += fn_;
		return s;
	}

	//	���\�b�h��Release�����I�u�W�F�N�g�̉�����s��
	//	Release()���\�b�h���s���ɓ�����delete���Ă��镨�̂ݗL��
	template<class T>
	static void SafeRelease(T** obj_)
	{
		if(*obj_ != nullptr)
		{
			(*obj_)->Release();
			(*obj_) = nullptr;
		}
	}
	template<class T>
	static void SafeDelete(T** obj_)
	{
		if(*obj_ != nullptr)
		{
			delete (*obj_);
			(*obj_) = nullptr;
		}
	}
	template<class T>
	static void SafeDeleteArray(T** obj_)
	{
		if(*obj_ != nullptr)
		{
			delete[] (*obj_);
			(*obj_) = nullptr;
		}
	}
	static RECT Rect(int x, int y, int w, int h)
	{
		RECT t = {x, y, x + w, y + h};
		return t;
	}

	class Box2D
	{
	public:
		int x, y, w, h;
		Box2D( )
		:x(0), y(0), w(0), h(0)
		{
		}
		Box2D(int x_, int y_, int w_, int h_)
		:x(x_), y(y_), w(w_), h(h_)
		{
		}
		Box2D(const Box2D& b_)
		:x(b_.x), y(b_.y), w(b_.w), h(b_.h)
		{
		}
		bool Hit(const  Box2D& b_) const
		{
			if( x		< b_.x + b_.w	&&
				b_.x	< x + w			&&
				y		< b_.y + b_.h	&&
				b_.y	< y + h			){
				return true;
			}
			return false;
		}
		bool Hit(const  Vec2& b_) const
		{
			if( x		< b_.x	&&
				b_.x	< x + w	&&
				y		< b_.y	&&
				b_.y	< y + h	){
				return true;
			}
			return false;
		}
		bool Hit(const  POINT& b_) const
		{
			if( x		< b_.x	&&
				b_.x	< x + w	&&
				y		< b_.y	&&
				b_.y	< y + h	){
				return true;
			}
			return false;
		}
		void Offset(const  Vec2&  v_)
		{
			x += (int)v_.x;
			y += (int)v_.y;
		}
		void Offset(float x_, float y_)
		{
			x += (int)x_;
			y += (int)y_;
		}
		void Offset(int x_, int y_)
		{
			x += x_;
			y += y_;
		}
		void Offset(POINT  p_)
		{
			x += p_.x;
			y += p_.y;
		}
		RECT Rect( ) const
		{
			RECT r = {x,y,x+w,y+h};
			return r;
		}
		Box2D OffsetCopy(int x_, int y_) const
		{
			Box2D r(*this);
			r.Offset(x_, y_);
			return r;
		}
		Box2D OffsetCopy(POINT p_) const
		{
			Box2D r(*this);
			r.Offset(p_);
			return r;
		}
		Box2D OffsetCopy(float x_, float y_) const
		{
			Box2D r(*this);
			r.Offset(x_, y_);
			return r;
		}
		Box2D OffsetCopy(const  Vec2&  v_) const
		{
			Box2D r(*this);
			r.Offset(v_);
			return r;
		}
	};


	class Box3D
	{
	public:
		int x, y, z, w, h, d;
		Box3D( )
		:x(0), y(0), z(0), w(0), h(0), d(0)
		{
		}
		Box3D(int x_, int y_, int z_, int w_, int h_, int d_)
		:x(x_), y(y_), z(z_), w(w_), h(h_), d(d_)
		{
		}
		Box3D(const Box3D&  b_)
		:x(b_.x), y(b_.y), z(b_.z), w(b_.w), h(b_.h), d(b_.d)
		{
		}
		bool Hit(const  Box3D&  b_) const
		{
			if( x		< b_.x + b_.w	&&
				b_.x	< x + w			&&
				y		< b_.y + b_.h	&&
				b_.y	< y + h			&&
				z		< b_.z + b_.d	&&
				b_.z	< z + d			){
				return true;
			}
			return false;
		}
		bool Hit(const  Vec3&  b_) const
		{
			if( x		< b_.x	&&
				b_.x	< x + w &&
				y		< b_.y	&&
				b_.y	< y + h	&&
				z		< b_.z	&&
				b_.z	< z + d	){
				return true;
			}
			return false;
		}
		void Offset(int x_, int y_, int z_){	x += x_;	y += y_;	z += z_;	}
		void Offset(float x_, float y_, float z_){	x += (int)x_;	y += (int)y_;	z += (int)z_;	}
		void Offset(const  Vec3&  v_){	x += (int)v_.x;	y += (int)v_.y;	z += (int)v_.z;	}
		Box3D OffsetCopy(int x_, int y_, int z_) const
		{
			Box3D r(*this);
			r.Offset(x_, y_, z_);
			return r;
		}
		Box3D OffsetCopy(float x_, float y_, float z_) const
		{
			Box3D r(*this);
			r.Offset(x_, y_, z_);
			return r;
		}
		Box3D OffsetCopy(const  Vec3&  v_) const
		{
			Box3D r(*this);
			r.Offset(v_);
			return r;
		}
	};
	int	CheckStrings(TCHAR* s_[], int n_, const string& c_);
	extern map<string, bool>  msgboxlog;
	void  MsgBox(const string&  ms);

}

#include "xnacollision.h"

namespace DG_{
	class  VertexBuffer;
	class  IndexBuffer;
}
namespace ML
{
	namespace Collsion	//COLLISION
	{
		//���C�ƃ|���S���̐ڐG����
		bool HitRayTriangle(
			const  ML::Vec3&  p_,		//���C�n�_
			const  ML::Vec3&  d_,		//���C�����x�N�g��
			const  ML::Vec3&  v0_,		//�|���S�����_
			const  ML::Vec3&  v1_,		//�|���S�����_
			const  ML::Vec3&  v2_,		//�|���S�����_
			float&  dist_);			//�ڐG�_�܂ł̋����i�߂�lfalse�Ȃ�g�p�s�j



		//���C�ƃo�[�e�b�N�X�o�b�t�@�̐ڐG����i�o�[�e�b�N�X�o�b�t�@�̍\���ɒ��Ӂj
		//���_���R���ɂP�|���S�����\�����Ă���f�[�^�łȂ���΋@�\���Ȃ�
		bool HitRayTriangle(
			const  ML::Vec3&  p_,						//���C�n�_
			const  ML::Vec3&  d_,						//���C�����x�N�g��
			bool  qmode_,								//�N�C�b�N���[�h�i�ڐG����|���S�����������瑦�I���j
			shared_ptr<DG_::VertexBuffer>		vb_,	//�o�[�e�b�N�X�o�b�t�@
			const  UINT pos_,							//���_����̒��_���W�̈ʒu�i�o�C�g���j
			float&  dist_);							//�ڐG�_�܂ł̋����i�߂�lfalse�Ȃ�g�p�s�j
		//���C�ƃo�[�e�b�N�X�o�b�t�@�̐ڐG����
		//���_�C���f�b�N�X���܂ޏꍇ
		bool HitRayTriangle(
			const  ML::Vec3&  p_,						//���C�n�_
			const  ML::Vec3&  d_,						//���C�����x�N�g��
			bool  qmode_,								//�N�C�b�N���[�h�i�ڐG����|���S�����������瑦�I���j
			shared_ptr<DG_::VertexBuffer>		vb_,	//�o�[�e�b�N�X�o�b�t�@
			const  UINT pos_,							//���_����̒��_���W�̈ʒu�i�o�C�g���j
			shared_ptr<DG_::IndexBuffer>			ib_,	//�C���f�b�N�X�o�b�t�@
			float&  dist_);							//�ڐG�_�܂ł̋����i�߂�lfalse�Ȃ�g�p�s�j
		//�}�`�N���X�̊��N���X----------------------------------------------------
		class Shape
		{
		public:
			//�g�����(�ڐG����ŐڐG�̗L���ȊO�̏�񂪕K�v�ȂƂ��g���j
			struct  ExtParam{
				ML::Vec3	pushOut;	//�r�o�����x�N�g��	Ray�ȊO
				ExtParam()
					:pushOut(0, 0, 0)
				{}
			};
		protected:
			enum  KIND{ eUnNon, eAABB, eSphere, eOBB };
			KIND  kind;
		public:
			ML::Vec3			pos;	//���S�_				ALL?
			ML::Vec3			size_h;	//�����Ƃ̃T�C�Y(�����j AABB  OBB
			float				rad;	//���a					Sphere
			ML::QT				angle;	//�����i�N�I�[�^�j�I���jOBB
			typedef  shared_ptr<Shape>    SP;
		protected:
			//�����o�ϐ��̃N���A�i�����o��ǉ�������Y�ꂸ�ɃN���A�������ǉ�����j
			Shape()
				:kind(Shape::eUnNon)
				, pos(0, 0, 0)
				, size_h(0, 0, 0)
				, rad(0)
				, angle(0)
			{};
			//�e�}�`�Ƃ̐ڐG����i�����ǉ��j
			virtual bool Hit(const class AABB&   b_, ExtParam&  ex_) = 0;		//AABB�Ƃ̐ڐG
			virtual bool Hit(const class Sphere&   b_, ExtParam&  ex_) = 0;		//���̂Ƃ̐ڐG
			virtual bool Hit(const class OBB&   b_, ExtParam&  ex_) = 0;		//OBB�Ƃ̐ڐG
		public:
			virtual bool Hit(const Shape::SP  b_, ExtParam&  ex_ = ExtParam());			//�ڐG����
			virtual bool HitRay(const ML::Vec3&  p_,
								const ML::Vec3&  d_,
								float&  dist_) = 0;			//Ray�Ƃ̐ڐG
			virtual Shape::SP   Clone() = 0;					//�R�s�[�𐶐����ĕԂ�
			virtual void  Offset(const  ML::Vec3  ofs_){ pos += ofs_; }	//�ړ�������
			virtual void  Rotation(const  ML::QT  rot_){ angle *= rot_; }	//��]������
		};
		//�`�`�a�a�N���X----------------------------------------------------
		class AABB : public Shape
		{
		public:
			typedef  shared_ptr<AABB>    SP;
		private:
			AABB(){}
			bool Initialize(const ML::Vec3&  p_, const ML::Vec3& s_);
			//���}�`�𑝂₵����ǉ���
			bool Hit(const class AABB&   b_, ExtParam&  ex_);		//AABB�Ƃ̐ڐG
			bool Hit(const class Sphere&   b_, ExtParam&  ex_);		//���̂Ƃ̐ڐG
			bool Hit(const class OBB&   b_, ExtParam&  ex_);		//OBB�Ƃ̐ڐG
		public:
			bool HitRay(const ML::Vec3&  p_,
						const ML::Vec3&  d_,
						float&  dist_);					//Ray�Ƃ̐ڐG
			static  SP  Create(const ML::Vec3&  p_, const ML::Vec3&  s_);
			Shape::SP   Clone();		//�R�s�[�𐶐����ĕԂ�
		};
		//���̃N���X----------------------------------------------------
		class Sphere : public Shape
		{
		public:
			typedef  shared_ptr<Sphere>    SP;
		private:
			Sphere(){}
			bool Initialize(const ML::Vec3&  p_, float  r_);
			//���}�`�𑝂₵����ǉ���
			bool Hit(const class AABB&   b_, ExtParam&  ex_);		//AABB�Ƃ̐ڐG
			bool Hit(const class Sphere&   b_, ExtParam&  ex_);		//���̂Ƃ̐ڐG
			bool Hit(const class OBB&   b_, ExtParam&  ex_);		//OBB�Ƃ̐ڐG
		public:
			bool HitRay(const ML::Vec3&  p_,
						const ML::Vec3&  d_,
						float&  dist_);					//Ray�Ƃ̐ڐG
			static  SP  Create(const ML::Vec3&  p_, float  r_);
			Shape::SP   Clone();		//�R�s�[�𐶐����ĕԂ�
		};
		//�n�a�a�N���X----------------------------------------------------
		class OBB : public Shape
		{
		public:
			typedef  shared_ptr<OBB>    SP;
		private:
			OBB(){}
			bool Initialize(const  ML::Vec3&  p_, const  ML::Vec3&  s_, const  ML::QT&  a_);
			//���}�`�𑝂₵����ǉ���
			bool Hit(const class AABB&   b_, ExtParam&  ex_);		//AABB�Ƃ̐ڐG
			bool Hit(const class Sphere&   b_, ExtParam&  ex_);		//���̂Ƃ̐ڐG
			bool Hit(const class OBB&   b_, ExtParam&  ex_);		//OBB�Ƃ̐ڐG
		public:
			bool HitRay(const ML::Vec3&  p_,
						const ML::Vec3&  d_,
						float&  dist_);					//Ray�Ƃ̐ڐG
			static  SP  Create(const ML::Vec3&  p_, const ML::Vec3&  s_, const ML::QT&  a_);
			Shape::SP   Clone();		//�R�s�[�𐶐����ĕԂ�
		};
	//�����}�`�N���X----------------------------------------------------
		class ShapeContainer
		{
		public:
			typedef  shared_ptr<ShapeContainer>    SP;
			vector<Shape::SP>  child;
			//�����ŏ���������Ǝq���̐}�`�͒u���Ă��ڂ�ɂȂ�̂Œ��Ӂiprivate���]�܂����j
			ML::Vec3			pos;	//���S�_
			ML::QT				angle;	//�����i�N�I�[�^�j�I���j
		protected:
			ShapeContainer()
				:pos(0, 0, 0)
				, angle(0)
			{}
		private:
			bool Initialize();
		public:
			static  SP  Create();
			virtual  bool Hit(const ShapeContainer::SP  b_, struct  Shape::ExtParam&  ep_);	//�ڐG����
			virtual  bool Hit(const Shape::SP  b_, struct  Shape::ExtParam&  ep_);			//�ڐG����
			virtual  bool HitRay(const ML::Vec3&  p_,
				const ML::Vec3&  d_,
				float&  dist_);			//Ray�Ƃ̐ڐG
			virtual  ShapeContainer::SP   Clone();		//�R�s�[�𐶐����ĕԂ�

			virtual  void  Offset(const  ML::Vec3  ofs_);	//�ړ�������
			virtual  void  Rotation(const  ML::QT  rot_);	//��]������
		};
		//�����}�`�N���X(�J�v�Z���^)----------------------------------------------------
		class  Capsule : public  ShapeContainer
		{
		public:
			typedef  shared_ptr<Capsule>    SP;
		private:
			Capsule(){ }
			bool Initialize(const ML::Vec3&  p_, float  tb_, float  r_);
		public:
			static  SP  Create(const ML::Vec3&  p_, float  tb_, float  r_);
		};
	}
}