#include "ShaderCompilation.h"

#include <d3dcompiler.h>
#include "Renderer.h"

#pragma comment(lib, "d3dcompiler.lib")

Shader::Shader(std::type_index cShaderType, std::vector<ShaderMacro> tMacros, LPCWSTR chFilePath, LPCSTR chEntryPoint, LPCSTR chShaderModel) : cType(cShaderType)
{
	tMacroArr = tMacros; // Store current set of macros
	m_chFilePath = chFilePath;
	m_chEntryPoint = chEntryPoint;
	m_chShaderModel = chShaderModel;
	
	// Calculate Number Of Variations //
	for (unsigned int i = 0; i < tMacroArr.size(); i++)
		nVariations *= tMacroArr[i].nVariants;

	OUTPUT(std::string("Shaders To Compile: ") + std::to_string(nVariations) + "\n");

	// Allocate Shader Array //
	if (cType == typeid(ID3D11VertexShader))
		d3dVertexShader = new ID3D11VertexShader*[nVariations];
	else if (cType == typeid(ID3D11PixelShader))
		d3dPixelShader = new ID3D11PixelShader*[nVariations];
	else if (cType == typeid(ID3D11HullShader))
		d3dHullShader = new ID3D11HullShader*[nVariations];
	else if (cType == typeid(ID3D11DomainShader))
		d3dDomainShader = new ID3D11DomainShader*[nVariations];
	else if (cType == typeid(ID3D11GeometryShader))
		d3dGeometryShader = new ID3D11GeometryShader*[nVariations];
	else if (cType == typeid(ID3D11ComputeShader))
		d3dComputeShader = new ID3D11ComputeShader*[nVariations];
	else
		assert(false && "The Shader class needs to be passed in an actual DirectX shader type. Example: <ID3D11VertexShader> (Don't make it a pointer)");

	stringArr = new std::string[nVariations];

	// Set Up Recursive Call //
	std::vector<unsigned int> nMinValues;
	for (unsigned int i = 0; i < tMacroArr.size(); i++)
		nMinValues.push_back(0);

	float fStartTime = CURRENT_TIME();
	CreateShaders(nMinValues, 0); // Recursive creation

	// Debug Info //
	OUTPUT(".......Done!" + std::string("\nTotal Shaders Compiled: ") + std::to_string(nShadersCreated) + "\n");
	OUTPUT(std::string("Time Elapsed: ") + std::to_string(CURRENT_TIME() - fStartTime) + " Seconds\n");
}

Shader::~Shader()
{
	for (unsigned int i = 0; i < nVariations; i++) // Release all shader variations
	{
		if (cType == typeid(ID3D11VertexShader))
			SAFE_RELEASE(d3dVertexShader[i])
		else if (cType == typeid(ID3D11PixelShader))
			SAFE_RELEASE(d3dPixelShader[i])
		else if (cType == typeid(ID3D11HullShader))
			SAFE_RELEASE(d3dHullShader[i])
		else if (cType == typeid(ID3D11DomainShader))
			SAFE_RELEASE(d3dDomainShader[i])
		else if (cType == typeid(ID3D11GeometryShader))
			SAFE_RELEASE(d3dGeometryShader[i])
		else if (cType == typeid(ID3D11ComputeShader))
			SAFE_RELEASE(d3dComputeShader[i]);
	}

	// Delete Array Of Pointers //
	if (cType == typeid(ID3D11VertexShader))
		delete[] d3dVertexShader;
	else if (cType == typeid(ID3D11PixelShader))
		delete[] d3dPixelShader;
	else if (cType == typeid(ID3D11HullShader))
		delete[] d3dHullShader;
	else if (cType == typeid(ID3D11DomainShader))
		delete[] d3dDomainShader;
	else if (cType == typeid(ID3D11GeometryShader))
		delete[] d3dGeometryShader;
	else if (cType == typeid(ID3D11ComputeShader))
		delete[] d3dComputeShader;
}

unsigned int Shader::FlattenIndex(std::vector<unsigned int> nMacroValues, unsigned int nIndex, unsigned int nDepth)
{
	//nIndex += (indexArr[nDepth] * maxArr[1] * maxArr[2] * maxArr[3]);
	
	unsigned int number = 1;
	for (unsigned int i = nDepth; i < nMacroValues.size(); i++)
		(i == nDepth) ? (number *= nMacroValues[nDepth]) : (number *= (tMacroArr[i].nVariants));
	
	nIndex += number;
	
	if (++nDepth == nMacroValues.size())
		return nIndex;
	else
		return FlattenIndex(nMacroValues, nIndex, nDepth);
}

