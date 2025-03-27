#include "ComputePipeline.h"
#include "ComputeOperations.h"

#include <iostream>

struct UserDataType: public DataType {
    std::string userId;
    std::string name;
    std::string email;
    
    std::string_view GetType() const override { return Type; }
    constexpr static std::string_view Type = "type/object/userdata";
};

class UserDataDeserializationOperation: public Operation {
    using InputDataType = JsonDataType;
    
    std::string_view GetInputType() const override { return InputDataType::Type; }
    std::string_view GetName() const override { return "UserDataDeserializationOperation"; }
    
    OperationResult Execute(const OperationResult& input) const override {
        // mock data processing
        auto result = std::make_unique<UserDataType>();
        result->userId = "11";
        result->name = "John Smith";
        result->email = "john.smith@gmail.com";
        return std::unique_ptr<DataType>(std::move(result));
    }
};

int main() {
    ComputePipeline pipeline;
    
    // Register operations
    pipeline.RegisterOperation(std::make_unique<LoadOperation>());
    pipeline.RegisterOperation(std::make_unique<JsonDeserializeOperation>());
    pipeline.RegisterOperation(std::make_unique<UserDataDeserializationOperation>());
    
    auto jsonResult = pipeline.Execute(URIDataType("http://website.com/data.json"));
    if (!jsonResult.error && jsonResult.data->GetType() == UserDataType::Type) {
        auto* jsonData = static_cast<UserDataType*>(jsonResult.data.get());
        std::cout << "Received user: " << jsonData->name << std::endl;
    }
    
    return 0;
}
