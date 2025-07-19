# Design Document

## Overview

This design addresses the Qt Image Processor parameter widget display and algorithm processing issues by implementing a unified parameter management system, proper widget lifecycle management, and robust signal-slot connections for real-time parameter updates.

## Architecture

The solution involves three main architectural improvements:

1. **Unified Parameter Interface**: Consolidate the dual parameter systems (`AlgorithmParameters` and `AlgorithmParams`) into a single, consistent interface
2. **Widget Lifecycle Management**: Implement proper creation, display, and cleanup of parameter widgets
3. **Real-time Processing Pipeline**: Establish reliable signal-slot connections for immediate parameter-to-image processing

## Components and Interfaces

### 1. Parameter Management System

**Enhanced AlgorithmParameters Base Class**
- Extend the existing `AlgorithmParameters` class to include algorithm type information
- Add virtual methods for parameter serialization and widget creation
- Implement proper signal emission for parameter changes

**Parameter Factory Pattern**
- Create a factory method in `AlgorithmFactory` to generate appropriate parameter objects
- Ensure each algorithm type has a corresponding parameter class
- Maintain type safety between algorithms and their parameters

### 2. Widget Management in ImageProcessorWindow

**Parameter Widget Container**
- Enhance the existing `m_parameterScrollArea` and `m_parameterWidget` management
- Implement proper widget cleanup to prevent memory leaks
- Add error handling for widget creation failures

**Layout Management**
- Improve the `m_parameterLayout` handling for dynamic widget addition/removal
- Implement proper spacing and sizing for parameter controls
- Add placeholder widgets for algorithms without parameters

### 3. Signal-Slot Connection System

**Parameter Change Propagation**
```cpp
// Connection pattern for real-time updates
AlgorithmParameters* params -> parametersChanged() signal
-> ImageProcessorWindow::updateProcessedImage() slot
-> ImageProcessor::processImage() method
```

**Debounced Processing**
- Implement a QTimer-based debouncing mechanism to prevent excessive processing
- Set appropriate delay (e.g., 100ms) for parameter change batching
- Ensure the last parameter change always gets processed

## Data Models

### Enhanced Parameter Classes

Each algorithm parameter class will follow this pattern:

```cpp
class BlurParameters : public AlgorithmParameters {
    Q_OBJECT
public:
    // Algorithm type identification
    AlgorithmParams::Algorithm algorithmType() const override { return AlgorithmParams::Blur; }

    // Widget creation with proper parent management
    QWidget* createParameterWidget(QWidget* parent) override;

    // Parameter access methods
    int kernelSize() const;
    void setKernelSize(int size);

    // Serialization for consistency
    QVariantMap toVariantMap() const override;
    void fromVariantMap(const QVariantMap& map) override;
};
```

### ImageProcessor Parameter Interface

The `ImageProcessor` will be modified to:
- Accept only `AlgorithmParameters*` (not `AlgorithmParams*`)
- Use the algorithm type from parameters to select the correct processing algorithm
- Handle parameter type mismatches gracefully

## Error Handling

### Widget Creation Errors
- Catch exceptions during parameter widget creation
- Display error messages in the parameter area
- Provide fallback to basic parameter display

### Processing Errors
- Handle OpenCV processing exceptions
- Display error messages without crashing the application
- Maintain the original image when processing fails

### Parameter Type Mismatches
- Validate parameter types before processing
- Log warnings for type mismatches
- Use default parameters when types don't match

## Testing Strategy

### Unit Tests
- Test each parameter class widget creation
- Verify signal emission on parameter changes
- Test parameter serialization/deserialization

### Integration Tests
- Test algorithm switching with parameter updates
- Verify real-time image processing with parameter changes
- Test error handling scenarios

### UI Tests
- Verify parameter widgets display correctly for each algorithm
- Test scrolling behavior with many parameters
- Verify layout updates when switching algorithms

## Implementation Approach

### Phase 1: Parameter System Unification
1. Extend `AlgorithmParameters` base class with algorithm type information
2. Update all parameter classes to include algorithm type
3. Modify `AlgorithmFactory` to create unified parameter objects

### Phase 2: Widget Management Enhancement
1. Improve widget cleanup in `onAlgorithmChanged()`
2. Add error handling for widget creation
3. Implement proper layout management

### Phase 3: Signal-Slot Connection Implementation
1. Connect parameter change signals to image processing
2. Implement debounced processing
3. Add error handling for processing failures

### Phase 4: Testing and Validation
1. Test all algorithm parameter combinations
2. Verify real-time processing performance
3. Test error scenarios and edge cases

## Performance Considerations

- **Widget Creation**: Cache parameter widgets when possible to reduce creation overhead
- **Image Processing**: Implement debouncing to prevent excessive processing during rapid parameter changes
- **Memory Management**: Ensure proper cleanup of parameter widgets to prevent memory leaks
- **OpenCV Integration**: Handle large images efficiently and provide progress feedback for long operations