#include <filesystem>
#include <imgui.h>
#include "Graphics.h"
#include "ResourceManager.h"

// ���f�����\�[�X�ǂݍ���
std::shared_ptr<ModelResource> ResourceManager::LoadModelResource(const char* filename)
{
	//���ɓǂݍ��܂�Ă����ꍇ�͓ǂݍ��ݍς݂̃��\�[�X��Ԃ�
	std::string key = std::string(filename);
	for (auto& model : models)
	{
		if (model.first == key)
		{
			if (model.second.expired())
				break;

			return std::shared_ptr<ModelResource>(model.second);
		}
	}


	
	//�V�K���f�����\�[�X�쐬���ǂݍ���
	std::shared_ptr<ModelResource> p = std::make_shared<ModelResource>();
	p->Load(Graphics::Instance().GetDevice(), filename);


	//�ǂݍ��݊Ǘ��p�̕ϐ��ɓo�^
	models.insert_or_assign(key, std::weak_ptr<ModelResource>(p));

	//�쐬�������\�[�X��Ԃ�
	return p;
}

// �f�o�b�OGUI�`��
void ResourceManager::DrawDebugGUI()
{
	if (ImGui::CollapsingHeader("Resource", ImGuiTreeNodeFlags_DefaultOpen))
	{
		for (auto it = models.begin(); it != models.end(); ++it)
		{
			std::filesystem::path filepath(it->first);

			int use_count = it->second.use_count();
			ImGui::Text("use_count = %5d : %s", use_count, filepath.filename().u8string().c_str());
		}
	}
}

