#include "ComputeOperations.h"

#include <cassert>
#include <iostream>

OperationResult LoadOperation::Execute(const OperationResult& input) const {
    std::cout << "LoadOperation::Execute\n";
    auto* data = static_cast<InputDataType*>(input.data.get());
    if (data->uri.starts_with("http://")) {
        // http request ...
        const bool success = true;
        if (!success) {
            return OperationError{.type = OperationError::Type::NetworkError,
                                  .description = "Injected error"};
        }
        
        auto rawJson = "{ 'objectType': 'ImageMeta', 'format': 'jpeg'}";
        return std::unique_ptr<DataType>(new TextDataType(std::move(rawJson)));
    } else if (data->uri.starts_with("https://")) {
        // http request ...
        const bool success = true;
        if (!success) {
            return OperationError{.type = OperationError::Type::NetworkError,
                                  .description = "Injected error"};
        }
        return std::unique_ptr<DataType>(new TextDataType("Hello World!"));
    } else if (data->uri.starts_with("file://")) {
        // file load ...
        return std::unique_ptr<DataType>(new CompressedDataType({std::byte{0xFF},
                                                                 std::byte{0xAA},
                                                                 std::byte{0x11}}));
    } else if (data->uri.starts_with("bundle://")) {
        // load from bundle ...
        return std::unique_ptr<DataType>(new RawImageDataType({std::byte{0x00},
                                                               std::byte{0x11},
                                                               std::byte{0x22}}));
    }
    
    return OperationError{.type = OperationError::Type::ReadError, .description = "Unsupported URI"};
}

OperationResult ImageDecodeOperation::Execute(const OperationResult& input) const {
    std::cout << "ImageDecodeOperation::Execute\n";
    auto* data = static_cast<InputDataType*>(input.data.get());
    
    // mock data processing
    auto* resultData = new DecodedImageDataType(std::move(data->bytes));
    std::reverse(resultData->bytes.begin(), resultData->bytes.end());
    
    return std::unique_ptr<DataType>(std::move(resultData));
}

OperationResult DecompressOperation::Execute(const OperationResult& input) const {
    std::cout << "DecompressOperation::Execute\n";
    // mock data processing
    
    return std::unique_ptr<DataType>(new TextDataType("{ 'object': 'a'}"));
}

OperationResult JsonDeserializeOperation::Execute(const OperationResult& input) const {
    std::cout << "JsonDeserializeOperation::Execute\n";
    auto* data = static_cast<InputDataType*>(input.data.get());
    
    // mock data processing
    std::unordered_map<std::string, std::string> map;
    map[data->text] = data->text;
    
    return std::unique_ptr<DataType>(new JsonDataType(JsonData(std::move(map))));
}