void Shader::CreateShaders(std::vector<unsigned int> nMacroValues, unsigned int nDepth)
{
	#if !_DEBUG
		if (nDots++ > num && nShadersCreated < nVariations)
		{
			OUTPUT(std::to_string(nShadersCreated) + "\n");
			num = (rand() % 90) + 6; // Rand from 5 - 95
			nDots = 0;
		}
		else
			OUTPUT(".");
	#endif

	for (unsigned int& nCurrentMacroValue = nMacroValues[nDepth]; nCurrentMacroValue < tMacroArr[nDepth].nVariants; nCurrentMacroValue++)
	{
		if (nDepth == tMacroArr.size() - 1) // Check if we're on the last macro
		{
			// Create a macro set based on passed in CompileOptions, and compile this recursive instance's version of the shader
			std::vector<string*> stringPtrs; // Strings will leak, and they have to be dynamically allocated
			D3D_SHADER_MACRO* d3dMacros = new D3D_SHADER_MACRO[tMacroArr.size() + 1]; // Needs to be dynamically allocated because the size of the array is dependent on number of macros passed in
			for (unsigned int i = 0; i < tMacroArr.size(); i++)
			{
				stringPtrs.push_back(new string(to_string(nMacroValues[i])));
				d3dMacros[i] = { tMacroArr[i].chMacroName, stringPtrs[i]->c_str() };

				#if _DEBUG
					OUTPUT(std::string(d3dMacros[i].Name) + "=" + d3dMacros[i].Definition + "  ");
				#endif
			}
			d3dMacros[tMacroArr.size()] = { NULL, NULL };

			#if _DEBUG
				OUTPUT("\n");
			#endif

			// Create And Store Compiled Shader // - All types of shaders
			ID3DBlob* d3dCompiledShader = CompileShader(m_chFilePath, m_chEntryPoint, m_chShaderModel, d3dMacros); // Create shader blob

			HRESULT result;
			if (cType == typeid(ID3D11VertexShader))
				result = GRAPHICS->GetDevice()->CreateVertexShader(d3dCompiledShader->GetBufferPointer(), d3dCompiledShader->GetBufferSize(), nullptr, &d3dVertexShader[FlattenIndex(nMacroValues)]);
			else if (cType == typeid(ID3D11PixelShader))
				result = GRAPHICS->GetDevice()->CreatePixelShader(d3dCompiledShader->GetBufferPointer(), d3dCompiledShader->GetBufferSize(), nullptr, &d3dPixelShader[FlattenIndex(nMacroValues)]);
			else if (cType == typeid(ID3D11HullShader))
				result = GRAPHICS->GetDevice()->CreateHullShader(d3dCompiledShader->GetBufferPointer(), d3dCompiledShader->GetBufferSize(), nullptr, &d3dHullShader[FlattenIndex(nMacroValues)]);
			else if (cType == typeid(ID3D11DomainShader))
				result = GRAPHICS->GetDevice()->CreateDomainShader(d3dCompiledShader->GetBufferPointer(), d3dCompiledShader->GetBufferSize(), nullptr, &d3dDomainShader[FlattenIndex(nMacroValues)]);
			else if (cType == typeid(ID3D11GeometryShader))
				result = GRAPHICS->GetDevice()->CreateGeometryShader(d3dCompiledShader->GetBufferPointer(), d3dCompiledShader->GetBufferSize(), nullptr, &d3dGeometryShader[FlattenIndex(nMacroValues)]);
			else if (cType == typeid(ID3D11ComputeShader))
				result = GRAPHICS->GetDevice()->CreateComputeShader(d3dCompiledShader->GetBufferPointer(), d3dCompiledShader->GetBufferSize(), nullptr, &d3dComputeShader[FlattenIndex(nMacroValues)]);

			if (FAILED(result))
				assert(!"Shader creation failed!");

			nShadersCreated++;

			delete[] d3dMacros;
			SAFE_RELEASE(d3dCompiledShader);
			for (unsigned int i = 0; i < stringPtrs.size(); i++) delete stringPtrs[i];
		}
		else // Haven't reached final depth
		{
			std::vector<unsigned int> tNewValues = nMacroValues;
			tNewValues[nDepth] = nCurrentMacroValue; // Update with current value based on iteration of for loop
			CreateShaders(tNewValues, nDepth + 1);
		}
	}
}

ID3DBlob* Shader::CompileShader(LPCWSTR chFilePath, LPCSTR chEntryPoint, LPCSTR chShaderModel, D3D_SHADER_MACRO* d3dMacros)
{
	UINT flags = D3DCOMPILE_WARNINGS_ARE_ERRORS;

	#if _DEBUG
		flags |= D3DCOMPILE_DEBUG;
	#endif

	ID3DBlob* d3dShaderBlob;
	ID3DBlob* d3dErrorBlob;
	HRESULT result = D3DCompileFromFile(chFilePath, d3dMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, chEntryPoint, chShaderModel,
		flags, NULL, &d3dShaderBlob, &d3dErrorBlob);

 	if (FAILED(result))
	{
		if (d3dErrorBlob)
		{
			OUTPUT((char*)d3dErrorBlob->GetBufferPointer()); // Error message
			assert(!"Shader Blob creation failed!");
		}

		SAFE_RELEASE(d3dErrorBlob);
		SAFE_RELEASE(d3dShaderBlob);
		return nullptr;
	}

	SAFE_RELEASE(d3dErrorBlob);
	return d3dShaderBlob;
}