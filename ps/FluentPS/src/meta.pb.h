// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: meta.proto

#ifndef PROTOBUF_meta_2eproto__INCLUDED
#define PROTOBUF_meta_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3005000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3005001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
// @@protoc_insertion_point(includes)

namespace protobuf_meta_2eproto {
// Internal implementation detail -- do not use these members.
struct TableStruct {
  static const ::google::protobuf::internal::ParseTableField entries[];
  static const ::google::protobuf::internal::AuxillaryParseTableField aux[];
  static const ::google::protobuf::internal::ParseTable schema[3];
  static const ::google::protobuf::internal::FieldMetadata field_metadata[];
  static const ::google::protobuf::internal::SerializationTable serialization_table[];
  static const ::google::protobuf::uint32 offsets[];
};
void InitDefaultsPBNodeImpl();
void InitDefaultsPBNode();
void InitDefaultsPBControlImpl();
void InitDefaultsPBControl();
void InitDefaultsPBMetaImpl();
void InitDefaultsPBMeta();
inline void InitDefaults() {
  InitDefaultsPBNode();
  InitDefaultsPBControl();
  InitDefaultsPBMeta();
}
}  // namespace protobuf_meta_2eproto
namespace ps {
class PBControl;
class PBControlDefaultTypeInternal;
extern PBControlDefaultTypeInternal _PBControl_default_instance_;
class PBMeta;
class PBMetaDefaultTypeInternal;
extern PBMetaDefaultTypeInternal _PBMeta_default_instance_;
class PBNode;
class PBNodeDefaultTypeInternal;
extern PBNodeDefaultTypeInternal _PBNode_default_instance_;
}  // namespace ps
namespace ps {

// ===================================================================

class PBNode : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:ps.PBNode) */ {
 public:
  PBNode();
  virtual ~PBNode();

  PBNode(const PBNode& from);

