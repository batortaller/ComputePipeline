# ComputePipeline

A data-driven compute pipeline implementation in C++ for an assessment task.

## Build Instructions

```bash
mkdir build
cd build
cmake ..
cmake --build .
./ComputePipeline/compute_pipeline
```

## Usage Example

### Execution
```cpp
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
```

### Defining Custom Operations
```cpp
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
        auto* data = static_cast<InputDataType*>(input.data.get());
        
        auto result = ProcessData(data);

        return std::unique_ptr<DataType>(std::move(result));
    }
};
```

## Architecture

1. **ComputePipeline**
  - Central orchestrator that manages operation registration and execution
  - Key methods:
    - `bool RegisterOperation(std::unique_ptr<Operation> op)`
      - registers an operation
    - `OperationResult Execute(URIDataType input)`
      - starts the pipeline with a URI that can be `http://`, `https://`, `file://` or `bundle://`

2. **Operation**
  - Abstract base class for all pipeline operations
  - Key methods:
     - `std::string_view GetInputType()`
       - Declares the type of data the operation can process
     - `std::string_view GetName()`
       - Provides operation identification
     - `OperationResult Execute(const OperationResult& input)`
       - Implements the operation's processing logic

3. **DataType**
  - Base class for all data types that can flow through the pipeline

4. **OperationResult**
  - Wrapper class that handles operation outputs
  - Contains either successful data or error information

### Data Flow

1. Pipeline receives input data
2. System identifies the appropriate operation based on input type
3. Operation processes data and produces new DataType
4. Process repeats until no matching operation is found
5. Final result or error is returned

### Error Handling

The system uses the `OperationError` structure to handle various failure scenarios.

Each error includes:
- Error type
- Descriptive message
- Failed operation name
- Last successful result (if any)
