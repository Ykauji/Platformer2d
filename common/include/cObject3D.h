#ifndef _H_OBJECT3D_
#define _H_OBJECT3D_

// Common includes
#include "Common.h"
#include "AGKShader.h"
#include "cMesh.h"
#include "cNode.h"

#define AGK_OBJECT_CUSTOM_SHADER		0x0001
#define AGK_OBJECT_USE_LIGHTS			0x0002
#define AGK_OBJECT_DRAW_BACKFACES		0x0004
#define AGK_OBJECT_Z_WRITE				0x0008
#define AGK_OBJECT_IDENTITY_TRANSFORM	0x0010
//#define AGK_OBJECT_UNIFORM_SCALE		0x0020
#define AGK_OBJECT_VISIBLE				0x0040
#define AGK_OBJECT_TRANSCHANGED			0x0080
#define AGK_OBJECT_MANAGED				0x0100
#define AGK_OBJECT_IS_QUAD				0x0200
#define AGK_OBJECT_NO_FRUSTUM_CULLING	0x0400
#define AGK_OBJECT_COLLISION_ON			0x0800
#define AGK_OBJECT_ALPHA_MASK			0x1000
#define AGK_OBJECT_USE_FOG				0x2000
#define AGK_OBJECT_IS_HEIGHTMAP			0x4000

#define AGK_OBJECT_MAX_BONES 100
//#define AGK_OBJECT_MAX_BONES_STR "100"

// Bullet physics forward declarations
class btRigidBody;


// Namespace
namespace AGK
{
	class cImage;
	class Skeleton3D;
	class Animation3D;
	class CollisionObject;
	class CollisionResults;

	class cObject3D : public cNode
	{
		protected:

			friend class agk;
			friend class cObjectMgr;
			friend class AGO;
			friend class cMesh;

			struct sObjectUniform
			{
				uString m_sName;
				int index;
				float v1;
				float v2;
				float v3;
				float v4;

				sObjectUniform() { index=-1; v1=0; v2=0; v3=0; v4=0; };
				sObjectUniform( const sObjectUniform* pOther ) { m_sName.SetStr(pOther->m_sName); index = pOther->index; v1 = pOther->v1; v2 = pOther->v2; v3 = pOther->v3; v4 = pOther->v4; };
				~sObjectUniform() {}
			};

			struct lightItem 
			{
				float dist;
				int index;
			};

			// Bone Animation
			Skeleton3D *m_pSkeleton;
			UINT m_iNumAnims;
			Animation3D **m_pAnims;
			UINT m_iNumChildren;
			UINT *m_iLoadedChildren; // list of object IDs that were loaded from the same model file

			// 3D Physics
			btRigidBody* m_pRigidBody;

			// unique ID for tier 1 only
			UINT m_iID;
			uString m_sName;

			// meshes that hold raw vertices and VBO vertices
			UINT m_iNumMeshes;
			cMesh **m_pMeshes;
			
			// shader variables
			cHashedList<sObjectUniform> m_cShaderVariables;

			// if object is instanced then it shares a collision object
			cObject3D *m_pSharedColObject;
			
			// local transform now stored in cNode
			
			// attributes
			UINT m_iObjFlags;
			AGKVector m_Color;
			float m_fAlpha;
			AGKVector m_ColorEmissive;
			unsigned char m_iTransparency;
			unsigned char m_iZFunc;
			unsigned char m_iCullMode; // 0=no culling, 1=front faces (default), 2=back faces
			float m_fZBias;
			float m_fDepthNear, m_fDepthFar;

			// collision
			CollisionObject *m_pColObject;

			// for quick terrain height look up
			unsigned short *m_pHeightMap;
			int m_iHeightMapPixelsX;
			int m_iHeightMapPixelsZ;
			float m_fHeightMapSizeX;
			float m_fHeightMapSizeY;
			float m_fHeightMapSizeZ;

			// global array of all objects
			cObject3D *m_pNextObject;
			cObject3D *m_pPrevObject;
			static cObject3D *g_pAllObjects;

			void PlatformSetupDrawing();

			void CreateCollisionData();

		public:
			static void ReloadAll();

			static int CompareLightItem( const void* a, const void* b );

			cObject3D();
			cObject3D( cObject3D *pOther, int share );
			~cObject3D();
			
			bool IsStandardSettings();

			bool IsManaged() { return (m_iObjFlags & AGK_OBJECT_MANAGED) != 0; }

			void DeleteMeshes();
			void DeleteChildren();
			void DeleteAnimations();

