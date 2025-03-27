#include "ComputePipeline.h"

#include <iostream>
#include <cassert>

OperationResult::OperationResult(std::unique_ptr<DataType> data): data(std::move(data)), error(std::nullopt) {}

OperationResult::OperationResult(OperationError error): data(nullptr), error(std::move(error)) {}

const std::vector<std::string>& OperationResult::GetExecutedOperations() const {
    return executedOperations;
}

const DataType* OperationResult::GetLastResult() const {
    return lastResult.get();
}

OperationResult ComputePipeline::Execute(std::unique_ptr<DataType> input) {
    std::cout << ">>> ComputePipeline::Execute: Begin <<<\n";
    
    OperationResult previousResult = std::move(input);
    while (auto* operation = GetOperationForDataType(previousResult.data.get())) {
        if (previousResult.data->GetType() != operation->GetInputType()) {
            assert(false);
            return OperationError{.description = "Wrong type registration in ComputePipeline"};
        }
        
        auto result = operation->Execute(std::move(previousResult));
        if (result.error) {
            std::cout << ">>> ComputePipeline::Execute: Operation Error\n";
            result.error->operation = operation->GetName();
            result.lastResult = std::move(previousResult.data);
            return result;
        }
        result.executedOperations = std::move(previousResult.executedOperations);
        result.executedOperations.push_back(std::string(operation->GetName()));
        previousResult = std::move(result);
    }
    std::cout << ">>> ComputePipeline::Execute: End <<<\n";
    return previousResult;
}

bool ComputePipeline::RegisterOperation(std::unique_ptr<Operation> op) {
    if (operationRegistry.contains(std::string(op->GetInputType()))) {
        std::cout << "Operation already contains an operation for this type\n";
        assert(false);
        return false;
    }
    
    operationRegistry.insert({std::string(op->GetInputType()), std::move(op)});
    std::cout << "Operation registered\n";
    return true;
}

OperationResult ComputePipeline::Execute(URIDataType input) {
    return Execute(std::unique_ptr<DataType>(new URIDataType(std::move(input))));
}

const Operation* ComputePipeline::GetOperationForDataType(const DataType* type) {
    if (auto op = operationRegistry.find(std::string(type->GetType())); op != operationRegistry.end()) {
        return op->second.get();
    }
    return nullptr;
}
