#pragma once

#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class Operation;
struct DataType;
struct URIDataType;
class ComputePipeline;

struct OperationError {
    enum class Type { RegistryError, NetworkError, FileError, ReadError, ParseError } type;
    std::string description;
    std::string operation;
};

/**
 * Holds the result of an `Operation`.
 * If the operation was successful the data pointer holds the results.
 * Otherwise the error objects hold data about the error that occured.
 */
struct OperationResult {
    std::unique_ptr<DataType> data;
    std::optional<OperationError> error;
    
    OperationResult(std::unique_ptr<DataType> data);
    OperationResult(OperationError error);
    
    /** Returns the names of the succesfully executed operations. */
    const std::vector<std::string>& GetExecutedOperations() const;
    
    /** 
     * Returns the result of the last successful operation if an error occured.
     * The returned pointer is destroyed when the `OperationResult` object is destroyed,
     * therefore it should not be stored.
     * Returns `nullptr` if no error occured.
     */
    const DataType* GetLastResult() const;

private:
    std::vector<std::string> executedOperations;
    std::unique_ptr<DataType> lastResult;

    friend class ComputePipeline;
};

/**
 * A data-driven compute pipeline implementation.
 */
class ComputePipeline final {
public:
    /**
     * Registers an operation to the pipeline based on `Operation::GetInputType()`
     * @return returns false if an operation already registered for this type
     */
    bool RegisterOperation(std::unique_ptr<Operation> op);
    
    /**
     * Executes the pipeline from a `DataType` input object
     */
    OperationResult Execute(std::unique_ptr<DataType> input);
    
    /**
     * Executes the pipeline for the given input URI
     */
    OperationResult Execute(URIDataType input);
    
private:
    std::unordered_map<std::string, std::unique_ptr<Operation>> operationRegistry;
    
    const Operation* GetOperationForDataType(const DataType* type);
};

/**
 * A class that implements an operation that can be registered in `ComputePipeline`.
 */
class Operation {
public:
    virtual ~Operation() = default;
    
    /**
     * Returns the unique identfier for the input type this object accepts in `Execute`.
     */
    virtual std::string_view GetInputType() const = 0;
    /**
     * Returns the unique name for the operation.
     */
    virtual std::string_view GetName() const = 0;
    /**
     * Executes the operation with the given input and returns the output of the operation.
     */
    virtual OperationResult Execute(const OperationResult& input) const = 0;
};

/**
 * A struct that defines a data type that can be an input or output of `Operation::Execute()`.
 */
struct DataType {
    /**
     * Returns the unique identfier for this type.
     */
    virtual std::string_view GetType() const = 0;
    virtual ~DataType() = default;
};

struct URIDataType: public DataType {
    std::string uri;
    
    URIDataType(std::string_view uri): uri(uri) {}
    
    std::string_view GetType() const override { return "type/uri"; }
};
