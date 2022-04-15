#pragma once
#include"stdafx.h"
namespace ButiScript {
class Compiler;
}
namespace ButiEngine {
/// <summary>
/// �R���p�C���ς݃f�[�^�̓ǂݍ���
/// </summary>
/// <param name="arg_filePath">���̓t�@�C���p�X</param>
/// <param name="arg_compiler">�R���p�C��(nullptr�Ńf�t�H���g���g�p)</param>
/// <returns>�ǂݍ��񂾃v���O�����f�[�^</returns>
Value_ptr<IResource_Script> CreateButiScriptResource(const std::string& arg_filePath ,ButiScript::Compiler* arg_compiler=nullptr);
/// <summary>
/// �v���O�����̃R���p�C��
/// </summary>
/// <param name="arg_inputfilePath">�R���p�C������t�@�C���̃p�X</param>
/// <param name="arg_outputfilePath">�R���p�C���ς݃f�[�^�̏o�͐�</param>
/// <param name="arg_compiler">�g�p����R���p�C��</param>
/// <returns>0=����/���̑�=���s</returns>
std::int32_t CompileButiScipt(const std::string& arg_inputfilePath, const std::string& arg_outputfilePath, ButiScript::Compiler* arg_compiler = nullptr);
}