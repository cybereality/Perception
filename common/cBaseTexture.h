#pragma once
#include "cBase.h"
#include <unordered_map>
/*
template<class T>
class cBaseTexture : public cBase<T> {
public:

	cBaseTexture( T* l , T* r , D3DProxyDevice* d , IUnknown* c=0 ) : cBase( l , d , c ) {
		right     = r;
	}

	~cBaseTexture( ){
		for( auto i : surfaceLevels ){
			// we have to explicitly delete the Surfaces here as the Release behaviour of the surface would get stuck in a loop
			// calling back to the container Release.
			delete i.second;
		}

		if( right ){
			right->Release();
		}
	}

	T* right;

	std::unordered_map<UINT,D3D9ProxySurface*> surfaceLevels;


	HRESULT WINAPI SetPrivateData(REFGUID refguid, CONST void* pData, DWORD SizeOfData, DWORD Flags){
		if( right ){
			right->SetPrivateData(refguid, pData, SizeOfData, Flags);
		}

		return actual->SetPrivateData(refguid, pData, SizeOfData, Flags);
	}


	HRESULT WINAPI GetPrivateData(REFGUID refguid, void* pData, DWORD* pSizeOfData){
		return actual->GetPrivateData(refguid, pData, pSizeOfData);
	}


	HRESULT WINAPI FreePrivateData(REFGUID refguid){
		if( right ){
			right->FreePrivateData(refguid);
		}

		return actual->FreePrivateData(refguid);
	}


	DWORD WINAPI SetPriority(DWORD PriorityNew){
		if( right ){
			right->SetPriority(PriorityNew);
		}

		return actual->SetPriority(PriorityNew);
	}


	DWORD WINAPI GetPriority(){
		return actual->GetPriority();
	}


	void WINAPI PreLoad(){
		if( right ){
			right->PreLoad();
		}

		return actual->PreLoad();
	}


	D3DRESOURCETYPE WINAPI GetType(){
		return actual->GetType();
	}


	DWORD WINAPI SetLOD(DWORD LODNew){
		if( right ){
			right->SetLOD(LODNew);
		}

		return actual->SetLOD(LODNew);
	}


	DWORD WINAPI GetLOD(){
		return actual->GetLOD();
	}


	DWORD WINAPI GetLevelCount(){
		return actual->GetLevelCount();
	}


	HRESULT WINAPI SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType){
		if( right ){
			right->SetAutoGenFilterType(FilterType);
		}

		return actual->SetAutoGenFilterType(FilterType);
	}


	D3DTEXTUREFILTERTYPE WINAPI GetAutoGenFilterType(){
		return actual->GetAutoGenFilterType();
	}


	void WINAPI GenerateMipSubLevels(){
		if( right ){
			right->GenerateMipSubLevels();
		}

		return actual->GenerateMipSubLevels();
	}

};

*/
//(GetDevice)(THIS_ IDirect3DDevice9** ppDevice) PURE;
//(SetPrivateData)(THIS_ REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags) PURE;
//(GetPrivateData)(THIS_ REFGUID refguid,void* pData,DWORD* pSizeOfData) PURE;
//(FreePrivateData)(THIS_ REFGUID refguid) PURE;
//_(DWORD, SetPriority)(THIS_ DWORD PriorityNew) PURE;
//_(DWORD, GetPriority)(THIS) PURE;
//_(void, PreLoad)(THIS) PURE;
//_(D3DRESOURCETYPE, GetType)(THIS) PURE;
//_(DWORD, SetLOD)(THIS_ DWORD LODNew) PURE;
//_(DWORD, GetLOD)(THIS) PURE;
//_(DWORD, GetLevelCount)(THIS) PURE;
//(SetAutoGenFilterType)(THIS_ D3DTEXTUREFILTERTYPE FilterType) PURE;
//_(D3DTEXTUREFILTERTYPE, GetAutoGenFilterType)(THIS) PURE;
//_(void, GenerateMipSubLevels)(THIS) PURE;