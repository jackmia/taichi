#include "taichi/ir/type_factory.h"

TLANG_NAMESPACE_BEGIN

TypeFactory &TypeFactory::get_instance() {
  static TypeFactory *type_factory = new TypeFactory;
  return *type_factory;
}

Type *TypeFactory::get_primitive_type(PrimitiveTypeID id) {
  std::lock_guard<std::mutex> _(mut_);

  if (primitive_types_.find(id) == primitive_types_.end()) {
    primitive_types_[id] = std::make_unique<PrimitiveType>(id);
  }

  return primitive_types_[id].get();
}

Type *TypeFactory::get_vector_type(int num_elements, Type *element) {
  auto key = std::make_pair(num_elements, element);
  if (vector_types_.find(key) == vector_types_.end()) {
    vector_types_[key] = std::make_unique<VectorType>(num_elements, element);
  }
  return vector_types_[key].get();
}

Type *TypeFactory::get_pointer_type(Type *element) {
  auto key = element;  // may need to add is_bit_ptr later
  if (pointer_types_.find(key) == pointer_types_.end()) {
    pointer_types_[key] = std::make_unique<PointerType>(element, false);
  }
  return pointer_types_[key].get();
}

TypeFactory::TypeFactory() {
}

DataType TypeFactory::create_vector_or_scalar_type(int width,
                                                   DataType element,
                                                   bool element_is_pointer) {
  TI_ASSERT(width == 1);
  if (element_is_pointer) {
    return TypeFactory::get_instance().get_pointer_type(element.get_ptr());
  } else {
    return element;
  }
}

TLANG_NAMESPACE_END
