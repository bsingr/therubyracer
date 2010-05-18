#include "v8_array.h"
#include "v8_ref.h"
#include "v8_obj.h"

using namespace v8;


namespace {
  
  VALUE ArrayClass;
  
  Local<Array> unwrap(VALUE self) {
    return V8_Ref_Get<Array>(self);
  }
  
  VALUE New(int argc, VALUE *argv, VALUE self) {
    VALUE length;
    rb_scan_args(argc, argv, "0", &length);
    if (NIL_P(length)) {
      length = INT2FIX(0);
    }
    return V8_Ref_Create(self, Array::New(NUM2INT(length)));
  }
  
  VALUE Length(VALUE self) {
    return rr_v82rb(unwrap(self)->Length());
  }
  
  VALUE CloneElementAt(VALUE self, VALUE index) {
    return rr_v82rb(unwrap(self)->CloneElementAt(NUM2UINT(index)));
  }
}

void rr_init_v8_array() {
  ArrayClass = rr_define_class("Array", rr_cV8_C_Object);
  rr_define_singleton_method(ArrayClass, "New", New, -1);
  rr_define_method(ArrayClass, "Length", Length, 0);
  rr_define_method(ArrayClass, "CloneElementAt", CloneElementAt, 1);
}

VALUE rr_reflect_v8_array(Handle<Value> value) {
  Local<Array> array(Array::Cast(*value));
  Local<Value> peer = array->GetHiddenValue(String::New("TheRubyRacer::RubyObject"));
  if (peer.IsEmpty()) {
    VALUE arr = V8_Ref_Create(ArrayClass, array);
    rb_iv_set(arr, "@context", rr_v82rb(Context::GetEntered()));
    return arr;
  } else {
    return (VALUE)External::Unwrap(peer);
  }
}