  inline PBNode& operator=(const PBNode& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  PBNode(PBNode&& from) noexcept
    : PBNode() {
    *this = ::std::move(from);
  }

  inline PBNode& operator=(PBNode&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::std::string& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::std::string* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const PBNode& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const PBNode* internal_default_instance() {
    return reinterpret_cast<const PBNode*>(
               &_PBNode_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    0;

  void Swap(PBNode* other);
  friend void swap(PBNode& a, PBNode& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline PBNode* New() const PROTOBUF_FINAL { return New(NULL); }

  PBNode* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    PROTOBUF_FINAL;
  void CopyFrom(const PBNode& from);
  void MergeFrom(const PBNode& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  void DiscardUnknownFields();
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(PBNode* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::std::string GetTypeName() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional string hostname = 3;
  bool has_hostname() const;
  void clear_hostname();
  static const int kHostnameFieldNumber = 3;
  const ::std::string& hostname() const;
  void set_hostname(const ::std::string& value);
  #if LANG_CXX11
  void set_hostname(::std::string&& value);
  #endif
  void set_hostname(const char* value);
  void set_hostname(const char* value, size_t size);
  ::std::string* mutable_hostname();
  ::std::string* release_hostname();
  void set_allocated_hostname(::std::string* hostname);

  // required int32 role = 1;
  bool has_role() const;
  void clear_role();
  static const int kRoleFieldNumber = 1;
  ::google::protobuf::int32 role() const;
  void set_role(::google::protobuf::int32 value);

  // optional int32 id = 2;
  bool has_id() const;
  void clear_id();
  static const int kIdFieldNumber = 2;
  ::google::protobuf::int32 id() const;
  void set_id(::google::protobuf::int32 value);

  // optional int32 port = 4;
  bool has_port() const;
  void clear_port();
  static const int kPortFieldNumber = 4;
  ::google::protobuf::int32 port() const;
  void set_port(::google::protobuf::int32 value);

  // optional bool is_recovery = 5;
  bool has_is_recovery() const;
  void clear_is_recovery();
  static const int kIsRecoveryFieldNumber = 5;
  bool is_recovery() const;
  void set_is_recovery(bool value);

  // optional int32 customer_id = 10;
  bool has_customer_id() const;
  void clear_customer_id();
  static const int kCustomerIdFieldNumber = 10;
  ::google::protobuf::int32 customer_id() const;
  void set_customer_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:ps.PBNode)
 private:
  void set_has_role();
  void clear_has_role();
  void set_has_id();
  void clear_has_id();
  void set_has_hostname();
  void clear_has_hostname();
  void set_has_port();
  void clear_has_port();
  void set_has_is_recovery();
  void clear_has_is_recovery();
  void set_has_customer_id();
  void clear_has_customer_id();

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::internal::ArenaStringPtr hostname_;
  ::google::protobuf::int32 role_;
  ::google::protobuf::int32 id_;
  ::google::protobuf::int32 port_;
  bool is_recovery_;
  ::google::protobuf::int32 customer_id_;
  friend struct ::protobuf_meta_2eproto::TableStruct;
  friend void ::protobuf_meta_2eproto::InitDefaultsPBNodeImpl();
};
// -------------------------------------------------------------------

class PBControl : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:ps.PBControl) */ {
 public:
  PBControl();
  virtual ~PBControl();

  PBControl(const PBControl& from);

  inline PBControl& operator=(const PBControl& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  PBControl(PBControl&& from) noexcept
    : PBControl() {
    *this = ::std::move(from);
  }

  inline PBControl& operator=(PBControl&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::std::string& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::std::string* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const PBControl& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const PBControl* internal_default_instance() {
    return reinterpret_cast<const PBControl*>(
               &_PBControl_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    1;

  void Swap(PBControl* other);
  friend void swap(PBControl& a, PBControl& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline PBControl* New() const PROTOBUF_FINAL { return New(NULL); }

  PBControl* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    PROTOBUF_FINAL;
  void CopyFrom(const PBControl& from);
  void MergeFrom(const PBControl& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  void DiscardUnknownFields();
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(PBControl* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::std::string GetTypeName() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated .ps.PBNode node = 2;
  int node_size() const;
  void clear_node();
  static const int kNodeFieldNumber = 2;
  const ::ps::PBNode& node(int index) const;
  ::ps::PBNode* mutable_node(int index);
  ::ps::PBNode* add_node();
  ::google::protobuf::RepeatedPtrField< ::ps::PBNode >*
      mutable_node();
  const ::google::protobuf::RepeatedPtrField< ::ps::PBNode >&
      node() const;

  // required int32 cmd = 1;
  bool has_cmd() const;
  void clear_cmd();
  static const int kCmdFieldNumber = 1;
  ::google::protobuf::int32 cmd() const;
  void set_cmd(::google::protobuf::int32 value);

  // optional int32 barrier_group = 3;
  bool has_barrier_group() const;
  void clear_barrier_group();
  static const int kBarrierGroupFieldNumber = 3;
  ::google::protobuf::int32 barrier_group() const;
  void set_barrier_group(::google::protobuf::int32 value);

  // optional uint64 msg_sig = 4;
  bool has_msg_sig() const;
  void clear_msg_sig();
  static const int kMsgSigFieldNumber = 4;
  ::google::protobuf::uint64 msg_sig() const;
  void set_msg_sig(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:ps.PBControl)
 private:
  void set_has_cmd();
  void clear_has_cmd();
  void set_has_barrier_group();
  void clear_has_barrier_group();
  void set_has_msg_sig();
  void clear_has_msg_sig();

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::RepeatedPtrField< ::ps::PBNode > node_;
  ::google::protobuf::int32 cmd_;
  ::google::protobuf::int32 barrier_group_;
  ::google::protobuf::uint64 msg_sig_;
  friend struct ::protobuf_meta_2eproto::TableStruct;
  friend void ::protobuf_meta_2eproto::InitDefaultsPBControlImpl();
};
// -------------------------------------------------------------------

class PBMeta : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:ps.PBMeta) */ {
 public:
  PBMeta();
  virtual ~PBMeta();

  PBMeta(const PBMeta& from);

  inline PBMeta& operator=(const PBMeta& from) {
    CopyFrom(from);
    return *this;
  }
  #if LANG_CXX11
  PBMeta(PBMeta&& from) noexcept
    : PBMeta() {
    *this = ::std::move(from);
  }

  inline PBMeta& operator=(PBMeta&& from) noexcept {
    if (GetArenaNoVirtual() == from.GetArenaNoVirtual()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }
  #endif
  inline const ::std::string& unknown_fields() const {
    return _internal_metadata_.unknown_fields();
  }
  inline ::std::string* mutable_unknown_fields() {
    return _internal_metadata_.mutable_unknown_fields();
  }

  static const PBMeta& default_instance();

  static void InitAsDefaultInstance();  // FOR INTERNAL USE ONLY
  static inline const PBMeta* internal_default_instance() {
    return reinterpret_cast<const PBMeta*>(
               &_PBMeta_default_instance_);
  }
  static PROTOBUF_CONSTEXPR int const kIndexInFileMessages =
    2;

  void Swap(PBMeta* other);
  friend void swap(PBMeta& a, PBMeta& b) {
    a.Swap(&b);
  }

  // implements Message ----------------------------------------------

  inline PBMeta* New() const PROTOBUF_FINAL { return New(NULL); }

  PBMeta* New(::google::protobuf::Arena* arena) const PROTOBUF_FINAL;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from)
    PROTOBUF_FINAL;
  void CopyFrom(const PBMeta& from);
  void MergeFrom(const PBMeta& from);
  void Clear() PROTOBUF_FINAL;
  bool IsInitialized() const PROTOBUF_FINAL;

  size_t ByteSizeLong() const PROTOBUF_FINAL;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input) PROTOBUF_FINAL;
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const PROTOBUF_FINAL;
  void DiscardUnknownFields();
  int GetCachedSize() const PROTOBUF_FINAL { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(PBMeta* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return NULL;
  }
  inline void* MaybeArenaPtr() const {
    return NULL;
  }
  public:

  ::std::string GetTypeName() const PROTOBUF_FINAL;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // repeated int32 data_type = 10 [packed = true];
  int data_type_size() const;
  void clear_data_type();
  static const int kDataTypeFieldNumber = 10;
  ::google::protobuf::int32 data_type(int index) const;
  void set_data_type(int index, ::google::protobuf::int32 value);
  void add_data_type(::google::protobuf::int32 value);
  const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
      data_type() const;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
      mutable_data_type();

  // optional bytes body = 2;
  bool has_body() const;
  void clear_body();
  static const int kBodyFieldNumber = 2;
  const ::std::string& body() const;
  void set_body(const ::std::string& value);
  #if LANG_CXX11
  void set_body(::std::string&& value);
  #endif
  void set_body(const char* value);
  void set_body(const void* value, size_t size);
  ::std::string* mutable_body();
  ::std::string* release_body();
  void set_allocated_body(::std::string* body);

  // optional .ps.PBControl control = 3;
  bool has_control() const;
  void clear_control();
  static const int kControlFieldNumber = 3;
  const ::ps::PBControl& control() const;
  ::ps::PBControl* release_control();
  ::ps::PBControl* mutable_control();
  void set_allocated_control(::ps::PBControl* control);

  // optional int32 head = 1;
  bool has_head() const;
  void clear_head();
  static const int kHeadFieldNumber = 1;
  ::google::protobuf::int32 head() const;
  void set_head(::google::protobuf::int32 value);

  // optional bool request = 4 [default = false];
  bool has_request() const;
  void clear_request();
  static const int kRequestFieldNumber = 4;
  bool request() const;
  void set_request(bool value);

  // optional bool push = 5;
  bool has_push() const;
  void clear_push();
  static const int kPushFieldNumber = 5;
  bool push() const;
  void set_push(bool value);

  // optional bool simple_app = 6 [default = false];
  bool has_simple_app() const;
  void clear_simple_app();
  static const int kSimpleAppFieldNumber = 6;
  bool simple_app() const;
  void set_simple_app(bool value);

  // optional int32 app_id = 7;
  bool has_app_id() const;
  void clear_app_id();
  static const int kAppIdFieldNumber = 7;
  ::google::protobuf::int32 app_id() const;
  void set_app_id(::google::protobuf::int32 value);

  // optional int32 timestamp = 8;
  bool has_timestamp() const;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 8;
  ::google::protobuf::int32 timestamp() const;
  void set_timestamp(::google::protobuf::int32 value);

  // optional int32 staleness = 9;
  bool has_staleness() const;
  void clear_staleness();
  static const int kStalenessFieldNumber = 9;
  ::google::protobuf::int32 staleness() const;
  void set_staleness(::google::protobuf::int32 value);

  // optional int32 customer_id = 11;
  bool has_customer_id() const;
  void clear_customer_id();
  static const int kCustomerIdFieldNumber = 11;
  ::google::protobuf::int32 customer_id() const;
  void set_customer_id(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:ps.PBMeta)
 private:
  void set_has_head();
  void clear_has_head();
  void set_has_body();
  void clear_has_body();
  void set_has_control();
  void clear_has_control();
  void set_has_request();
  void clear_has_request();
  void set_has_app_id();
  void clear_has_app_id();
  void set_has_timestamp();
  void clear_has_timestamp();
  void set_has_staleness();
  void clear_has_staleness();
  void set_has_customer_id();
  void clear_has_customer_id();
  void set_has_push();
  void clear_has_push();
  void set_has_simple_app();
  void clear_has_simple_app();

  ::google::protobuf::internal::InternalMetadataWithArenaLite _internal_metadata_;
  ::google::protobuf::internal::HasBits<1> _has_bits_;
  mutable int _cached_size_;
  ::google::protobuf::RepeatedField< ::google::protobuf::int32 > data_type_;
  mutable int _data_type_cached_byte_size_;
  ::google::protobuf::internal::ArenaStringPtr body_;
  ::ps::PBControl* control_;
  ::google::protobuf::int32 head_;
  bool request_;
  bool push_;
  bool simple_app_;
  ::google::protobuf::int32 app_id_;
  ::google::protobuf::int32 timestamp_;
  ::google::protobuf::int32 staleness_;
  ::google::protobuf::int32 customer_id_;
  friend struct ::protobuf_meta_2eproto::TableStruct;
  friend void ::protobuf_meta_2eproto::InitDefaultsPBMetaImpl();
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// PBNode

// required int32 role = 1;
inline bool PBNode::has_role() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void PBNode::set_has_role() {
  _has_bits_[0] |= 0x00000002u;
}
inline void PBNode::clear_has_role() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void PBNode::clear_role() {
  role_ = 0;
  clear_has_role();
}
inline ::google::protobuf::int32 PBNode::role() const {
  // @@protoc_insertion_point(field_get:ps.PBNode.role)
  return role_;
}
inline void PBNode::set_role(::google::protobuf::int32 value) {
  set_has_role();
  role_ = value;
  // @@protoc_insertion_point(field_set:ps.PBNode.role)
}

// optional int32 id = 2;
inline bool PBNode::has_id() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void PBNode::set_has_id() {
  _has_bits_[0] |= 0x00000004u;
}
inline void PBNode::clear_has_id() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void PBNode::clear_id() {
  id_ = 0;
  clear_has_id();
}
inline ::google::protobuf::int32 PBNode::id() const {
  // @@protoc_insertion_point(field_get:ps.PBNode.id)
  return id_;
}
inline void PBNode::set_id(::google::protobuf::int32 value) {
  set_has_id();
  id_ = value;
  // @@protoc_insertion_point(field_set:ps.PBNode.id)
}

// optional string hostname = 3;
inline bool PBNode::has_hostname() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void PBNode::set_has_hostname() {
  _has_bits_[0] |= 0x00000001u;
}
inline void PBNode::clear_has_hostname() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void PBNode::clear_hostname() {
  hostname_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_hostname();
}
inline const ::std::string& PBNode::hostname() const {
  // @@protoc_insertion_point(field_get:ps.PBNode.hostname)
  return hostname_.GetNoArena();
}
inline void PBNode::set_hostname(const ::std::string& value) {
  set_has_hostname();
  hostname_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:ps.PBNode.hostname)
}
#if LANG_CXX11
inline void PBNode::set_hostname(::std::string&& value) {
  set_has_hostname();
  hostname_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:ps.PBNode.hostname)
}
#endif
inline void PBNode::set_hostname(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_hostname();
  hostname_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:ps.PBNode.hostname)
}
inline void PBNode::set_hostname(const char* value, size_t size) {
  set_has_hostname();
  hostname_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:ps.PBNode.hostname)
}
inline ::std::string* PBNode::mutable_hostname() {
  set_has_hostname();
  // @@protoc_insertion_point(field_mutable:ps.PBNode.hostname)
  return hostname_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PBNode::release_hostname() {
  // @@protoc_insertion_point(field_release:ps.PBNode.hostname)
  clear_has_hostname();
  return hostname_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PBNode::set_allocated_hostname(::std::string* hostname) {
  if (hostname != NULL) {
    set_has_hostname();
  } else {
    clear_has_hostname();
  }
  hostname_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), hostname);
  // @@protoc_insertion_point(field_set_allocated:ps.PBNode.hostname)
}

// optional int32 port = 4;
inline bool PBNode::has_port() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void PBNode::set_has_port() {
  _has_bits_[0] |= 0x00000008u;
}
inline void PBNode::clear_has_port() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void PBNode::clear_port() {
  port_ = 0;
  clear_has_port();
}
inline ::google::protobuf::int32 PBNode::port() const {
  // @@protoc_insertion_point(field_get:ps.PBNode.port)
  return port_;
}
inline void PBNode::set_port(::google::protobuf::int32 value) {
  set_has_port();
  port_ = value;
  // @@protoc_insertion_point(field_set:ps.PBNode.port)
}

// optional bool is_recovery = 5;
inline bool PBNode::has_is_recovery() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void PBNode::set_has_is_recovery() {
  _has_bits_[0] |= 0x00000010u;
}
inline void PBNode::clear_has_is_recovery() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void PBNode::clear_is_recovery() {
  is_recovery_ = false;
  clear_has_is_recovery();
}
inline bool PBNode::is_recovery() const {
  // @@protoc_insertion_point(field_get:ps.PBNode.is_recovery)
  return is_recovery_;
}
inline void PBNode::set_is_recovery(bool value) {
  set_has_is_recovery();
  is_recovery_ = value;
  // @@protoc_insertion_point(field_set:ps.PBNode.is_recovery)
}

// optional int32 customer_id = 10;
inline bool PBNode::has_customer_id() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void PBNode::set_has_customer_id() {
  _has_bits_[0] |= 0x00000020u;
}
inline void PBNode::clear_has_customer_id() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void PBNode::clear_customer_id() {
  customer_id_ = 0;
  clear_has_customer_id();
}
inline ::google::protobuf::int32 PBNode::customer_id() const {
  // @@protoc_insertion_point(field_get:ps.PBNode.customer_id)
  return customer_id_;
}
inline void PBNode::set_customer_id(::google::protobuf::int32 value) {
  set_has_customer_id();
  customer_id_ = value;
  // @@protoc_insertion_point(field_set:ps.PBNode.customer_id)
}

// -------------------------------------------------------------------

// PBControl

// required int32 cmd = 1;
inline bool PBControl::has_cmd() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void PBControl::set_has_cmd() {
  _has_bits_[0] |= 0x00000001u;
}
inline void PBControl::clear_has_cmd() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void PBControl::clear_cmd() {
  cmd_ = 0;
  clear_has_cmd();
}
inline ::google::protobuf::int32 PBControl::cmd() const {
  // @@protoc_insertion_point(field_get:ps.PBControl.cmd)
  return cmd_;
}
inline void PBControl::set_cmd(::google::protobuf::int32 value) {
  set_has_cmd();
  cmd_ = value;
  // @@protoc_insertion_point(field_set:ps.PBControl.cmd)
}

// repeated .ps.PBNode node = 2;
inline int PBControl::node_size() const {
  return node_.size();
}
inline void PBControl::clear_node() {
  node_.Clear();
}
inline const ::ps::PBNode& PBControl::node(int index) const {
  // @@protoc_insertion_point(field_get:ps.PBControl.node)
  return node_.Get(index);
}
inline ::ps::PBNode* PBControl::mutable_node(int index) {
  // @@protoc_insertion_point(field_mutable:ps.PBControl.node)
  return node_.Mutable(index);
}
inline ::ps::PBNode* PBControl::add_node() {
  // @@protoc_insertion_point(field_add:ps.PBControl.node)
  return node_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::ps::PBNode >*
PBControl::mutable_node() {
  // @@protoc_insertion_point(field_mutable_list:ps.PBControl.node)
  return &node_;
}
inline const ::google::protobuf::RepeatedPtrField< ::ps::PBNode >&
PBControl::node() const {
  // @@protoc_insertion_point(field_list:ps.PBControl.node)
  return node_;
}

// optional int32 barrier_group = 3;
inline bool PBControl::has_barrier_group() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void PBControl::set_has_barrier_group() {
  _has_bits_[0] |= 0x00000002u;
}
inline void PBControl::clear_has_barrier_group() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void PBControl::clear_barrier_group() {
  barrier_group_ = 0;
  clear_has_barrier_group();
}
inline ::google::protobuf::int32 PBControl::barrier_group() const {
  // @@protoc_insertion_point(field_get:ps.PBControl.barrier_group)
  return barrier_group_;
}
inline void PBControl::set_barrier_group(::google::protobuf::int32 value) {
  set_has_barrier_group();
  barrier_group_ = value;
  // @@protoc_insertion_point(field_set:ps.PBControl.barrier_group)
}

// optional uint64 msg_sig = 4;
inline bool PBControl::has_msg_sig() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void PBControl::set_has_msg_sig() {
  _has_bits_[0] |= 0x00000004u;
}
inline void PBControl::clear_has_msg_sig() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void PBControl::clear_msg_sig() {
  msg_sig_ = GOOGLE_ULONGLONG(0);
  clear_has_msg_sig();
}
inline ::google::protobuf::uint64 PBControl::msg_sig() const {
  // @@protoc_insertion_point(field_get:ps.PBControl.msg_sig)
  return msg_sig_;
}
inline void PBControl::set_msg_sig(::google::protobuf::uint64 value) {
  set_has_msg_sig();
  msg_sig_ = value;
  // @@protoc_insertion_point(field_set:ps.PBControl.msg_sig)
}

// -------------------------------------------------------------------

// PBMeta

// optional int32 head = 1;
inline bool PBMeta::has_head() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void PBMeta::set_has_head() {
  _has_bits_[0] |= 0x00000004u;
}
inline void PBMeta::clear_has_head() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void PBMeta::clear_head() {
  head_ = 0;
  clear_has_head();
}
inline ::google::protobuf::int32 PBMeta::head() const {
  // @@protoc_insertion_point(field_get:ps.PBMeta.head)
  return head_;
}
inline void PBMeta::set_head(::google::protobuf::int32 value) {
  set_has_head();
  head_ = value;
  // @@protoc_insertion_point(field_set:ps.PBMeta.head)
}

// optional bytes body = 2;
inline bool PBMeta::has_body() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void PBMeta::set_has_body() {
  _has_bits_[0] |= 0x00000001u;
}
inline void PBMeta::clear_has_body() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void PBMeta::clear_body() {
  body_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  clear_has_body();
}
inline const ::std::string& PBMeta::body() const {
  // @@protoc_insertion_point(field_get:ps.PBMeta.body)
  return body_.GetNoArena();
}
inline void PBMeta::set_body(const ::std::string& value) {
  set_has_body();
  body_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:ps.PBMeta.body)
}
#if LANG_CXX11
inline void PBMeta::set_body(::std::string&& value) {
  set_has_body();
  body_.SetNoArena(
    &::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::move(value));
  // @@protoc_insertion_point(field_set_rvalue:ps.PBMeta.body)
}
#endif
inline void PBMeta::set_body(const char* value) {
  GOOGLE_DCHECK(value != NULL);
  set_has_body();
  body_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:ps.PBMeta.body)
}
inline void PBMeta::set_body(const void* value, size_t size) {
  set_has_body();
  body_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:ps.PBMeta.body)
}
inline ::std::string* PBMeta::mutable_body() {
  set_has_body();
  // @@protoc_insertion_point(field_mutable:ps.PBMeta.body)
  return body_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* PBMeta::release_body() {
  // @@protoc_insertion_point(field_release:ps.PBMeta.body)
  clear_has_body();
  return body_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void PBMeta::set_allocated_body(::std::string* body) {
  if (body != NULL) {
    set_has_body();
  } else {
    clear_has_body();
  }
  body_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), body);
  // @@protoc_insertion_point(field_set_allocated:ps.PBMeta.body)
}

// optional .ps.PBControl control = 3;
inline bool PBMeta::has_control() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void PBMeta::set_has_control() {
  _has_bits_[0] |= 0x00000002u;
}
inline void PBMeta::clear_has_control() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void PBMeta::clear_control() {
  if (control_ != NULL) control_->Clear();
  clear_has_control();
}
inline const ::ps::PBControl& PBMeta::control() const {
  const ::ps::PBControl* p = control_;
  // @@protoc_insertion_point(field_get:ps.PBMeta.control)
  return p != NULL ? *p : *reinterpret_cast<const ::ps::PBControl*>(
      &::ps::_PBControl_default_instance_);
}
inline ::ps::PBControl* PBMeta::release_control() {
  // @@protoc_insertion_point(field_release:ps.PBMeta.control)
  clear_has_control();
  ::ps::PBControl* temp = control_;
  control_ = NULL;
  return temp;
}
inline ::ps::PBControl* PBMeta::mutable_control() {
  set_has_control();
  if (control_ == NULL) {
    control_ = new ::ps::PBControl;
  }
  // @@protoc_insertion_point(field_mutable:ps.PBMeta.control)
  return control_;
}
inline void PBMeta::set_allocated_control(::ps::PBControl* control) {
  ::google::protobuf::Arena* message_arena = GetArenaNoVirtual();
  if (message_arena == NULL) {
    delete control_;
  }
  if (control) {
    ::google::protobuf::Arena* submessage_arena = NULL;
    if (message_arena != submessage_arena) {
      control = ::google::protobuf::internal::GetOwnedMessage(
          message_arena, control, submessage_arena);
    }
    set_has_control();
  } else {
    clear_has_control();
  }
  control_ = control;
  // @@protoc_insertion_point(field_set_allocated:ps.PBMeta.control)
}

// optional bool request = 4 [default = false];
inline bool PBMeta::has_request() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void PBMeta::set_has_request() {
  _has_bits_[0] |= 0x00000008u;
}
inline void PBMeta::clear_has_request() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void PBMeta::clear_request() {
  request_ = false;
  clear_has_request();
}
inline bool PBMeta::request() const {
  // @@protoc_insertion_point(field_get:ps.PBMeta.request)
  return request_;
}
inline void PBMeta::set_request(bool value) {
  set_has_request();
  request_ = value;
  // @@protoc_insertion_point(field_set:ps.PBMeta.request)
}

// optional int32 app_id = 7;
inline bool PBMeta::has_app_id() const {
  return (_has_bits_[0] & 0x00000040u) != 0;
}
inline void PBMeta::set_has_app_id() {
  _has_bits_[0] |= 0x00000040u;
}
inline void PBMeta::clear_has_app_id() {
  _has_bits_[0] &= ~0x00000040u;
}
inline void PBMeta::clear_app_id() {
  app_id_ = 0;
  clear_has_app_id();
}
inline ::google::protobuf::int32 PBMeta::app_id() const {
  // @@protoc_insertion_point(field_get:ps.PBMeta.app_id)
  return app_id_;
}
inline void PBMeta::set_app_id(::google::protobuf::int32 value) {
  set_has_app_id();
  app_id_ = value;
  // @@protoc_insertion_point(field_set:ps.PBMeta.app_id)
}

// optional int32 timestamp = 8;
inline bool PBMeta::has_timestamp() const {
  return (_has_bits_[0] & 0x00000080u) != 0;
}
inline void PBMeta::set_has_timestamp() {
  _has_bits_[0] |= 0x00000080u;
}
inline void PBMeta::clear_has_timestamp() {
  _has_bits_[0] &= ~0x00000080u;
}
inline void PBMeta::clear_timestamp() {
  timestamp_ = 0;
  clear_has_timestamp();
}
inline ::google::protobuf::int32 PBMeta::timestamp() const {
  // @@protoc_insertion_point(field_get:ps.PBMeta.timestamp)
  return timestamp_;
}
inline void PBMeta::set_timestamp(::google::protobuf::int32 value) {
  set_has_timestamp();
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:ps.PBMeta.timestamp)
}

// optional int32 staleness = 9;
inline bool PBMeta::has_staleness() const {
  return (_has_bits_[0] & 0x00000100u) != 0;
}
inline void PBMeta::set_has_staleness() {
  _has_bits_[0] |= 0x00000100u;
}
inline void PBMeta::clear_has_staleness() {
  _has_bits_[0] &= ~0x00000100u;
}
inline void PBMeta::clear_staleness() {
  staleness_ = 0;
  clear_has_staleness();
}
inline ::google::protobuf::int32 PBMeta::staleness() const {
  // @@protoc_insertion_point(field_get:ps.PBMeta.staleness)
  return staleness_;
}
inline void PBMeta::set_staleness(::google::protobuf::int32 value) {
  set_has_staleness();
  staleness_ = value;
  // @@protoc_insertion_point(field_set:ps.PBMeta.staleness)
}

// repeated int32 data_type = 10 [packed = true];
inline int PBMeta::data_type_size() const {
  return data_type_.size();
}
inline void PBMeta::clear_data_type() {
  data_type_.Clear();
}
inline ::google::protobuf::int32 PBMeta::data_type(int index) const {
  // @@protoc_insertion_point(field_get:ps.PBMeta.data_type)
  return data_type_.Get(index);
}
inline void PBMeta::set_data_type(int index, ::google::protobuf::int32 value) {
  data_type_.Set(index, value);
  // @@protoc_insertion_point(field_set:ps.PBMeta.data_type)
}
inline void PBMeta::add_data_type(::google::protobuf::int32 value) {
  data_type_.Add(value);
  // @@protoc_insertion_point(field_add:ps.PBMeta.data_type)
}
inline const ::google::protobuf::RepeatedField< ::google::protobuf::int32 >&
PBMeta::data_type() const {
  // @@protoc_insertion_point(field_list:ps.PBMeta.data_type)
  return data_type_;
}
inline ::google::protobuf::RepeatedField< ::google::protobuf::int32 >*
PBMeta::mutable_data_type() {
  // @@protoc_insertion_point(field_mutable_list:ps.PBMeta.data_type)
  return &data_type_;
}

// optional int32 customer_id = 11;
inline bool PBMeta::has_customer_id() const {
  return (_has_bits_[0] & 0x00000200u) != 0;
}
inline void PBMeta::set_has_customer_id() {
  _has_bits_[0] |= 0x00000200u;
}
inline void PBMeta::clear_has_customer_id() {
  _has_bits_[0] &= ~0x00000200u;
}
inline void PBMeta::clear_customer_id() {
  customer_id_ = 0;
  clear_has_customer_id();
}
inline ::google::protobuf::int32 PBMeta::customer_id() const {
  // @@protoc_insertion_point(field_get:ps.PBMeta.customer_id)
  return customer_id_;
}
inline void PBMeta::set_customer_id(::google::protobuf::int32 value) {
  set_has_customer_id();
  customer_id_ = value;
  // @@protoc_insertion_point(field_set:ps.PBMeta.customer_id)
}

// optional bool push = 5;
inline bool PBMeta::has_push() const {
  return (_has_bits_[0] & 0x00000010u) != 0;
}
inline void PBMeta::set_has_push() {
  _has_bits_[0] |= 0x00000010u;
}
inline void PBMeta::clear_has_push() {
  _has_bits_[0] &= ~0x00000010u;
}
inline void PBMeta::clear_push() {
  push_ = false;
  clear_has_push();
}
inline bool PBMeta::push() const {
  // @@protoc_insertion_point(field_get:ps.PBMeta.push)
  return push_;
}
inline void PBMeta::set_push(bool value) {
  set_has_push();
  push_ = value;
  // @@protoc_insertion_point(field_set:ps.PBMeta.push)
}

// optional bool simple_app = 6 [default = false];
inline bool PBMeta::has_simple_app() const {
  return (_has_bits_[0] & 0x00000020u) != 0;
}
inline void PBMeta::set_has_simple_app() {
  _has_bits_[0] |= 0x00000020u;
}
inline void PBMeta::clear_has_simple_app() {
  _has_bits_[0] &= ~0x00000020u;
}
inline void PBMeta::clear_simple_app() {
  simple_app_ = false;
  clear_has_simple_app();
}
inline bool PBMeta::simple_app() const {
  // @@protoc_insertion_point(field_get:ps.PBMeta.simple_app)
  return simple_app_;
}
inline void PBMeta::set_simple_app(bool value) {
  set_has_simple_app();
  simple_app_ = value;
  // @@protoc_insertion_point(field_set:ps.PBMeta.simple_app)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------

// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

}  // namespace ps

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_meta_2eproto__INCLUDED