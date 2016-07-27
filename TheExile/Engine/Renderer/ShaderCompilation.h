#pragma once

#include "Lights.h"

// Option Variants //
#define NORMAL_MAP 2
#define SPEC_MAP 2
#define SPEC_OVERRIDE 2
#define TRANSPARENCY_OVERRIDE 2
#define MAX_VARIANTS 10

struct ShaderMacro
{
	LPCSTR chMacroName;
	unsigned int nVariants;
};

class Shader
{
	std::type_index cType;

	std::string* stringArr;
	std::vector<unsigned int> nCurrentValues;

	unsigned int num = (rand() % 90) + 6;
	unsigned int nDots = 0;
	unsigned int nShadersCreated = 0;
	unsigned int nVariations = 1; // No multiply by zero
	std::vector<ShaderMacro> tMacroArr;

	// File Options //
	LPCWSTR m_chFilePath;
	LPCSTR m_chEntryPoint;
	LPCSTR m_chShaderModel;

	union // Shader Arrays //
	{
		ID3D11VertexShader** d3dVertexShader;
		ID3D11PixelShader** d3dPixelShader;
		
		ID3D11HullShader** d3dHullShader;
		ID3D11DomainShader** d3dDomainShader;
		ID3D11GeometryShader** d3dGeometryShader;

		ID3D11ComputeShader** d3dComputeShader;
	};

	unsigned int FlattenIndex(std::vector<unsigned int> nMacroValues, unsigned int nIndex = 0, unsigned int nDepth = 0); // Needs to be super fast // Being used to index into an array
	void CreateShaders(std::vector<unsigned int> tCurrentOptions, unsigned int nDepth); // Recursive shader creation function
	ID3DBlob* CompileShader(LPCWSTR chFilePath, LPCSTR cEntryPoint, LPCSTR chShaderModel, D3D_SHADER_MACRO* d3dMacros); // Actual compiling of individual shaders.

public:

	Shader(std::type_index cShaderType) : cType(cShaderType) {} // Temp - DELETE -
	Shader(std::type_index cShaderType, std::vector<ShaderMacro> tMacros, LPCWSTR chFilePath, LPCSTR chEntryPoint, LPCSTR chShaderModel);
	~Shader();

	void SetOptions(std::vector<unsigned int> nMacroValues) { nCurrentValues = nMacroValues; }

	// Operator= Overloads // - Requires nCurrentValues to be kept up to date. Set it every context switch
	operator ID3D11VertexShader*() { return d3dVertexShader[FlattenIndex(nCurrentValues)]; }
	operator ID3D11PixelShader*() { return d3dPixelShader[FlattenIndex(nCurrentValues)]; }
	
	operator ID3D11HullShader*() { return d3dHullShader[FlattenIndex(nCurrentValues)]; }
	operator ID3D11DomainShader*() { return d3dDomainShader[FlattenIndex(nCurrentValues)]; }
	operator ID3D11GeometryShader*() { return d3dGeometryShader[FlattenIndex(nCurrentValues)]; }

	operator ID3D11ComputeShader*() { return d3dComputeShader[FlattenIndex(nCurrentValues)]; }

	// Templated Accessors // - Original declarations. Do not use
	template <typename ShaderType> static void Create(Shader*& d3dBlankShader, std::vector<ShaderMacro> tMacros, LPCWSTR chFilePath, LPCSTR chEntryPoint, LPCSTR chShaderModel) { assert(!"Shader::Create needs to be given an actual DirectX shader type. Example: <ID3D11VertexShader>"); }

	// Specialized Template Accessors //
	template <> static void Create<ID3D11VertexShader>(Shader*& d3dBlankShader, std::vector<ShaderMacro> tMacros, LPCWSTR chFilePath, LPCSTR chEntryPoint, LPCSTR chShaderModel) { d3dBlankShader = new Shader(typeid(ID3D11VertexShader), tMacros, chFilePath, chEntryPoint, chShaderModel); }
	template <> static void Create<ID3D11PixelShader>(Shader*& d3dBlankShader, std::vector<ShaderMacro> tMacros, LPCWSTR chFilePath, LPCSTR chEntryPoint, LPCSTR chShaderModel) { d3dBlankShader = new Shader(typeid(ID3D11PixelShader), tMacros, chFilePath, chEntryPoint, chShaderModel); }
	
	template <> static void Create<ID3D11HullShader>(Shader*& d3dBlankShader, std::vector<ShaderMacro> tMacros, LPCWSTR chFilePath, LPCSTR chEntryPoint, LPCSTR chShaderModel) { d3dBlankShader = new Shader(typeid(ID3D11HullShader), tMacros, chFilePath, chEntryPoint, chShaderModel); }
	template <> static void Create<ID3D11DomainShader>(Shader*& d3dBlankShader, std::vector<ShaderMacro> tMacros, LPCWSTR chFilePath, LPCSTR chEntryPoint, LPCSTR chShaderModel) { d3dBlankShader = new Shader(typeid(ID3D11DomainShader), tMacros, chFilePath, chEntryPoint, chShaderModel); }
	template <> static void Create<ID3D11GeometryShader>(Shader*& d3dBlankShader, std::vector<ShaderMacro> tMacros, LPCWSTR chFilePath, LPCSTR chEntryPoint, LPCSTR chShaderModel) { d3dBlankShader = new Shader(typeid(ID3D11GeometryShader), tMacros, chFilePath, chEntryPoint, chShaderModel); }

	template <> static void Create<ID3D11ComputeShader>(Shader*& d3dBlankShader, std::vector<ShaderMacro> tMacros, LPCWSTR chFilePath, LPCSTR chEntryPoint, LPCSTR chShaderModel) { d3dBlankShader = new Shader(typeid(ID3D11ComputeShader), tMacros, chFilePath, chEntryPoint, chShaderModel); }
};