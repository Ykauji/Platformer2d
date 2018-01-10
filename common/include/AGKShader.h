// Image Header
#ifndef _H_AGKSHADER_
#define _H_AGKSHADER_

// Common includes
#include "Common.h"
#include "3DMath.h"
#include "uString.h"
#include "cMaterial.h"

#define AGK_SHADER_WORLD_CHANGED		0x0001
#define AGK_SHADER_VIEW_CHANGED			0x0002
#define AGK_SHADER_PROJ_CHANGED			0x0004
#define AGK_SHADER_WORLDNORMAL_CHANGED	0x0008
#define AGK_SHADER_ORTHO_CHANGED		0x0010
#define AGK_SHADER_RELOAD_UNIFORMS		0x0020
//#define AGK_SHADER_CAMERA_CHANGED		0x0040
#define AGK_SHADER_PER_SPRITE_UNIFORM	0x0080
#define AGK_SHADER_ATTRIBUTES_CHANGED	0x0100
#define AGK_SHADER_IS_CUSTOM			0x0200 // is a user custom shader or AGK generated shader
#define AGK_SHADER_USES_FOG				0x0400
#define AGK_SHADER_USES_PS_LIGHTING		0x0800 // shader needs, or is currently using, pixel lighting
#define AGK_SHADER_USES_VS_LIGHTING		0x1000 // shader needs, or is currently using, vertex lighting
#define AGK_SHADER_IS_DEFAULT			0x2000 // a core shader that should never be deleted

#define AGK_TRIANGLES 0
#define AGK_TRIANGLE_STRIP 1
#define AGK_TRIANGLE_FAN 2
#define AGK_LINE_LOOP 3
#define AGK_LINES 4
#define AGK_POINTS 5

#define AGK_MAX_ATTRIBUTES 16 
#define AGK_MAX_VERTEX_LIGHTS 8 // if greater than 16 then modify the nums array to include additional characters
#define AGK_MAX_PIXEL_LIGHTS 4 // if greater than 16 then modify the nums array to include additional characters

// Namespace
namespace AGK
{
	class cCamera;

	struct cShaderAttrib
	{
		uString m_sName; // position, normal, texcoord, color, etc
		int m_iType; // 0=float, 1=matrix
		int m_iLocation; // max 16 on most cards
		UINT m_iComponents; // valid values: 1,2,3,4 for float or 2,3,4 for matrix
		cShaderAttrib *m_pNextAttrib;

		cShaderAttrib() 
		{ 
			m_iType = 0;
			m_iLocation = -1;
			m_iComponents = 4;
			m_pNextAttrib = 0;
		}

		~cShaderAttrib() { }
	};

	struct cShaderUniform
	{
		uString m_sName; // custom name
		int m_iType; // 0=float, 1=matrix
		int m_iLocation;
		UINT m_iComponents; // valid values: 1,2,3,4 for float or 2,3,4 for matrix
		UINT m_iArrayMembers;
		cShaderUniform *m_pNextUniform;
		bool m_bChanged;
		float* m_pValues;

		cShaderUniform() 
		{ 
			m_iType = 0;
			m_iLocation = -1;
			m_iComponents = 4;
			m_iArrayMembers = 1;
			m_pNextUniform = 0;
			m_bChanged = false;
			m_pValues = 0;
		}

		~cShaderUniform() 
		{
			if ( m_pValues ) delete [] m_pValues;
		}
	};

	class AGKShader
	{
		friend class cObject3D;

		protected:
			UINT m_iShaderID;
			uString m_sVSFilename;
			uString m_sPSFilename;
			bool m_bValid;
			bool m_bReloading;
			bool m_bDeleting;

			AGKShader* m_pBaseShader;
			UINT m_iShaderHash;
			cHashedList<AGKShader> m_cDerivedShaders;
			void AddDerived( AGKShader *pShader );
			void RemoveDerived( AGKShader *pShader );

			uString m_sVSSource;
			uString m_sPSSource;

			// uniforms (VS + PS)
			int m_iUniformWorldMat;
			int m_iUniformNormalMat;
			int m_iUniformViewMat;
			int m_iUniformOrthoMat;
			int m_iUniformProjMat;
			int m_iUniformVPMat;
			int m_iUniformWVPMat;
			int m_iUniformWVOMat;

			int m_iNumUniforms;
			cHashedList<cShaderUniform> m_cUniformList;
			cShaderUniform* m_pChangedUniforms;

			// agk uniforms
			int m_iAGKTime;
			int m_iAGKSinTime;
			int m_iAGKResolution;
			int m_iAGKInvert;
			int m_iAGKCameraPos;
			