			void CreateBox( float width, float height, float length );
			void CreateSphere( float diameter, int rows, int columns );
			void CreateCapsule( float diameter, float height, int axis );
			void CreateCone( float height, float diameter, int segments );
			void CreateCylinder( float height, float diameter, int segments );
			void CreatePlane( float width, float height );
			void CreateQuad();
			void CreateFromHeightMap( const char *szHeightMap, float width, float height, float length, int smoothing, int split );
			void CreateFromMeshes( int numMeshes, cMesh **pMeshes );
			void AddMesh( cMesh *pMesh, int updateCollision=1 );

			UINT GetNumMeshes() { return m_iNumMeshes; }
			cMesh* GetMesh( UINT index );

			void LoadObject( const char *szFilename, int withChildren=0, float height=0 );
			void LoadOBJ( const char *szFilename, float height=0 );
			void LoadAGOAscii( const char *szFilename, float height=0 );

			void SaveObject( const char *szFilename );

			void SetPosition( float x, float y, float z );
			void SetRotationQuat( const AGKQuaternion &rot );
			void SetRotationEuler( float angX, float angY, float angZ );
			void SetScale( float x, float y, float z );
			void SetScalePermanent( float x, float y, float z );
			void FixPivot();

			void SetShader( AGKShader *pShader );
			void SetImage( cImage *pImage, UINT stage=0 );
			void SetLightMap( cImage *pImage );
			void SetColor( int red, int green, int blue, int alpha );
			void SetRed( int iRed );
			void SetGreen( int iGreen );
			void SetBlue( int iBlue );
			void SetAlpha( int iAlpha );
			void SetColorEmissive( int red, int green, int blue );
			void SetLightMode( UINT mode );
			void SetScreenCulling( UINT mode );
			void SetUVOffset( UINT stage, float offsetU, float offsetV );
			void SetUVScale( UINT stage, float scaleU, float scaleV );
			void SetFogMode( UINT mode );

			void SetDepthReadMode( UINT mode );
			void SetDepthWrite( UINT mode );
			void SetDepthBias( float bias );
			void SetDepthRange( float zNear, float zFar );
			void SetTransparency( UINT mode );
			void SetAlphaMask( UINT mode );
			void SetCullMode( UINT mode );
			void SetVisible( UINT mode );

			void UpdateCollisionData();

			// get
			UINT GetDepthReadMode() const { return m_iZFunc; }
			UINT GetDepthWrite() const { return (m_iObjFlags & AGK_OBJECT_Z_WRITE) ? 1 : 0; }
			float GetDepthBias() const { return m_fZBias; }
			UINT GetTransparency() const { return m_iTransparency; }
			UINT GetCullMode() const { return m_iZFunc; }
			UINT GetVisible() const { return (m_iObjFlags & AGK_OBJECT_VISIBLE) ? 1 : 0; }
			UINT GetInScreen();
			bool GetTransparencyChanged();
			bool HasAlphaMask() const { return (m_iObjFlags & AGK_OBJECT_ALPHA_MASK) ? 1 : 0; }
			bool CheckTransparencyChanged() const;
			int GetLightMode() const { return (m_iObjFlags & AGK_OBJECT_USE_LIGHTS) ? 1 : 0; }
			int GetFogMode() const { return (m_iObjFlags & AGK_OBJECT_USE_FOG) ? 1 : 0; }

			const char* GetName() { return m_sName.GetStr(); }

			float GetMinX() const;
			float GetMinY() const;
			float GetMinZ() const;
			float GetMaxX() const;
			float GetMaxY() const;
			float GetMaxZ() const;

			Skeleton3D* GetSkeleton() { return m_pSkeleton; }

			void SetShaderConstantByName( const char *name, float v1, float v2, float v3, float v4 );
			void SetShaderConstantArrayByName( const char *name, UINT index, float v1, float v2, float v3, float v4 );
			void SetShaderConstantDefault( const char *name );
			
			// collision
			void SetCollisionMode( int mode );

			bool RayCast( float oldx, float oldy, float oldz, float x, float y, float z, CollisionResults *res );
			bool RayCast( const AGKVector &p, const AGKVector &v, const AGKVector &vn, CollisionResults *res );

			bool SphereCast( float oldx, float oldy, float oldz, float x, float y, float z, float rRadius, CollisionResults* cRes);
			bool SphereCast( const AGKVector &p, const AGKVector &v, const AGKVector &vn, float rRadius, CollisionResults* cRes);

			bool SphereSlide( float oldx, float oldy, float oldz, float x, float y, float z, float rRadius, CollisionResults* cRes);
			bool SphereSlide( const AGKVector &p, const AGKVector &v, const AGKVector &vn, float rRadius, CollisionResults* cRes);

			//bool Intersects( cObject3D* obj2 );

			// height map look up
			float GetHeightMapHeight( float x, float z );

			void CheckLights();

			void Update( float time );
			void Draw(); // draw manually, all meshes, full setup, no batching. Batched rendering should not use this function

			void SetupDrawing(); // only does blend/depth states and per object shader variables (e.g. bones)
	};
}

#endif