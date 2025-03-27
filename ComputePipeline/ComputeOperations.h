#pragma once

#include "ComputePipeline.h"

#include <cstddef>
#include <vector>

struct TextDataType: public DataType {
    std::string text;
    
    TextDataType(std::string text): text(std::move(text)) {}
    std::string_view GetType() const override { return Type; }
    
    constexpr static std::string_view Type = "type/text";
};

struct DecodedImageDataType: public DataType {
    std::vector<std::byte> bytes;
    
    DecodedImageDataType(std::vector<std::byte> bytes): bytes(std::move(bytes)) {}
    std::string_view GetType() const override { return Type; }
    
    constexpr static std::string_view Type = "type/decoded_image";
};

struct CompressedDataType: public DataType {
    std::vector<std::byte> bytes;
    
    CompressedDataType(std::vector<std::byte> bytes): bytes(std::move(bytes)) {}
    std::string_view GetType() const override { return Type; }
    
    constexpr static std::string_view Type = "type/compressed";
};

struct RawImageDataType: public DataType {
    std::vector<std::byte> bytes;
    
    RawImageDataType(std::vector<std::byte> bytes): bytes(std::move(bytes)) {}
    std::string_view GetType() const override { return Type; }
    
    constexpr static std::string_view Type = "type/raw_image";
};

/** Dummy JSON object */
struct JsonData {
    JsonData() = default;
    JsonData(const JsonData&) = default;
    JsonData(std::unordered_map<std::string, std::string> data): data(data) {}
    
    std::optional<std::string> operator[](std::string key) const {
        if (data.find(key) != data.end()) {
            return data.at(key);
        }
        
        return std::nullopt;
    }
    
private:
    std::unordered_map<std::string, std::string> data;
};

struct JsonDataType: public DataType {
    JsonData data;
    
    JsonDataType(JsonData data): data(data) {}
    std::string_view GetType() const override { return Type; }
    
    constexpr static std::string_view Type = "type/json";
};

class LoadOperation: public Operation {
    using InputDataType = URIDataType;

    std::string_view GetInputType() const override { return "type/uri";}
    std::string_view GetName() const override { return "LoadOperation"; }
    
    OperationResult Execute(const OperationResult& input) const override;
};

class ImageDecodeOperation: public Operation {
    using InputDataType = RawImageDataType;
    
    std::string_view GetInputType() const override { return "type/raw_image"; }
    std::string_view GetName() const override { return "ImageDecodeOperation"; }
    
    OperationResult Execute(const OperationResult& input) const override;
};

class DecompressOperation: public Operation {
    using InputDataType = CompressedDataType;
    
    std::string_view GetInputType() const override { return InputDataType::Type; }
    std::string_view GetName() const override { return "DecompressOperation"; }

    OperationResult Execute(const OperationResult& input) const override;
};

class JsonDeserializeOperation: public Operation {
    using InputDataType = TextDataType;
    
    std::string_view GetInputType() const override { return "type/text"; }
    std::string_view GetName() const override { return "JsonDeserializeOperation"; }

    
    OperationResult Execute(const OperationResult& input) const override;
};