			// textures
			int m_iTexture2D[ AGK_MAX_TEXTURES ];
			int m_iTextureCube[ AGK_MAX_TEXTURES ];
			int m_iTextureBounds[ AGK_MAX_TEXTURES ];
			float m_fTextureU1[ AGK_MAX_TEXTURES ];
			float m_fTextureV1[ AGK_MAX_TEXTURES ];
			float m_fTextureU2[ AGK_MAX_TEXTURES ];
			float m_fTextureV2[ AGK_MAX_TEXTURES ];
			UINT m_bTextureChanged;

			// vertex attributes, copy some common attributes to separate variables
			int m_iPositionAttrib;
			int m_iNormalAttrib;
			int m_iColorAttrib;
			int m_iTexCoordAttrib;
			cHashedList<cShaderAttrib> m_cAttribList;
			int m_iNumAttribs;

			UINT m_bFlags;

			cCamera *m_pCurrentCamera;

			// matrices
			static AGKMatrix4 g_matOrtho;
			AGKMatrix4 m_matProj;
			AGKMatrix4 m_matView;
			AGKMatrix4 m_matWorld;
			AGKMatrix3 m_matWorldNormal;

			uString m_sVSLog;
			uString m_sPSLog;
			uString m_sLinkLog;

			int m_iMeshUseCount;

			AGKShader *m_pPrevShader;
			AGKShader *m_pNextShader;

			static AGKShader *g_pAllShaders;
			static AGKShader *g_pLastShader;
			static int g_iNumShaders;
			static char g_iAttributeActive[ AGK_MAX_ATTRIBUTES ];

			static float g_fFogColorR, g_fFogColorG, g_fFogColorB;
			static float g_fFogColor2R, g_fFogColor2G, g_fFogColor2B;
			static float g_fFogMinDist, g_fFogMaxDist;

			void PlatformInit();
			void PlatformDelete();
			//void PlatformDrawTriangles( int first, UINT count );
			void PlatformDrawPrimitives( UINT primitive, int first, UINT count );
			void PlatformDrawIndices( UINT primitive, UINT count, unsigned short *pIndices );
			void PlatformDrawIndicesInt( UINT primitive, UINT count, unsigned int *pIndices );
			void PlatformSetUniform( int location, int size, float* values );

			static AGKShader *g_pCurrentShader;

		public:

			static AGKShader* GetCurrentShader() { return g_pCurrentShader; }

			// default sprite shaders
			static AGKShader *g_pShaderColor;
			static AGKShader *g_pShaderTexColor;
			static AGKShader *g_pShaderFont;

			// default object shaders
			static AGKShader *g_pObjectQuad;

			static void SetProjMatrices( const float *matrix4x4 );
			static void SetOrthoMatrices( const float *matrix4x4 );
			static void NoShader();
			static void CreateDefaultShaders();
			static void SetCurrentCamera( cCamera* pCamera );

			static void ReloadAll();
			static void UpdateAllCamera();

			static void SetFogColor( float red, float green, float blue );
			static void SetFogSunColor( float red, float green, float blue );
			static void SetFogRange( float minDist, float maxDist );

			AGKShader();
			~AGKShader();

			void AddRef();
			void RemoveRef();

			const char* GetVSLog() const { return m_sVSLog.GetStr(); }
			const char* GetPSLog() const { return m_sPSLog.GetStr(); }
			const char* GetVSFilename() const { return m_sVSFilename.GetStr(); }
			const char* GetPSFilename() const { return m_sPSFilename.GetStr(); }

			bool IsValid() const { return m_bValid; }
			void SetValid( bool valid ) { m_bValid = valid; }
			UINT GetHash() const { return m_iShaderHash; }
			AGKShader* GetBaseShader() const { return m_pBaseShader; }
			int IsCustom() const { return (m_bFlags & AGK_SHADER_IS_CUSTOM) ? 1 : 0; }
			int NeedsAdditionalCode() const { return (m_bFlags & (AGK_SHADER_USES_VS_LIGHTING|AGK_SHADER_USES_PS_LIGHTING|AGK_SHADER_USES_FOG)) ? 1 : 0; }
			int NeedsFog() const { return (m_bFlags & AGK_SHADER_USES_FOG) ? 1 : 0; }
			int NeedsVSLighting() const { return (m_bFlags & AGK_SHADER_USES_VS_LIGHTING) ? 1 : 0; }
			int NeedsPSLighting() const { return (m_bFlags & AGK_SHADER_USES_PS_LIGHTING) ? 1 : 0; }

			void LoadShader( const char* vertexFile, const char* pixelFile );
			void LoadFullScreenShader( const char* pixelFile );
			void LoadSpriteShader( const char* pixelFile );
			void SetVideoTextureShader();
			void SetCameraTextureShader();
			void SetShaderSource( const char* vertex, const char* pixel );

