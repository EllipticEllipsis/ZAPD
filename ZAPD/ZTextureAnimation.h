#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include "ZResource.h"
#include "tinyxml2.h"

enum class TextureAnimationParamsType
{
	/* 0 */ SingleScroll,
	/* 1 */ DualScroll,
	/* 2 */ ColorChange,
	/* 3 */ ColorChangeLERP,
	/* 4 */ ColorChangeLagrange,
	/* 5 */ TextureCycle
};

struct ZTextureAnimationParams : public ZResource
{
	ZTextureAnimationParams(ZFile* parent);

	virtual void ExtractFromBinary(uint32_t paramsOffset);
	virtual void ExtractFromBinary(uint32_t paramsOffset, int count);

	std::string GetSourceOutputCode(const std::string& prefix);

	virtual std::string GetDefaultName(const std::string& prefix, uint32_t address) const;
	virtual void DeclareVar(const std::string& prefix, const std::string& bodyStr) const;
	ZResourceType GetResourceType() const;

	TextureAnimationParamsType type;
};

struct TextureScrollingParamsEntry
{
	int8_t xStep;
	int8_t yStep;
	uint8_t width;
	uint8_t height;
};

struct TextureScrollingParams : public ZTextureAnimationParams
{
	TextureScrollingParams(ZFile* parent);

	void ParseRawData() override;
	void ExtractFromBinary(uint32_t nRawDataIndex, int count) override;

	std::string GetSourceTypeName() const override;
	std::string GetDefaultName(const std::string& prefix, uint32_t address) const override;
	size_t GetRawDataSize() const override;

	void DeclareVar(const std::string& prefix, const std::string& bodyStr) const override;
	std::string GetBodySourceCode() const override;

	int count;  // 1 for Single, 2 for Dual
	TextureScrollingParamsEntry rows[2];
};

struct F3DPrimColor
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
	uint8_t lodFrac;
};

struct F3DEnvColor
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

struct TextureColorChangingParams : public ZTextureAnimationParams
{
	TextureColorChangingParams(ZFile* parent);

	void ParseRawData() override;
	void ExtractFromBinary(uint32_t nRawDataIndex) override;

	std::string GetSourceTypeName() const override;
	std::string GetDefaultName(const std::string& prefix, uint32_t address) const override;
	size_t GetRawDataSize() const override;

	void DeclareReferences(const std::string& prefix) override;

	std::string GetBodySourceCode() const override;

	uint16_t count1;
	uint16_t count2;
	segptr_t primColorListAddress;
	segptr_t envColorListAddress;
	segptr_t frameDataListAddress;
	std::vector<F3DPrimColor> primColorList;
	std::vector<F3DEnvColor> envColorList;
	std::vector<uint16_t> frameDataList;
};

struct TextureCyclingParams : public ZTextureAnimationParams
{
	TextureCyclingParams(ZFile* parent);

	void ParseRawData() override;
	void ExtractFromBinary(uint32_t nRawDataIndex) override;

	std::string GetSourceTypeName() const override;
	std::string GetDefaultName(const std::string& prefix, uint32_t address) const override;
	size_t GetRawDataSize() const override;

	void DeclareReferences(const std::string& prefix) override;

	std::string GetBodySourceCode() const override;

	uint16_t cycleLength;
	segptr_t textureListAddress;
	segptr_t textureIndexListAddress;
	std::vector<segptr_t> textureList;
	std::vector<uint8_t> textureIndexList;
};

struct TextureAnimationEntry
{
	int8_t segment;
	TextureAnimationParamsType type;
	segptr_t paramsPtr;
};

class ZTextureAnimation : public ZResource
{
public:
	ZTextureAnimation(ZFile* nParent);

	void ExtractFromXML(tinyxml2::XMLElement* reader, uint32_t nRawDataIndex) override;
	void ParseRawData() override;

	void DeclareReferences(const std::string& prefix) override;

	std::string GetSourceTypeName() const override;
	ZResourceType GetResourceType() const override;
	size_t GetRawDataSize() const override;
	std::string GetDefaultName(const std::string& prefix, uint32_t address) const;

	void DeclareVar(const std::string& prefix, const std::string& bodyStr) const;
	std::string GetBodySourceCode() const override;
	std::string GetSourceOutputCode(const std::string& prefix) override;

private:
	std::vector<TextureAnimationEntry> entries;
};