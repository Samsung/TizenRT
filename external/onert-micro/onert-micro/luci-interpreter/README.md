# luci-interpreter

`luci-interpreter` is an inference engine for neural networks represented in luci IR.
See `compiler/luci/lang` directory for details about IR.
You can find useful infrastructure, like importer/exporter, optimizations in `compiler/luci`.

`luci-interpreter` provides:
- Basic inference functionality, input setters and output getters
- Interface for inspecting hidden interpreter state, like activation values during inference
- Customization mechanisms to fit the interpreter to specific platforms, like MCUs

Public interface headers are placed in `luci-interpreter/include/luci_interpreter` directory

## Basic usage

Minimal usage includes:
- Setting input data
- Running inference
- Fetching inference results

Interpreter object is reusable and can run multiple inferences.
Elements in tensors (input/output/internal) are stored contiguously and have C-like layout:
This means for tensor t=[[0, 1],[2, 3]], t[0,1] == 1.

Input and output tensors have the same indexes as in original luci model. 

**Usage example:**
``` c++
// Note getTensorSize is a function that computes tensor size,
// it is not part of interpreter and should be implemented by user 

luci_interpreter::Interpreter interpreter(luci_module);

// Set inputs
// assuming model has only one input and one output
const auto input_nodes = loco::input_nodes(module->graph());

const auto *input_node = dynamic_cast<const luci::CircleInput *>(input_nodes[0]);
std::vector<char> input_data(getTensorSize(input_node));
// Initialize input data here

interpreter.writeInputTensor(input_node, input_data.data(), input_data.size());

// Start inference
interpreter.interpret();

// Fetch inference results
const auto output_nodes = loco::output_nodes(module->graph());
const auto *output_node = dynamic_cast<const luci::CircleOutput *>(output_nodes[0]);
std::vector<char> output_data(getTensorSize(output_node));
interpreter.readOutputTensor(output_node, output_data.data(), output_data.size());
```

## Inspecting intermediate state

Interpreter provides interfaces to investigate internal state of interpreter during inference.

This is done by "observer" mechanism:
- `Interpreter` class has `attachObserver` method, which takes pointer to `ExecutionObserver` object
- `ExecutionObserver` defines several callback methods user can override to inject custom code

ExecutionObserver provides three callbacks:
- `postTensorWrite` checks contents of output tensor after operation execution
- `preOperatorExecute` notifies that interpreter is going to execute operation
- `postOperatorExecute` notifies that interpreter has finished execution of an operation

See `luci-interpreter/include/luci_interpreter/Interpreter.h` for this interface details.

**Usage example:**
``` c++
class CustomExecutionObserver: public luci_interpreter::ExecutionObserver
{
public:
  void postTensorWrite(const luci::CircleNode *node, const Tensor *tensor) override
  {
    if (tensor->element_type() != loco::DataType::FLOAT32)
      return;
    for (int i = 0; i < tensor->shape().num_elements(); ++i)
      std::cout << tensor->data<float>[i] << ", ";
  }

  // User observer can override only needed methods,
  // others will inherit empty implementation from base observer.

  // void preOperatorExecute(const luci::CircleNode *node);
  // void postOperatorExecute(const luci::CircleNode *node);
};

luci_interpreter::Interpreter interpreter(module);
CustomExecutionObserver observer;
interpreter.attachObserver(&observer);

// initialize input_data
interpreter.writeInputTensor(input_node, input_data.data(), input_data.size());

interpreter.interpret();
```

## Customizing inference

### Memory manager

Interpreter provides a handle for altering default memory management mechanisms.

This is done by `MemoryManger` interface, see `luci-interpreter/include/luci_interpreter/MemoryManager.h` for implementation details.

This header contains `IMemoryManager` abstract class which is responsible for allocation and dealocation of tensors' memory.

User can construct an interpreter with one of predefined memory managers or their own custom memory manager.
Note that one memory manager could be shared between multiple interpreter instances, because an interpreter does not own the manager object. 

List of predefined memory managers:
- `SimpleMemoryManager` This is a simple wrapper around new/delete, default one.
- `TestMemoryManager` Memorizes all allocated memory and releases it in Manager destructor, used in kernel unit tests.
- `BuddyMemoryManager` Implements Buddy algorithm, uses external buffer for tensor data allocations, does not need new/delete.
- `StaticMemoryManger` Uses precomputed memory allocation plan. Requires preparation with MemoryPlanner, but could reduce memory consumption in restricted environments (like MCUs).

**SimpleMemoryManager usage example:**

No need to select anything, to use this memory manager.
``` c++
luci_interpreter::Interpreter interpreter(module);
```

**TestMemoryManager usage example:**

``` c++
luci_interpreter::TestMemoryManager mm;
luci_interpreter::Interpreter interpreter(module, &mm);
```

**BuddyMemoryManager usage example:**

`BuddyMemoryManager` implements a classic allocation algorithm: https://en.wikipedia.org/wiki/Buddy_memory_allocation.

This allocator uses an external buffer as a memory pool. That allows to use static memory arrays for allocations.

Limitations
- Current implementation uses only lower power-of-two bytes of given buffer.

  For example for 1000 bytes buffer, only lower 512 bytes will be used.
- Current implementation can handle maximum 4 gigabyte memory pool

``` c++
  constexpr int buffer_size = 2048;
  static uint8_t buffer[buffer_size];
  luci_interpreter::BuddyMemoryManager memory_manager(buffer, buffer_size);
  luci_interpreter::Interpreter interpreter(module.get(), &memory_manager);
```

**StaticMemoryManager usage example:**
``` c++
TBD when it is merged
```

## Further reading

If you want to participate in development, please read `DEVELOPER.md` for SW architecture details.