			void MakeColorShader(); // for 2D
			void MakeTexColorShader(); // for 2D
			void MakeFontShader(); // For Freetype font rendering
			void MakeQuadShader(); // fullscreen quad
			void MakeSkyBoxSunShader(); // skybox with sun
			void MakeSkyBoxNoSunShader(); // skybox no sun

			static UINT GetMeshShaderHash( cMesh *pMesh );
			static UINT GetFinalShaderHash( int sunActive, int VSLights, int PSLights );
			static AGKShader* Make3DShader( cMesh *pMesh );
			static AGKShader* MakeFinalShader( AGKShader *pBaseShader, int sunActive, int VSLights, int PSLights );
			
			void MakeActive();
			void UpdateMatrices();
			void UpdateUniforms();
			void UpdateAGKUniforms();

			int GetAttribByName( const char* name ) const; 
			int GetAttribPosition() const { return GetAttribByName( "position" ); } 
			int GetAttribNormal() const { return GetAttribByName( "normal" ); } 
			int GetAttribColor() const { return GetAttribByName( "color" ); } 
			int GetAttribTexCoord() const { return GetAttribByName( "uv" ); } 
			int GetAttribTexCoord1() const { return GetAttribByName( "uv1" ); } 

			const char* GetVSSource() const { return m_sVSSource.GetStr(); }
			const char* GetPSSource() const { return m_sPSSource.GetStr(); }

			int GetNumAttribs() const { return m_iNumAttribs; }
			int GetNumConstants() const { return m_iNumUniforms; }

			const char* GetFirstConstantName();
			const char* GetNextConstantName();

			cShaderUniform* GetFirstConstant();
			cShaderUniform* GetNextConstant();

			cShaderAttrib* GetFirstAttribute();
			cShaderAttrib* GetNextAttribute();

			int GetConstantExists( const char* name ) const;
			bool HasPerSpriteUniforms() const { return (m_bFlags & AGK_SHADER_PER_SPRITE_UNIFORM) != 0; }

			// set by objects to temporary use a specific value, shader must be active
			void SetTempConstantByName( const char* szName, float f1, float f2, float f3, float f4 );
			void SetTempConstantArrayByName( const char* szName, UINT index, float f1, float f2, float f3, float f4 );
			void SetTempConstantArrayByName( const char* szName, const float *values );
			void SetTempConstantMatrixByName( const char* szName, const float* values );
			void SetTempConstantMatrixArrayByName( const char* szName, UINT index, const float* values );

			// set by the user for objects that don't specify a temporary value
			void SetConstantByName( const char* szName, float f1, float f2, float f3, float f4 );
			void SetConstantArrayByName( const char* szName, UINT index, float f1, float f2, float f3, float f4 );
			void SetConstantMatrixByName( const char* szName, int numValues, const float* values );
			void SetConstantMatrixArrayByName( const char* szName, UINT index, int numValues, const float* values );

			void SetAttribFloat( UINT location, int size, int stride, const float* ptr ); // position, normal, texcoord, etc
			void SetAttribUByte( UINT location, int size, int stride, bool normalise, const unsigned char* ptr ); // color

			void SetWorldMatrix( const float *matrix4x4 ) { if ( m_matWorld == matrix4x4 ) return; m_matWorld.Set( matrix4x4 ); m_bFlags |= AGK_SHADER_WORLD_CHANGED; }
			void SetViewMatrix( const float *matrix4x4 ) { if ( m_matView == matrix4x4 ) return; m_matView.Set( matrix4x4 ); m_bFlags |= AGK_SHADER_VIEW_CHANGED; }
			void SetProjMatrix( const float *matrix4x4 ) { m_matProj.Set( matrix4x4 ); m_bFlags |= AGK_SHADER_PROJ_CHANGED; }
			void SetOrthoMatrixChanged() { m_bFlags |= AGK_SHADER_ORTHO_CHANGED; }
			void SetNormalMatrix( const float *matrix3x3 ) { if ( m_matWorldNormal == matrix3x3 ) return; m_matWorldNormal.Set( matrix3x3 ); m_bFlags |= AGK_SHADER_WORLDNORMAL_CHANGED; }

			void SetTextureStage( cImage *pImage, cMesh *pMesh, UINT stage );

			void DrawTriangles( int first, UINT count );
			void DrawPrimitives( int type, int first, UINT count ); // 0=TRIANGLES, 1=TRIANGLE_STRIP, 2=TRIANGLE_FAN, 3=LINE_LOOP, 4=LINES, 5=POINTS
			void DrawIndices( UINT count, unsigned short *pIndices, int primative=AGK_TRIANGLES );
			void DrawIndicesInt( UINT count, unsigned int *pIndices, int primative=AGK_TRIANGLES );
		};
}

#endif