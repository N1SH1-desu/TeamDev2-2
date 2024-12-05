#pragma once
#include<DirectXMath.h>
#include<vector>
#include<set>
#include<memory>
#include "Graphics.h"

template<typename T>
class Manager
{
protected:
	Manager() {};
	virtual ~Manager() {};

public:
	virtual void Initialize() {};

	virtual void Update(float elapsedTime) 
	{ 
		for (const auto& object : objects)
			object->Update(elapsedTime);

		Manage(elapsedTime); 
	};

	virtual void Render(ModelRenderer* modelRenderer, RenderContext& rc, ShaderId ID)
	{
		for (const auto& object : objects)
			object->Render(modelRenderer, rc, ID);
	}

	//オブジェクトの登録
	void Register(T* object) { objects.emplace_back(object); }

	//オブジェクトの削除
	void Remove(T* object) { removes.insert(object); }

	//オブジェクトを取得
	T* GetObject_(int index) { return objects[index].get(); }

	//要素数
	int GetObjectCount() const { return static_cast<int>(objects.size()); }

	//要素をすべて削除
	void Clear()
	{
		if (!objects.empty())
			objects.clear();

		if (!removes.empty())
			removes.clear();
	}

protected:
	virtual void Manage(float elapsedTime)
	{
		for (auto object : removes)
		{
			auto it = std::find_if(objects.begin(), objects.end(), [&object](const auto& ptr) { return ptr.get() == object;  });

			if (it != objects.end())
				objects.erase(it);
		}

		removes.clear();
	}

protected:
	std::vector<std::unique_ptr<T>> objects;
	std::set<T*> removes;
};