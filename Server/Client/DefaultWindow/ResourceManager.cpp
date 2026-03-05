#include "pch.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Sprite.h"
#include "Flipbook.h"
#include "Tilemap.h"
//#include "Sound.h"

ResourceManager::~ResourceManager()
{
	Clear();
}

void ResourceManager::Init(fs::path resourcePath)
{
	_resourcePath = resourcePath;
}

void ResourceManager::Clear()
{
	for (auto& item : _textures)
		SAFE_DELETE(item.second);

	_textures.clear();

	for (auto& item : _sprites)
		SAFE_DELETE(item.second);

	_sprites.clear();

	for (auto& item : _flipbooks)
		SAFE_DELETE(item.second);

	_flipbooks.clear();

	for (auto& item : _tilemaps)
		SAFE_DELETE(item.second);

	_tilemaps.clear();

	//for (auto& item : _sounds)
	//	SAFE_DELETE(item.second);

	//_sounds.clear();
}

Texture* ResourceManager::LoadTexture(const wstring& key, const wstring& path, uint32 transparent /*= RGB(255, 0, 255)*/)
{
	if (_textures.find(key) != _textures.end())
		return _textures[key];

	fs::path fullPath = _resourcePath / path;

	Texture* texture = new Texture();
	texture->LoadBmp(fullPath.c_str());
	texture->SetTransparent(transparent);
	_textures[key] = texture;

	return texture;
}

Sprite* ResourceManager::CreateSprite(const wstring& key, Texture* texture, Vec2Int pos, Vec2Int size)
{
	if (_sprites.find(key) != _sprites.end())
		return _sprites[key];

	if (size.x == 0)
		size.x = texture->GetSize().x;

	if (size.y == 0)
		size.y = texture->GetSize().y;

	Sprite* sprite = new Sprite(texture, pos, size);
	_sprites[key] = sprite;

	return sprite;
}

Sprite* ResourceManager::CreateSprite(const wstring& key, Texture* texture, Vec2Int pos, Vec2Int size, Vec2Int pivot)
{
	if (_sprites.find(key) != _sprites.end())
		return _sprites[key];

	if (size.x == 0)
		size.x = texture->GetSize().x;

	if (size.y == 0)
		size.y = texture->GetSize().y;

	Sprite* sprite = new Sprite(texture, pos, size, pivot);
	_sprites[key] = sprite;

	return sprite;
}

Flipbook* ResourceManager::CreateFlipbook(const wstring& key, Texture* texture, Vec2Int size, int32 start, int32 end, int32 line, float duration, bool loop)
{
	if (_flipbooks.find(key) != _flipbooks.end())
		return _flipbooks[key];

	Flipbook* fb = new Flipbook(texture, size, start, end, line, duration, loop);
	_flipbooks[key] = fb;

	return fb;
}

Flipbook* ResourceManager::CreateFlipbook(const wstring& key, Texture* texture, Vec2Int size, Vec2Int pivot, int32 start, int32 end, int32 line, float duration, bool loop)
{
	if (_flipbooks.find(key) != _flipbooks.end())
		return _flipbooks[key];

	Flipbook* fb = new Flipbook(texture, size, pivot, start, end, line, duration, loop);
	_flipbooks[key] = fb;

	return fb;
}

Tilemap* ResourceManager::CreateTilemap(const wstring& key)
{
	if (_tilemaps.find(key) != _tilemaps.end())
		return _tilemaps[key];

	Tilemap* tm = new Tilemap();
	_tilemaps[key] = tm;

	return tm;
}

void ResourceManager::SaveTilemap(const wstring& key, const wstring& path)
{
	Tilemap* tilemap = GetTilemap(key);

	fs::path fullPath = _resourcePath / path;
	tilemap->SaveFile(fullPath);
}

Tilemap* ResourceManager::LoadTilemap(const wstring& key, const wstring& path)
{
	Tilemap* tilemap = nullptr;

	if (_tilemaps.find(key) == _tilemaps.end())
		_tilemaps[key] = new Tilemap();

	tilemap = _tilemaps[key];

	fs::path fullPath = _resourcePath / path;
	tilemap->LoadFile(fullPath);

	return tilemap;
}
//
//Sound* ResourceManager::LoadSound(const wstring& key, const wstring& path)
//{
//	if (_sounds.find(key) != _sounds.end())
//		return _sounds[key];
//
//	fs::path fullPath = _resourcePath / path;
//
//	Sound* sound = new Sound();
//	sound->LoadWave(fullPath);
//	_sounds[key] = sound;
//
//	return sound;
//}
