#pragma once

#include "SpiralLibrary/GameSystem/SingletonBase.hpp"

#include "Resource/ResourceHandle.h"
#include "Resource/Texture2D.h"

#include <string>
#include <unordered_map>

class FileManager : public libspiral::SingletonBase<FileManager>
{
public:
	FileManager();

	std::string MakeRelativePath(std::string filename);
	std::string MakeAssetPath(std::string filename);

	bool FileExists(std::string key);

	template<class HANDLE_TYPE>
	HANDLE_TYPE GetResourceHandleFromCache(std::string key) {
		auto it = CacheMap<HANDLE_TYPE>().find(key);
		if (it != CacheMap<HANDLE_TYPE>().end()) {
			return it->second.CreateWeakHandle();
		}

		return HANDLE_TYPE();
	}

	template<class HANDLE_TYPE, class... Args>
	HANDLE_TYPE CreateCachedResourceHandle(std::string key, Args&&... args) {
		auto it = CacheMap<HANDLE_TYPE>().find(key);
		if (it != CacheMap<HANDLE_TYPE>().end()) {
			return it->second.CreateWeakHandle();
		}

		AddCache<HANDLE_TYPE>(key, std::forward<Args>(args)...);

		return CacheMap<HANDLE_TYPE>()[key].CreateWeakHandle();
	}

	template<class HANDLE_TYPE, class... Args>
	void AddCache(std::string key, Args&&... args) {
		CacheMap<HANDLE_TYPE>().insert(std::make_pair(key, HANDLE_TYPE(std::forward<Args>(args)...)));
	}

	void purgeCacheAll();

protected:
	template<class HANDLE_TYPE>
	std::unordered_map<std::string, HANDLE_TYPE>& CacheMap();

	template<>
	std::unordered_map<std::string, ResourceHandle>& CacheMap<ResourceHandle>() {
		return _resourceCache;
	}

	template<>
	std::unordered_map<std::string, Texture2D>& CacheMap<Texture2D>() {
		return _textureCache;
	}

private:
	int RID;
	std::string _projDir;

	std::unordered_map<std::string, ResourceHandle> _resourceCache;
	std::unordered_map<std::string, Texture2D> _textureCache;
};

